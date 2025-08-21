#include "huffman_algorithm.h"

/**
 * Constrói a árvore de Huffman a partir das frequências dos caracteres
 * @param frequencies Array com as frequências de cada caractere
 * @return Raiz da árvore de Huffman construída
 */
HuffmanNode* buildHuffmanTree(unsigned long* frequencies) {
    // Conta quantos caracteres únicos existem
    int unique_chars = countUniqueCharacters(frequencies);
    
    if (unique_chars == 0) {
        return NULL;
    }
    
    // Cria a fila de prioridade
    PriorityQueue* pq = createPriorityQueue(unique_chars);
    
    // Insere todos os caracteres com frequência > 0 na fila
    for (int i = 0; i < MAX_CHAR; i++) {
        if (frequencies[i] > 0) {
            HuffmanNode* node = createNode((unsigned char)i, frequencies[i]);
            insert(pq, node);
        }
    }
    
    // Constrói a árvore combinando os nós com menor frequência
    while (!isSizeOne(pq)) {
        // Remove os dois nós com menor frequência
        HuffmanNode* left = extractMin(pq);
        HuffmanNode* right = extractMin(pq);
        
        // Cria um novo nó interno com a soma das frequências
        HuffmanNode* internal = createNode(0, left->frequency + right->frequency);
        internal->left = left;
        internal->right = right;
        
        // Insere o novo nó na fila
        insert(pq, internal);
    }
    
    // O último nó restante é a raiz da árvore
    HuffmanNode* root = extractMin(pq);
    
    // Libera a fila de prioridade (não os nós da árvore)
    freePriorityQueue(pq);
    
    return root;
}

/**
 * Gera os códigos de Huffman para cada caractere
 * @param root Raiz da árvore de Huffman
 * @param current_code Código atual sendo construído
 * @param depth Profundidade atual na árvore
 * @param codes Array onde os códigos serão armazenados
 */
void generateHuffmanCodes(HuffmanNode* root, char* current_code, int depth, char codes[MAX_CHAR][MAX_TREE_HT]) {
    if (root == NULL) {
        return;
    }
    
    // Se é uma folha, armazena o código
    if (isLeaf(root)) {
        current_code[depth] = '\0';
        strcpy(codes[root->data], current_code);
        return;
    }
    
    // Percorre o filho esquerdo (bit 0)
    if (root->left != NULL) {
        current_code[depth] = '0';
        generateHuffmanCodes(root->left, current_code, depth + 1, codes);
    }
    
    // Percorre o filho direito (bit 1)
    if (root->right != NULL) {
        current_code[depth] = '1';
        generateHuffmanCodes(root->right, current_code, depth + 1, codes);
    }
}

/**
 * Comprime um arquivo usando o algoritmo de Huffman
 * @param input_filename Nome do arquivo de entrada
 * @param output_filename Nome do arquivo de saída comprimido
 * @return 0 se sucesso, -1 se erro
 */
int compressFile(const char* input_filename, const char* output_filename) {
    // Verifica se o arquivo de entrada existe
    if (!fileExists(input_filename)) {
        fprintf(stderr, "Erro: Arquivo de entrada '%s' não encontrado\n", input_filename);
        return -1;
    }
    
    // Calcula as frequências dos caracteres
    unsigned long* frequencies = calculateFrequencies(input_filename);
    
    // Constrói a árvore de Huffman
    HuffmanNode* root = buildHuffmanTree(frequencies);
    if (root == NULL) {
        fprintf(stderr, "Erro: Falha ao construir a árvore de Huffman\n");
        free(frequencies);
        return -1;
    }
    
    // Gera os códigos de Huffman
    char codes[MAX_CHAR][MAX_TREE_HT] = {{0}};
    char current_code[MAX_TREE_HT] = {0};
    generateHuffmanCodes(root, current_code, 0, codes);
    
    // Abre os arquivos
    FILE* input = fopen(input_filename, "rb");
    FILE* output = fopen(output_filename, "wb");
    
    if (input == NULL || output == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir os arquivos\n");
        freeHuffmanTree(root);
        free(frequencies);
        return -1;
    }
    
    // Escreve o cabeçalho com a árvore
    writeCompressedHeader(output, root);
    
    // Escreve os dados comprimidos
    writeCompressedData(input, output, codes);
    
    // Fecha os arquivos
    fclose(input);
    fclose(output);
    
    // Libera a memória
    freeHuffmanTree(root);
    free(frequencies);
    
    return 0;
}

/**
 * Descomprime um arquivo usando o algoritmo de Huffman
 * @param input_filename Nome do arquivo comprimido
 * @param output_filename Nome do arquivo de saída descomprimido
 * @return 0 se sucesso, -1 se erro
 */
