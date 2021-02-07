#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CharToIndex(c) ((int)c - (int)'a')

typedef struct node{
	char item[24];
	struct node *next;
} Node;

typedef struct TrieNode{
	char clave[24];
	struct TrieNode *hijos[26];
	int hoja;
	Node *sinonimos;
	Node *antonimos;
} Trie;

/*					Utiles					*/

//	Recibe una cadena y la devuelve con sus caracteres en minusculas.
char *strLow(char *str);


/*				Manejo de Cadenas			 */

//	Libera todos los elementos de listp.
void free_word(Trie *listp);

//	Muestra los elementos en listp.
void printList(Node *listp);

//  Inserta newp ordenado alfabeticamente en listp y retorna la nueva lista.
Node *insertList(Node *listp, Node *newp);

//	Busca un nombre en listp retornando 1 si lo encuentra o 0 en caso contrario.
int lookup(Node *listp, char *key);

//	Crea un nuevo elemento con el item ingresado.
Node *new_item(char *item);


/*///////////////////////////////////////////////////////*/

/*					TRIE					 */

//	Genera un nodo del Trie.
Trie *getNode(void);

//	Inserta una palabra en el Trie
void insertTrie(Trie *root, const char *key);

//	Busca una palabra en el Trie retornando 1 si la encuentra o 0 en caso contrario.
int search(Trie *root, const char *key);

//  Busca una palabra en el Trie retornando la Direccion de memoria de su nodo
//  lista sinonimo o antonimo dependiendo del modo indicado.
Trie *searchNode(Trie *root, const char *key);

//  Genera nodo de sinonimos o antonimos con las palabra dadas en el Trie
//  dependiendo del modo indicado.
void addList(Trie *root, char *w1, char *w2, int modo);

//	Ingresa informacion del Archivo al Trie generando las listas de Sinonimos 
//	y Antonimos correspondientes a los prefijos, retorna 1 si la carga fue
//	exitosa o 0 en caso contrario.
int cargarTrie(Trie *root, char *name);


/*///////////////////////////////////////////////////////*/

/*					Comandos					*/

//	Asigna el archivo a cargar al Trie para futuras ejecuciones.
int cmdCargar(char *name);

//	Busca la palabra ingresada e imprime la lista pedida, si no imprime un mensaje de
//	no encontrar la palabra.
void cmdPalabra(Trie *root, char *key, int modo);

//	Recibe una oracion e imprime las listas de sinonimos y antonimos de las
//	palabras que se encuentran en el trie.
void cmdExpresion(Trie *root, char texto[]);

//	Muestra los comandos disponibles.
void cmdAyuda(void);