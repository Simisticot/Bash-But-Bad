#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sgf.h"
#include "bbb.h"

void direfichier(){
	printf("fichier");
}

void formatter(Disque* disque){
	int i;
	int j;

	for (i = 0; i < 15; i++)
	{
		disque->inode[i].typefichier = 0;
		for (j = 0; j < 30; j++)
		{
			disque->inode[i].blocutilise[j] = -1;
		}
	}
}

int inode_via_chemin(char* chemin, Disque disque){
	int inode;

	char** etapes;
	etapes = decouper(chemin, SGF_DELIMITEURS_CHEMIN);

	int curseur;
	curseur = 0;

	int bloc;
	bloc = 0;
	int curseur_bloc;

	do{
		printf("curseur :%d, inode : %d, bloc : %d\n",curseur,inode,bloc);
		inode = inode_via_bloc(etapes[curseur], bloc, disque);
		bloc = disque.inode[inode].blocutilise[0];
		curseur++;
	}while(etapes[curseur] != NULL);

	return inode;
}

int inode_via_bloc(char* nom, int bloc, Disque disque){
	int inode;
	inode = -1;

	int check;

	char** lignes;
	lignes = decouper(disque.bloc[bloc].donnees, SGF_DELIMITEURS_REPERTOIRE);

	int i;
	i = 0;

	while(lignes[i] != NULL){
		check = inode_si_nom_dans_ligne(nom, lignes[i]);
		if(check != -1){
			inode = check;
		}
		i++;
	}
	free(lignes);

	return inode;
}

int inode_si_nom_dans_ligne(char* nom, char* ligne){
	char** donnees;
	donnees = decouper(ligne, SGF_DELIMITEURS_LIGNE_REPERTOIRE);
	int retour;
	retour = -1;

	printf("%s vs %s\n",donnees[0], nom);
	if(!strcmp(donnees[0],nom)){
		retour = atoi(donnees[1]);
	}

	free(donnees);

	return retour;
}

int inode_via_ligne(char* ligne){
	char** donnees;
	donnees = decouper(ligne, SGF_DELIMITEURS_LIGNE_REPERTOIRE);

	printf("%s\n",ligne);
	int inode;
	inode = atoi(donnees[1]);
	printf("e\n");
	free(donnees);

	return inode;
}