int decompressFile(const char* input_filename, const char* output_filename) {
    // Verifica se o arquivo de entrada existe
    if (!fileExists(input_filename)) {
        fprintf(stderr, "Erro: Arquivo de entrada '%s' não encontrado\n", input_filename);
        return -1;
    }
    
    // Abre os arquivos
    FILE* input = fopen(input_filename, "rb");
    FILE* output = fopen(output_filename, "wb");
    
    if (input == NULL || output == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir os arquivos\n");
        return -1;
    }
    
    // Lê o cabeçalho e reconstrói a árvore
    HuffmanNode* root = readCompressedHeader(input);
    if (root == NULL) {
        fprintf(stderr, "Erro: Falha ao ler o cabeçalho do arquivo\n");
        fclose(input);
        fclose(output);
        return -1;
    }
    
    // Lê e descomprime os dados
    readCompressedData(input, output, root);
    
    // Fecha os arquivos
    fclose(input);
    fclose(output);
    
    // Libera a memória
    freeHuffmanTree(root);
    
    return 0;
}

/**
 * Imprime os códigos de Huffman gerados
 * @param codes Array com os códigos de Huffman
 */
void printHuffmanCodes(char codes[MAX_CHAR][MAX_TREE_HT]) {
    printf("Códigos de Huffman:\n");
    printf("Caractere | Código\n");
    printf("----------|-------\n");
    
    for (int i = 0; i < MAX_CHAR; i++) {
        if (strlen(codes[i]) > 0) {
            if (i >= 32 && i <= 126) {
                printf("'%c'       | %s\n", i, codes[i]);
            } else {
                printf("0x%02X     | %s\n", i, codes[i]);
            }
        }
    }
}

/**
 * Imprime a árvore de Huffman (para debug)
 * @param root Raiz da árvore
 * @param depth Profundidade atual
 */
void printHuffmanTree(HuffmanNode* root, int depth) {
    if (root == NULL) {
        return;
    }
    
    // Imprime indentação
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    if (isLeaf(root)) {
        if (root->data >= 32 && root->data <= 126) {
            printf("'%c' (freq: %lu)\n", root->data, root->frequency);
        } else {
            printf("0x%02X (freq: %lu)\n", root->data, root->frequency);
        }
    } else {
        printf("Nó interno (freq: %lu)\n", root->frequency);
        printHuffmanTree(root->left, depth + 1);
        printHuffmanTree(root->right, depth + 1);
    }
}

/**
 * Calcula a taxa de compressão
 * @param original_file Arquivo original
 * @param compressed_file Arquivo comprimido
 * @return Taxa de compressão (0.0 a 1.0)
 */
double calculateCompressionRatio(const char* original_file, const char* compressed_file) {
    long original_size = getFileSize(original_file);
    long compressed_size = getFileSize(compressed_file);
    
    if (original_size <= 0 || compressed_size <= 0) {
        return -1.0;
    }
    
    return 1.0 - ((double)compressed_size / original_size);
}

/**
 * Valida se a compressão foi bem-sucedida comparando arquivos
 * @param original_file Arquivo original
 * @param decompressed_file Arquivo descomprimido
 * @return 1 se os arquivos são idênticos, 0 caso contrário
 */
int validateCompression(const char* original_file, const char* decompressed_file) {
    FILE* original = fopen(original_file, "rb");
    FILE* decompressed = fopen(decompressed_file, "rb");
    
    if (original == NULL || decompressed == NULL) {
        if (original) fclose(original);
        if (decompressed) fclose(decompressed);
        return 0;
    }
    
    int identical = 1;
    int ch1, ch2;
    
    while ((ch1 = fgetc(original)) != EOF && (ch2 = fgetc(decompressed)) != EOF) {
        if (ch1 != ch2) {
            identical = 0;
            break;
        }
    }
    
    // Verifica se ambos os arquivos terminaram ao mesmo tempo
    if (ch1 != EOF || ch2 != EOF) {
        identical = 0;
    }
    
    fclose(original);
    fclose(decompressed);
    
    return identical;
}

/**
 * Imprime estatísticas da compressão
 * @param original_file Arquivo original
 * @param compressed_file Arquivo comprimido
 */
void printCompressionStats(const char* original_file, const char* compressed_file) {
    long original_size = getFileSize(original_file);
    long compressed_size = getFileSize(compressed_file);
    
    if (original_size <= 0 || compressed_size <= 0) {
        printf("Erro ao obter tamanhos dos arquivos\n");
        return;
    }
    
    double ratio = calculateCompressionRatio(original_file, compressed_file);
    
    printf("\n=== Estatísticas de Compressão ===\n");
    printf("Tamanho original: %ld bytes\n", original_size);
    printf("Tamanho comprimido: %ld bytes\n", compressed_size);
    printf("Taxa de compressão: %.2f%%\n", ratio * 100);
    printf("Economia de espaço: %ld bytes\n", original_size - compressed_size);
}
