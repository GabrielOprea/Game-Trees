#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*Structura pentru arbore de joc */
typedef struct tree
{
	char** game; /*matricea de jos */
	struct tree* firstSon; /*referinta catre primul copil */
	struct tree* next; /*referinta catre urm nod de pe acelasi nivel */
}TNode, *TTree;

/*Structura pentru arbore MinMax */
typedef struct tree2
{
	char* val; /*sir de caractere(cu paranteze) */
	int num; /*numarul din sir */
	struct tree2* firstSon;
	struct tree2* next;
}TNode2, *TTree2;

typedef struct treeB
{
	char* val; /*sir de caractere, cu paranteze */
	int* num; /*vector de valori */
	struct treeB* firstSon;
	struct treeB* next;
}TNodeB, *TTreeB;



TTree2 alocTreeC2(char val[3]); /*aloca arbore minMax */
/*Costruieste un arbore minMax cu valorile din linii */
void buildTreeC2(TTree2 root, char** lines, int val, int level);
char* readString(char* line); /*Preia primul cuvant din sir */
/*Calculeaza min si max pt arborele de tip MinMax */
int MinMaxTree(TTree2 root, int flag);
/*Schimba flag-ul pt minMax */
int changeFlag(int flag);
/*Afiseaza tot arborele MinMax */
void printMinMaxTree(TTree2 root, int level, FILE* output);
/*Afiseaza cu format un nod */
void printMinMax(TTree2 root, int nrTabs, FILE* output);
/*Obtine nr de caractere din fisier */
int getFileSize(char* fileName);
/*Elibereaza un arbore minMax */
void FreeTreeC2(TTree2* rootAdr);

/*Aplica algoritmul de prunning */
int AlphaBetaTree(TTree2 root, int flag, int alpha, int beta);

/*Obtine un vector de valori */
int* getValues(char* val, int nrJucatori);
/*Aloca un arbore pt bonus */
TTreeB alocTreeB(char* val, int nrJucatori);
/*Construieste arborele */
void buildTreeB(TTreeB root, char** lines, int val, int level, int nrJucatori);
/*Afiseaza un nod, respectiv tot arborele cu format */
void printParanoicTree(TTreeB root, int level, int nrJucatori, FILE* output);
void printParanoic(TTreeB root, int nrTabs, int nrJucatori, FILE* output);
/*Calculeaza min si max pt arborele paranoic */
int* ParanoicTree(TTreeB root, int flag, int nrCrtJucatori, int nrJucatori);
int changeFlagB(int nrCrtJucatori, int nrJucatori);
/*Elibereaza memoria pt arborele paranoic */
void FreeTreeB(TTreeB* rootAdr);