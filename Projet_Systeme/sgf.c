#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sgf.h"
#include "bbb.h"

//formate le disque
void formatter(Disque* disque){
	int i;
	int j;

	for (i = 0; i < 15; i++)
	{
		//on passe tous les typefichiers à 0
		disque->inode[i].typefichier = 0;
		for (j = 0; j < 30; j++)
		{
			//on passe tous les blocs utilises à -1 (les  fichiers n'existent pas il n'utilisent donc aucun bloc)
			disque->inode[i].blocutilise[j] = -1;
		}
	}
	for(i = 0; i < 30; i++){
		//on passe tous les indicateurs des blocs à 0 pour signifier qu'ils ne sont pas occupés
		disque->bloc[i].occupe = 0;
		disque->bloc[i].donnees[0] = '\0';
	}
}

//Retourne l'inode d'un fichier à partir de son chemin
int inode_via_chemin(char* chemin, int position_courante, Disque* disque){
	//entier contenant l'inode de l'étape actuelle, initialement 0 qui est l'inode de /
	int inode;
	inode = 0;
	//entier dans lequel on va insérer l'inode de l'étape suivante
	int prochain_inode;

	//curseur marquant l'étape actuelle dans la recherche (le répertoire dans lequel on se trouve)
	int curseur;
	curseur = 0;

	//on ne lance la recherche que si le chemin recherché n'est pas / si c'est le cas l'inode reste 0 et on le renvoie
	if(strcmp(chemin,"/")){
		//tableau de chaines de caractères, chacune contient un répertoire du chemin
		char** etapes;
		//on découpe le chemin pour obtenir une liste des noms de répertoires + nom du fichier
		etapes = decouper(chemin, SGF_DELIMITEURS_CHEMIN);

		do{
			//on cherche le prochain inode dans le bloc courant
			if(!strcmp(etapes[curseur],".")){
				prochain_inode = position_courante;
			}else{
				prochain_inode = inode_via_repertoire(etapes[curseur], inode, disque);
			}
			//si on ne trouve pas l'inode suivant on crash
			if(prochain_inode == -1){
				fprintf(stderr,"Inode introuvable dans le répertoire %d",inode);
				exit(EXIT_FAILURE);
			//si on trouve l'inode on le place dans la variable inode, on incrémente le curseur pour passer au répertoire suivant
			}else{
				inode = prochain_inode;
				curseur++;
			}
		//on arrête la recherche lorsque le curseur atteint une chaine vide, cela signifie que l'étape actuelle est l'extrémité du chemin et que l'inode actuel est celui que l'on recherche
		}while(etapes[curseur] != NULL);
		//on désalloue le tableau des étapes
		free(etapes);
	}else{
		inode = 0;
	}
	//on renvoie l'inode
	return inode;
}

//Retourne l'inode du parent d'un fichier à partir de son chemin
int inode_parent_via_chemin(char* chemin, int position_courante, Disque* disque){
	//entier contenant l'inode de l'étape actuelle, initialement 0 qui est l'inode de /
	int inode;
	inode = 0;
	//entier dans lequel on va insérer l'inode de l'étape suivante
	int prochain_inode;

	//curseur marquant l'étape actuelle dans la recherche (le répertoire dans lequel on se trouve)
	int curseur;
	curseur = 0;

	char** etapes;
	//on découpe le chemin pour obtenir une liste des noms de répertoires + nom du fichier
	etapes = decouper(chemin, SGF_DELIMITEURS_CHEMIN);

	//on ne lance la recherche que si le chemin recherché n'est pas / si c'est le cas l'inode reste 0 et on le renvoie
	if(etapes[1] != NULL){
		//tableau de chaines de caractères, chacune contient un répertoire du chemin
		
		

		do{
			//on cherche le prochain inode dans le bloc courant
			if(!strcmp(etapes[curseur],".")){
				prochain_inode = position_courante;
			}else{
				prochain_inode = inode_via_repertoire(etapes[curseur], inode, disque);
			}
			//si on ne trouve pas l'inode suivant on crash
			if(prochain_inode == -1){
				fprintf(stderr,"Inode introuvable dans le répertoire %d",inode);
				exit(EXIT_FAILURE);
			//si on trouve l'inode on le place dans la variable inode, on incrémente le curseur pour passer au répertoire suivant
			}else{
				inode = prochain_inode;
				curseur++;
			}
		//on arrête la recherche lorsque le curseur atteint une chaine vide, cela signifie que l'étape actuelle est juste avant l'extrémité du chemin et que l'inode actuel est celui que l'on recherche
		}while(etapes[curseur+1] != NULL);
		//on désalloue le tableau des étapes
		
	}else{
		inode = 0;
	}
	free(etapes);
	//on renvoie l'inode
	return inode;
}

