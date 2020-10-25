#include "Trees.h"


/*Functie ce primeste un sir de caractere de tipul [a,b,c,...] si numarul lor
si intoarce un vector cu aceste valori */
int* getValues(char* val, int nrJucatori)
{
	/*Aloc vectorul */
	int* values = calloc(nrJucatori, sizeof(int));
	if(!values) return NULL;

	int index = 0;

	/*Impart sirul in subsiruri separate de paranteze si virgule */
	char* strToSplit = calloc(strlen(val) + 1, sizeof(char));
	strcpy(strToSplit, val);
	char* token = strtok(strToSplit,"[],");
	values[index] = atoi(token);
	while(token != NULL)
	{
		index++;
		token = strtok(NULL, "[],");
		/*Adaug in vector valoarea convertita */
		if(token != NULL)
			values[index] = atoi(token);
	}
	free(strToSplit);
	return values;
}

/*Functie ce aloca memorie pentru un arbore Paranoic
(vectorul si sirul de caractere)*/
TTreeB alocTreeB(char* val, int nrJucatori)
{
	TTreeB new = calloc(1, sizeof(TNodeB));
	new->val = val;
	new->firstSon = NULL;
	new->next = NULL;

	/*In cazul vectorului, voi apela functia descrisa anterior */
	if(val[0] == '[')
		new->num = getValues(val, nrJucatori);
	else
	{
		new->num = calloc(nrJucatori, sizeof(int));
		new->num[0] = atoi(val + 1);
	}

	return new;
}


/* Funtie ce construieste un arbore Paranoic din liniile date ca parametru,
fara a calcula momentan min sau max */
void buildTreeB(TTreeB root, char** lines, int val, int level, int nrJucatori)
{

	if(root == NULL)
		return;
	if(root->val[0] == '[')
		return;

	TTreeB currentNode = NULL;

	int i;
	for(i = 0; i < val; i++)
	{
		if(i == 0)
		{
			char* new_val = readString(lines[level]);
			root->firstSon = alocTreeB(new_val, nrJucatori);
			currentNode = root->firstSon;
			buildTreeB(currentNode, lines, atoi(new_val + 1), level + 1,
			nrJucatori);
		}
		else
		{
			char* new_val = readString(lines[level]);
			currentNode->next = alocTreeB(new_val, nrJucatori);
			currentNode = currentNode->next;
			buildTreeB(currentNode, lines, atoi(new_val + 1), level + 1,
			nrJucatori);
		}
	}
}

/*Functie ce elibereaza toate memoria alocata pentru un arbore de tip Paranoic */
void FreeTreeB(TTreeB* rootAdr)
{

	TTreeB root = *rootAdr;
	if(root == NULL) /*conditia de iesire din recursivitate */
		return;

	TTreeB currentNode = root;
	/*Parcurg nodurile de pe acelasi nivel */
	while(currentNode != NULL)
	{
		/*Pentru fiecare nod, reapelez recursiv pentru primul copil */
		FreeTreeB(&currentNode->firstSon);
		TTreeB aux = currentNode;
		currentNode = currentNode->next;
		/*Eliberez vectorul, sirul, iar apoi nodul */
		if(aux->val != NULL)
		{
			free(aux->val);
			aux->val = NULL;
		}

		if(aux->firstSon == NULL)
			free(aux->num);
		
		aux->firstSon = NULL;
		aux->next = NULL;

		if(aux != NULL)
			free(aux);
		
		aux = NULL;
	}
}

/*Functie ce schimba flag-ul. Spre deosebire de arborii MinMax simpli,
schimbarea flag-ului depinde de nr de jucatori, astfel incat, pentru 3 jucatori,
voi avea o data flag = 1(max) si de 2 ori flag = 0(min)*/
int changeFlagB(int nrCrtJucatori, int nrJucatori)
{
	if(nrCrtJucatori == nrJucatori)
		return 1;
	return 0;
}

/*Functie ce calculeaza minimele si maximele si construieste complet arborele
Paranoic */
int* ParanoicTree(TTreeB root, int flag, int nrCrtJucatori, int nrJucatori)
{

	TTreeB currentNode = root;
	int max = INT_MIN;
	int min = INT_MAX;
	int* arr;


	nrCrtJucatori++;
	if(nrCrtJucatori > nrJucatori)
		nrCrtJucatori = 1;

	while(currentNode != NULL)
	{
		/*Pentru fiecare nod care nu este frunza, reapelez recursiv */
		if(currentNode->val[0] == '(')
		{
			int newFlag = changeFlagB(nrCrtJucatori, nrJucatori);
			/*Copiez in vectorul nodului curent referinta catre vectorul care
			respecta conditia de maxim sau minim */
			free(currentNode->num);
			currentNode->num = ParanoicTree(currentNode->firstSon, newFlag,
			nrCrtJucatori, nrJucatori);
		}
		if(flag == 1) /*max */
		{
			/*Este necesar pentru maxim si minim sa compar doar
			primul element din vector */
			if(currentNode->num[0] > max)
			{
				max = currentNode->num[0];
				arr = currentNode->num;
			}
		}
		else
		{
			if(currentNode->num[0] < min)
			{
				min = currentNode->num[0];
				arr = currentNode->num;
			}
		}
		currentNode = currentNode->next;
	}

	return arr;
}

/*Functie ce afiseaza cu format vectorul dintr-un nod */
void printParanoic(TTreeB root, int nrTabs, int nrJucatori, FILE* output)
{
	int i,j;
	for(i = 0; i < nrTabs; i++)
		fprintf(output, "\t");
	
	for(j = 0; j < nrJucatori; j++)
	{
		if(j == 0) fprintf(output, "[");
		fprintf(output, "%d", root->num[j]);
		if(j == nrJucatori - 1) fprintf(output, "]");
		else fprintf(output, ",");
	}
	
	fprintf(output, "\n");
}

/*Functie ce se reapeleaza recursiv si afiseaza cu format intreg arborele */
void printParanoicTree(TTreeB root, int level, int nrJucatori, FILE* output)
{
	if(root == NULL)
		return;
	TTreeB currentNode = root;
	while(currentNode != NULL)
	{
		printParanoic(currentNode, level, nrJucatori, output);
		printParanoicTree(currentNode->firstSon, level + 1, nrJucatori, output);
		currentNode = currentNode->next;
	}
}