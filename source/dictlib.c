#include "dictlib.h"

int flag;

//*
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

//*
void printList(Trie *listp, int modo){
	printf("--> ");
    if(modo){
        Node *pNode = listp->sinonimos;
	    for(; pNode != NULL; pNode = pNode->next)
		    printf("%s --> ", pNode->item);
	    printf("NULL\n");
    }
    else{
        Node *pNode = listp->antonimos;
        for(; pNode != NULL; pNode = pNode->next)
		    printf("%s --> ", pNode->item);
	    printf("NULL\n");
    }
}

//*
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

//*
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

//*
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

//*
void insertTrie(Trie *root, const char *key){
    Trie *pTrie = root;
	int level, length = strlen(key), index;

	for (level = 0; level < length; level++){
		index = CharToIndex(key[level]);
		pTrie->sinonimos = NULL;
		pTrie->antonimos = NULL;
		if (!(pTrie->hijos[index]))
			pTrie->hijos[index] = getNode();
		pTrie = pTrie->hijos[index];
	}
	pTrie->hoja = 1;
}

//*
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

//*
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

//*
void addList(Trie *root, char *w1, char *w2, int modo){
    Trie *listp;

    if(modo){       //  Lista de Sinonimos
        listp = searchNode(root, w1);
        listp->sinonimos = insertList(listp->sinonimos, new_item(w2));
        listp = searchNode(root, w2);
        listp->sinonimos = insertList(listp->sinonimos, new_item(w1));
    }
    else{           //  Lista de Antonimos
        listp = searchNode(root, w1);
        listp->antonimos = insertList(listp->antonimos, new_item(w2));
        listp = searchNode(root, w2);
        listp->antonimos = insertList(listp->antonimos, new_item(w1));
    }
}

//*
void cargarTrie(Trie *root, char *name){
	FILE *arch;
    char word[32], aux[32];
	int modo;

	//	Verifica el archivo que cargara si este no existe aborta.
	if((arch = fopen(name, "r")) == NULL){
		fprintf(stderr, "Error: No se pudo abrir %s\n", name);
		exit(1);
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

//*
int cmdCargar(char *name){
	FILE *arch = NULL;
    char word[32];

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

void cmdPalabra();

void cmdExpresion();

void cmdAyuda(void){
	printf("Comandos Disponibles\n\n ");
	printf("> cargar + [Nombre]   -- Asigna los archivos que deseas abrir con el modo comando o carga archivo en modo iterativo\n ");
	printf("> limpiar             -- Renueva la memoria del comando cargar\n ");
    printf("> s + [Palabra]       -- Enlista los sinonimos de la palabra ingresada\n ");
	printf("> a + [Palabra]       -- Enlista los antonimos de la palabra ingresada\n ");
	printf("> e + [Expresion]     -- Enlista los sinonimos y antonimos de las palabras que componen la expresion\n ");
	printf("> carch               -- Carga los archivos que abririas normalmente en modo comando (Solo en Modo Iterativo)\n ");
    printf("> ayuda               -- Muestra los comandos disponibles\n ");
	printf("> x                   -- Finaliza la ejecucion del programa (Solo para Modo Iterativo)\n");
}