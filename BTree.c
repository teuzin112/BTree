#include "BTree.h"

#include <stdlib.h>

#include "string.h"

/**
 * esta função cria um cabeçalho para uma árvore B vazia e escreve no arquivo indicado
 * @param f é o arquivo a ser escrito
 * @precondition arquivo tem que estar aberto e ter direitos de escrita
 * @postcondition cabeçalho para árvore B vazia é escrito no arquivo
 */
void createEmptyBTree(FILE *f) {
  BTreeHeader header = {.root = -1, .topPos = 0};

  /// inicializa cabeçalho para valores padrões
  // header.root = -1;
  // header.topPos = 0;

  /// escreve cabecalho em arquivo
  writeBTreeHeaderToFile(f, &header);
}

/**
 * esta função aloca memória e inicializa um nó de árvore B
 * @return nó de árvore B alocado e inicializado
 * @precondition nenhuma
 * @postcondition nó é alocado e inicializado em memória
 */
BTreeNode *createNode() {
  /// aloca espaço em memória para o nó
  BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));

  /// para cada elemento nos campos
  for (int i = 0; i < ORDER; ++i) {
    /// inicializa para seus valores padrões
    node->keys[i].id = 0;
    node->keys[i].regPos = -1;
    node->children[i] = -1;
  }

  /// quantidade de chaveze e posição de nó inicializados para valores padrão
  node->nKeys = 0;
  node->nodePos = -1;

  /// retorna nó
  return node;
}

/**
 * esta função escreve no arquivo indicado o cabeçalho passado
 * @param f é o arquivo indicado
 * @param header é o cabeçalho a ser escrito no arquivo
 * @precondition arquivo tem que estar aberto e ter direitos de escrita
 */
void writeBTreeHeaderToFile(FILE *f, BTreeHeader *header) {
  if (!header)
    return;

  fseek(f, 0, SEEK_SET);

  fwrite(header, sizeof(BTreeHeader), 1, f);
}

/**!
 * esta função escreve um nó de árvore B em um arquivo indicado
 * @param f arquivo indicado
 * @param header cabeçalho do arquivo
 * @param node nó a ser escrito
 * @return posição na qual o nó foi escrito no arquivo
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition nó é escrito no arquivo
 */
int writeNodeToFile(FILE *f, BTreeHeader *header, BTreeNode *node) {
  int pos;

  /// se header ou nó são nulos
  if (!header || !node)
    return -1;  //< retorna posição inválida
  /// se nó possui posição inválida no arquivo
  if ((pos = node->nodePos) == -1) {
    /// se não existem posições livres dentro do arquivo
    /// coloca posição para topo do arquivo
    pos = header->topPos++;
    writeBTreeHeaderToFile(f, header);
  }

  //    printf("pos = %d\n", pos);

  /// armazena posição do nó em arquivo no nó
  node->nodePos = pos;

  /// calcula posição para escrita do nó em arquivo
  fseek(f, sizeof(BTreeHeader) + sizeof(BTreeNode) * pos, SEEK_SET);

  /// escreve nó
  fwrite(node, sizeof(BTreeNode), 1, f);

  /// retorna posição de escrita
  return pos;
}

/**
 * esta função insere um valor na árvore B que se encontra no arquivo indicado
 * @param f arquivo de indices
 * @param id valor a ser inserido na árvore
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition valor é inserido no arquivo
 */
void insertBTree(FILE *f, int id, int regPos) {
  /// cabeçalho do arquivo
  BTreeHeader *header = readBTreeHeader(f);
  /// raiz da árvore
  BTreeNode *root = readBTreeNode(f, header->root);
  BTreeNode *newNode;

  RegData data;
  data.regPos = regPos;
  data.id = id;

  // printBTreeHeader(header);
  /// se árvore é vazia
  if (!root) {
    /// cria um novo nó para ser raiz
    root = createNode();

    /// atribui valor para chaves
    root->keys[0] = data;
    root->nKeys = 1;

    /// atualiza cabeçalho para apontar para raiz
    header->root = writeNodeToFile(f, header, root);
  } else {
    /// função auxiliar para inserção
    insertAux(f, header, root, data);
    /// se ocorreu overflow no nó
    if (overflow(root)) {
      /// splita o nó
      RegData m;

      newNode = split(root, &m);
      BTreeNode *newRoot = createNode();

      /// inicializa novos nós criados no split
      newRoot->keys[0] = m;
      newRoot->children[0] = header->root;
      newRoot->nKeys = 1;

      newRoot->children[1] = writeNodeToFile(f, header, newNode);
      header->root = writeNodeToFile(f, header, newRoot);

      /// libera memória utilizada pelos nós em memória
      free(newNode);
      free(newRoot);
    }
    /// escreve raiz no arquivo
    writeNodeToFile(f, header, root);
  }

  //    printBTreeHeader(header);
  /// escreve cabeçalho no arquivo
  writeBTreeHeaderToFile(f, header);
  // printBTreeHeader(header);
  //     printf("-----------\n");
  //     printBTree(f);
  //     printf("-----------\n");

  /// libera memória utilizada pela raiz e pelo cabeçalho
  free(root);
  free(header);
}

