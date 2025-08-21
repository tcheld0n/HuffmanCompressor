#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define MAX_TREE_HT 100
#define MAX_CHAR 256

// Estrutura para o nó da árvore de Huffman
typedef struct HuffmanNode {
    unsigned char data;           // Caractere (byte)
    unsigned long frequency;      // Frequência do caractere
    struct HuffmanNode *left;     // Filho esquerdo
    struct HuffmanNode *right;    // Filho direito
} HuffmanNode;

// Estrutura para a fila de prioridade (min-heap)
typedef struct PriorityQueue {
    unsigned size;                // Tamanho atual da fila
    unsigned capacity;            // Capacidade máxima da fila
    HuffmanNode** array;          // Array de ponteiros para nós
} PriorityQueue;

// Funções para criação e manipulação de nós
HuffmanNode* createNode(unsigned char data, unsigned long freq);

// Funções para a fila de prioridade
PriorityQueue* createPriorityQueue(unsigned capacity);
void insert(PriorityQueue* pq, HuffmanNode* node);
HuffmanNode* extractMin(PriorityQueue* pq);
int isSizeOne(PriorityQueue* pq);
int isLeaf(HuffmanNode* node);

// Funções auxiliares para manipulação da fila
void swapNodes(HuffmanNode** a, HuffmanNode** b);
void minHeapify(PriorityQueue* pq, int idx);
void buildMinHeap(PriorityQueue* pq);

// Funções de limpeza de memória
void freeHuffmanTree(HuffmanNode* root);
void freePriorityQueue(PriorityQueue* pq);

#endif // DATA_STRUCTURES_H
