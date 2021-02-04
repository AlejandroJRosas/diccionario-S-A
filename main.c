#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CharToIndex(c) ((int)c - (int)'a')

typedef struct node{
	char *item;
	struct node *next;
} Node;

typedef struct TrieNode{
	char *clave;
	struct TrieNode *hijos[26];
	int hoja;
	Node *sinonimos;
	Node *antonimos;
} Trie;

//	Crea un nuevo elemento con el item dado.
Node *new_item(char *item){
    Node *newp;

    newp = (Node *)malloc(sizeof(Node));
    if(newp == NULL){
        fprintf(stderr, "new_item: error en malloc\n");
        exit(1);
    }
    strcpy(newp->item, item);
    newp->next = NULL;
    return newp;
}

//	Genera una rama del Trie.
Trie *getNode(void){
	int i;
	Trie *pNode = NULL;

	pNode = (Trie *)malloc(sizeof(Trie));
	if(pNode){
		pNode->hoja = 0;
		for (i = 0; i < 26; i++)
			pNode->hijos[i] = NULL;
	}
	else{
		printf("Error de Malloc");
		exit(1);
	}
	return pNode;
}

//	Inserta una palabra al Trie
void insert(Trie *root, const char *key){
	int level, length = strlen(key), index;
	Trie *pCrawl = root;
	Node *listSin = NULL, *listAnt = NULL;

	for (level = 0; level < length; level++){
		index = CharToIndex(key[level]);
		pCrawl->clave = key[level];
		pCrawl->sinonimos = NULL;
		pCrawl->antonimos = NULL;
		if (!(pCrawl->hijos[index]))
			pCrawl->hijos[index] = getNode();
		pCrawl = pCrawl->hijos[index];
	}
	pCrawl->hoja = 1;
}

//	Busca una palabra al Trie retornando 1 si la encuentra o 0 en caso contrario.
int search(Trie *root, const char *key){
	int level, length = strlen(key), index;
	Trie *pCrawl = root;

	for (level = 0; level < length; level++){
		index = CharToIndex(key[level]);
		if (!(pCrawl->hijos[index]))
			return 0;
		pCrawl = pCrawl->hijos[index];
	}

	return ((pCrawl != NULL) && (pCrawl->hoja));
}

//	Ingresa informacion del Archivo al Trie generando las listas de Sinonimos 
//	y Antonimos correspondientes a los prefijos.
void cargarTrie(Trie *root, char *name){
	char word[32];
	int sin, ant;
	FILE *arch;

	//	Verifica el archivo que cargara si este no existe aborta.
	if((arch = fopen(name, "r")) == NULL){
		fprintf(stderr, "Error: No se pudo abrir %s\n", name);
		exit(1);
	}
	/***********************************************************/
	
	sin = ant = 0;
	while(!feof(arch)){
		fscanf(arch, "%s", word);

		if(!(strcmp(word, "S"))){	//	Condicional de Prefijo "S"
			sin = 1;
			continue;
		}
		else
			if(!(strcmp(word, "A"))){	//	Condicional de Prefijo "A"
				ant = 1;
				continue;
			}
		insert(root, word);
		if(sin == 1){
			printf("xd\n");
			sin = 0;
		}
		else
			if(ant == 1){
				printf("dx\n");
				ant = 0;
			}
	}
	fclose(arch);
}

/*void expresion(char texto[]){
	char cadAux[32];
	int i, j, aux;

	for(i = 0, j = 0; i < strlen(texto); i++){
		if(texto[i] == " "){
			for(aux = 0; j < i; j++, aux++)
				cadAux[aux] = texto[j];
			j = i + 1;
			//comparar(cadAux);	//	La funcion comparar compara la palabra con el arbol Trie
			cadAux[0] = "\0";
		}
		if(i == (strlen(texto) - 1)){
			for(aux = 0; j <= i; j++, aux++)
				cadAux[aux] = texto[j];
			j = i + 1;
			//comparar(cadAux);	//	^^^
			cadAux[0];
		}
	}
}//*/

//	Asigna el archivo a cargar al Trie para futuras ejecuciones.
void cmdCargar(char *name){
	FILE *arch = NULL;

	arch = fopen("loaded.txt", "w");
	fputs(name, arch);
	fclose(arch);
}

void cmdSinonimo();

void cmdAntonimo();

void cmdExpresion();

//	Muestra los comandos disponibles.
void cmdAyuda(void){
	printf("Comandos Disponibles\n\n ");
	printf("> cargar + [Nombre]   -- Asigna el archivo que se estara usando para futuras ejecuciones\n ");
	printf("> s + [Palabra]       -- Enlista los sinonimos de la palabra ingresada\n ");
	printf("> a + [Palabra]       -- Enlista los antonimos de la palabra ingresada\n ");
	printf("> e + [Expresion]     -- Enlista los sinonimos y antonimos de las palabras que componen la expresion\n ");
	printf("> ayuda               -- Muestra los comandos disponibles\n ");
	printf("> x                   -- Finaliza la ejecucion del programa (Solo para Modo Iterativo)\n");
}

int main(int argc, char *argv[]){
	Trie *root = getNode();
	FILE *arch = NULL;
	char word[32];

	//	Modo Comando
	if(argc > 1){
		if(!(strcmp(argv[1], "cargar"))){	//	cmdCargar
			cmdCargar(argv[2]);
			exit(1);
		}
		else{
			if(!(strcmp(argv[1], "ayuda"))){	//	cmdAyuda
				cmdAyuda();
				exit(1);
			}
			else{	//	Comando Erroneo
				printf("Error comando -%s- no encontrado\n", argv[1]);
				exit(1);
			}
		}
	}

	//	Modo Iterativo
	if((arch = fopen("loaded.txt", "r")) == NULL){	//	Verifica el archivo que cargara si este no existe aborta.
		printf("Es necesario asignar el archivo a cargar por medio del comando cargar + nombre\n");
		exit(1);
	}
	fscanf(arch, "%s", word);
	fclose(arch);
	cargarTrie(root, word);
	return 0;
}