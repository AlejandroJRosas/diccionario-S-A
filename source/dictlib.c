#include "dictlib.h"

/*					Utiles					*/

char *strLow(char *str){
	int i;

	for(i = 0; *(str + i); i++)
		*(str + i) = tolower(*(str + i));
	return str;
}

/*///////////////////////////////////////////////////////*/

/*				Manejo de Cadenas			 */

void free_word(Trie *listp){
	Node *next;

	for(; listp->sinonimos != NULL; listp->sinonimos = next){
		next = listp->sinonimos->next;
		free(listp->sinonimos);
	}
    for(; listp->antonimos != NULL; listp->antonimos = next){
        next = listp->antonimos->next;
        free(listp->antonimos);
    }
    listp->sinonimos = NULL;
    listp->antonimos = NULL;
}

void printList(Node *listp){
	Node *aux = listp;

	if(!aux)
		printf("\n");
    for(aux = listp; aux != NULL; aux = aux->next){
		if(aux->next != NULL)
        	printf("%s, ", aux->item);
		else
			printf("%s.\n", aux->item);
	}
}

Node *insertList(Node *listp, Node *newp){
    Node *p, *prev = NULL;

    for(p = listp; p != NULL && (strcmp(p->item, newp->item) < 0); p = p->next)
        prev = p;
    newp->next = p;
    if(prev == NULL)
        return newp;
    prev->next = newp;
    return listp;
}

int lookup(Node *listp, char *key){
	Node *p;

    for(p = listp; p != NULL; p = p->next)
        if(!strcmp(p->item, key))
            return 0;
    return 1;
}

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

/*///////////////////////////////////////////////////////*/

/*					TRIE					 */

