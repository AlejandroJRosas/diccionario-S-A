#include "dictlib.h"

int main(int argc, char *argv[]){
	FILE *arch = NULL;
	Trie *root = getNode();
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
	printList(searchNode(root, "Hombre"));
	return 0;
}