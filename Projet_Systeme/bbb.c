#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sgf.h"
#include "bbb.h"


char** decouper_guillemet(char* entree, char* delimiteurs){
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

	//on regarde si on a des guillemets
	int i =0, long_chaine=0,long_chaine_actuel=0;
	int guillemet = 0;
	int posguill=0;
	int nbmotguill;
	int tabguil=0;
	int j=0;
	char** decoupesguill;
	decoupesguill = malloc(taille_decoupes * sizeof(char*));


	for (i=0;i<curseur;i++){
		//ouverture guillemet
		long_chaine_actuel = strlen(decoupes[i]);
		guillemet = 1;
		if(decoupes[i][0] == '"'){ //mot avec " en début
			
			if (decoupes[i][long_chaine_actuel-1]!= '"'){ //si on a plusieurs mots dans guillements
			nbmotguill = 0;
			posguill=i;
			long_chaine = strlen(decoupes[i+1]);
			
			//chaine = decoupes[i];
			while(decoupes[i+1][long_chaine-1] != '"'){
				nbmotguill++;
				decoupes[posguill] = ajout_a_la_suite(decoupes[posguill],decoupes[i+1]);
				i++;
				long_chaine = strlen(decoupes[i+1]);
			}
			nbmotguill = nbmotguill +2; //le premier et el dernier
			decoupes[posguill] = ajout_a_la_suite(decoupes[posguill],decoupes[i+1]);
			i++;
			
			//chaine avec ""
			decoupesguill[tabguil] = strdup(decoupes[posguill]);

			//printf("ajout motavec:%s\n",decoupesguill[tabguil]);	
			j = j + nbmotguill;
			tabguil++;

			}else{ //un seul mot dans les guillemets
				//ajout du mot sans les guillemets
				decoupesguill[tabguil]= strdup(suppr_guillemet(decoupes[j]));
				//printf("ajout motavecseul:%s\n",decoupesguill[tabguil]);	
				j++;
				tabguil++;

			}
			
		}else{ //mot sans guillement
			decoupesguill[tabguil]=strdup(decoupes[j]);
			//printf("ajout motsans:%s\n",decoupes[tabguil]);
			j++;
			tabguil++;
		}
		

	}
	//on renvoie le tableau de découpes

	decoupesguill[j] = NULL;
	free(decoupes);
	return decoupesguill;
	
	
}

//créer la phrase compris dans les guillemets
char* ajout_a_la_suite(char* chaine,char* ajout){

	int i,j,z=0;
	int long_chaine = strlen(chaine);
	int long_ajout = strlen(ajout);

	//supression "
	if(chaine[0]=='"'){
		i=1;
	}else{
		i=0;
	}

	if(ajout[strlen(ajout)-1]=='"'){
		long_ajout--;
	}
	
	//ajout du premier mot
	for (i;i<long_chaine;i++){
		chaine[z] = chaine[i];
		z++;
	}

	//ajout de l'espace
	chaine[z] = ' ';
	z++;

	//ajout du deuxieme mot
	for(j=0;j<long_ajout;j++){
		chaine[z] = ajout[j];
		z++;
		
	}

	chaine[z] = '\0';
	return chaine;
}

//supprime les guillemets entourant un mot ex:"test"
char* suppr_guillemet(char* chaine){ 
	int i,z=0;

	//on décale chaque caractère a gauche pour supprimer le "
	//on s'arrete avant le dernier "
	for(i=1;i<strlen(chaine)-1;i++){
		chaine[z] = chaine[i];
		z++;
	}
	//on supprime les caractères qui suivent
	chaine[z] = '\0';
	chaine[z+1] = '\0';

	return chaine;
}


void cat(char** arguments,Disque* disque){

	//verif
	if (arguments[1] != NULL && arguments[2] == NULL){
		//afficher contenu fichier
		int inode = inode_via_chemin(arguments[1],disque);
		//test
		//ecrire_fichier(inode,"Bonjour et bienvenue sur \nle fichier",disque);
		
		char* contenu = contenu_fichier(inode,disque);
		printf("%s\n",contenu);
		free(contenu);
	}else if (arguments[2] != NULL && strcmp(arguments[2],">") == 0 && arguments[3] != NULL) {
		int inodesrc = inode_via_chemin(arguments[1],disque);
		int inodedest = inode_via_chemin(arguments[3],disque);

		//recup le contenu du fichier src
		char* contenusrc = contenu_fichier(inodesrc,disque);
		printf("contenu source:%s\n",contenusrc);

		//test cat
		//char* contenudest = contenu_fichier(inodedest,disque);
		//printf("contenu dest avant cat:%s\n",contenudest);

		//copie le contenu dans le fichier dest
		ecrire_fichier(inodedest,contenusrc,disque);

		//test cat
		//contenudest = contenu_fichier(inodedest,disque);
		//printf("contenu dest:%s\n",contenudest);
		//free(contenudest);

		free(contenusrc);
	}else{
		printf("Mauvaise utilisation de cat\n");
	}
}

