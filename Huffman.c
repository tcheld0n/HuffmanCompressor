#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define TAM 256 // Tamanho da tabela de frequências

typedef unsigned char byte;

// Função para iniciar a lista encadeada
void iniciar_lista(Lista *lista)
{
    lista->inicio = NULL;
    lista->tamanho = 0;
}

// Função para inserir um nó na lista em ordem crescente de frequência
void inserir_na_lista(noLista *novo, Lista *lista)
{
    if (lista->inicio == NULL || novo->frequencia < lista->inicio->frequencia)
    {
        // Insere no início da lista se for o menor ou se a lista estiver vazia
        novo->proximo = lista->inicio;
        lista->inicio = novo;
    }
    else
    {
        // Encontra a posição correta para inserir o novo nó
        noLista *atual = lista->inicio;
        while (atual->proximo != NULL && atual->proximo->frequencia <= novo->frequencia)
        {
            atual = atual->proximo;
        }
        novo->proximo = atual->proximo;
        atual->proximo = novo;
    }
    lista->tamanho++;
}

// Função para remover o primeiro nó da lista (com menor frequência)
noLista *remover_da_lista(Lista *lista)
{
    if (lista->inicio == NULL)
        return NULL;

    // Remove o primeiro nó da lista
    noLista *remover = lista->inicio;
    lista->inicio = remover->proximo;
    lista->tamanho--;
    return remover;
}

// Função para preencher a lista com os bytes e suas frequências
void preencher_lista(Lista *lista, unsigned int *tabela_frequencias)
{
    for (int i = 0; i < TAM; i++)
    {
        if (tabela_frequencias[i] > 0)
        {
            // Cria um novo nó com o byte e sua frequência
            noLista *novoNoLista = (noLista *)calloc(1, sizeof(noLista));
            novoNoLista->caracter = i;
            novoNoLista->frequencia = tabela_frequencias[i];
            inserir_na_lista(novoNoLista, lista);
        }
    }
}

// Função para liberar a memória da lista encadeada
void libera_lista(Lista *lista)
{
    noLista *aux = lista->inicio;
    while (aux != NULL)
    {
        noLista *temp = aux->proximo;
        free(aux);
        aux = temp;
    }
}

// Função que constrói a árvore de Huffman a partir da lista encadeada
noLista *construir_arvore_huffman(Lista *lista)
{
    while (lista->tamanho > 1)
    {
        // Remove os dois nós com menor frequência
        noLista *esquerda = remover_da_lista(lista);
        noLista *direita = remover_da_lista(lista);

        // Cria um novo nó combinando as frequências dos dois nós removidos
        noLista *novoNo = (noLista *)malloc(sizeof(noLista));
        novoNo->frequencia = esquerda->frequencia + direita->frequencia;
        novoNo->caracter = NULL; // Nós internos não têm caractere associado
        novoNo->esquerda = esquerda;
        novoNo->direita = direita;
        novoNo->proximo = NULL;

        // Insere o novo nó na lista
        inserir_na_lista(novoNo, lista);
    }
    return lista->inicio; // Retorna a raiz da árvore de Huffman
}

// Função para gerar os códigos de Huffman
void gerar_codigos_huffman(noLista *raiz, char *codigo_atual, int profundidade, char codigos[TAM][TAM])
{
    if (!raiz->esquerda && !raiz->direita)
    {
        // Quando chega a um nó folha, armazena o código atual
        codigo_atual[profundidade] = '\0';
        strcpy(codigos[raiz->caracter], codigo_atual);
        return;
    }
    if (raiz->esquerda)
    {
        codigo_atual[profundidade] = '0';
        gerar_codigos_huffman(raiz->esquerda, codigo_atual, profundidade + 1, codigos);
    }
    if (raiz->direita)
    {
        codigo_atual[profundidade] = '1';
        gerar_codigos_huffman(raiz->direita, codigo_atual, profundidade + 1, codigos);
    }
}