//retourne l'inode d'un fichier à partir de son nom et du répertoire où le chercher
int inode_via_repertoire(char* nom, int inode, Disque* disque){
	// entier dans lequel on insère l'inode trouvé ou -1 si on ne le trouve pas
	int retour;
	retour = -1;

	//tampon de vérification pour l'inode
	int check;

	//on extrait le contenu du répertoire
	char* contenu;
	contenu = contenu_fichier(inode,disque);

	//tableau de chaines de caractères 
	char** lignes;
	//on découpe le contenu du répertoire en une liste de lignes
	lignes = decouper(contenu, SGF_DELIMITEURS_REPERTOIRE);

	//itérateur pour parcourir les lignes;
	int i;
	i = 0;

	//pour chaque ligne on vérifie si elle contient l'inode recherché
	while(lignes[i] != NULL){
		check = inode_si_nom_dans_ligne(nom, lignes[i]);
		//si check est égal à -1 la ligne ne correspond pas au fichier recherché
		if(check != -1){
			//si check n'est pas égal à -1 alors la ligne contenait l'inode recherché et on le place dans inode
			retour = check;
		}
		i++;
	}
	//on désalloue le tableau de lignes
	free(lignes);
	//on désalloue le contenu du répertoire
	free(contenu);

	//on renvoie l'inode ou -1 si on ne l'a pas trouvé
	return retour;
}

//lit une ligne d'un répertoire et retourne l'inode si le nom de fichier est le nom recherché, -1 sinon
int inode_si_nom_dans_ligne(char* nom, char* ligne){
	//tableau de chaines de caractères contenant les différents champs contenu dans une ligne d'un répertoire
	char** donnees;
	//on découpe la ligne en champs
	donnees = decouper(ligne, SGF_DELIMITEURS_LIGNE_REPERTOIRE);
	//valeur de retour, contient l'inode ou -1 si le nom ne correspond pas
	int retour;
	retour = -1;

	//on vérifie si le premier champ (le nom du fichier) correspond au nom recherché
	if(!strcmp(donnees[0],nom)){
		//si oui on assigne le second champ (l'inode) à notre valeur de retour
		retour = atoi(donnees[1]);
	}

	//on désalloue le tableau de champs
	free(donnees);

	//on retourne l'inode ou -1 si le nom ne correspond pas
	return retour;
}

//retourne le contenu d'un fichier via son inode
char* contenu_fichier(int inode, Disque* disque){
	//nombre de blocs occupés par le fichier
	int nbrBlocs;
	nbrBlocs = 0;

	//curseur utilisé pour écrire le contenu du fichier dans notre tampon
	int curseur_ecriture;
	curseur_ecriture = 0;

	//curseur indiquant le bloc suivant à lire
	int bloc_suivant;
	bloc_suivant = 0;

	//tampon dans lequel on va écrire le contenu
	char* contenu;

	//on incrémente le nombre de blocs autant de fois que le fichier a de blocs utilisés
	while(disque->inode[inode].blocutilise[nbrBlocs] != -1){
		nbrBlocs++;
	}

	//on alloue le tampon en lui donnant la taille d'un bloc multipliée par le nombre de blocs que le fichier occupe
	contenu = malloc(sizeof(char)*SGF_TAILLE_BLOC*nbrBlocs);
	contenu[0] = '\0';


	//on teste l'allocation du tampon
	if(!contenu){
		fprintf(stderr, "Erreur d'allocation du contenu\n");
		exit(EXIT_FAILURE);
	}

	//on concatène le contenu de chaque bloc avec le tampon
	while(disque->inode[inode].blocutilise[curseur_ecriture] != -1){
		bloc_suivant = disque->inode[inode].blocutilise[curseur_ecriture];
		strcat(contenu,disque->bloc[bloc_suivant].donnees);
		curseur_ecriture++;
	}

	//on renvoie le tampon
	return contenu;
}

