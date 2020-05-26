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


void cat(char** arguments, int position,Disque* disque){

	//verif
	if (arguments[1] != NULL && arguments[2] == NULL){
		//afficher contenu fichier
		int inode = inode_via_chemin(arguments[1], position, disque);
		//test
		//ecrire_fichier(inode,"Bonjour et bienvenue sur \nle fichier",disque);
		
		char* contenu = contenu_fichier(inode,disque);
		printf("%s\n",contenu);
		free(contenu);
	}else if (arguments[2] != NULL && strcmp(arguments[2],">") == 0 && arguments[3] != NULL) {
		int inodesrc = inode_via_chemin(arguments[1], position, disque);
		int inodedest = inode_via_chemin(arguments[3], position, disque);

		//recup le contenu du fichier src
		char* contenusrc = contenu_fichier(inodesrc,disque);

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

void echo(char** arguments, int position, Disque* disque){

	//verif
	//commande a 2 arguments
	if (arguments[1] != NULL && arguments[2] == NULL){
		printf("%s\n",arguments[1]);

	} //ecriture dans un fichier
	else if(arguments[2]!= NULL && strcmp(arguments[2],">") == 0 && arguments[3]!= NULL)
	{
		int inode = 0;
		inode = inode_via_chemin(arguments[3], position,disque);
		if(!disque->inode[inode].typefichier){
			ecrire_fichier(inode,arguments[1],disque);
		}else{
			printf("L'écriture dans un répertoire n'est pas autorisée\n");
		}	
	}else{
		printf("Mauvaise utilisation de echo\n");
	}
	
}

void bbb_execution(char** arguments,int* position,Disque* disque)
{
		//fonction echo
	if (strcmp(arguments[0],"echo") == 0){
		echo(arguments, *position, disque);
		//fonction cat
	} else if (strcmp(arguments[0],"cat") == 0){
		cat(arguments, *position, disque);
		//fonction cd
	}else if (strcmp(arguments[0],"cd") == 0){
		cd(arguments, position, disque);
	}else if (strcmp(arguments[0],"ls") == 0){
		ls(arguments,*position,disque);
  	}else if (strcmp(arguments[0],"cp") == 0){
		cp(arguments, *position,disque);
	}else if (strcmp(arguments[0],"mv") == 0){
		mv(arguments, *position,disque);
  	}else if (strcmp(arguments[0],"mkf") == 0){
		mkf(arguments,*position,disque);
	}else if (strcmp(arguments[0],"mkdir") == 0){
		mkdir(arguments,*position,disque);
	}else if (strcmp(arguments[0],"rmdir") == 0){
		notre_rmdir(arguments, *position, disque);
	}else if (strcmp(arguments[0],"rm") == 0){
		notre_rm(arguments, *position, disque);
	}
}

void cp(char** arguments, int position, Disque* disque){
	//verif
	if (arguments[1] != NULL && arguments[2] != NULL){

		//verif fichier dest n'existe pas
		char* copie_chemin_parent = strdup(arguments[2]);
		char* copie_chemin_nom = strdup(arguments[2]);
		int inode_parent_dest = inode_parent_via_chemin(copie_chemin_parent, position, disque);
		//printf("inode parent :%d\n",inode_parent_dest);

		char* nomdest =nom_fichier_via_chemin(copie_chemin_nom);
		//printf("nom:%s\n",nomdest);


		if (inode_via_repertoire(nomdest,inode_parent_dest,disque) == -1){
			//le fichier n'existe pas

			//recup contenu du fichier src
			int inodesrc = inode_via_chemin(arguments[1], position, disque);
			char* contenu = contenu_fichier(inodesrc,disque);

			//creation du fichier destination
			int inodedest = creer_fichier(arguments[2], position, disque);
			//copie dans le fichier dest
			ecrire_fichier(inodedest,contenu,disque);

			char* contenudest = contenu_fichier(inodedest,disque);
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

void mv(char** arguments, int position, Disque* disque){
	//verif
	if (arguments[1] != NULL && arguments[2] != NULL){

		//verif fichier dest n'existe pas
		//copie chemin
		char* copie_chemin_parent = strdup(arguments[2]);
		char* copie_chemin_nom = strdup(arguments[2]);;
		char* copie_chemin_src = strdup(arguments[1]);

		int inode_parent_dest = inode_parent_via_chemin(copie_chemin_parent, position, disque);
		
		char* nomdest =nom_fichier_via_chemin(copie_chemin_nom);

		if (inode_via_repertoire(nomdest,inode_parent_dest,disque) == -1){
			//le fichier n'existe pas

			//recup contenu du fichier src
			int inodesrc = inode_via_chemin(copie_chemin_src, position, disque);
			char* contenu = contenu_fichier(inodesrc,disque);
			
			//creation du fichier destination
			int inodedest = creer_fichier(arguments[2], position, disque);
			//copie dans le fichier dest
			ecrire_fichier(inodedest,contenu,disque);

			//suppression premier fichier
			supprimer_fichier(arguments[1], position, disque);
			
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


void bbb_loop(Disque* disque){
	char* entree;
	char** arguments;
	int i;
	int position;
	position = 0;
	

	while(1){
		i=0;
		//affichage du curseur
		printf("[bbb]");

		//on lit l'entrée
		entree = bbb_lire_entree();
		
		//on découpe l'entrée en un tableau de pointeur vers les arguments
		arguments = decouper_guillemet(entree, BBB_DELIMITEURS);

		/*on affiche les arguments un à un (lors de tests)
		while(arguments[i] != NULL){
			printf("%s\n", arguments[i]);
			i++;
		}*/

		//execution de la commande
		bbb_execution(arguments,&position,disque);
		
		free(entree);
		i=0;
		while(arguments[i]!=NULL){
			free(arguments[i]);
			i++;
		}
		free(arguments);
		sauvegarder(disque);
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

//déplace la position actuelle dans le sgf
void cd(char** arguments, int* position, Disque* disque){
	int inode_dest;
	char* copie_chemin;
	char* nom_fichier;
	// si il n'y a pas de second argument on retourne à la racine
	if(arguments[1] == NULL){
		*position = 0;
	//sinon on déplace la position actuelle vers le repertoire indiqué par le chemin
	}else{
		copie_chemin = strdup(arguments[1]);
		inode_dest = inode_via_chemin(arguments[1], *position, disque);
		if(disque->inode[inode_dest].typefichier){
			*position = inode_dest;
		}else{
			nom_fichier = nom_fichier_via_chemin(copie_chemin);
			printf("%s n'est pas un répertoire\n",nom_fichier);
			free(nom_fichier);
		}
		free(copie_chemin);
	}
}

//affiche les fichiers contenus dans le répertoire courant ou dans le répertoire au chemin donné
void ls(char** arguments, int position, Disque* disque){
	//inode du répertoire indiqué
	int inode;
	//si on n'a pas de chemin on affiche le contenu du répertoire courant
	if(arguments[1] == NULL){
		afficher_noms(position,disque);
	//si on on a un chemin on affiche le contenu du répertoire indiqué
	}else{
		inode = inode_via_chemin(arguments[1],position,disque);
		afficher_noms(inode,disque);
	}
}

//affiche les noms des fichiers contenus dans un répertoire séparés par un espace
void afficher_noms(int inode_repertoire, Disque* disque){
	//curseur pour parcourir les lignes
	int i;
	//contenu du répertoire
	char* contenu;
	//lignes du répertoire
	char** lignes;
	//champs de chaque ligne
	char** donnees;

	int inode_ligne;

	i=0;
	//on extrait le contenu du répertoire
	contenu = contenu_fichier(inode_repertoire,disque);
	//on découpe le contenu en lignes
	lignes = decouper(contenu,SGF_DELIMITEURS_REPERTOIRE);
	
	//on découpe chaque ligne en champs et on affiche le premier champ qui contient le nom
	while(lignes[i] != NULL){
		donnees = decouper(lignes[i],SGF_DELIMITEURS_LIGNE_REPERTOIRE);
		inode_ligne = atoi(donnees[1]);
		if(disque->inode[inode_ligne].typefichier){
			//si le fichier est un répertoire on l'affiche en bleu
			printf("\033[1;34m");
			printf("%s ",donnees[0]);
			printf("\033[0m"); 
		}else{
			//autrement on l'affiche normalement
			printf("%s ",donnees[0]);
		}
		free(donnees);
		i++;
	}
	//on libère le contenu et les lignes
	free(contenu);
	free(lignes);

	//on termine la ligne sur l'affichage
	printf("\n");
}

//commande permettant de créer un fichier vide
void mkf(char** arguments, int position_courante, Disque* disque){
	//si il n'y a pas de chemin on affiche une erreur
	if(arguments[1] != NULL){
		//sinon on crée un fichier vide à l'emplacement indiqué
		creer_fichier_vide(arguments[1], position_courante, disque);
	}else{
		printf("Chemin manquant\n");
	}
}

//commande permettant de créer un répertoire vide
void mkdir(char** arguments, int position_courante, Disque* disque){
	//si il n'y a pas de chemin on affiche un message d'erreur
	if(arguments[1] != NULL){
		//sinon on crée un répertoire vide à l'emplacement indiqué
		creer_repertoire_vide(arguments[1],position_courante,disque);
	}else{
		printf("Chemin manquant\n");
	}
}

//commande permettant de supprimer un répertoire vide via son chemin
void notre_rmdir(char** arguments, int position, Disque* disque){
	//l'inode du répertoire à supprimer
	int inode;

	//copie du chemin pour récupérer l'inode
	char* copie_chemin_inode;

	//on vérifie si on a un chemin
	if(arguments[1] != NULL){
		//on vérifie si le fichier existe
		if(existe_fichier(arguments[1], position, disque)){
			//on copie le chemin
			copie_chemin_inode = strdup(arguments[1]);
			//on récupère l'inode via la copie du chemin
			inode = inode_via_chemin(copie_chemin_inode,position,disque);
			//on vérifie si le fichier est un répertoire
			if(disque->inode[inode].typefichier){
				//on vérifie si le répertoire est vide
				if(est_repertoire_vide(arguments[1], position, disque)){
					//si toutes les conditions sont remplies on supprime le répertoire
					supprimer_fichier(arguments[1], position, disque);
				}else{
					//si le répertoire n'est pas vide on affiche une erreur
					printf("Ce répertoire n'est pas vide\n");
				}
			}else{
				//si le fichier n'est pas un répertoire on affiche un message d'erreur
				printf("Ce fichier n'est pas un répertoire\n");
			}
			//on libère la copie du chemin
			free(copie_chemin_inode);
		}else{
				//si le fichier n'existe pas on affiche un message d'erreur
				printf("Ce répertoire n'existe pas\n");
			}
	}else{
		//si on a pas de chemin on affiche un message d'erreur
		printf("Chemin manquant\n");
	}
}

//commande permettant de supprimer un fichier via son chemin
void notre_rm(char** arguments, int position, Disque* disque){
	//inode du fichier à supprimer
	int inode;
	//copie du chemin pour récupérer l'inode
	char* copie_chemin_inode;

	//on vérifie si on a un chemin
	if(arguments[1] != NULL){
		//on vérifie si le fichier existe
		if(existe_fichier(arguments[1],position,disque)){
			//on copie le chemin
			copie_chemin_inode = strdup(arguments[1]);
			//on récupère l'inode via la copie du chemin
			inode = inode_via_chemin(copie_chemin_inode, position, disque);
			//on vérifie si le fichier est bien un simple fichier et non un répertoire
			if(!disque->inode[inode].typefichier){
				//si les conditions sont remplies on supprime le fichier
				supprimer_fichier(arguments[1], position, disque);
			}else{
				//si le fichier est un répertoire on affiche un message d'erreur 
				printf("Ce fichier est un répertoire, utilisez rmdir\n");
			}
			//on libère la copie du chemin
			free(copie_chemin_inode);
		}else{
			//si le fichier n'existe pas on affiche un message d'erreur
			printf("Ce fichier n'existe pas\n");
		}
	}else{
		//si on n'a pas de chemin on affiche une erreur
		printf("Chemin manquant\n");
	}
}