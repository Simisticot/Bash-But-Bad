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
void bbb_loop();

//execution de commande
void bbb_execution(char** arguments, Disque* disque);

//differentes commandes
void echo(char** arguments, Disque* disque);
void cat(char** arguments,Disque* disque);
void cp(char** arguments,Disque* disque);
void mv(char** arguments,Disque* disque);
