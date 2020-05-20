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
}

//Retourne l'inode d'un fichier à partir de son chemin absolu
int inode_via_chemin(char* chemin, Disque disque){
	//entier contenant l'inode de l'étape actuelle, initialement 0 qui est l'inode de /
	int inode;
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
			prochain_inode = inode_via_repertoire(etapes[curseur], inode, disque);
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
	}
	//on renvoie l'inode
	return inode;
}

//retourne l'inode d'un fichier à partir de son nom et du répertoire où le chercher
int inode_via_repertoire(char* nom, int inode, Disque disque){
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
char* contenu_fichier(int inode, Disque disque){
	//nombre de blocs occupés par le fichier
	int nbrBlocs;
	nbrBlocs = 0;

	//curseur utilisé pour écrire le contenu du fichier dans notre tampon
	int curseur_ecriture;
	curseur_ecriture = 0;

	//curseur indiquant le bloc suivant à lire
	int bloc_suivant;

	//tampon dans lequel on va écrire le contenu
	char* contenu;

	//on incrémente le nombre de blocs autant de fois que le fichier a de blocs utilisés
	while(disque.inode[inode].blocutilise[nbrBlocs] != -1){
		nbrBlocs++;
	}

	//on alloue le tampon en lui donnant la taille d'un bloc multipliée par le nombre de blocs que le fichier occupe
	contenu = malloc(sizeof(char)*SGF_TAILLE_BLOC*nbrBlocs);

	//on teste l'allocation du tampon
	if(!contenu){
		fprintf(stderr, "Erreur d'allocation du contenu\n");
		exit(EXIT_FAILURE);
	}

	//on concatène le contenu de chaque bloc avec le tampon
	while(disque.inode[inode].blocutilise[curseur_ecriture] != -1){
		bloc_suivant = disque.inode[inode].blocutilise[curseur_ecriture];
		strcat(contenu,disque.bloc[bloc_suivant].donnees);
		curseur_ecriture++;
	}
	//on renvoie le tampon
	return contenu;
}