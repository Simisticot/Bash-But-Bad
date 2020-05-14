#define SGF_TAILLE_BLOC 1024

#define SGF_DELIMITEURS_CHEMIN "/"

#define SGF_DELIMITEURS_REPERTOIRE "\n"
#define SGF_DELIMITEURS_LIGNE_REPERTOIRE ";"

#define SGF_TAILLE_ETAPES 8

typedef struct infoinode{
	char permissions[8];
	int typefichier;
	int blocutilise[30];
} Infoinode;

typedef struct bloc{
	char donnees[SGF_TAILLE_BLOC];
} Bloc;

typedef struct disque{
	Infoinode inode[15];
	Bloc bloc[30];
} Disque;

void direfichier();

void printFichier(char bloc);

void initialisation_sgf();

void initialisation_test();

void formatter(Disque* disque);

int inode_via_chemin(char* chemin, Disque disque);

int inode_via_bloc(char* nom, int bloc, Disque disque);

int inode_si_nom_dans_ligne(char* nom, char* ligne);

int inode_via_ligne(char* ligne);