/**
 * esta função é uma recursiva auxiliar de inserir
 * @param f é o arquivo a ser inserido o valor
 * @param header é o cabeçalho do arquivo
 * @param currentNode nó atual
 * @param info valor a ser inserido no arquivo
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition valor é inserido no arquivo
 */
void insertAux(FILE *f, BTreeHeader *header, BTreeNode *currentNode, RegData info) {
  int pos = 0;

  /// se valor não existe no nó atual
  if (!searchBTreePos(currentNode, info.id, &pos)) {
    /// se nó atual é folha
    if (isLeaf(currentNode)) {
      /// insere valor no nó atual
      insertOnRight(currentNode, pos, info, -1);
    } else {
      /// lê nó indicado para inserção
      BTreeNode *node = readBTreeNode(f, currentNode->children[pos]);
      /// insere no nó lido
      insertAux(f, header, node, info);
      /// se ocorreu overflow no nó indicado
      if (overflow(node)) {
        /// splita o nó
        RegData m;
        BTreeNode *aux = split(node, &m);
        insertOnRight(currentNode, pos, m, writeNodeToFile(f, header, aux));

        /// escreve nós splitados no arquivo
        writeNodeToFile(f, header, node);

        /// libera memória utilizada pelo nó indicado
        free(aux);
      }
    }
  }
  /// atualiza nó atual em arquivo
  writeNodeToFile(f, header, currentNode);
}

/**
 * esta função insere o o valor passado no seu lugar correto no nó
 * @param node é o nó a ser inserido o valor
 * @param pos é a posição a ser inserido o nó
 * @param info é o valor a ser inserido
 * @param p é o endereço para filho a direita do valor inserido
 */
void insertOnRight(BTreeNode *node, int pos, RegData info, int p) {
  /// move todos valores antes de pos para direita
  for (int i = node->nKeys; i > pos; --i) {
    node->keys[i] = node->keys[i - 1];
    node->children[i + 1] = node->children[i];
  }

  /// insere valor em seu lugar correto
  node->keys[pos] = info;
  node->children[pos + 1] = p;
  node->nKeys++;
}

/**
 * esta função testa se ocorreu everflow em um nó
 * @param node nó a ser testado
 * @return 1 para verdadeiro e 0 para falso
 * @precondition node não pode ser nulo
 * @postcondition nenhuma
 */
int overflow(BTreeNode *node) {
  return (node->nKeys == ORDER);
}

/**
 * esta função faz a operação split no nó indicado
 * @param node é o nó a ser feito o splot
 * @param m é o valor mediano do nó
 * @return novo nó criado no split
 * @precondition node não pode ser nulo
 * @postcondition um novo nó é criado e retornado
 */
BTreeNode *split(BTreeNode *node, RegData *m) {
  /// cria novo nó
  BTreeNode *newNode = createNode();

  /// calcula posição mediana
  int q = (ORDER - 1) / 2;
  newNode->nKeys = q;
  node->nKeys = q;

  /// atribui para m valor da chave mediana
  *m = node->keys[q];

  /// atribui valores acima da posição mediana de node para newNode
  newNode->children[0] = node->children[q + 1];
  for (int i = 0; i < q; ++i) {
    newNode->keys[i] = node->keys[q + 1 + i];
    newNode->children[i + 1] = node->children[q + 2 + i];
  }

  /// retona nó criado na operação
  return newNode;
}

/**
 * esta função procura se um valor existe em um nó e se não modifica pos \
 * para apontar o lugar que este iria se encaixar
 * @param node nó a ser pesquisado
 * @param info valor a ser pesquisado
 * @param pos posição apropriada para o valor
 * @return se valor foi encontrado ou não
 * @precondition node e pos não podem ser nulos
 */
int searchBTreePos(BTreeNode *node, int info, int *pos) {
  /// passa pelas chaves procurando por um valor maior que o valor passado
  for ((*pos) = 0; (*pos) < node->nKeys; (*pos)++) {
    if (info == node->keys[(*pos)].id)
      return 1;
    else if (info < node->keys[(*pos)].id)
      return 0;
  }
  return 0;
}

/**
 * testa se nó passado é folha
 * @param node é o nó a ser testado
 * @return 1 para verdadeiro e 0 para falso
 * @precondition node não pode ser nulo
 * @postcondition nenhuma
 */
int isLeaf(BTreeNode *node) {
  return (node->children[0] == -1);
}

/**
 * esta função lê o cabeçalho do arquivo indicado
 * @param f arquivo indicado
 * @return poteiro para cabeçalho lido
 * @precondition arquivo tem que estar aberto e com direito de escrita
 * @postcondition espaço em memória para o cabeçalho é alocado e inicializado
 */
BTreeHeader *readBTreeHeader(FILE *f) {
  /// aloca espaço na memória para o cabeçalho
  BTreeHeader *header = (BTreeHeader *)malloc(sizeof(BTreeHeader));

  /// vai para posição correta
  fseek(f, 0, SEEK_SET);

  /// lê cabeçalho
  fread(header, sizeof(BTreeHeader), 1, f);

  /// retorna cabeçalho lido
  return header;
}

