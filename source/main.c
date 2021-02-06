#include "dictlib.h"

int main(int argc, char *argv[]){
	FILE *arch = NULL;
	Trie *root = getNode();
	int i;
	char word[32];

	//	Modo Comando
	if(argc > 1){
		if(!(strcmp(argv[1], "cargar"))){	//	cmdCargar
			for(i = 2; i < argc; i++)
				cmdCargar(argv[i]);
			exit(1);
		}
		else{
			if(!strcmp(argv[1], "limpiar")){	//	cmdLimpiar
				system("rm loaded.txt");
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
	}

	//	Modo Iterativo
	printf("  ----------------------\n");
	printf("    > DICCIONARIO S&A_\n");
	printf("  ----------------------\n");
	printf("\nQue desea realizar:\n\n");
	printf("\tcargar [nombre]\n");
	printf("\ts + [palabra]\n");
	printf("\ta + [palabra]\n");
	printf("\n\n\t> ");
	return 0;
}