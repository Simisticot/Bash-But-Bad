//Macro déterminant la taille du buffer de lecture utilisé dans bbb_lire_entree()
#define BBB_TAILLE_BUFFER 128
//Macro déterminant la taille d'un argument dans bbb_decouper_entree()
#define BBB_TAILLE_ARGUMENTS 64
//Macro déterminant les caractères utilisés pour découper l'entree
#define BBB_DELIMITEURS " \t\n\r\a"


// lit l'entrée au clavier
char* bbb_lire_entree();

// divise l'entrée en arguments
char** bbb_decouper_entree(char* entree);

//boucle d'execution du shell
void bbb_loop();