void echo(char** arguments,Disque* disque){

	//verif
	//commande a 2 arguments
	if (arguments[1] != NULL && arguments[2] == NULL){
		printf("%s\n",arguments[1]);

	} //ecriture dans un fichier
	else if(arguments[2]!= NULL && strcmp(arguments[2],">") == 0 && arguments[3]!= NULL)
	{

		int inode = 0;
		inode = inode_via_chemin(arguments[3],disque);
		//printf("inode= %d\n",inode);
		char* contenuavant = contenu_fichier(inode,disque);
		ecrire_fichier(inode,arguments[1],disque);
		char* contenuapres = contenu_fichier(inode,disque);
		printf("avant:%s\n",contenuavant);
		printf("après:%s\n",contenuapres);

		free(contenuavant);
		free(contenuapres);
	}else{
		printf("Mauvaise utilisation de echo\n");
	}
	
}

void cp(char** arguments,Disque* disque){
	//verif
	if (arguments[1] != NULL && arguments[2] != NULL){

		//verif fichier dest n'existe pas
		char* copie_chemin_parent = strdup(arguments[2]);
		char* copie_chemin_nom = strdup(arguments[2]);
		int inode_parent_dest = inode_parent_via_chemin(copie_chemin_parent,disque);
		//printf("inode parent :%d\n",inode_parent_dest);

		char* nomdest =nom_fichier_via_chemin(copie_chemin_nom);
		//printf("nom:%s\n",nomdest);


		if (inode_via_repertoire(nomdest,inode_parent_dest,disque) == -1){
			//le fichier n'existe pas

			//recup contenu du fichier src
			int inodesrc = inode_via_chemin(arguments[1],disque);
			char* contenu = contenu_fichier(inodesrc,disque);
			printf("source:%s\n",contenu);

			//creation du fichier destination
			int inodedest = creer_fichier(arguments[2], disque);
			//copie dans le fichier dest
			ecrire_fichier(inodedest,contenu,disque);

			char* contenudest = contenu_fichier(inodedest,disque);
			printf("dest:%s\n",contenudest);
			free(contenudest);

			free(contenu);

		}else{
			//le fichier existe deja
			printf("Le fichier de destination existe déja !\n");
		}

		free(nomdest);
		free(copie_chemin_parent);
		free(copie_chemin_nom);
		
	}

}

void mv(char** arguments,Disque* disque){
	//verif
	if (arguments[1] != NULL && arguments[2] != NULL){

		//verif fichier dest n'existe pas
		//copie chemin
		char* copie_chemin_parent = strdup(arguments[2]);
		char* copie_chemin_nom = strdup(arguments[2]);;
		char* copie_chemin_src = strdup(arguments[1]);

		int inode_parent_dest = inode_parent_via_chemin(copie_chemin_parent,disque);
		
		char* nomdest =nom_fichier_via_chemin(copie_chemin_nom);

		if (inode_via_repertoire(nomdest,inode_parent_dest,disque) == -1){
			//le fichier n'existe pas

			//recup contenu du fichier src
			int inodesrc = inode_via_chemin(copie_chemin_src,disque);
			char* contenu = contenu_fichier(inodesrc,disque);
			
			//creation du fichier destination
			int inodedest = creer_fichier(arguments[2], disque);

			//copie dans le fichier dest
			ecrire_fichier(inodedest,contenu,disque);

			//suppression premier fichier
			supprimer_fichier(arguments[1],disque);
			
			free(contenu);

		}else{
			//le fichier existe deja
			printf("Le fichier de destination existe déja !\n");
		}

		free(nomdest);
		free(copie_chemin_parent);
		free(copie_chemin_nom);
		free(copie_chemin_src);
		
	}

}

void bbb_execution(char** arguments,Disque* disque)
{
		//fonction echo
	if (strcmp(arguments[0],"echo") == 0){
		echo(arguments,disque);
	}else if (strcmp(arguments[0],"cat") == 0){
		cat(arguments,disque);
	}else if (strcmp(arguments[0],"cp") == 0){
		cp(arguments,disque);
	}else if (strcmp(arguments[0],"mv") == 0){
		mv(arguments,disque);
	}
}


void bbb_loop(Disque* disque){
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
		arguments = decouper_guillemet(entree, BBB_DELIMITEURS);

		//on affiche les arguments un à un (pour le moment)
		while(arguments[i] != NULL){
			printf("%s\n", arguments[i]);
			i++;
		}

		//execution de la commande
		bbb_execution(arguments,disque);
		

		free(entree);
		i=0;
		while(arguments[i]!=NULL){
			free(arguments[i]);
			i++;
		}
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