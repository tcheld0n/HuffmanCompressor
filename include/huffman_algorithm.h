#ifndef HUFFMAN_ALGORITHM_H
#define HUFFMAN_ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "file_io.h"

// Constantes para o algoritmo de Huffman
#define MAX_CODE_LENGTH 256

// Funções para construção da árvore de Huffman
HuffmanNode* buildHuffmanTree(unsigned long* frequencies);
void generateHuffmanCodes(HuffmanNode* root, char* current_code, int depth, char codes[MAX_CHAR][MAX_TREE_HT]);

// Funções para compressão e descompressão
int compressFile(const char* input_filename, const char* output_filename);
int decompressFile(const char* input_filename, const char* output_filename);

// Funções auxiliares para análise de dados
void printHuffmanCodes(char codes[MAX_CHAR][MAX_TREE_HT]);
void printHuffmanTree(HuffmanNode* root, int depth);
double calculateCompressionRatio(const char* original_file, const char* compressed_file);

// Funções para validação e verificação
int validateCompression(const char* original_file, const char* decompressed_file);
void printCompressionStats(const char* original_file, const char* compressed_file);

#endif // HUFFMAN_ALGORITHM_H
