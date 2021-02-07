#include "dictlib.h"

int main(int argc, char *argv[]){
	FILE *arch = NULL;
	Trie *root = getNode();
	size_t bytesLongitud;
	ssize_t bytesLeidos;
	char *cadena, arg[8][24], word[24];
	int i, cont, cc;

	//	Modo Comando
	if(argc > 1){
		if(!(strcmp(argv[1], "cargar"))){	//	cmdCargar
			for(i = 2; i < argc; i++){
				if(cargarTrie(root, argv[i])){
					cmdCargar(argv[i]);
					printf("Carga de -%s- exitosa!\n", argv[i]);
				}
			}
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
				else{	
					if((!(strcmp(argv[1], "s"))) || (!(strcmp(argv[1], "a")))){	//	cmdPalabra
						if((arch = fopen("loaded.txt", "r")) == NULL){
							printf("Es necesario asignar el archivo a cargar ");
							printf("por medio del comando cargar + nombre\n");
						}
						else{
							while(!feof(arch)){
								fscanf(arch, "%s", word);
								cargarTrie(root, word);
							}
							fclose(arch);
						}
						if(!strcmp(argv[1], "s")){
							cmdPalabra(root, argv[2], 1);
							search(root, argv[2]);
							exit(1);
						}
						else{
							cmdPalabra(root, argv[2], 0);
							search(root, argv[2]);
							exit(1);
						}
					}
					else{	
						if(!strcmp(argv[1], "e")){	//	cmdExpresion
							if((arch = fopen("loaded.txt", "r")) == NULL){
								printf("Es necesario asignar el archivo a cargar ");
								printf("por medio del comando cargar + nombre\n");
							}
							else{
								while(!feof(arch)){
									fscanf(arch, "%s", word);
									cargarTrie(root, word);
								}
								fclose(arch);
							}
							for(i = 2; i < argc; i++){
								cmdPalabra(root, argv[i], 1);
								cmdPalabra(root, argv[i], 0);
							}
							exit(1);
						}
						else{	//	Comando Erroneo
							printf("Error comando -%s- no encontrado\n", argv[1]);
							exit(1);
						}
					}
				}
			}
		}
	}


	/*///////////////////////////////////////////////////////*/

	//	Modo Iterativo
	cadena = NULL;
	while(1){
		printf(">");

		//	Lectura de linea
		bytesLongitud = 0;
		bytesLeidos = getline(&cadena, &bytesLongitud, stdin);
		if(bytesLeidos == -1){
			puts("Error de getline\n");
			exit(1);
		}
		/*******************************************************/

		//	Sacado de argumentos y guardado en vector
		cc = cont = 0;
		for(i = 0; (cadena[i] != '\0') && (cadena[i] != '\n'); i++){
			if(cadena[i] != ' '){
            	arg[cont][cc] = cadena[i];
				cc++;
			}
			else
				arg[cont][cc] = '\0';
			if((cadena[i] == ' ') && (cadena[i + 1] != ' ')){
				cont++;
				cc = 0;
			}
		}
		arg[cont][cc] = '\0';
		for(i = 0; i < cont + 1; i++)
			strcpy(arg[i], strLow(arg[i]));
		/*********************************************************/

		//	cmdCargar
		if(!(strcmp(arg[0], "cargar"))){
			if(cont == 0){
				if((arch = fopen("loaded.txt", "r")) == NULL){
					printf("Es necesario asignar el archivo a cargar ");
					printf("por medio del comando cargar + nombre\n");
				}
				else{
					while(!feof(arch)){
						fscanf(arch, "%s", word);
						cargarTrie(root, word);
						printf("Carga de -%s- exitosa!\n", word);
						
					}
					fclose(arch);
				}
			}
			else{
				for(i = 1; i < cont + 1; i++){
					if(cargarTrie(root, arg[i])){
						cmdCargar(arg[i]);
						printf("Carga de -%s- exitosa!\n", arg[i]);
					}
				}
			}
		}
		/***************************************************************/

		else{

			//	cmdLimpiar
			if(!strcmp(arg[0], "limpiar"))
				system("rm loaded.txt");
			/******************************/

			else{
				
				//	cmdAyuda
				if(!(strcmp(arg[0], "ayuda")))
					cmdAyuda();
				/*******************************/

				else{

					//	cmdPalabra
					if((!(strcmp(arg[0], "s"))) || (!(strcmp(arg[0], "a")))){
						if(!strcmp(arg[0], "s"))
							cmdPalabra(root, arg[1], 1);
						else
							cmdPalabra(root, arg[1], 0);
					}
					/***********************************************************/

					else{

						//	cmdExpresion
						if((!strcmp(arg[0], "e"))){
							cmdExpresion(root, cadena);
							exit(1);
						}

						else{

						//	cmdSalir
						if(!strcmp(arg[0], "salir"))
							exit(1);
						/******************************/

						else	//	Comando Erroneo
							printf("Error comando -%s- no encontrado\n", argv[1]);
						}
					}
				}
			}
		}
		free(cadena);
	}
	return 0;
}