//recherche un bloc libre sur le disque
int bloc_libre(Disque* disque){
	//curseur initialisé à 1 car le bloc 0 est réservé à /
	int i;
	i = 1;
	
	//on incrémente le curseur jusqu'à atteindre un bloc non occupé
	while(disque->bloc[i].occupe && i<30){
		i++;
	}

	//si on a dépassé le dernier bloc (le 29e) on retourne une erreur
	if(i == 30){
		i == -1;
	}

	//on retourne le curseur
	return i;
} 

// attribue le bloc indiqué au fichier indiqué
void attribuer_bloc(int inode, int bloc, Disque* disque){
	//curseur utilisé pour itérer sur les blocutilise du fichier
	int i;
	i = 0;
	
	//on signale que le bloc est occupé
	disque->bloc[bloc].occupe = 1;

	//on incrémente le curseur jusqu'à trouver le premier blocutilise non initialisé
	while(disque->inode[inode].blocutilise[i] != -1){
		i++;
	}

	//On affiche une erreur si on dépasse le 29e et dernier blocutilise
	if (i > 29){
		fprintf(stderr,"Tous les blocutilises de l'inode %d sont occupés",inode);
	//sinon on y insère le bloc à attribuer
	}else{
		disque->inode[inode].blocutilise[i] = bloc;
	}
}

//écrit le texte indiqué dans le fichier indiqué de manière destructive
void ecrire_fichier(int inode, char* texte, Disque* disque){
	//curseur indiquant le bloc du fichier dans lequel écrire ensuite, initialisé à 0 car on commence toujours par écrire dans le premier bloc du fichier
	int curseur_bloc;
	curseur_bloc = 0;
	//curseur indiquant combien de caractères ont été écrits dans le fichier
	int curseur_texte;
	curseur_texte = 0;
	//curseur indiquant combien de caractères restent à écrire dans le fichier, initialisé à la longueur du texte à écrire
	int caracteres_a_ecrire;
	caracteres_a_ecrire = strlen(texte);

	//on boucle tant qu'il reste des caractères à écrire
	while(caracteres_a_ecrire > 0){
		//avant d'écrire dans un bloc on efface son contenu
		effacer_bloc(disque->inode[inode].blocutilise[curseur_bloc],disque);
		if(caracteres_a_ecrire > SGF_TAILLE_BLOC-2){
			//si il reste trop de caractères pour tous les écrire dans un bloc on rempli le bloc courant puis on arrête d'écrire
			strncpy(disque->bloc[disque->inode[inode].blocutilise[curseur_bloc]].donnees, &texte[curseur_texte],SGF_TAILLE_BLOC-2);
			//si le fichier n'a pas de bloc suivant déjà attribué on en attribue un nouveau
			if(disque->inode[inode].blocutilise[curseur_bloc+1] == -1){
				attribuer_bloc(inode,bloc_libre(disque),disque);
			}
			//on incrémente le curseur texte d'autant de caractères qu'on en a écrit
			curseur_texte+=SGF_TAILLE_BLOC-2;
			//on décrémente le curseur de caractères restants du nombre de caractères écrits
			caracteres_a_ecrire-=SGF_TAILLE_BLOC-2;
		}else{
			//on écrit la chaine entière dans le bloc
			strncpy(disque->bloc[disque->inode[inode].blocutilise[curseur_bloc]].donnees, &texte[curseur_texte],caracteres_a_ecrire);
			//on passe le bnombre de caratères restants à 0
			caracteres_a_ecrire = 0;
		}
		//on incrémente le curseur bloc pour passer au bloc suivant
		curseur_bloc++;
	}

	//si le fichier a des bloc attribués au delà de ceux dans lesquels on a écrit, on les libère
	while(disque->inode[inode].blocutilise[curseur_bloc] != -1){
		disque->bloc[disque->inode[inode].blocutilise[curseur_bloc]].occupe = 0;
		disque->inode[inode].blocutilise[curseur_bloc] = -1;
	}
}

