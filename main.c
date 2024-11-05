#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "huffman.h"

#define TAM 256

int main()
{
    clock_t inicio, fim;
    double tempo_total;

    unsigned int tabela_frequencias[TAM] = {0};
    unsigned char buffer[2048];
    FILE *arquivo_original = fopen("img.jpg", "rb");
    size_t bytes_lidos;

    if (!arquivo_original)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Ler o arquivo e calcular a frequência de cada byte
    while ((bytes_lidos = fread(buffer, 1, sizeof(buffer), arquivo_original)) > 0)
    {
        for (size_t i = 0; i < bytes_lidos; i++)
        {
            tabela_frequencias[buffer[i]]++;
        }
    }
    fclose(arquivo_original);

    Lista lista;
    iniciar_lista(&lista);
    preencher_lista(&lista, tabela_frequencias);

    // Construir a árvore de Huffman
    noLista *arvore_huffman = construir_arvore_huffman(&lista);

    // Codificar o arquivo
    char codigos[TAM][TAM] = {{0}};
    const char *nome_arquivo_comprimido = "compressed.huf";

    arquivo_original = fopen("img.jpg", "rb");
    codificar(codigos, nome_arquivo_comprimido, arquivo_original, arvore_huffman);
    fclose(arquivo_original);

    // Descompactar o arquivo
    descompactar(nome_arquivo_comprimido);
    libera_lista(&lista);

    return 0;
}