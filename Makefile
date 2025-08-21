# Makefile para o Compressor Huffman Modular
# Autor: Sistema de Compressão Huffman
# Versão: 3.0

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g -Iinclude
LDFLAGS = 

# Nome do executável
TARGET = bin/huffman_compressor

# Arquivos fonte
SOURCES = src/main.c \
          src/data_structures.c \
          src/file_io.c \
          src/huffman_algorithm.c

# Arquivos objeto
OBJECTS = $(SOURCES:.c=.o)

# Arquivos de cabeçalho
HEADERS = include/data_structures.h \
          include/file_io.h \
          include/huffman_algorithm.h

# Regra padrão
all: $(TARGET)

# Compila o executável
$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída! Executável: $(TARGET)"

# Regras para compilar arquivos objeto
src/main.o: src/main.c $(HEADERS)
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/data_structures.o: src/data_structures.c include/data_structures.h
	$(CC) $(CFLAGS) -c src/data_structures.c -o src/data_structures.o

src/file_io.o: src/file_io.c include/file_io.h include/data_structures.h
	$(CC) $(CFLAGS) -c src/file_io.c -o src/file_io.o

src/huffman_algorithm.o: src/huffman_algorithm.c include/huffman_algorithm.h include/data_structures.h include/file_io.h
	$(CC) $(CFLAGS) -c src/huffman_algorithm.c -o src/huffman_algorithm.o

# Limpa arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Arquivos de compilação removidos"

# Instala o executável (opcional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	@echo "Executável instalado em /usr/local/bin/"

# Remove a instalação
uninstall:
	rm -f /usr/local/bin/huffman_compressor
	@echo "Executável removido de /usr/local/bin/"

# Executa testes básicos
test: $(TARGET)
	@echo "Executando testes básicos..."
	@echo "Criando arquivo de teste..."
	echo "Este é um arquivo de teste para compressão Huffman. Contém texto repetitivo para demonstrar a eficiência do algoritmo." > examples/teste.txt
	@echo "Comprimindo arquivo..."
	./$(TARGET) -c -v examples/teste.txt examples/teste.huf
	@echo "Descomprimindo arquivo..."
	./$(TARGET) -d -v examples/teste.huf examples/teste_decompressed.txt
	@echo "Verificando integridade..."
	diff examples/teste.txt examples/teste_decompressed.txt && echo "✓ Teste passou!" || echo "✗ Teste falhou!"
	@echo "Limpando arquivos de teste..."
	rm -f examples/teste.txt examples/teste.huf examples/teste_decompressed.txt

# Compila e executa testes unitários
test-unit: $(TARGET)
	@echo "Compilando testes unitários..."
	$(CC) $(CFLAGS) -o tests/test_runner tests/test_huffman.c src/data_structures.c src/file_io.c src/huffman_algorithm.c
	@echo "Executando testes unitários..."
	./tests/test_runner

# Mostra ajuda
help:
	@echo "Makefile para o Compressor Huffman Modular"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make        - Compila o projeto"
	@echo "  make clean  - Remove arquivos de compilação"
	@echo "  make test   - Executa testes básicos"
	@echo "  make test-unit - Executa testes unitários"
	@echo "  make install   - Instala o executável (requer privilégios)"
	@echo "  make uninstall - Remove a instalação"
	@echo "  make help   - Mostra esta ajuda"

# Regra para verificar dependências
deps:
	@echo "Verificando dependências..."
	@which $(CC) > /dev/null || (echo "Erro: Compilador $(CC) não encontrado" && exit 1)
	@echo "✓ Compilador $(CC) encontrado"

# Regra para debug
debug: CFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Regra para release
release: CFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Regra para verificar warnings
check: CFLAGS += -Werror
check: clean $(TARGET)

.PHONY: all clean install uninstall test test-unit help deps debug release check
