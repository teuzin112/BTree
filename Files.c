#include "Files.h"

#include <stdlib.h>

/*!
 * função auxiliar para remocao de caracteres em branco do lado esquerdo de uma string
 * @param s string a ser processada
 * @return s string processada
 * @precondition string nao nula
 * @postcondition nenhuma
 */
char *ltrim(char *s) {
  while (isspace(*s)) s++;
  return s;
}

/*!
 * função auxiliar para remocao de caracteres em branco do lado direito de uma string
 * @param s string a ser processada
 * @return s string processada
 * @precondition string nao nula
 * @postcondition nenhuma
 */
char *rtrim(char *s) {
  char *back = s + strlen(s);
  while (isspace(*--back))
    ;
  *(back + 1) = '\0';
  return s;
}

/*!
 * função auxiliar para remocao de caracteres em branco do lado esquerdo e direito de uma string
 * @param s string a ser processada
 * @return s string processada
 * @precondition string nao nula
 * @postcondition nenhuma
 */
char *trim(char *s) {
  if (s == NULL) return s;
  return rtrim(ltrim(s));
}

/*!
 * função para verificar se um string contem apenas caracteres numericos
 * @param s string a verifica
 * @return 1 para verdadeiro e 0 para falso
 * @precondition string nao nula
 * @postcondition nenhuma
 */
int isNumber(char *s) {
  while (*s != '\0') {
    if (!isdigit(*s)) return 0;
    s++;
  }
  return 1;
}

/**
 * função para abertura do arquivo de dados
 * @param registriesF arquivo de dados
 * @precondition nenhuma
 * @postcondition nenhuma
 */
FILE *openRegFile(FILE *registriesF) {
  // Abre arquivo de registros se ja existe
  registriesF = fopen("data", "r+b");
  if (registriesF == NULL) {  // Se nao existe cria na hora
    registriesF = fopen("data", "w+b");
    createEmptyRegistryFile(registriesF);
  }
  return registriesF;
}

/**
 * função para abertura do arquivo de indices
 * @param bTreeF arquivo de indices
 * @precondition nenhuma
 * @postcondition nenhuma
 */
FILE *openIndexFile(FILE *bTreeF) {
  // Abre arquivo de registros se ja existe
  bTreeF = fopen("data", "r+b");
  if (bTreeF == NULL) {  // Se nao existe cria na hora
    bTreeF = fopen("data", "w+b");
    createEmptyBTree(bTreeF);
  }
  return bTreeF;
}

