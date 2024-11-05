// huffman.h

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 256 // Tamanho da tabela de frequências

typedef unsigned char byte;

// Estrutura para o nó da lista encadeada
typedef struct noLista
{
    byte caracter;            // Valor do byte (caractere)
    unsigned int frequencia;  // Frequência do byte
    struct noLista *esquerda; // Ponteiro para a esquerda (para árvore de Huffman)
    struct noLista *direita;  // Ponteiro para a direita (para árvore de Huffman)
    struct noLista *proximo;  // Ponteiro para o próximo nó da lista
} noLista;

// Estrutura para a lista encadeada
typedef struct
{
    noLista *inicio; // Ponteiro para o primeiro nó da lista
    int tamanho;     // Tamanho da lista
} Lista;

// Funções de manipulação da lista encadeada
void iniciar_lista(Lista *lista);
void inserir_na_lista(noLista *novo, Lista *lista);
noLista *remover_da_lista(Lista *lista);
void preencher_lista(Lista *lista, unsigned int *tabela_frequencias);
void libera_lista(Lista *lista);

// Funções para construir e manipular a árvore de Huffman
noLista *construir_arvore_huffman(Lista *lista);
void gerar_codigos_huffman(noLista *raiz, char *codigo_atual, int profundidade, char codigos[TAM][TAM]);

// Funções para codificação e decodificação
void serializar_arvore(FILE *arquivo, noLista *raiz);
noLista *desserializar_arvore(FILE *arquivo);
void codificar(char codigos[TAM][TAM], const char *nome_arquivo_comprimido, FILE *arquivo_original, noLista *raiz);
void descompactar(const char *nome_arquivo_comprimido);

#endif // HUFFMAN_H
