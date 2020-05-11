#include <stdio.h>
#include <stdlib.h>

//Macro gouvernant la taille du buffer de lecture utilisé dans bbb_lire_entree()
#define BBB_TAILLE_BUFFER 128

// lit l'entrée au clavier
char* bbb_lire_entree();

void bbb_loop(){
	char* entree;

	while(1){
		//affichage du curseur
		printf("[bbb]");
		//on lit l'entrée
		entree = bbb_lire_ligne();
		//on affiche l'entrée... pour le moment :)
		printf("entree : %s\n",entree);
	}
}

int main(int argc, char const *argv[])
{
	//point d'entrée du programme
	bbb_loop();

	return 0;
}

char* bbb_lire_entree(){

	//Taille variable du buffer, elle est augmentée lorsque que le buffer est rempli
	int taille_buffer = BBB_TAILLE_BUFFER;

	//curseur contenant la position du caractère à lire lors de l'itération actuelle
	int curseur = 0;

	//buffer alloué dynamiquement dans lequel sont stockés les caractères de l'entrée un par un
	char *buffer = malloc(sizeof(char)*taille_buffer);

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