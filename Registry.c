#include "Registry.h"

#include <stdlib.h>
#include <string.h>
//#include <unistd.h>

/**
 * função para inicializar arquivo de registros
 * @param f arquivo de registros a ser inicializado
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
void createEmptyRegistryFile(FILE *f) {
  /// inicializa cabeçalho
  RegistryHeader header;
  header.topPos = 0;

  /// escreve novo cabeçalho no arquivo
  writeRegistryHeader(f, &header);
}

/**
 * função para criação de registro
 * @return registro alocado e inicializado
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
Registry *createRegistry(int id, char *name, char *cpf, char *code, char *addres, char *phone) {
  /// aloca memória para novo registro
  Registry *newReg = (Registry *)malloc(sizeof(Registry));

  /// inicializa campos do registro
  newReg->id = id;
  strcpy(newReg->name, name);
  strcpy(newReg->cpf, cpf);
  strcpy(newReg->code, code);
  strcpy(newReg->addres, addres);
  strcpy(newReg->phone, phone);
  newReg->pos = -1;

  return newReg;
}

/**
 * função para escrita de cabeçalho em arquivo de registros
 * @param f arquivo a ser escrito
 * @param header cabeçalho a ser escrito no arquivo
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
void writeRegistryHeader(FILE *f, RegistryHeader *header) {
  /// vai para inicio do arquivo
  fseek(f, 0, SEEK_SET);

  /// escreve novo cabeçalho
  fwrite(header, sizeof(RegistryHeader), 1, f);
}

/**
 * função para escrita de registro em arquivo
 * @param f arquivo de registros
 * @param header cabeçalho do arquivo
 * @param reg registro a ser escrito
 * @return posição de escrita do registro
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
int writeRegistryToFile(FILE *f, RegistryHeader *header, Registry *reg) {
  int pos;

  /// se posição do registro é invalida
  if ((pos = reg->pos) == -1) {
    /// atribui posição para final do arquivo
    pos = header->topPos++;
    /// escreve cabeçalho no arquivo
    writeRegistryHeader(f, header);
  }

  /// atualiza posição no registro
  reg->pos = pos;

  /// vai para posição calculada
  fseek(f, sizeof(RegistryHeader) + sizeof(Registry) * pos, SEEK_SET);

  /// escreve registro
  fwrite(reg, sizeof(Registry), 1, f);

  return pos;
}

/**
 * lê cabeçalho do arquivo passado
 * @param f arquivo de registros
 * @return cabeçalho do arquivo
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
RegistryHeader *readRegistryHeader(FILE *f) {
  /// aloca memória para cabeçalho a ser lida
  RegistryHeader *header = (RegistryHeader *)malloc(sizeof(RegistryHeader));

  /// vai para o inicio do arquivo
  fseek(f, 0, SEEK_SET);

  /// lê cabeçalho
  fread(header, sizeof(RegistryHeader), 1, f);

  return header;
}

/**
 * função para leitura de registro a partir de posição dada
 * @param f arquivo de registros
 * @param pos posição para leitura
 * @return registro lido do arquivo
 * @precondition arquivos tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
Registry *readRegistryFromFile(FILE *f, int pos) {
  /// aloca memória para registro a ser lido
  Registry *reg = (Registry *)malloc(sizeof(Registry));

  /// se posição a ser lida é invalida retorna NULL
  if (pos == -1) return NULL;

  /// vai para posição calculada
  fseek(f, sizeof(RegistryHeader) + sizeof(Registry) * pos, SEEK_SET);

  /// lê registro do arquivo
  fread(reg, sizeof(Registry), 1, f);

  return reg;
}

/**
 * função para atualizacao de registro em arquivo
 * @param f arquivo de registros
 * @param reg registro a ser escrito
 * @param addres endereco a ser alterado
 * @param phone telefone a ser alterado
 * @precondition arquivo tem que estar abertos e ter direito de leitura
 * @postcondition nenhuma
 */
void updateRegistry(FILE *f, Registry *reg, char *addres, char *phone) {
  /// atualiza dados
  if (addres != NULL)
    strcpy(reg->addres, addres);
  if (phone != NULL)
    strcpy(reg->phone, phone);

  /// vai para posição calculada
  fseek(f, sizeof(RegistryHeader) + sizeof(Registry) * reg->pos, SEEK_SET);

  /// reescreve registro
  fwrite(reg, sizeof(Registry), 1, f);
}

void printRegistryHeader(RegistryHeader *header) {
  printf("-----------------\n");
  printf("topPos = %d\n", header->topPos);
  printf("-----------------\n");
}