//ajoute le texte indiqué à la suite du fichier indiqué
void ajouter_fichier(int inode, char* texte, Disque* disque){
	//pointeur qui pointera vers le contenu du fichier
	char* contenuFichier;
	//on extrait le contenu du fichier
	contenuFichier = contenu_fichier(inode, disque);
	//On calcule la taille combinée du contenu du fichier et du texte à ajouter
	int taille_combinee = strlen(contenuFichier)+strlen(texte)+1; // +1 pour le caractère de fermeture que strlen ne comptabilise pas
	//on réalloue la taille de notre chaine pour qu'elle puisse contenir l'ensemble du texte
	contenuFichier = (char*)realloc(contenuFichier, sizeof(char)*taille_combinee);

	//on vérifie que la réallocation a fonctionné
	if(!contenuFichier){
		fprintf(stderr,"Erreur de réallocation du contenu du fichier avant insertion du texte à ajouter");
		exit(EXIT_FAILURE);
	}

	//on concatène les textes
	strcat(contenuFichier, texte);

	//on écrit la concaténation des textes dans le fichier
	ecrire_fichier(inode, contenuFichier, disque);

	//on désaloue notre chaine concaténée
	free(contenuFichier);
	contenuFichier = NULL;
}

//efface le contenu du bloc indiqué
void effacer_bloc(int bloc, Disque* disque){
	//curseur permettant d'itérer sur les caractères du bloc
	int i;
	for ( i = 0; i < SGF_TAILLE_BLOC; i++)
	{
		//pour chaque caractère on le passe à \0
		disque->bloc[bloc].donnees[i] = '\0';
	}	
}

//retourne le nom d'un fichier via son chemin
char* nom_fichier_via_chemin(char* chemin){
	//curseur pour parcourir les étapes du chemin
	int curseur;
	curseur = 0;
	//tableau dans lequel on va mettre les étapes du chemin
	char** etapes;
	//nom du fichier
	char* nom;
	
	//on découpe le chemin en étapes
	etapes = decouper(chemin,SGF_DELIMITEURS_CHEMIN);

	//tant que l'étape suivante n'est pas vide on incrémente le curseur
	while(etapes[curseur+1] != NULL){
		curseur++;
	}

	//on duplique la dernière étape contenant le nom du fichier
	nom = strdup(etapes[curseur]);

	//on libère le tableau d'étapes 
	free(etapes);

	//on retourne le nom
	return nom;
}

int inode_libre(Disque* disque){
	//curseur que l'on va utiliser pour chercher l'inode
	int inode;
	//initialisation à 1 car l'inode de la racine ne doit jamais être attribué
	inode = 1;

	//tant que l'inode n'est pas libre on passe au suivant
	while(disque->inode[inode].utilise == 1){
		inode++;
	}

	//si on dépasse le dernier inode on crash
	if(inode > 14){
		fprintf(stderr,"Aucun inode disponible\n");
		exit(EXIT_FAILURE);
	}

	//on renvoie l'inode
	return inode;
}

//crée un fichier et retourne son inode
int creer_fichier(char* chemin, int position_courante, Disque* disque){
	//inode du fichier
	int inode;
	//inode du parent
	int inode_parent;
	//ligne de répertoire contenant les informations sur le fichier
	char ligne_fichier[SGF_TAILLE_LIGNE_REPERTOIRE];
	//nom du fichier
	char* nom_fichier;
	//copie du chemin
	char* copie_chemin;
	//premier bloc du fichier
	int premier_bloc;

	//on copie le chemin car on va le découper de manière destructive
	copie_chemin = strdup(chemin);
	//on trouve un inode libre sur le disque
	inode = inode_libre(disque);
	//on trouve un bloc libre sur le disque pour le premier bloc
	premier_bloc = bloc_libre(disque);
	//on attribue le bloc libre à l'inode libre
	attribuer_bloc(inode,premier_bloc,disque);
	//on indique que l'inode est désormais utilisé
	disque->inode[inode].utilise = 1;

	//on coupe le nom du fichier 
	nom_fichier = nom_fichier_via_chemin(chemin);

	//si le nom est trop long on crash
	if(strlen(nom_fichier) > SGF_TAILLE_NOM_FICHIER){
		fprintf(stderr,"Nom de fichier trop long (255 caractères max)\n");
		exit(EXIT_FAILURE);
	}

	//on cherche l'inode du parent à l'aide de la copie du chemin
	inode_parent = inode_parent_via_chemin(copie_chemin, position_courante,disque);
	//on compose la ligne de données à placer dans le parent
	sprintf(ligne_fichier,"%s;%d\n",nom_fichier,inode);
	//on ajoute la ligne au parent
	ajouter_fichier(inode_parent,ligne_fichier,disque); 

	//on libère le nom du fichier et la copie du chemin (tous les deux alloués par strdup)
	free(nom_fichier);
	free(copie_chemin);

	return inode;
}