/**
 * esta função lê um nó em um arquivo inidacado na posição passada
 * @param f é o arquivo indicado
 * @param pos é a posição passada
 * @return nó lido
 * @precondition arquivo tem que estar aberto e ter direito de escrita
 * @postcondition espaço em memória para o nó é alocado e inicializado
 */
BTreeNode *readBTreeNode(FILE *f, int pos) {
  BTreeNode *node;

  /// se posição é invalida
  if (pos == -1)
    return NULL;

  /// aloca espaço em memória para o nó
  node = (BTreeNode *)malloc(sizeof(BTreeNode));

  /// calcula posição do nó em arquivo
  fseek(f, sizeof(BTreeHeader) + sizeof(BTreeNode) * pos, SEEK_SET);

  /// lê nó do arquivo
  fread(node, sizeof(BTreeNode), 1, f);

  /// retorna nó lido
  return node;
}

/**
 * pesquisa por registro a partir de código e o retorna
 * @param treeFile arquivo de árvore B
 * @param regFile arquivo de registros
 * @param code código a ser pesquisado
 * @return registro se encontrado
 * @precondition arquivos tem que estar abertos e ter direito de escrita
 * @postcondition nenhuma
 */
Registry *searchRegistryBTree(FILE *treeFile, FILE *regFile, int code) {
  BTreeHeader *bTreeHeader = readBTreeHeader(treeFile);
  RegistryHeader *regFileHeader = readRegistryHeader(regFile);
  BTreeNode *root = readBTreeNode(treeFile, bTreeHeader->root);
  BTreeNode *currentNode = root;
  Registry *foundRegistry = NULL;
  int pos, found;

  if (root) {
    while (!(found = searchBTreePos(currentNode, code, &pos)) && !isLeaf(currentNode)) {
      BTreeNode *nextNode = readBTreeNode(treeFile, currentNode->children[pos]);
      free(currentNode);
      currentNode = nextNode;
    }

    if (found) {
      //            system("CLS");
      foundRegistry = readRegistryFromFile(regFile, currentNode->keys[pos].regPos);
    } else {
      // printf("Registry not found!\n");
    }

    free(currentNode);
  }

  free(bTreeHeader);
  free(regFileHeader);

  return foundRegistry;
}

/**
 * esta função printa na saida padrão o cabeçalho passado
 * @param header cabeçalho a ser printado
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTreeHeader(BTreeHeader *header) {
  printf("--------------------\n");
  printf("Root = %d\n", header->root);
  printf("Node Quantity = %d\n", header->topPos);
  printf("--------------------\n");
}

/**
 * esta função printa na saida padrão a árvore B no arquivo indicado
 * @param f arquivo que contém árvore B
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTree(FILE *f) {
  /// lê cabeçalho e raiz da árvore
  BTreeHeader *header = readBTreeHeader(f);
  if (header->root == -1) return;
  printf("------------------------------------------------------------\n");
  BTreeNode *root = readBTreeNode(f, header->root);
  /// cria fila
  Queue *q = createQueue();
  int nodeCount;

  /// coloca raiz na fila
  pushQ(q, root);

  /// enquanto não se acabarem os nós
  nodeCount = q->count;
  while (nodeCount) {
    /// printa os nós do nível atual na saida padrão
    while (nodeCount) {
      QNode *qNode = popQ(q);
      BTreeNode *bTreeNode = (BTreeNode *)qNode->info;

      printBTreeNodeKeys(bTreeNode);
      printf(" ");

      /// puxa os seus filhos para a fila
      for (int i = 0; i <= bTreeNode->nKeys; ++i) {
        if (bTreeNode->children[i] != -1)
          pushQ(q, readBTreeNode(f, bTreeNode->children[i]));
      }
      nodeCount--;

      free(qNode);
      free(bTreeNode);
    }
    printf("\n");
    nodeCount = q->count;
  }
  printf("------------------------------------------------------------\n");

  deleteQueue(q);
}

/**
 * esta função printa a posição em arquivo dos filhos do nó indicado
 * @param node é o nó a ser printado na saida padrão
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTreeNodeChildren(BTreeNode *node) {
  printf("[");
  for (int i = 0; i < node->nKeys; ++i) {
    printf("%d,", node->children[i]);
  }
  printf("%d", node->children[node->nKeys]);
  printf("]");
}

/**
 * esta função printa o valor das chaves nó indicado
 * @param node é o nó a ser printado na saida padrão
 * @precondition nenhuma
 * @postcondition nenhuma
 */
void printBTreeNodeKeys(BTreeNode *node) {
  printf("[");
  if (node->nKeys) {
    for (int i = 0; i < node->nKeys - 1; ++i) {
      printf("%d, ", node->keys[i].id);
    }
    printf("%d", node->keys[node->nKeys - 1].id);
  } else {
    printf("!");
  }
  printf("]");
}