void serializar_arvore(FILE *arquivo, noLista *raiz)
{
    if (!raiz)
        return;

    if (!raiz->esquerda && !raiz->direita)
    {
        fputc(1, arquivo);              // Marcador para nó folha
        fputc(raiz->caracter, arquivo); // Armazena o byte do caractere
    }
    else
    {
        fputc(0, arquivo); // Marcador para nó interno
        serializar_arvore(arquivo, raiz->esquerda);
        serializar_arvore(arquivo, raiz->direita);
    }
}

// Desserializar a árvore de Huffman a partir do cabeçalho
noLista *desserializar_arvore(FILE *arquivo)
{
    int tipo = fgetc(arquivo);
    if (tipo == 1) // Nó folha
    {
        noLista *folha = (noLista *)malloc(sizeof(noLista));
        folha->caracter = fgetc(arquivo);
        folha->esquerda = folha->direita = NULL;
        return folha;
    }
    else if (tipo == 0) // Nó interno
    {
        noLista *interno = (noLista *)malloc(sizeof(noLista));
        interno->caracter = 0; // Nós internos não têm caractere
        interno->esquerda = desserializar_arvore(arquivo);
        interno->direita = desserializar_arvore(arquivo);
        return interno;
    }
    return NULL;
}

// Função para codificar o arquivo com o cabeçalho da árvore
void codificar(char codigos[TAM][TAM], const char *nome_arquivo_comprimido, FILE *arquivo_original, noLista *raiz)
{
    FILE *arquivo_comprimido = fopen(nome_arquivo_comprimido, "wb");
    if (arquivo_comprimido == NULL)
    {
        printf("Erro ao criar o arquivo compactado.\n");
        return;
    }

    // Serializar a árvore no cabeçalho do arquivo
    serializar_arvore(arquivo_comprimido, raiz);

    // Gerar códigos de Huffman para cada byte
    char codigo_atual[TAM] = {0};
    gerar_codigos_huffman(raiz, codigo_atual, 0, codigos);

    rewind(arquivo_original); // Retorna ao início do arquivo
    unsigned char bit_buffer = 0;
    int bit_count = 0;
    unsigned char buffer[2048];
    size_t bytes_lidos;

    // Ler o arquivo original e substituir cada byte pelo código correspondente
    while ((bytes_lidos = fread(buffer, 1, sizeof(buffer), arquivo_original)) > 0)
    {
        for (size_t i = 0; i < bytes_lidos; i++)
        {
            char *codigo = codigos[buffer[i]];
            for (int j = 0; codigo[j] != '\0'; j++)
            {
                bit_buffer = (bit_buffer << 1) | (codigo[j] - '0');
                bit_count++;
                if (bit_count == 8)
                {
                    fputc(bit_buffer, arquivo_comprimido);
                    bit_buffer = 0;
                    bit_count = 0;
                }
            }
        }
    }
    if (bit_count > 0)
    {
        bit_buffer <<= (8 - bit_count); // Preenche os bits restantes
        fputc(bit_buffer, arquivo_comprimido);
    }
    fclose(arquivo_comprimido);
}

// Função para descompactar o arquivo usando o cabeçalho da árvore
void descompactar(const char *nome_arquivo_comprimido)
{
    FILE *arquivo_comprimido = fopen(nome_arquivo_comprimido, "rb");
    FILE *arquivo_descompactado = fopen("descompactado.img", "wb");

    if (!arquivo_comprimido || !arquivo_descompactado)
    {
        printf("Erro ao abrir o arquivo compactado ou descompactado.\n");
        return;
    }

    // Reconstruir a árvore a partir do cabeçalho
    noLista *arvore = desserializar_arvore(arquivo_comprimido);

    unsigned char byte;
    noLista *atual = arvore;

    // Ler o arquivo comprimido bit a bit e navegar na árvore de Huffman para descompactar
    while (fread(&byte, 1, 1, arquivo_comprimido) == 1)
    {
        for (int bit = 7; bit >= 0; bit--)
        {
            atual = (byte & (1 << bit)) ? atual->direita : atual->esquerda;
            if (!atual->esquerda && !atual->direita)
            {
                fputc(atual->caracter, arquivo_descompactado);
                atual = arvore;
            }
        }
    }

    fclose(arquivo_comprimido);
    fclose(arquivo_descompactado);
}
