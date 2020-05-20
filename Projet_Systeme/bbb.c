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
		arguments = decouper(entree, BBB_DELIMITEURS);

		//on affiche les arguments un à un (pour le moment)
		while(arguments[i] != NULL){
			printf("%s\n", arguments[i]);
			i++;
		}
		free(entree);
		free(arguments);
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

char** decouper(char* entree, char* delimiteurs){
	//Taille variable de la liste de découpes
	int taille_decoupes;
	taille_decoupes = BBB_TAILLE_DECOUPES;

	//Curseur contenant la case dans laquelle stocker la prochaine découpe dans le tableau de découpes
	int curseur;
	curseur = 0;

	//tableau alloué dynamiquement dans lequel on va stocker des pointeurs vers les découpes
	char** decoupes;
	decoupes = malloc(taille_decoupes * sizeof(char*));

	//pointeur vers la découpe en cours de traitement
	char* decoupe;

	//test de l'allocation dynamique du tableau de pointeurs
	if(!decoupes){
		fprintf(stderr, "Erreur d'allocation de la liste de découpes\n");
		exit(EXIT_FAILURE);
	}

	//on utilise strtok pour découper notre chaine de caractères via no délimiteurs, strok retourne un pointeur vers la première chaine de caractères
	decoupe = strtok(entree, delimiteurs);

	while(decoupe != NULL){
		//on insère le pointeur vers notre chaîne dans le tableau
		decoupes[curseur] = decoupe;

		//on incrémente le curseur pour l'itération suivante
		curseur++;

		//on compare la position du curseur à la taille actuelle du tableau
		if(curseur >= taille_decoupes){
			//si le curseur a atteint la taille du tableau, on augmente la taille du tableau
			//lors de chaque extension on ajoute la taille intiale du tableau
			taille_decoupes += BBB_TAILLE_DECOUPES;
			decoupes = realloc(decoupes, taille_decoupes * sizeof(char*));

			//test de la réallocation dynamique du tableau de pointeurs
			if(!decoupes){
				fprintf(stderr, "Erreur de réallocation de la liste d'arguments\n");
				exit(EXIT_FAILURE);
			}
		}

		//on récupère un pointeur vers la chaine de caractères suivante
		decoupe = strtok(NULL, delimiteurs);
	}
	//on insère NULL à la suite de nos pointeurs pour signaler la fin de la liste
	decoupes[curseur] = NULL;

	//on renvoie le tableau de découpes
	return decoupes;
}