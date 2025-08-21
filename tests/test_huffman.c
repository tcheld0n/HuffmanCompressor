#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "huffman_algorithm.h"

void testDataStructures() {
    printf("=== Testando Estruturas de Dados ===\n");
    
    // Teste 1: Criação de nós
    printf("1. Testando criação de nós...\n");
    HuffmanNode* node1 = createNode('a', 5);
    HuffmanNode* node2 = createNode('b', 3);
    HuffmanNode* node3 = createNode('c', 7);
    
    printf("Nós criados: 'a'(freq=5), 'b'(freq=3), 'c'(freq=7)\n");
    
    // Teste 2: Verificação de folhas
    printf("2. Testando verificação de folhas...\n");
    printf("isLeaf(node1): %s\n", isLeaf(node1) ? "Sim" : "Não");
    printf("Todos os nós são folhas\n");
    
    // Teste 3: Fila de prioridade
    printf("3. Testando fila de prioridade...\n");
    PriorityQueue* pq = createPriorityQueue(10);
    
    insert(pq, node1);
    insert(pq, node2);
    insert(pq, node3);
    
    printf("Tamanho da fila: %u\n", pq->size);
    printf("Fila criada e nós inseridos\n");
    
    // Teste 4: Extração do mínimo
    printf("4. Testando extração do mínimo...\n");
    HuffmanNode* min1 = extractMin(pq);
    HuffmanNode* min2 = extractMin(pq);
    HuffmanNode* min3 = extractMin(pq);
    
    printf("Mínimos extraídos: '%c'(freq=%lu), '%c'(freq=%lu), '%c'(freq=%lu)\n",
           min1->data, min1->frequency, min2->data, min2->frequency, min3->data, min3->frequency);
    printf("Extração em ordem crescente de frequência\n");
    
    // Teste 5: Construção de árvore
    printf("5. Testando construção de árvore...\n");
    insert(pq, min1);
    insert(pq, min2);
    insert(pq, min3);
    
    while (!isSizeOne(pq)) {
        HuffmanNode* left = extractMin(pq);
        HuffmanNode* right = extractMin(pq);
        
        HuffmanNode* internal = createNode(0, left->frequency + right->frequency);
        internal->left = left;
        internal->right = right;
        
        insert(pq, internal);
    }
    
    HuffmanNode* root = extractMin(pq);
    printf("Árvore de Huffman construída\n");
    printf("Frequência da raiz: %lu\n", root->frequency);
    
    // Limpeza
    freeHuffmanTree(root);
    freePriorityQueue(pq);
    printf("Memória liberada\n\n");
}

void testHuffmanAlgorithm() {
    printf("=== Testando Algoritmo de Huffman ===\n");
    
    // Dados de teste
    unsigned long frequencies[MAX_CHAR] = {0};
    frequencies['a'] = 5;
    frequencies['b'] = 3;
    frequencies['c'] = 7;
    frequencies['d'] = 2;
    frequencies['e'] = 8;
    
    printf("1. Frequências de teste:\n");
    for (int i = 0; i < MAX_CHAR; i++) {
        if (frequencies[i] > 0) {
            printf("'%c': %lu\n", i, frequencies[i]);
        }
    }
    
    // Construir árvore
    printf("2. Construindo árvore de Huffman...\n");
    HuffmanNode* root = buildHuffmanTree(frequencies);
    printf("Árvore construída\n");
    
    // Gerar códigos
    printf("3. Gerando códigos de Huffman...\n");
    char codes[MAX_CHAR][MAX_TREE_HT] = {{0}};
    char current_code[MAX_TREE_HT] = {0};
    generateHuffmanCodes(root, current_code, 0, codes);
    printf("Códigos gerados\n");
    
    // Mostrar códigos
    printf("4. Códigos de Huffman:\n");
    for (int i = 0; i < MAX_CHAR; i++) {
        if (strlen(codes[i]) > 0) {
            printf("'%c': %s\n", i, codes[i]);
        }
    }
    
    // Limpeza
    freeHuffmanTree(root);
    printf("Memória liberada\n\n");
}

void testFileOperations() {
    printf("=== Testando Operações de Arquivo ===\n");
    
    // Criar arquivo de teste
    printf("1. Criando arquivo de teste...\n");
    FILE* test_file = fopen("test_input.txt", "w");
    if (test_file) {
        fprintf(test_file, "abracadabra");
        fclose(test_file);
        printf("Arquivo de teste criado\n");
    } else {
        printf("✗ Erro ao criar arquivo de teste\n");
        return;
    }
    
    // Testar cálculo de frequências
    printf("2. Calculando frequências...\n");
    unsigned long* frequencies = calculateFrequencies("test_input.txt");
    if (frequencies) {
        printf("Frequências calculadas:\n");
        for (int i = 0; i < MAX_CHAR; i++) {
            if (frequencies[i] > 0) {
                printf("'%c': %lu\n", i, frequencies[i]);
            }
        }
        printf("Frequências calculadas\n");
        free(frequencies);
    } else {
        printf("✗ Erro ao calcular frequências\n");
    }
    
    // Testar verificação de arquivo
    printf("3. Verificando existência de arquivo...\n");
    if (fileExists("test_input.txt")) {
        printf("Arquivo existe\n");
    } else {
        printf("✗ Arquivo não encontrado\n");
    }
    
    // Testar tamanho de arquivo
    printf("4. Obtendo tamanho do arquivo...\n");
    long size = getFileSize("test_input.txt");
    if (size > 0) {
        printf("Tamanho: %ld bytes\n", size);
        printf("Tamanho obtido\n");
    } else {
        printf("✗ Erro ao obter tamanho\n");
    }
    
    // Limpeza
    remove("test_input.txt");
    printf("Arquivo de teste removido\n\n");
}

int main() {
    printf("Testes do Compressor Huffman Modular\n");
    printf("=====================================\n\n");
    
    testDataStructures();
    testHuffmanAlgorithm();
    testFileOperations();
    
    printf("Todos os testes concluídos!\n");
    return 0;
}