Trie *getNode(void){
    Trie *pNode = NULL;
	int i;

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

void insertTrie(Trie *root, const char *key){
    Trie *pTrie = root;
	int level, length = strlen(key), index;

	for (level = 0; level < length; level++){
		index = CharToIndex(key[level]);
		strncpy(pTrie->clave, key, level);
		pTrie->sinonimos = NULL;
		pTrie->antonimos = NULL;
		if (!(pTrie->hijos[index]))
			pTrie->hijos[index] = getNode();
		pTrie = pTrie->hijos[index];
	}
	strcpy(pTrie->clave, key);
	pTrie->hoja = 1;
}

int search(Trie *root, const char *key){
    Trie *pTrie = root;
	int level, length = strlen(key), index;

	for(level = 0; level < length; level++){
		index = CharToIndex(key[level]);
		if (!(pTrie->hijos[index]))
			return 0;
		pTrie = pTrie->hijos[index];
	}
	return ((pTrie != NULL) && (pTrie->hoja));
}

Trie *searchNode(Trie *root, const char *key){
    Trie *pTrie = root;
	int level, length = strlen(key), index;

	for(level = 0; level < length; level++){
		index = CharToIndex(key[level]);
		if (!(pTrie->hijos[index]))
			return NULL;
		pTrie = pTrie->hijos[index];
	}
	return pTrie;
}

void addList(Trie *root, char *w1, char *w2, int modo){
    Trie *listp;

    if(modo){       //  Lista de Sinonimos
        listp = searchNode(root, w1);
		if(lookup(listp->sinonimos, w2))
        	listp->sinonimos = insertList(listp->sinonimos, new_item(w2));
        listp = searchNode(root, w2);
		if(lookup(listp->sinonimos, w1))
        	listp->sinonimos = insertList(listp->sinonimos, new_item(w1));
    }
    else{           //  Lista de Antonimos
        listp = searchNode(root, w1);
		if(lookup(listp->antonimos, w2))
        	listp->antonimos = insertList(listp->antonimos, new_item(w2));
        listp = searchNode(root, w2);
		if(lookup(listp->antonimos, w1))
        	listp->antonimos = insertList(listp->antonimos, new_item(w1));
    }
}

int cargarTrie(Trie *root, char *name){
	FILE *arch;
    char word[24], aux[24];
	int modo, flag;

	//	Verifica el archivo que cargara si este no existe aborta.
	if((arch = fopen(name, "r")) == NULL){
		fprintf(stderr, "Error: No se pudo abrir %s\n", name);
		return 0;
	}
	/***********************************************************/
	
    flag = 0;
	while(!feof(arch)){
		fscanf(arch, "%s", word);
		if(!(strcmp(word, "S"))){	//	Condicional de Prefijo "S"
			modo = 1;
			continue;
		}
		else
			if(!(strcmp(word, "A"))){	//	Condicional de Prefijo "A"
				modo = 0;
				continue;
			}
		strcpy(word, strLow(word));
		insertTrie(root, word);
        if(!flag){
            strcpy(aux, word);
            flag++;
            continue;
        }
        addList(root, word, aux, modo);
        flag = 0;
	}
	fclose(arch);
	return 1;
}

/*///////////////////////////////////////////////////////*/

int cmdCargar(char *name){
	FILE *arch = NULL;
    char word[24];

    if((arch = fopen("loaded.txt", "r+")) == NULL){
	    arch = fopen("loaded.txt", "w+");
	    fputs(name, arch);
    }
    else{
        arch = fopen("loaded.txt", "r+");
        while(!feof(arch)){
		    fscanf(arch, "%s", word);
            if(!strcmp(word, name))
                return 0;
        }
        fputs("\n", arch);
        fputs(name, arch);
    }
	fclose(arch);
    return 0;
}

void cmdPalabra(Trie *root, char *key, int modo){
	Trie *aux;

	strcpy(key, strLow(key));
	if(search(root, key)){
		aux = searchNode(root, key);
		if(modo){
			printf("%s (Sinonimos): ", key);
			printList(aux->sinonimos);
		}
		else{
			printf("%s (Antonimos): ", key);
			printList(aux->antonimos);
		}
	}
	else{
		printf("La palabra -%s- no se encuentra en la base de datos.\n", key);
	}
}

void cmdExpresion(Trie *root, char texto[]){
	char cadAux[24];
	int i, j;

	for(i = 0, j = 0; (texto[i] != '\0') && (texto[i] != '\n'); i++){
		if(texto[i] != ' '){
            cadAux[j] = texto[i];
			j++;
		}
		else{
			cadAux[j] = '\0';
			if(strcmp("e", cadAux)){
				cmdPalabra(root, cadAux, 1);
				cmdPalabra(root, cadAux, 0);
			}
		}
		if((texto[i] == ' ') && (texto[i + 1] != ' '))
			j = 0;
	}
	cadAux[j] = '\0';
	cmdPalabra(root, cadAux, 1);
	cmdPalabra(root, cadAux, 0);
}

void cmdAyuda(void){
	printf("\nComandos Disponibles\n\n ");
	printf("> cargar              -- Carga los archivos que abririas normalmente en modo comando (Solo en Modo Iterativo)\n ");
	printf("> cargar + [Archivo]  -- Asigna los archivos que deseas abrir con el modo comando o carga archivo en modo iterativo\n ");
	printf("> liberar             -- Libera toda la informacion en la base de datos [EN PROCESO] [Solo en Modo Iterativo]\n ");
	printf("> limpiar             -- Renueva la memoria del comando cargar\n ");
	printf("> mostrar             -- Muestra la informacion cargada en la base de datos\n ");
    printf("> s + [Palabra]       -- Enlista los sinonimos de la palabra ingresada\n ");
	printf("> a + [Palabra]       -- Enlista los antonimos de la palabra ingresada\n ");
	printf("> e + [Expresion]     -- Enlista los sinonimos y antonimos de las palabras que componen la expresion\n ");
    printf("> ayuda               -- Muestra los comandos disponibles\n ");
	printf("> salir               -- Finaliza la ejecucion del programa (Solo para Modo Iterativo)\n\n");
}

void cmdLiberar(Trie *root){
	Trie *pTrie = root;
	int index;

	for(index = 0; index < 26; index++) {
		if(pTrie->hijos[index] != NULL)	
			cmdLiberar(pTrie->hijos[index]);
	}
	if(pTrie->hoja)
		free_word(pTrie);
	free(root);
}

void cmdMostrar(Trie *root){
	Trie *pTrie = root;
	int index;

	for(index = 0; index < 26; index++) {
		if(pTrie->hijos[index] != NULL)	
			cmdMostrar(pTrie->hijos[index]);
	}
	if(pTrie->hoja){
		printf("%s, ", pTrie->clave);
		conteo++;
		if(conteo == 7){
			printf("\n");
			conteo = 0;
		}
	}
}