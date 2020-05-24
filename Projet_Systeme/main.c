#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sgf.h"
#include "bbb.h"


int main(int argc, char const *argv[])
{

	Disque disque;

	char nom[] = "bdr.txt";
	char chemin[] = "/home/simon/bdr.txt";
	char autre_chemin[] = "/home/simon";
	char autre_chemin_2[] = "/home/simon/hello.txt";
	char texte[] = "Lorem ipsum dolor sit amet, consectetur adipiscing euis. Interdum et malesuada fames ac ante ipsum primis in faucibus. Suspendisse gravida sed nisi eget vulputate. Nam interdum vulputate sapien euismod sollicitudin. Etiam mollis massa ultrices risus pellentesque tempor. In rutrum maximus lectus nec posuere. Fusce justo lacus, aliquam consectetur ex non, cursus tincidunt urna. Suspendisse consequat imperdiet tortor, ac vulputate sapien. Mauris nisi ante, pretium ac rhoncus eget, mattis sit amet eros. Duis condimentum dolor ut est aliquam pellentesque. Proin quam ex, sagittis eget lobortis a, lobortis id urna. Suspendisse scelerisque leo vitae est iaculis commodo. Nam semper tortor id tellus bibendum, nec fringilla massa imperdiet.Etiam sollicitudin luctus velit, sit amet mollis justo bibendum et. Nullam sit amet fermentum risus. Sed maximus eu elit ac pretium. Praesent sed nibh venenatis, ornare nisi in, rhoncus nisi. Integer risus mauris, aliquam a orci eu, rhoncus lacinia sem. Suspendisse potenti. In et convallis velit. Mauris vel lectus dui.Integer vel est a ex consectetur efficitur at eu quam. Aenean non magna vel magna vestibulum laoreet ut eget turpis. In euismod sodales viverra. Aliquam rutrum urna at varius congue. Vivamus nunc turpis, condimentum vitae ipsum id, tempor luctus ligula. Sed felis risus, luctus molestie ante et, euismod convallis mi. Sed mi velit, ullamcorper sodales purus sed, auctor volutpat ante. Proin lobortis mauris eget mi vulputate auctor eu a erat. Etiam ac egestas lectus.Suspendisse pulvinar nibh in tellus hendrerit facilisis. Nam ut nibh quis neque egestas lacinia. Nullam eget ante justo. Praesent accumsan dictum felis, eu laoreet tortor lacinia ut. Etiam lobortis ex vel orci pretium mattis. Maecenas lacinia metus nisl, at ornare purus convallis nec. Mauris malesuada mauris eget magna sagittis fermentum.Pellentesque et elit velit. Donec luctus tortor non odio laoreet ornare. Vestibulum tincidunt, dui in maximus suscipit, erat justo tristique purus, id tincidunt tortor leo nec sem. Ut consectetur, dui nec rutrum maximus, magna sem mollis lacus, in fringilla ex lectus vel purus. Vestibulum malesuada magna vel dolor pulvinar, et consectetur massa congue. Donec quis mauris vitae ex condimentum tristique id luctus leo. Duis mollis dapibus elit vel rhoncus.Sed lorem velit, vestibulum venenatis purus ac, rhoncus lobortis lacus. Vestibulum consectetur, leo a dignissim gravida, enim urna volutpat mi, quis semper ante metus sed lectus. Fusce ullamcorper a mauris efficitur luctus. Morbi nec porttitor dui, sed cursus arcu. Morbi at condimentum massa. In pharetra et risus in tincidunt. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Sed tristique dui et purus ornare, eget facilisis eros faucibus. Morbi mattis lectus quis lacus vulputate ultrices. Vestibulum iaculis quam leo. Aenean pharetra nisl sed nulla. ";
	char autreTexte[] = "Lorem ipsum dolor sit amet, consectetur adipiscing euis. Interdum et malesuada fames ac ante ipsum primis in faucibus. Suspendisse gravida sed nisi eget vulputate. Nam interdum vulputate sapien euismod sollicitudin. Etiam mollis massa ultrices risus pellentesque tempor. In rutrum maximus lectus nec posuere. Fusce justo lacus, aliquam consectetur ex non, cursus tincidunt urna. Suspendisse consequat imperdiet tortor, ac vulputate sapien. Mauris nisi ante, pretium ac rhoncus eget, mattis sit amet eros. Duis condimentum dolor ut est aliquam pellentesque. Proin quam ex, sagittis eget lobortis a, lobortis id urna. Suspendisse scelerisque leo vitae est iaculis commodo. Nam semper tortor id tellus bibendum, nec fringilla massa imperdiet.Etiam sollicitudin luctus velit, sit amet mollis justo bibendum et. Nullam sit amet fermentum risus. Sed maximus eu elit ac pretium. Praesent sed nibh venenatis, ornare nisi in, rhoncus nisi. Integer risus mauris, aliquam a orci eu, rhoncus lacinia sem.";
	int inode;
	char* contenu;
	char* nom_du_fichier;

	formatter(&disque);

	disque.inode[0].utilise = 1;
	attribuer_bloc(0,0,&disque);

	strncpy(disque.bloc[0].donnees,"etc;2\nhome;4\ntmp;6",SGF_TAILLE_BLOC);
	disque.inode[2].utilise = 1;
	attribuer_bloc(2,6,&disque);
	disque.inode[4].utilise = 1;
	attribuer_bloc(4,4,&disque);
	disque.inode[6].utilise = 1;
	attribuer_bloc(6,2,&disque);

	strncpy(disque.bloc[4].donnees,"sylvie;3\nsimon;12",SGF_TAILLE_BLOC);
	disque.inode[3].utilise = 1;
	attribuer_bloc(3,15,&disque);
	disque.inode[12].utilise = 1;
	attribuer_bloc(12,7,&disque);
	attribuer_bloc(12,16,&disque);

	strncpy(disque.bloc[7].donnees,"bdr.txt;9\n",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[16].donnees,"bjr.txt;8\n",SGF_TAILLE_BLOC);
	disque.inode[9].utilise = 1;
	disque.inode[8].utilise = 1;
	attribuer_bloc(8,27,&disque);
	attribuer_bloc(9,28,&disque);

	strncpy(disque.bloc[27].donnees,"salut :)",SGF_TAILLE_BLOC);

	strncpy(disque.bloc[15].donnees,"",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[6].donnees,"",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[2].donnees,"",SGF_TAILLE_BLOC);
	strncpy(disque.bloc[28].donnees,"aslut ; (",SGF_TAILLE_BLOC);


	bbb_loop(&disque);



	inode = inode_via_chemin(autre_chemin,&disque);
	printf("via chemin : %d\n", inode);

	contenu = contenu_fichier(inode,&disque);

	printf("contenu : \n%s\n", contenu);

	printf("contenu du bloc 28 : %s\n",disque.bloc[28].donnees);
	printf("Bloc 28 occupé  : %d\n",disque.bloc[28].occupe);
	printf("Inode 9 occupé : %d\n",disque.inode[9].utilise);
	
	printf("Bloc libre : %d\n",bloc_libre(&disque));
	printf("Taille contenu : %ld\n", strlen(contenu));
	printf("Blocutilise 0 : %d\n",disque.inode[inode].blocutilise[0]);
	printf("Blocutilise 1 : %d\n",disque.inode[inode].blocutilise[1]);
	printf("Blocutilise 2 : %d\n",disque.inode[inode].blocutilise[2]);
	printf("Blocutilise 3 : %d\n",disque.inode[inode].blocutilise[3]);
	printf("Blocutilise 4 : %d\n",disque.inode[inode].blocutilise[4]);
	printf("Blocutilise 5 : %d\n",disque.inode[inode].blocutilise[5]);
	printf("Blocutilise 6 : %d\n",disque.inode[inode].blocutilise[6]);
	free(contenu);

	return 0;
}