/**
 * função para leitura de arquivo texto com operacoes
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void readTxtFile(FILE *registriesF, FILE *btreeF) {
  char txt[100];
  printf("Entre com o caminho do arquivo: ");
  gets(txt);
  FILE *f = fopen(txt, "r");

  while (!feof(f)) {
    char *op, *idAux, *name, *cpf, *code, *addres, *phone;
    int id;

    char str[255];
    fgets(str, 256, f);
    // printf("%s", str);

    // le a operacao
    char *token = strtok(str, ";");
    op = token;
    op = trim(op);
    // printf("%s\n", op);

    // le a chave
    token = strtok(NULL, ";");
    idAux = token;
    id = atoi(trim(idAux));
    // printf("%d\n", id);

    Registry *reg;
    if (!strcmp(op, "I")) {
      // le todos os dados e armazena en variaveis auxiliares
      token = strtok(NULL, ";");
      name = token;
      name = trim(name);
      // printf("%s\n", name);

      token = strtok(NULL, ";");
      cpf = token;
      cpf = trim(cpf);
      // printf("%s\n", cpf);

      token = strtok(NULL, ";");
      code = token;
      code = trim(code);
      // printf("%s\n", code);

      token = strtok(NULL, ";");
      addres = token;
      addres = trim(addres);
      // printf("%s\n", addres);

      token = strtok(NULL, ";");
      phone = token;
      phone = trim(phone);
      // printf("%s\n", phone);

      // caso registro ja exista ignora a insercao
      if (!searchRegistryBTree(btreeF, registriesF, id)) {
        // cria registro e insere na arvore B
        reg = createRegistry(id, name, cpf, code, addres, phone);
        RegistryHeader *regHeader = readRegistryHeader(registriesF);

        int pos = writeRegistryToFile(registriesF, regHeader, reg);
        insertBTree(btreeF, id, pos);
        free(regHeader);
      } else
        ;
    }

    else if (!strcmp(op, "A")) {
      token = strtok(NULL, ";");
      addres = token;
      addres = trim(addres);

      // altera apenas o telefone
      if (isNumber(addres)) {
        phone = addres;
        addres = NULL;
      }

      // altera endereco e telefone
      else {
        token = strtok(NULL, ";");
        phone = token;
        phone = trim(phone);
      }

      // procura o registro se existe, e o altera apenas no arquivo de registros
      if ((reg = searchRegistryBTree(btreeF, registriesF, id)))
        updateRegistry(registriesF, reg, addres, phone);
    }
    // free(reg);
  }
  fclose(f);
}
/**
 * função para imprimir todas as informacoes de um profissional
 * @param id codigo do profissional
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void printInfo(int id, FILE *registriesF, FILE *btreeF) {
  Registry *reg = searchRegistryBTree(btreeF, registriesF, id);
  if (reg != NULL) {
    printf("\n------------------------------------------------------------\n");
    printf("Codigo: %d\n", reg->id);
    printf("Nome: %s\n", reg->name);
    printf("CPF: %s\n", reg->cpf);
    printf("Registro profissional: %s\n", reg->code);
    printf("Endereco: %s\n", reg->addres);
    printf("Telefone: %s\n", reg->phone);
    printf("------------------------------------------------------------\n");
  }

  free(reg);
}

/**
 * função recursiva para listar todos os dados dos profissionais em ordem crescente de codigo
 * @param node raiz da arvore B
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void listInfo(BTreeNode *node, FILE *registriesF, FILE *btreeF) {
  // imprime in ordem
  if (node != NULL) {
    for (int i = 0; i < node->nKeys; i++) {
      // imprime no da "esquerda"
      listInfo(readBTreeNode(btreeF, node->children[i]), registriesF, btreeF);
      // imprime a chave atual
      printInfo(node->keys[i].id, registriesF, btreeF);

      // se for a ultima chave imprime o no da "direita"
      if (i == node->nKeys - 1)
        listInfo(readBTreeNode(btreeF, node->children[i + 1]), registriesF, btreeF);
    }
  }
}

/**
 * função para inserir um profissional manualmente
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void insert(FILE *registriesF, FILE *btreeF) {
  int id;
  char name[51], cpf[12], code[31], addres[101], phone[16];

  printf("Codigo: ");
  scanf("%d", &id);
  getchar();

  printf("\nNome: ");
  gets(name);

  printf("\nCPF: ");
  gets(cpf);

  printf("\nRegistro profissional: ");
  gets(code);

  printf("\nEndereco: ");
  gets(addres);

  printf("\nTelefone: ");
  gets(phone);

  // caso registro ja exista ignora a insercao
  if (!searchRegistryBTree(btreeF, registriesF, id)) {
    // cria registro e le o cabecalho do arquivo de dados
    Registry *reg = createRegistry(id, name, cpf, code, addres, phone);
    RegistryHeader *regHeader = readRegistryHeader(registriesF);

    // insere registro no arquivo de dados e de indices
    int pos = writeRegistryToFile(registriesF, regHeader, reg);
    insertBTree(btreeF, id, pos);
    free(reg);
    free(regHeader);
  } else
    printf("Codigo ja registrado!\n");
}

/**
 * função para alterar endereco de um profissional
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void updateAddres(FILE *registriesF, FILE *btreeF) {
  int id;
  char addres[100];
  printf("Codigo: ");
  scanf("%d", &id);
  getchar();

  printf("Novo endereco: ");
  gets(addres);

  Registry *reg;
  if ((reg = searchRegistryBTree(btreeF, registriesF, id)))
    updateRegistry(registriesF, reg, addres, NULL);
  else
    printf("Registro nao encontrado!\n");
  free(reg);
}

/**
 * função para alterar telefone de um profissional
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void updatePhone(FILE *registriesF, FILE *btreeF) {
  int id;
  char phone[100];
  printf("Codigo: ");
  scanf("%d", &id);
  getchar();

  printf("Novo telefone: ");
  gets(phone);

  Registry *reg;
  if ((reg = searchRegistryBTree(btreeF, registriesF, id)))
    updateRegistry(registriesF, reg, NULL, phone);

  else
    printf("Registro nao encontrado!\n");
  free(reg);
}
