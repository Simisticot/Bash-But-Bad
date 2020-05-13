#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bbb.h"

void bbb_loop(){
	char* entree;
	char** arguments;
	int i;
	

	while(1){
		i=0;
		//affichage du curseur
		printf("[bbb]");

		//on lit l'entrée
		entree = bbb_lire_entree();
		
		//on découpe l'entrée en un tableau de pointeur vers les arguments
		arguments = bbb_decouper_entree(entree);

		//on affiche les arguments un à un (pour le moment)
		while(arguments[i] != NULL){
			printf("%s\n", arguments[i]);
			i++;
		}
	}
}

char* bbb_lire_entree(){

	//Taille variable du buffer, elle est augmentée lorsque que le buffer est rempli
	int taille_buffer;
	taille_buffer = BBB_TAILLE_BUFFER;

	//curseur contenant la position du caractère à lire lors de l'itération actuelle
	int curseur;
	curseur = 0;

	//buffer alloué dynamiquement dans lequel sont stockés les caractères de l'entrée un par un
	char *buffer;
	buffer = malloc(sizeof(char)*taille_buffer);

	//variable dans laquelle on stocke le caractère lu avant de l'insérer dans le buffer
	//tete_lecture est un int pour pouvoir le comparer à EOF (End Of File)
	int tete_lecture;

	//test de l'allocation dynamique du buffer
	if(!buffer){
		fprintf(stderr, "Erreur d'allocation du buffer de lecture.\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		//on stocke un caractère de l'entrée dans la variable intermédiaire
		tete_lecture = getchar();

		//on vérifie si le caractère est une fin de fichier ou de ligne
		if(tete_lecture == EOF || tete_lecture == '\n'){
			//si c'est le cas on insère une fin de chaine de caractère et on retourne le buffer
			buffer[curseur]= '\0';
			return buffer;
		}else{
			//sinon on insère le caractère lu dans le buffer et on continue
			buffer[curseur] = tete_lecture;
		}
		//on incrémente le curseur pour l'itération suivante
		curseur++;

		//on compare le curseur à la taille actuelle du buffer
		if(curseur >= taille_buffer){
			//si le curseur a atteint la taille du buffer, on augmente la taille du buffer
			//lors de chaque extension on ajoute la taille initiale
			taille_buffer += BBB_TAILLE_BUFFER;
			//on réalloue le buffer pour lui donné la taille fraichement étendue
			buffer = realloc(buffer, taille_buffer);

			//test de la réallocation dynamique du buffer
			if(!buffer){
				fprintf(stderr, "Erreur de réallocation du buffer de lecture.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

char** bbb_decouper_entree(char* entree){
	//Taille variable de la liste d'arguments
	int taille_arguments;
	taille_arguments = BBB_TAILLE_ARGUMENTS;

	//Curseur contenant la case dans laquelle stocker le prochain argument dans le tableau arguments
	int curseur;
	curseur = 0;

	//tableau alloué dynamiquement dans lequel on va stocker des pointeurs vers les arguments
	char** arguments;
	arguments = malloc(taille_arguments * sizeof(char*));

	//pointeur vers l'argument en cours de traitement
	char* argument;

	//test de l'allocation dynamique du tableau de pointeurs
	if(!arguments){
		fprintf(stderr, "Erreur d'allocation de la liste d'arguments");
		exit(EXIT_FAILURE);
	}

	//on utilise strtok pour découper notre entrée via no délimiteurs, strok retourne un pointeur vers la première chaine de caractères
	argument = strtok(entree, BBB_DELIMITEURS);

	while(argument != NULL){
		//on insère le pointeur vers notre chaîne dans le tableau
		arguments[curseur] = argument;

		//on incrémente le curseur pour l'itération suivante
		curseur++;

		//on compare la position du curseur à la taille actuelle du tableau
		if(curseur >= taille_arguments){
			//si le curseur a atteint la taille du tableau, on augmente la taille du tableau
			//lors de chaque extension on ajoute la taille intiale du tableau
			taille_arguments += BBB_TAILLE_ARGUMENTS;
			arguments = realloc(arguments, taille_arguments * sizeof(char*));

			//test de la réallocation dynamique du tableau de pointeurs
			if(!arguments){
				fprintf(stderr, "Erreur de réallocation de la liste d'arguments\n");
			}
		}

		//on récupère un pointeur vers la chaine de caractères suivante
		argument = strtok(NULL, BBB_DELIMITEURS);
	}
	//on insère NULL à la suite de nos pointeurs pour signaler la fin de la liste
	arguments[curseur] = NULL;

	//on renvoie le tableau d'arguments
	return arguments;
}