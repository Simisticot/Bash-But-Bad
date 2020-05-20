#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bbb.h"
#include "sgf.h"

int main(int argc, char const *argv[])
{
	Disque disque;

	char nom[] = "home";
	char chemin[] = "/home/simon/bjr.txt";

	formatter(&disque);

	disque.inode[0].blocutilise[0] = 0;

	strncpy(disque.bloc[0].donnees,"etc;2\nhome;4\ntmp;6",SGF_TAILLE_BLOC);
	disque.inode[2].blocutilise[0] = 6;
	disque.inode[4].blocutilise[0] = 4;
	disque.inode[6].blocutilise[0] = 5;

	strncpy(disque.bloc[4].donnees,"sylvie;3\nsimon;12",SGF_TAILLE_BLOC);
	disque.inode[3].blocutilise[0] = 15;
	disque.inode[12].blocutilise[0] = 7;
	disque.inode[12].blocutilise[1] = 16;

	strncpy(disque.bloc[7].donnees,"bdr.txt;9",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[16].donnees,"bjr.txt;8",SGF_TAILLE_BLOC);
	disque.inode[8].blocutilise[0] = 1;

	strncpy(disque.bloc[8].donnees,"salut :)",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[2].donnees,"",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[6].donnees,"",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[3].donnees,"",SGF_TAILLE_BLOC);

	printf("via chemin : %d\n", inode_via_chemin(chemin,disque));

	return 0;
}