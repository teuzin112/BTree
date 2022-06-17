#include <stdio.h>
#include <stdlib.h>

#include "BTree.h"
#include "Files.h"
#include "Registry.h"

void printInfoMenu(FILE *registriesF, FILE *btreeF) {
  int id;
  printf("Codigo: ");
  scanf("%d", &id);
  getchar();
  printInfo(id, registriesF, btreeF);
}

void listInfoMenu(FILE *registriesF, FILE *btreeF) {
  BTreeHeader *treeHeader = readBTreeHeader(btreeF);
  BTreeNode *node = readBTreeNode(btreeF, treeHeader->root);

  listInfo(node, registriesF, btreeF);
}

void printMenu() {
  printf("[1] - Inserir\n");
  printf("[2] - Alterar endereco\n");
  printf("[3] - Alterar telefone\n");
  printf("[4] - Carregar arquivo texto\n");
  printf("[5] - Informacoes de um profissional\n");
  printf("[6] - Listar todos profissionais\n");
  printf("[7] - Imprimir arvore\n");
  printf("[0] - Fechar programa\n");
}

int main() {
  // Abre arquivo de dados se ja existe
  FILE *registriesFile = fopen("data", "rb+");
  if (registriesFile == NULL) {  // Se nao existe cria na hora
    registriesFile = fopen("data", "wb+");
    createEmptyRegistryFile(registriesFile);
  }
  // Abre arquivo de indices se ja existe
  FILE *bTreeFile = fopen("indexes", "rb+");
  if (bTreeFile == NULL) {  // Se nao existe cria na hora
    bTreeFile = fopen("indexes", "wb+");
    createEmptyBTree(bTreeFile);
  }

  int choice;

  printMenu();
  scanf("%d", &choice);
  getchar();
  system("cls");

  while (choice) {
    switch (choice) {
      case 1:
        insert(registriesFile, bTreeFile);
        break;
      case 2:
        updateAddres(registriesFile, bTreeFile);
        break;
      case 3:
        updatePhone(registriesFile, bTreeFile);
        break;
      case 4:
        readTxtFile(registriesFile, bTreeFile);
        break;
      case 5:
        printInfoMenu(registriesFile, bTreeFile);
        break;
      case 6:
        listInfoMenu(registriesFile, bTreeFile);
        break;
      case 7:
        printBTree(bTreeFile);
        break;

      default:
        break;
    }
    printf("PRESS ANY KEY TO RETURN TO THE MENU\n");
    getchar();
    system("cls");

    printMenu();
    scanf("%d", &choice);
    getchar();
    system("cls");
  }

  fclose(registriesFile);
  fclose(bTreeFile);

  return 0;
}
