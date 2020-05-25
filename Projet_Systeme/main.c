#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sgf.h"
#include "bbb.h"


int main(int argc, char const *argv[])
{
	//disque dur
	Disque disque;
	//si on trouve un fichier de sauvegarde on le charge
	if(access("disque.dat",F_OK) != -1){
		charger(&disque);
	//sinon on intialise un nouveau disque
	}else{
		initialisation(&disque);
	}
	//on lance la boucle du shell
	bbb_loop(&disque);

	return 0;
}