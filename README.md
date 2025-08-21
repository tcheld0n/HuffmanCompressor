# Compressor Huffman

Um sistema de compressão e descompressão de arquivos baseado no algoritmo de codificação de Huffman. Este projeto implementa compressão sem perdas, garantindo que os dados originais sejam perfeitamente recuperados após a descompressão.

## 🎯 Funcionamento

O Compressor Huffman funciona através dos seguintes passos:

1. **Análise de Frequência**: Conta a ocorrência de cada byte no arquivo de entrada
2. **Construção da Árvore**: Cria uma árvore binária baseada nas frequências dos caracteres
3. **Geração de Códigos**: Atribui códigos binários mais curtos aos caracteres mais frequentes
4. **Compressão**: Substitui cada caractere pelo seu código Huffman correspondente
5. **Armazenamento**: Salva a árvore de Huffman junto com os dados comprimidos

### Comportamento Esperado

- **Compressão**: Reduz o tamanho do arquivo mantendo todos os dados originais
- **Descompressão**: Recupera exatamente o arquivo original
- **Validação**: Verifica automaticamente a integridade dos dados
- **Estatísticas**: Fornece informações detalhadas sobre a compressão

## 🏗️ Estrutura do Projeto

```
HuffmanCompressor/
├── src/                    # Código fonte
│   ├── main.c             # Programa principal
│   ├── data_structures.c  # Implementação das estruturas de dados
│   ├── file_io.c          # Operações de entrada/saída
│   └── huffman_algorithm.c # Algoritmo de Huffman
├── include/               # Arquivos de cabeçalho
│   ├── data_structures.h  # Definições das estruturas
│   ├── file_io.h          # Interface de I/O
│   └── huffman_algorithm.h # Interface do algoritmo
├── bin/                   # Executáveis compilados
├── tests/                 # Testes unitários
│   └── test_huffman.c     # Testes automatizados
├── examples/              # Exemplos de uso
├── docs/                  # Documentação adicional
├── Makefile               # Sistema de build
└── README.md              # Este arquivo
```

## 🚀 Compilação e Uso

### Pré-requisitos
- Compilador GCC (ou compatível)
- Sistema Unix/Linux/Windows com suporte a C99

### Compilação
```bash
# Compilar o projeto
make

# Compilar com otimizações
make release

# Compilar para debug
make debug
```

### Uso Básico
```bash
# Comprimir um arquivo
./bin/huffman_compressor -c arquivo.txt arquivo.huf

# Descomprimir um arquivo
./bin/huffman_compressor -d arquivo.huf arquivo_descomprimido.txt

# Modo verboso (mostra estatísticas)
./bin/huffman_compressor -c -v imagem.jpg imagem.huf
```

### Opções Disponíveis
- `-c, --compress` - Comprime o arquivo de entrada
- `-d, --decompress` - Descomprime o arquivo de entrada
- `-v, --verbose` - Modo verboso com estatísticas detalhadas
- `-h, --help` - Mostra a mensagem de ajuda

## 🧪 Testes

### Testes Básicos
```bash
# Executar testes de integração
make test
```

### Testes Unitários
```bash
# Executar testes unitários
make test-unit
```

### Validação Automática
O sistema inclui validação automática que verifica se a descompressão reproduz exatamente o arquivo original.

## 📊 Características Técnicas

### Estruturas de Dados
- **Árvore de Huffman**: Implementada com nós dinâmicos
- **Fila de Prioridade**: Min-heap para construção eficiente da árvore
- **Buffer de Bits**: Otimização para manipulação de bits

### Algoritmo
- **Análise de Frequência**: Contagem de caracteres únicos
- **Construção da Árvore**: Algoritmo de Huffman clássico
- **Codificação**: Geração de códigos prefix-free
- **Compressão**: Substituição de caracteres por códigos binários

### Otimizações
- **Buffer de Leitura**: Processamento em chunks para arquivos grandes
- **Manipulação de Bits**: Operações eficientes de bit-level
- **Gestão de Memória**: Alocação e liberação cuidadosa

## 📈 Performance

O algoritmo de Huffman oferece:
- **Compressão sem perdas**: Recuperação perfeita dos dados originais
- **Eficiência**: O(log n) para construção da árvore
- **Flexibilidade**: Funciona com qualquer tipo de arquivo
- **Escalabilidade**: Processa arquivos de qualquer tamanho

## 🔧 Comandos Makefile

```bash
make              # Compila o projeto
make clean        # Remove arquivos de compilação
make test         # Executa testes básicos
make test-unit    # Executa testes unitários
make debug        # Compila com flags de debug
make release      # Compila com otimizações
make check        # Verifica warnings
make help         # Mostra ajuda
```

## 📝 Exemplos de Saída

### Compressão Verbosa
```
Arquivo de entrada: documento.txt (1024 bytes)
Arquivo de saída: documento.huf
Operação: Compressão
Iniciando...
Comprimindo 'documento.txt' para 'documento.huf'...
Compressão concluída com sucesso!
Taxa de compressão: 45.2%
Tempo de execução: 0.023 segundos
```

### Descompressão com Validação
```
Descomprimindo 'documento.huf' para 'documento_descomprimido.txt'...
Descompressão concluída com sucesso!
✓ Validação: Arquivo descomprimido é idêntico ao original
Tempo de execução: 0.018 segundos
```

## 🤝 Contribuição

Para contribuir com o projeto:
1. Mantenha a modularidade
2. Adicione testes para novas funcionalidades
3. Documente as mudanças
4. Siga os padrões de código estabelecidos

## 📄 Licença

Este projeto é de código aberto e está disponível sob a licença MIT.
