#include "Trees.h"

int max(int a, int b)
{
	return a > b ? a : b;
}
int min(int a, int b)
{
	return a > b ? b : a;
}

/*Functie ce elimina subarborii care nu respecta conditia beta > alpha */
int AlphaBetaTree(TTree2 root, int flag, int alpha, int beta)
{

	/*Ies din recursivitate daca intalnesc o frunza */
	if(root->val[0] == '[')
		return root->num;

	/*Parcurg copii nodului curent */
	TTree2 currentNode = root->firstSon;

	/*Tratez separat nivelul de maxim sau de minim */
	if(flag == 1)
	{
		int maximum = INT_MIN;
		while(currentNode != NULL)
		{
			/*Obtin valoarea curenta a nodului apeland recursiv */
			int crtVal = AlphaBetaTree(currentNode, 0, alpha, beta);
			currentNode->num = crtVal;
			
			/*Obtin valoarea maxima de pe acest nivel, respectiv
			valoarea lui alpha */
			maximum = max(maximum, crtVal);
			alpha = max(alpha, crtVal);

			if(beta <= alpha)
			{
				/*In cazul in care am de facut un prunning,
				distrug subarborele corespunzator */
				FreeTreeC2(&currentNode->next);
				currentNode->next = NULL;
				break;
			}
			currentNode = currentNode->next;
		}
		return maximum;
	}

	else
	{
		int minimum = INT_MAX;
		while(currentNode != NULL)
		{
			int crtVal = AlphaBetaTree(currentNode, 1, alpha, beta);
			currentNode->num = crtVal;
			minimum = min(minimum, crtVal);
			beta = min(beta, crtVal);
			if(beta <= alpha)
			{
				FreeTreeC2(&currentNode->next);
				currentNode->next = NULL;
				break;
			}	
			currentNode = currentNode->next;
		}
		return minimum;
	}
}