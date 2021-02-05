#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CharToIndex(c) ((int)c - (int)'a')

typedef struct node{
	char *item;
	struct node *next;
} Node;

typedef struct TrieNode{
	char clave[32];
    struct TrieNode *hijos[26];
	int hoja;
	Node *sinonimos;
	Node *antonimos;
} Trie;

Node *new_item(char *item);

//	Genera un nodo del Trie.
Trie *getNode(void);

//	Inserta una palabra al Trie
void insert(Trie *root, const char *key);

//	Busca una palabra al Trie retornando 1 si la encuentra o 0 en caso contrario.
int search(Trie *root, const char *key);

//	Ingresa informacion del Archivo al Trie generando las listas de Sinonimos 
//	y Antonimos correspondientes a los prefijos.
void cargarTrie(Trie *root, char *name);


/*  Comandos    */

//	Asigna el archivo a cargar al Trie para futuras ejecuciones.
void cmdCargar(char *name);

void cmdSinonimo();

void cmdAntonimo();

void cmdExpresion();

//	Muestra los comandos disponibles.
void cmdAyuda(void);