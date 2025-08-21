#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "huffman_algorithm.h"

#define MAX_FILENAME 256

void printUsage(const char* program_name) {
    printf("Compressor e Descompressor Huffman\n");
    printf("Uso: %s [opção] arquivo_entrada [arquivo_saída]\n\n", program_name);
    printf("Opções:\n");
    printf("  -c, --compress    Comprime o arquivo de entrada\n");
    printf("  -d, --decompress  Descomprime o arquivo de entrada\n");
    printf("  -h, --help        Mostra esta mensagem de ajuda\n");
    printf("  -v, --verbose     Modo verboso (mostra estatísticas detalhadas)\n\n");
    printf("Exemplos:\n");
    printf("  %s -c arquivo.txt arquivo.huf\n", program_name);
    printf("  %s -d arquivo.huf arquivo_descomprimido.txt\n", program_name);
    printf("  %s -c -v imagem.jpg imagem.huf\n", program_name);
}

void printVerboseInfo(const char* input_file, const char* output_file, int is_compression) {
    long file_size = getFileSize(input_file);
    if (file_size > 0) {
        printf("Arquivo de entrada: %s (%ld bytes)\n", input_file, file_size);
        printf("Arquivo de saída: %s\n", output_file);
        printf("Operação: %s\n", is_compression ? "Compressão" : "Descompressão");
        printf("Iniciando...\n");
    }
}

int main(int argc, char* argv[]) {
    clock_t start_time, end_time;
    double cpu_time_used;
    int verbose_mode = 0;
    int operation = 0; // 0 = nenhuma, 1 = compressão, 2 = descompressão
    
    char input_file[MAX_FILENAME] = {0};
    char output_file[MAX_FILENAME] = {0};
    
    // Parse dos argumentos da linha de comando
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose_mode = 1;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compress") == 0) {
            operation = 1;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decompress") == 0) {
            operation = 2;
        } else if (input_file[0] == '\0') {
            strncpy(input_file, argv[i], MAX_FILENAME - 1);
            input_file[MAX_FILENAME - 1] = '\0';
        } else if (output_file[0] == '\0') {
            strncpy(output_file, argv[i], MAX_FILENAME - 1);
            output_file[MAX_FILENAME - 1] = '\0';
        } else {
            fprintf(stderr, "Erro: Argumento inválido '%s'\n", argv[i]);
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Verifica se os argumentos necessários foram fornecidos
    if (operation == 0) {
        fprintf(stderr, "Erro: Deve especificar uma operação (-c ou -d)\n");
        printUsage(argv[0]);
        return 1;
    }
    
    if (input_file[0] == '\0') {
        fprintf(stderr, "Erro: Deve especificar um arquivo de entrada\n");
        printUsage(argv[0]);
        return 1;
    }
    
    // Se não foi especificado arquivo de saída, gera um nome padrão
    if (output_file[0] == '\0') {
        if (operation == 1) {
            // Compressão: adiciona extensão .huf
            snprintf(output_file, MAX_FILENAME, "%s.huf", input_file);
        } else {
            // Descompressão: remove extensão .huf ou adiciona _decompressed
            if (strstr(input_file, ".huf") != NULL) {
                strncpy(output_file, input_file, MAX_FILENAME - 1);
                output_file[MAX_FILENAME - 1] = '\0';
                char* ext = strstr(output_file, ".huf");
                if (ext) *ext = '\0';
            } else {
                snprintf(output_file, MAX_FILENAME, "%s_decompressed", input_file);
            }
        }
    }
    
    // Verifica se o arquivo de entrada existe
    if (!fileExists(input_file)) {
        fprintf(stderr, "Erro: Arquivo de entrada '%s' não encontrado\n", input_file);
        return 1;
    }
    
    if (verbose_mode) {
        printVerboseInfo(input_file, output_file, operation == 1);
    }
    
    // Inicia o cronômetro
    start_time = clock();
    
    int result = 0;
    
    // Executa a operação solicitada
    if (operation == 1) {
        // Compressão
        printf("Comprimindo '%s' para '%s'...\n", input_file, output_file);
        result = compressFile(input_file, output_file);
        
        if (result == 0) {
            printf("Compressão concluída com sucesso!\n");
            
            if (verbose_mode) {
                printCompressionStats(input_file, output_file);
            }
        } else {
            fprintf(stderr, "Erro durante a compressão\n");
        }
        
    } else if (operation == 2) {
        // Descompressão
        printf("Descomprimindo '%s' para '%s'...\n", input_file, output_file);
        result = decompressFile(input_file, output_file);
        
        if (result == 0) {
            printf("Descompressão concluída com sucesso!\n");
            
            if (verbose_mode) {
                // Valida se a descompressão foi bem-sucedida
                if (validateCompression(input_file, output_file)) {
                    printf("✓ Validação: Arquivo descomprimido é idêntico ao original\n");
                } else {
                    printf("✗ Validação: Arquivo descomprimido difere do original\n");
                }
            }
        } else {
            fprintf(stderr, "Erro durante a descompressão\n");
        }
    }
    
    // Para o cronômetro e calcula o tempo
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    if (verbose_mode && result == 0) {
        printf("Tempo de execução: %.3f segundos\n", cpu_time_used);
    }
    
    return result;
}
