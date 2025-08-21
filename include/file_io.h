#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"

// Constantes para operações de arquivo
#define BUFFER_SIZE 4096
#define MAX_FILENAME 256

// Estrutura para buffer de bits
typedef struct BitBuffer {
    unsigned char buffer;     // Buffer de 8 bits
    int bit_count;           // Número de bits no buffer
} BitBuffer;

// Funções para cálculo de frequências
unsigned long* calculateFrequencies(const char* filename);
int countUniqueCharacters(unsigned long* frequencies);

// Funções para escrita de arquivos comprimidos
void writeCompressedHeader(FILE* output, HuffmanNode* root);
void writeCompressedData(FILE* input, FILE* output, char codes[MAX_CHAR][MAX_TREE_HT]);
void writeBit(BitBuffer* bit_buffer, int bit, FILE* output);
void flushBitBuffer(BitBuffer* bit_buffer, FILE* output);

// Funções para leitura de arquivos comprimidos
HuffmanNode* readCompressedHeader(FILE* input);
void readCompressedData(FILE* input, FILE* output, HuffmanNode* root);
int readBit(BitBuffer* bit_buffer, FILE* input);

// Funções auxiliares para manipulação de arquivos
void initBitBuffer(BitBuffer* bit_buffer);
int fileExists(const char* filename);
long getFileSize(const char* filename);

// Funções para serialização e desserialização da árvore
void serializeTree(FILE* file, HuffmanNode* root);
HuffmanNode* deserializeTree(FILE* file);

#endif // FILE_IO_H
