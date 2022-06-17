#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <ctype.h>
#include <string.h>

#include "BTree.h"
#include "Registry.h"

/**
 * função auxiliar para remocao de caracteres em branco do lado esquerdo de uma string
 * @param s string a ser processada
 * @return s string processada
 * @precondition string nao nula
 * @postcondition nenhuma
 */
char *ltrim(char *s);

/**
 * função auxiliar para remocao de caracteres em branco do lado direito de uma string
 * @param s string a ser processada
 * @return s string processada
 * @precondition string nao nula
 * @postcondition nenhuma
 */
char *rtrim(char *s);

/**
 * função auxiliar para remocao de caracteres em branco do lado esquerdo e direito de uma string
 * @param s string a ser processada
 * @return s string processada
 * @precondition string nao nula
 * @postcondition nenhuma
 */
char *trim(char *s);

/**
 * função para verificar se um string contem apenas caracteres numericos
 * @param s string a verifica
 * @return 1 para verdadeiro e 0 para falso
 * @precondition string nao nula
 * @postcondition nenhuma
 */
int isNumber(char *s);

/**
 * função para abertura do arquivo de dados
 * @param registriesF arquivo de dados
 * @precondition nenhuma
 * @postcondition nenhuma
 */
FILE *openRegFile(FILE *registriesF);

/**
 * função para abertura do arquivo de indices
 * @param bTreeF arquivo de indices
 * @precondition nenhuma
 * @postcondition nenhuma
 */
FILE *openIndexFile(FILE *bTreeF);

/**
 * função para leitura de arquivo texto com operacoes
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void readTxtFile(FILE *registriesF, FILE *btreeF);

/**
 * função para imprimir todas as informacoes de um profissional
 * @param id codigo do profissional
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void printInfo(int id, FILE *registriesF, FILE *btreeF);

/**
 * função recursiva para listar todos os dados dos profissionais em ordem crescente de codigo
 * @param node raiz da arvore B
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void listInfo(BTreeNode *node, FILE *registriesF, FILE *btreeF);

/**
 * função para inserir um profissional manualmente
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void insert(FILE *registriesF, FILE *btreeF);

/**
 * função para alterar endereco de um profissional
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void updateAddres(FILE *registriesF, FILE *btreeF);

/**
 * função para alterar telefone de um profissional
 * @param registriesF arquivo de dados
 * @param btreeF arquivo de indices
 * @precondition arquivos nao nulos
 * @postcondition nenhuma
 */
void updatePhone(FILE *registriesF, FILE *btreeF);

#endif