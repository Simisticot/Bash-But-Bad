//Macro déterminant la taille d'un bloc dans le système de fichier
#define SGF_TAILLE_BLOC 1024

//Macro déterminant les délimiteurs utilisés pour découper un chemin en répertoires+fichier
#define SGF_DELIMITEURS_CHEMIN "/"

//Macro déterminant les délimiteurs utilisés pour découper un répertoire en lignes
#define SGF_DELIMITEURS_REPERTOIRE "\n"

//Macro déterminant les délimiteurs utilisés pour découper une ligne d'un répertoire en champs
#define SGF_DELIMITEURS_LIGNE_REPERTOIRE ";"

//Macro déterminant la taille maximum d'une ligne dans un répertoire
#define SGF_TAILLE_LIGNE_REPERTOIRE 259

//macro déterminant la taille maximum d'un nom de fichier
#define SGF_TAILLE_NOM_FICHIER 255

//structure contenant les ionformations sur un inode
typedef struct infoinode{
	//les permissions sur le fichier
	char permissions[3];
	//type de fichier 1 texte 2 binaire
	int typefichier;
	//index des blocs utilisés
	int blocutilise[30];
	//indique si l'inode est utilisé, 0 pour non et 1 pour oui
	int utilise;
} Infoinode;

//structure contenant le contenu d'un bloc
typedef struct bloc{
	//données du bloc
	char donnees[SGF_TAILLE_BLOC];
	//est-ce que le fichier est occupé 0 pour non, 1 pour oui
	int occupe;
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
int inode_via_chemin(char* chemin, Disque* disque);

//retourne l'inode du parent du parent d'un fichier via son chemin absolu
int inode_parent_via_chemin(char* chemin, Disque* disque);

//retourne l'inode d'un fichier via son nom et l'inode du répertoire où le rechercher (retourne -1 si le nom n'est pas dans le bloc)
int inode_via_repertoire(char* nom, int inode, Disque* disque);

//retourne l'inode d'un fichier si la ligne indiquée contient son nom
int inode_si_nom_dans_ligne(char* nom, char* ligne);

//retourne le contenu d'un fichier
char* contenu_fichier(int inode, Disque* disque);

//retourne l'index d'un bloc libre
int bloc_libre(Disque* disque);

//attribue un bloc au fichier indiqué
void attribuer_bloc(int inode, int bloc, Disque* disque);

//écrit le texte indiqué dans le fichier de manière destructive
void ecrire_fichier(int inode, char* texte, Disque* disque);

//ajoute le texte indiqué à la suite du fichier
void ajouter_fichier(int inode, char* texte, Disque* disque);

//efface le contenu du bloc indiqué
void effacer_bloc(int bloc, Disque* disque);

//retourne le nom d'un fichier via son chemin absolu
char* nom_fichier_via_chemin(char* chemin);

//crée un fichier via son chemin absolu
void creer_fichier(char* chemin, Disque* disque);