#include "file_io.h"
#include <string.h>

/**
 * Calcula as frequências de cada caractere em um arquivo
 * @param filename Nome do arquivo a ser analisado
 * @return Array com as frequências de cada caractere (0-255)
 */
unsigned long* calculateFrequencies(const char* filename) {
    unsigned long* frequencies = (unsigned long*)calloc(MAX_CHAR, sizeof(unsigned long));
    if (frequencies == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para frequências\n");
        exit(EXIT_FAILURE);
    }
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'\n", filename);
        free(frequencies);
        exit(EXIT_FAILURE);
    }
    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    // Lê o arquivo em chunks e conta as frequências
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            frequencies[buffer[i]]++;
        }
    }
    
    fclose(file);
    return frequencies;
}

/**
 * Conta quantos caracteres únicos existem no arquivo
 * @param frequencies Array de frequências
 * @return Número de caracteres únicos
 */
int countUniqueCharacters(unsigned long* frequencies) {
    int count = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (frequencies[i] > 0) {
            count++;
        }
    }
    return count;
}

/**
 * Inicializa um buffer de bits
 * @param bit_buffer Buffer a ser inicializado
 */
void initBitBuffer(BitBuffer* bit_buffer) {
    bit_buffer->buffer = 0;
    bit_buffer->bit_count = 0;
}

/**
 * Escreve um bit no arquivo usando buffer
 * @param bit_buffer Buffer de bits
 * @param bit Bit a ser escrito (0 ou 1)
 * @param output Arquivo de saída
 */
void writeBit(BitBuffer* bit_buffer, int bit, FILE* output) {
    // Adiciona o bit ao buffer
    bit_buffer->buffer = (bit_buffer->buffer << 1) | (bit & 1);
    bit_buffer->bit_count++;
    
    // Se o buffer está cheio, escreve no arquivo
    if (bit_buffer->bit_count == 8) {
        fputc(bit_buffer->buffer, output);
        bit_buffer->buffer = 0;
        bit_buffer->bit_count = 0;
    }
}

/**
 * Força a escrita dos bits restantes no buffer
 * @param bit_buffer Buffer de bits
 * @param output Arquivo de saída
 */
void flushBitBuffer(BitBuffer* bit_buffer, FILE* output) {
    if (bit_buffer->bit_count > 0) {
        // Preenche os bits restantes com zeros
        bit_buffer->buffer <<= (8 - bit_buffer->bit_count);
        fputc(bit_buffer->buffer, output);
        bit_buffer->buffer = 0;
        bit_buffer->bit_count = 0;
    }
}

/**
 * Serializa a árvore de Huffman no arquivo
 * @param file Arquivo de saída
 * @param root Raiz da árvore
 */
void serializeTree(FILE* file, HuffmanNode* root) {
    if (root == NULL) {
        return;
    }
    
    if (isLeaf(root)) {
        // Marca como nó folha e escreve o caractere
        fputc(1, file);
        fputc(root->data, file);
    } else {
        // Marca como nó interno
        fputc(0, file);
        serializeTree(file, root->left);
        serializeTree(file, root->right);
    }
}

/**
 * Escreve o cabeçalho do arquivo comprimido
 * @param output Arquivo de saída
 * @param root Raiz da árvore de Huffman
 */
void writeCompressedHeader(FILE* output, HuffmanNode* root) {
    // Serializa a árvore no cabeçalho
    serializeTree(output, root);
    
    // Marca o fim do cabeçalho com um byte especial
    fputc(0xFF, output);
}

/**
 * Escreve os dados comprimidos no arquivo
 * @param input Arquivo de entrada
 * @param output Arquivo de saída
 * @param codes Tabela de códigos de Huffman
 */
void writeCompressedData(FILE* input, FILE* output, char codes[MAX_CHAR][MAX_TREE_HT]) {
    BitBuffer bit_buffer;
    initBitBuffer(&bit_buffer);
    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    // Lê o arquivo original e escreve os códigos correspondentes
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            char* code = codes[buffer[i]];
            for (int j = 0; code[j] != '\0'; j++) {
                writeBit(&bit_buffer, code[j] - '0', output);
            }
        }
    }
    
    // Escreve os bits restantes
    flushBitBuffer(&bit_buffer, output);
}

/**
 * Desserializa a árvore de Huffman do arquivo
 * @param file Arquivo de entrada
 * @return Raiz da árvore reconstruída
 */
HuffmanNode* deserializeTree(FILE* file) {
    int marker = fgetc(file);
    
    if (marker == 1) {
        // Nó folha
        unsigned char data = fgetc(file);
        return createNode(data, 0); // Frequência não é necessária para decodificação
    } else if (marker == 0) {
        // Nó interno
        HuffmanNode* node = createNode(0, 0);
        node->left = deserializeTree(file);
        node->right = deserializeTree(file);
        return node;
    }
    
    return NULL;
}

/**
 * Lê o cabeçalho do arquivo comprimido
 * @param input Arquivo de entrada
 * @return Raiz da árvore de Huffman
 */
HuffmanNode* readCompressedHeader(FILE* input) {
    HuffmanNode* root = deserializeTree(input);
    
    // Lê o marcador de fim do cabeçalho
    int marker = fgetc(input);
    if (marker != 0xFF) {
        fprintf(stderr, "Erro: Formato de arquivo inválido\n");
        freeHuffmanTree(root);
        return NULL;
    }
    
    return root;
}

/**
 * Lê um bit do arquivo usando buffer
 * @param bit_buffer Buffer de bits
 * @param input Arquivo de entrada
 * @return Bit lido (0 ou 1) ou -1 se EOF
 */
int readBit(BitBuffer* bit_buffer, FILE* input) {
    if (bit_buffer->bit_count == 0) {
        int byte = fgetc(input);
        if (byte == EOF) {
            return -1;
        }
        bit_buffer->buffer = (unsigned char)byte;
        bit_buffer->bit_count = 8;
    }
    
    int bit = (bit_buffer->buffer >> (bit_buffer->bit_count - 1)) & 1;
    bit_buffer->bit_count--;
    return bit;
}

/**
 * Lê e descomprime os dados do arquivo
 * @param input Arquivo de entrada comprimido
 * @param output Arquivo de saída descomprimido
 * @param root Raiz da árvore de Huffman
 */
void readCompressedData(FILE* input, FILE* output, HuffmanNode* root) {
    BitBuffer bit_buffer;
    initBitBuffer(&bit_buffer);
    
    HuffmanNode* current = root;
    int bit;
    
    // Lê os bits e navega na árvore para decodificar
    while ((bit = readBit(&bit_buffer, input)) != -1) {
        if (bit == 0) {
            current = current->left;
        } else {
            current = current->right;
        }
        
        // Se chegou a uma folha, escreve o caractere
        if (isLeaf(current)) {
            fputc(current->data, output);
            current = root; // Volta para a raiz
        }
    }
}

/**
 * Verifica se um arquivo existe
 * @param filename Nome do arquivo
 * @return 1 se existe, 0 caso contrário
 */
int fileExists(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * Obtém o tamanho de um arquivo em bytes
 * @param filename Nome do arquivo
 * @return Tamanho do arquivo em bytes, ou -1 se erro
 */
long getFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}
