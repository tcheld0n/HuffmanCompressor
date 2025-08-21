#include "data_structures.h"

/**
 * Cria um novo nó da árvore de Huffman
 * @param data Caractere (byte) a ser armazenado
 * @param freq Frequência do caractere
 * @return Ponteiro para o novo nó criado
 */
HuffmanNode* createNode(unsigned char data, unsigned long freq) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (node == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o nó\n");
        exit(EXIT_FAILURE);
    }
    
    node->data = data;
    node->frequency = freq;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

/**
 * Cria uma nova fila de prioridade (min-heap)
 * @param capacity Capacidade máxima da fila
 * @return Ponteiro para a fila de prioridade criada
 */
PriorityQueue* createPriorityQueue(unsigned capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (pq == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para a fila de prioridade\n");
        exit(EXIT_FAILURE);
    }
    
    pq->size = 0;
    pq->capacity = capacity;
    pq->array = (HuffmanNode**)malloc(capacity * sizeof(HuffmanNode*));
    
    if (pq->array == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o array da fila\n");
        free(pq);
        exit(EXIT_FAILURE);
    }
    
    return pq;
}

/**
 * Troca dois nós na fila de prioridade
 * @param a Primeiro nó
 * @param b Segundo nó
 */
void swapNodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Função auxiliar para manter a propriedade do min-heap
 * @param pq Fila de prioridade
 * @param idx Índice do nó a ser verificado
 */
void minHeapify(PriorityQueue* pq, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    // Verifica se o filho esquerdo é menor que o pai
    if ((unsigned int)left < pq->size && 
        pq->array[left]->frequency < pq->array[smallest]->frequency) {
        smallest = left;
    }
    
    // Verifica se o filho direito é menor que o pai ou o filho esquerdo
    if ((unsigned int)right < pq->size && 
        pq->array[right]->frequency < pq->array[smallest]->frequency) {
        smallest = right;
    }
    
    // Se o menor não é o pai, troca e continua heapificando
    if (smallest != idx) {
        swapNodes(&pq->array[smallest], &pq->array[idx]);
        minHeapify(pq, smallest);
    }
}

/**
 * Constrói um min-heap a partir de um array
 * @param pq Fila de prioridade
 */
void buildMinHeap(PriorityQueue* pq) {
    int n = pq->size - 1;
    for (int i = (n - 1) / 2; i >= 0; i--) {
        minHeapify(pq, i);
    }
}

/**
 * Insere um novo nó na fila de prioridade
 * @param pq Fila de prioridade
 * @param node Nó a ser inserido
 */
void insert(PriorityQueue* pq, HuffmanNode* node) {
    if (pq->size >= pq->capacity) {
        fprintf(stderr, "Erro: Fila de prioridade cheia\n");
        return;
    }
    
    // Insere o novo nó no final
    int i = pq->size;
    pq->array[i] = node;
    pq->size++;
    
    // Ajusta a posição do nó inserido para manter a propriedade do min-heap
    while (i != 0 && 
           pq->array[(i - 1) / 2]->frequency > pq->array[i]->frequency) {
        swapNodes(&pq->array[i], &pq->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

/**
 * Remove e retorna o nó com menor frequência da fila
 * @param pq Fila de prioridade
 * @return Nó com menor frequência ou NULL se a fila estiver vazia
 */
HuffmanNode* extractMin(PriorityQueue* pq) {
    if (pq->size == 0) {
        return NULL;
    }
    
    // Remove o nó raiz (menor frequência)
    HuffmanNode* min = pq->array[0];
    pq->array[0] = pq->array[pq->size - 1];
    pq->size--;
    
    // Reajusta o heap se necessário
    if (pq->size > 0) {
        minHeapify(pq, 0);
    }
    
    return min;
}

/**
 * Verifica se a fila de prioridade contém apenas um elemento
 * @param pq Fila de prioridade
 * @return 1 se contém apenas um elemento, 0 caso contrário
 */
int isSizeOne(PriorityQueue* pq) {
    return (pq->size == 1);
}

/**
 * Verifica se um nó é uma folha (não tem filhos)
 * @param node Nó a ser verificado
 * @return 1 se é folha, 0 caso contrário
 */
int isLeaf(HuffmanNode* node) {
    return (node->left == NULL && node->right == NULL);
}

/**
 * Libera a memória de uma árvore de Huffman
 * @param root Raiz da árvore
 */
void freeHuffmanTree(HuffmanNode* root) {
    if (root == NULL) {
        return;
    }
    
    // Libera recursivamente os filhos
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    
    // Libera o nó atual
    free(root);
}

/**
 * Libera a memória de uma fila de prioridade
 * @param pq Fila de prioridade
 */
void freePriorityQueue(PriorityQueue* pq) {
    if (pq == NULL) {
        return;
    }
    
    // Libera o array de ponteiros (não os nós em si)
    if (pq->array != NULL) {
        free(pq->array);
    }
    
    // Libera a estrutura da fila
    free(pq);
}
