# Exemplos de Uso

Esta pasta contém exemplos de como usar o Compressor Huffman.

## Arquivos de Exemplo

- `teste.txt` - Arquivo de texto simples para testes
- `sample_data.txt` - Dados de exemplo para compressão

## Como Usar

```bash
# Comprimir um arquivo de exemplo
../bin/huffman_compressor -c -v teste.txt teste.huf

# Descomprimir o arquivo
../bin/huffman_compressor -d -v teste.huf teste_decompressed.txt

# Verificar se a descompressão foi bem-sucedida
diff teste.txt teste_decompressed.txt
```

## Testes Automatizados

Execute `make test` na raiz do projeto para executar testes automáticos que usam arquivos desta pasta.
