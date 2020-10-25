#include "Trees.h"


//Functie ce aloca memorie pentru un arbore MinMax
TTree2 alocTreeC2(char* val)
{
	TTree2 new = calloc(1, sizeof(TNode2));
	new->val = val;
	new->firstSon = NULL;
	new->next = NULL;
	/*Convertesc sirul de caractere asociat la un numar;
	pentru aceasta ignor paranteza '[' */
	new->num = atoi(val + 1);
	return new;
}

char* readString(char* line)
{
	//Functie ce extrage primul subsir dintr-un sir, separat prin spatiu
	int len = strlen(line);
	int i;
	for(i = 0; i < len; i++)
		if(line[i] == ' ')
			break;

	char* result = calloc(i + 1, sizeof(char));
	strncpy(result, line, i);
	//Scot cuvantul extras din sirul initial
	memmove(line, line + i + 1, strlen(line + i + 1) + 1);
	return result;
}


//Functie ce construieste arborele initial(cu paranteze patrate si rotunde)
// val reprezinta numarul de copii ai nodului curent
void buildTreeC2(TTree2 root, char** lines, int val, int level)
{

	if(root == NULL)
		return;
	if(root->val[0] == '[')
		return;

	TTree2 currentNode = NULL;
	int i;
	//Parcurg copii nodului curent
	for(i = 0; i < val; i++)
	{
		if(i == 0)
		{
			/*Pentru fiecare copil, parsez linia corespunzatoare nivelului
			si aloc memoria */
			char* new_val = readString(lines[level]);
			root->firstSon = alocTreeC2(new_val);
			currentNode = root->firstSon;
			buildTreeC2(currentNode, lines, atoi(new_val + 1), level + 1);
		}
		else
		{
			char* new_val = readString(lines[level]);
			currentNode->next = alocTreeC2(new_val);
			currentNode = currentNode->next;
			buildTreeC2(currentNode, lines, atoi(new_val + 1), level + 1);
		}
	}
}

//Elibereaza memoria pentru un arbore MinMax
void FreeTreeC2(TTree2* rootAdr)
{

	TTree2 root = *rootAdr;
	if(root == NULL)
		return;

	TTree2 currentNode = root;
	while(currentNode != NULL)
	{
		//Eliberez fiecare subarbore al radacinii curente
		FreeTreeC2(&currentNode->firstSon);
		TTree2 aux = currentNode;
		currentNode = currentNode->next;

		//Daca sirul de caractere din nodul curent a fost alocat, il eliberez
		if(aux->val != NULL)
		{
			free(aux->val);
			aux->val = NULL;
		}
		aux->firstSon = NULL;
		aux->next = NULL;
		free(aux);
		//Setez zonele eliberate la NULL
		aux = NULL;
	}
}

//Flag-ul va spune functiei sa calculeze ori minimul, ori maximul. 
/*Conventia este MAX pt flag = 1 si MIN pt flag = 0. Functia schimba
valoarea flag-ului */
int changeFlag(int flag)
{
	if(flag == 1) return 0;
	return 1;
}

/*Functie ce construieste arborele MinMax calculand minimele si maximele
corespunzatoare in cazul in care intalneste noduri ce contin paranteze
rotunde */
int MinMaxTree(TTree2 root, int flag)
{

	TTree2 currentNode = root;
	
	//Setez valorile default
	int max = INT_MIN;
	int min = INT_MAX;

	while(currentNode != NULL)
	{
		//Daca nodul curent nu este frunza, atunci apelez recursiv pt copil
		if(currentNode->val[0] == '(')
			currentNode->num = MinMaxTree(currentNode->firstSon,
			changeFlag(flag));
			
		if(flag == 1) //max
		{
			//calculez maximul
			if(currentNode->num > max)
				max = currentNode->num;
		} //sau minimul
		else
			if(currentNode->num < min)
				min = currentNode->num;
	
		currentNode = currentNode->next;
	}

	//In functie de flag, intorc rez dorit
	if(flag == 1) return max;
	return min;
}

//Functie ce afiseaza cu format numarul dintr-un arbore MinMax
void printMinMax(TTree2 root, int nrTabs, FILE* output)
{
	
	for(int i = 0; i < nrTabs; i++)
		fprintf(output, "\t");

			fprintf(output, "%d\n", root->num);
}

//Functie ce afiseaza recursiv tot arborele
void printMinMaxTree(TTree2 root, int level, FILE* output)
{
	if(root == NULL)
		return;
	TTree2 currentNode = root;
	while(currentNode != NULL)
	{
		printMinMax(currentNode, level, output);
		printMinMaxTree(currentNode->firstSon, level + 1, output);
		currentNode = currentNode->next;
	}
}

//Obtine nr de caractere din fisier
int getFileSize(char* fileName)
{
	//Deschid fisierul
	FILE* fp = fopen(fileName, "r");

	if(!fp) return -1;
	
	//Setez cursorul la sfarsit si obtin nr de bytes
	fseek(fp, 0L, SEEK_END);
	int nr = ftell(fp);
	
	fclose(fp);

	//Returnez nr de caractere din fisier
	return nr * sizeof(char);
}