//Macro déterminant la taille du buffer de lecture utilisé dans bbb_lire_entree()
#define BBB_TAILLE_BUFFER 128
//Macro déterminant la taille initiale du tableau de découpes dans la fonction de découpage
#define BBB_TAILLE_DECOUPES 64
//Macro déterminant les caractères utilisés pour découper l'entree
#define BBB_DELIMITEURS " \t\n\r\a"


// lit l'entrée au clavier
char* bbb_lire_entree();

//divise une chaine de caractères selon les caractères fournis dans "delimiteurs"
char** decouper(char* entree, char* delimiteurs);

//récupère la chaine découpée et regroupes les termes entre "" en supprimant les ""
char** decouper_guillemet(char* entree, char* delimiteurs);
char* ajout_a_la_suite(char* chaine,char* ajout);
char* suppr_guillemet(char* chaine);

//boucle d'execution du shell
void bbb_loop(Disque* disque);

//execution de commande
void bbb_execution(char** arguments,int* position, Disque* disque);

//affiche les noms des fichiers contenus dans un répertoire via l'inode du répertoire
void afficher_noms(int inode_repertoire, Disque* disque);

//differentes commandes
//commande echo
void echo(char** arguments, int position, Disque* disque);
//commande cd
void cd(char** arguments, int* position, Disque* disque);
//commande ls
void ls(char** arguments, int position, Disque* disque);
//commande cat
void cat(char** arguments, int position,Disque* disque);
//commande cp
void cp(char** arguments, int position, Disque* disque);
//commande mv
void mv(char** arguments, int position, Disque* disque);
//commande mkf
void mkf(char** arguments, int position, Disque* disque);
//commande mkdir
void mkdir(char** arguments, int position_courante, Disque* disque);
//commande rmdir
void notre_rmdir(char** arguments, int position, Disque* disque);
//commande rm
void notre_rm(char** arguments, int position, Disque* disque);
//commande df
void df(char** arguments, Disque* disque);
//clear
void clear();
//aide
void help(char* commande);
