#ifndef REGISTRY_H_INCLUDED
#define REGISTRY_H_INCLUDED

#include <stdio.h>

typedef struct Registry {
  int id;
  char name[51];
  char cpf[12];
  char code[31];
  char addres[101];
  char phone[16];
  int pos;
} Registry;

/// estrura de cabeçalho de arquivo de registros
typedef struct RegistryHeader {
  int topPos;
} RegistryHeader;

/**
 * função para inicializar arquivo de registros
 * @param f arquivo de registros a ser inicializado
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
void createEmptyRegistryFile(FILE *f);

/**
 * função para criação de registro
 * @return registro alocado e inicializado
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
Registry *createRegistry(int id, char *name, char *cpf, char *code, char *addres, char *phone);

/**
 * função para escrita de cabeçalgo em arquivo de registros
 * @param f arquivo a ser escrito
 * @param header cabeçalho a ser escrito no arquivo
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
void writeRegistryHeader(FILE *f, RegistryHeader *header);

/**
 * função para escrita de registro em arquivo
 * @param f arquivo de registros
 * @param header cabeçalho do arquivo
 * @param entry registro a ser escrito
 * @return posição de escrita do registro
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
int writeRegistryToFile(FILE *f, RegistryHeader *header, Registry *registry);

/**
 * lê cabeçalho do arquivo passado
 * @param f arquivo de registros
 * @return cabeçalho do arquivo
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
RegistryHeader *readRegistryHeader(FILE *f);

/**
 * função para leitura de registro a partir de posição dada
 * @param f arquivo de registros
 * @param pos posição para leitura
 * @return registro lido do arquivo
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
Registry *readRegistryFromFile(FILE *f, int pos);

/**
 * função para atualizacao de registro em arquivo
 * @param f arquivo de registros
 * @param reg registro a ser escrito
 * @param addres endereco a ser alterado
 * @param phone telefone a ser alterado
 * @precondition arquivo tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
void updateRegistry(FILE *f, Registry *reg, char *addres, char *phone);

void printRegistryHeader(RegistryHeader *header);

#endif
