//Macro déterminant la taille d'un bloc dans le système de fichier
#define SGF_TAILLE_BLOC 1024

//Macro déterminant les délimiteurs utilisés pour découper un chemin en répertoires+fichier
#define SGF_DELIMITEURS_CHEMIN "/"

//Macro déterminant les délimiteurs utilisés pour découper un répertoire en lignes
#define SGF_DELIMITEURS_REPERTOIRE "\n"

//Macro déterminant les délimiteurs utilisés pour découper une ligne d'un répertoire en champs
#define SGF_DELIMITEURS_LIGNE_REPERTOIRE ";"

//structure contenant les ionformations sur un inode
typedef struct infoinode{
	//les permissions sur le fichier
	char permissions[3];
	//type de fichier 1 texte 2 binaire
	int typefichier;
	//index des blocs utilisés
	int blocutilise[30];
} Infoinode;

//structure contenant le contenu d'un bloc
typedef struct bloc{
	//données du bloc
	char donnees[SGF_TAILLE_BLOC];
} Bloc;

//structure représentant un disque dur
typedef struct disque{
	//les inodes
	Infoinode inode[15];
	//les blocs
	Bloc bloc[30];
} Disque;

//formatte le disque
void formatter(Disque* disque);

//retourne l'inode d'un fichier via son chemin absolu
int inode_via_chemin(char* chemin, Disque disque);

//retourne l'inode d'un fichier via son nom et l'inode du répertoire où le rechercher (retourne -1 si le nom n'est pas dans le bloc)
int inode_via_repertoire(char* nom, int inode, Disque disque);

//retourne l'inode d'un fichier si la ligne indiquée contient son nom
int inode_si_nom_dans_ligne(char* nom, char* ligne);

//retourne le contenu d'un fichier
char* contenu_fichier(int inode, Disque disque);