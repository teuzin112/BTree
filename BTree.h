#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdio.h>

#include "Queue.h"
#include "Registry.h"

#define ORDER 5

typedef struct RegData {
  int id;
  int regPos;
} RegData;

/// estrutura de nó de árvore B em arquivo
typedef struct BTreeNode {
  /// numero de chaves populadas e posição do nó em arquivo
  int nKeys, nodePos;
  /// vetor de chaves, ponteiros dos registro e filhos no arquivo
  RegData keys[ORDER];
  int children[ORDER + 1];
} BTreeNode;

/// estrutura de cabeçalho de arquivo
typedef struct BTreeHeader {
  int root, topPos;
} BTreeHeader;

/**
 * esta função cria um cabeçalho para uma árvore B vazia e escreve no arquivo indicado
 * @param f é o arquivo a ser escrito
 * @precondition arquivo tem que estar aberto e ter direitos de escrita
 * @postcondition cabeçalho para árvore B vazia é escrito no arquivo
 */
void createEmptyBTree(FILE *f);

/**
 * esta função aloca memória e inicializa um nó de árvore B
 * @return nó de árvore B alocado e inicializado
 * @precondition nenhuma
 * @postcondition nó é alocado e inicializado em memória
 */
BTreeNode *createNode();

/**
 * esta função escreve no arquivo indicado o cabeçalho passado
 * @param f é o arquivo indicado
 * @param header é o cabeçalho a ser escrito no arquivo
 * @precondition arquivo tem que estar aberto e ter direitos de escrita
 */
void writeBTreeHeaderToFile(FILE *f, BTreeHeader *header);

/**!
 * esta função escreve um nó de árvore B em um arquivo indicado
 * @param f arquivo indicado
 * @param node nó a ser escrito
 * @param header cabeçalho do arquivo
 * @return posição na qual o nó foi escrito no arquivo
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition nó é escrito no arquivo
 */
int writeNodeToFile(FILE *f, BTreeHeader *header, BTreeNode *node);

/**
 * esta função insere um valor na árvore B que se encontra no arquivo indicado
 * @param f arquivo de indices
 * @param id valor a ser inserido na árvore
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition valor é inserido no arquivo
 */
void insertBTree(FILE *f, int id, int regPos);

/**
 * esta função é uma recursiva auxiliar de inserir
 * @param f é o arquivo a ser inserido o valor
 * @param header é o cabeçalho do arquivo
 * @param currentNode nó atual
 * @param info valor a ser inserido no arquivo
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition valor é inserido no arquivo
 */
void insertAux(FILE *f, BTreeHeader *header, BTreeNode *currentNode, RegData info);

/**
 * esta função insere o o valor passado no seu lugar correto no nó
 * @param node é o nó a ser inserido o valor
 * @param pos é a posição a ser inserido o nó
 * @param info é o valor a ser inserido
 * @param p é o endereço para filho a direita do valor inserido
 */
void insertOnRight(BTreeNode *node, int pos, RegData info, int p);

/**
 * esta função testa se ocorreu everflow em um nó
 * @param node nó a ser testado
 * @return 1 para verdadeiro e 0 para falso
 * @precondition node não pode ser nulo
 * @postcondition nenhuma
 */
int overflow(BTreeNode *node);

/**
 * esta função faz a operação split no nó indicado
 * @param node é o nó a ser feito o splot
 * @param m é o valor mediano do nó
 * @return novo nó criado no split
 * @precondition node não pode ser nulo
 * @postcondition um novo nó é criado e retornado
 */
BTreeNode *split(BTreeNode *node, RegData *m);

/**
 * esta função procura se um valor existe em um nó e se não modifica pos \
 * para apontar o lugar que este iria se encaixar
 * @param node nó a ser pesquisado
 * @param info valor a ser pesquisado
 * @param pos posição apropriada para o valor
 * @return se valor foi encontrado ou não
 * @precondition node e pos não podem ser nulos
 */
int searchBTreePos(BTreeNode *node, int info, int *pos);

/**
 * testa se nó passado é folha
 * @param node é o nó a ser testado
 * @return 1 para verdadeiro e 0 para falso
 * @precondition node não pode ser nulo
 * @postcondition nenhuma
 */
int isLeaf(BTreeNode *node);

/**
 * esta função lê o cabeçalho do arquivo indicado
 * @param f arquivo indicado
 * @return poteiro para cabeçalho lido
 * @precondition arquivo tem que estar aberto e com direito de escrita
 * @postcondition espaço em memória para o cabeçalho é alocado e inicializado
 */
BTreeHeader *readBTreeHeader(FILE *f);

/**
 * esta função lê um nó em um arquivo inidacado na posição passada
 * @param f é o arquivo indicado
 * @param pos é a posição passada
 * @return nó lido
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition espaço em memória para o nó é alocado e inicializado
 */
BTreeNode *readBTreeNode(FILE *f, int pos);

/**
 * pesquisa por registro a partir de código e o retorna
 * @param treeFile arquivo de árvore B
 * @param regFile arquivo de registros
 * @param code código a ser pesquisado
 * @return registro se encontrado
 * @precondition arquivos tem que estar abertos e ter direito de escrita
 * @postcondition nenhuma
 */
Registry *searchRegistryBTree(FILE *treeFile, FILE *regFile, int code);

/**
 * esta função printa na saida padrão o cabeçalho passado
 * @param header cabeçalho a ser printado
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTreeHeader(BTreeHeader *header);

/**
 * esta função printa na saida padrão a árvore B no arquivo indicado
 * @param f arquivo que contém árvore B
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTree(FILE *f);

/**
 * esta função printa a posição em arquivo dos filhos do nó indicado
 * @param node é o nó a ser printado na saida padrão
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTreeNodeChildren(BTreeNode *node);

/**
 * esta função printa o valor das chaves nó indicado
 * @param node é o nó a ser printado na saida padrão
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTreeNodeKeys(BTreeNode *node);

#endif