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
	disque.inode[4].blocutilise[0] = 4;

	strncpy(disque.bloc[4].donnees,"sylvie;3\nsimon;12",SGF_TAILLE_BLOC);
	disque.inode[12].blocutilise[0] = 7;

	strncpy(disque.bloc[7].donnees,"bjr.txt;8",SGF_TAILLE_BLOC);
	disque.inode[8].blocutilise[0] = 1;

	strncpy(disque.bloc[8].donnees,"salut :)",SGF_TAILLE_BLOC);

	printf("via chemin : %d\n", inode_via_chemin(chemin,disque));

	return 0;
}