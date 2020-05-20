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

	//le bloc dans lequel on va chercher l'inode suivant, initialement 0 qui est le premier bloc de /
	int bloc;
	bloc = 0;

	//le curseur marquant le bloc dans lequel on est en train de chercher l'inode suivant dans le cas où le répertoire actuel occupe plusieurs blocs
	int curseur_bloc;
	curseur_bloc = 0;

	//on ne lance la recherche que si le chemin recherché n'est pas / si c'est le cas l'inode reste 0 et on le renvoie
	if(strcmp(chemin,"/")){
		//tableau de chaines de caractères, chacune contient un répertoire du chemin
		char** etapes;
		//on découpe le chemin pour obtenir une liste des noms de répertoires + nom du fichier
		etapes = decouper(chemin, SGF_DELIMITEURS_CHEMIN);

		do{
			//on cherche le prochain inode dans le bloc courant
			prochain_inode = inode_via_bloc(etapes[curseur], bloc, disque);
			//si on ne trouve pas l'inode et que le répertoire occupe un autre bloc on passe à son bloc suivant
			if(prochain_inode == -1 && disque.inode[inode].blocutilise[curseur_bloc+1] != -1){
				curseur_bloc++;
			//si on ne trouve pas l'inode et que le répertoire n'occupe pas d'autres blocs on crash
			}else if(prochain_inode == -1){
				fprintf(stderr,"Inode introuvable dans le bloc %d",bloc);
				exit(EXIT_FAILURE);
			//si on trouve l'inode on le place dans la variable inode, on incrémente le curseur pour passer au répertoire suivant et on remet le curseur_bloc à 0 pour chercher dans le premier bloc du répertoire suivant
			}else{
				inode = prochain_inode;
				curseur++;
				curseur_bloc = 0;
			}
			// on assigne le numéro de bloc de la recherche suivante en fonction d'inode et curseur_bloc
			bloc = disque.inode[inode].blocutilise[curseur_bloc];
		//on arrête la recherche lorsque le curseur atteint une chaine vide, cela signifie que l'étape actuelle est l'extrémité du chemin et que l'inode actuel est celui que l'on recherche
		}while(etapes[curseur] != NULL);
		//on désalloue le tableau des étapes
		free(etapes);
	}
	//on renvoie l'inode
	return inode;
}

//retourne l'inode d'un fichier à partir de son nom et du bloc où le chercher
int inode_via_bloc(char* nom, int bloc, Disque disque){
	// entier dans lequel on insère l'inode trouvé ou -1 si on ne le trouve pas
	int inode;
	inode = -1;

	//tampon de vérification pour l'inode
	int check;

	//tableau de chaines de caractères 
	char** lignes;
	//on découpe le contenu du bloc en une liste de lignes
	lignes = decouper(disque.bloc[bloc].donnees, SGF_DELIMITEURS_REPERTOIRE);

	//itérateur pour parcourir les lignes;
	int i;
	i = 0;

	//pour chaque ligne on vérifie si elle continet l'inode recherché
	while(lignes[i] != NULL){
		check = inode_si_nom_dans_ligne(nom, lignes[i]);
		//si check est égal à -1 la ligne ne correspond pas au fichier recherché
		if(check != -1){
			//si check n'est pas égal à -1 alors la ligne contenait l'inode recherché et on le place dans inode
			inode = check;
		}
		i++;
	}
	//on désalloue le tableau de lignes
	free(lignes);

	//on renvoie l'inode ou -1 si on ne l'a pas trouvé
	return inode;
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