//efface le contenu d'un fichier et libère tous ses blocs excepté le premier
void effacer_fichier(int inode, Disque* disque){
	//curseur
	int i;
	i = 0;

	//on efface tous les blocs utilises par le fichier
	while(disque->inode[inode].blocutilise[i] != -1){
		effacer_bloc(disque->inode[inode].blocutilise[i],disque);
		i++;
	}

	//on libère les blocs effacés excepté le premier bloc du fichier
	i = 1;
	while(disque->inode[inode].blocutilise[i] != -1){
		disque->inode[inode].blocutilise[i] = -1;
		disque->bloc[disque->inode[inode].blocutilise[i]].occupe = 0;
		i++;
	}
}

//supprimer un fichier via son chemin
void supprimer_fichier(char* chemin, int position_courante, Disque* disque){
	//inode du fichier
	int inode;
	//nom du fichier
	char* nom_fichier;
	//nom du parent
	int inode_parent;
	//copies du chemin pour obtenir le nom d fichier et l'inode du parent
	char* copie_chemin_nom_fichier;
	char* copie_chemin_inode_parent;
	
	//on duplique le chemin pour pouvoir le découper dans la recherche
	copie_chemin_nom_fichier = strdup(chemin);
	copie_chemin_inode_parent = strdup(chemin);

	//on recherche les infos nécessaires via les trois exemplaires du chemin
	inode = inode_via_chemin(chemin, position_courante, disque);
	nom_fichier = nom_fichier_via_chemin(copie_chemin_nom_fichier);
	inode_parent = inode_parent_via_chemin(copie_chemin_inode_parent, position_courante,disque);
	//on efface le contenu du fichier
	effacer_fichier(inode,disque);
	//on libère le premier bloc et l'inode
	disque->bloc[disque->inode[inode].blocutilise[0]].occupe = 0;
	disque->inode[inode].utilise = 0;
	disque->inode[inode].blocutilise[0] = -1;
	//on retire le fichier du répertoire parent
	retirer_ligne_repertoire(nom_fichier,inode_parent,disque);

	//on libère la mémoire dynamique
	free(copie_chemin_inode_parent);
	free(copie_chemin_nom_fichier);
	free(nom_fichier);
}


//retire une ligne du répertoire via le nom du fichier à retirer et l'inode du répertoire d'où le retitrer
void retirer_ligne_repertoire(char* nom_fichier, int inode_repertoire, Disque* disque){
	//curseur pour parcourir les lignes du répertoire
	int i;
	//contenu du répertoire
	char* contenu_repo;
	//tableau contenant les lignes du répertoire
	char** lignes_repo;
	//tampon dans lequel on copie chaque ligne pour conserver son intégrité pendant le test
	char* copie_ligne_courante;
	i = 0;
	//on extrait le contenu du répertoire
	contenu_repo = contenu_fichier(inode_repertoire,disque);
	//on découpe le contenu en lignes
	lignes_repo = decouper(contenu_repo, SGF_DELIMITEURS_REPERTOIRE);

	//on efface le contenu du répertoire afin de de le ré-écrire
	effacer_fichier(inode_repertoire,disque);

	//pour chaque ligne on ajoute son contenu au répertoire si elle ne contient pas le nom du fichier à retirer
	while(lignes_repo[i] != NULL){
		//le chemin est découpé durant le test, on utilise donc la copie pour l'ajout
		copie_ligne_courante = strdup(lignes_repo[i]);
		if(inode_si_nom_dans_ligne(nom_fichier,lignes_repo[i]) == -1){
			//on réalloue la mémoire pour la copie en lui ajoutant un octet pour le retour à la ligne
			copie_ligne_courante = realloc(copie_ligne_courante,sizeof(char)*strlen(copie_ligne_courante)+1);
			//on ajoute le retour à la ligne
			strncat(copie_ligne_courante,"\n",1);
			//on ajoute la ligne au fichier
			ajouter_fichier(inode_repertoire,copie_ligne_courante,disque);
		}
		//on libère la copie après chaque utilisation car strdup alloue un nouveau bloc à chaque appel
		free(copie_ligne_courante);
		i++;
	}
	//on libère le contenu et les lignes
	free(contenu_repo);
	free(lignes_repo);
}