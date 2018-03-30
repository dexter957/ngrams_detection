#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treap.h"


/*
We want to create a data structure to store the most printed ngrams. For that, we need a way to access easily the ngrams by alphabetic order,
but also to retrieve them by popularity. That is the reason why a treap structured is implemented. The key used in insertion is the ngram,
but a node's position in the treap is also determined by a numeric value indicating its priority (or better, how many times this particular
ngram has been printed out)
*/



struct treapNode
{
	char* keyWord;
	int priority;
	pointerToTreapNode left;
	pointerToTreapNode right;
	pointerToTreapNode parent;  /*We need this for the balance algorithm*/
};

int treapInit(pointerToTreapNode* treapRoot)
{
	(*treapRoot)=malloc(sizeof(struct treapNode));
	if((*treapRoot)==NULL)
	{
		return FALSE;
	}
	(*treapRoot)->keyWord=NULL;
	(*treapRoot)->priority=0;
	(*treapRoot)->left=NULL;
	(*treapRoot)->right=NULL;
	(*treapRoot)->parent=NULL;
	return TRUE;
}

int treapEmpty(pointerToTreapNode treapRoot)
{
	return
	((treapRoot==NULL)||((treapRoot->keyWord==NULL)&&(treapRoot->left==NULL)&&(treapRoot->right==NULL)&&(treapRoot->priority==0)));
}

void insertAChildTreap(pointerToTreapNode* treapRoot, char* ngram)
{/*Check if you should insert in root level*/
	if(((*treapRoot)->keyWord==NULL)||(strcmp((*treapRoot)->keyWord,ngram)==0))
	{/*Initialised/empty root node*/
		(*treapRoot)->keyWord=malloc((strlen(ngram)+1)*sizeof(char));
		strcpy((*treapRoot)->keyWord,ngram);
		++((*treapRoot)->priority);
		return;
	}
	/*Root not empty, you need to search for your place*/
	int placeFound=FALSE;
	int side;
	pointerToTreapNode node=(*treapRoot);
	pointerToTreapNode parent=(*treapRoot);
	do
	{
		if(node!=NULL)
		{
			if(strcmp(node->keyWord,ngram)>0)
			{/*Go left*/
				parent=node;
				node=node->left;
				side=LEFT;
			}
			else if(strcmp(node->keyWord,ngram)<0)
			{/*Go right*/
				parent=node;
				node=node->right;
				side=RIGHT;
			}
			else
			{
				++(node->priority);
				placeFound=TRUE;
			}
		}
		else
		{
			node=malloc(sizeof(struct treapNode));
			node->keyWord=malloc((strlen(ngram)+1)*sizeof(char));
			node->parent=parent;
			strcpy(node->keyWord,ngram);
			node->priority=1;
			node->left=NULL;
			node->right=NULL;
			placeFound=TRUE;
			if(side==LEFT)
			{
				parent->left=node;
			}
			else
			{
				parent->right=node;
			}
		}
	} while (placeFound!=TRUE);
	/*Now balance the treap*/
	while((node->priority)>(parent->priority))
	{
		balanceTreap(treapRoot,parent,node,side);
		if(node->parent!=NULL)
		{
			parent=node->parent;
			if(node==(parent->right))
			{
				side=RIGHT;
			}
			else
			{
				side=LEFT;
			}
		}
		else
		{
			break;
		}
	}
}


void balanceTreap(pointerToTreapNode* treapRoot,pointerToTreapNode parent, pointerToTreapNode current, int side)
{	/*
	Rotation balances the tree. It needs to be performed only once, since each time the priority gets raised by 1
	It does not change the inorder traversal at all, or any other attributes. Code for both rotations is symmetric, and runs in O(1) time.
	*/
	pointerToTreapNode x;
	pointerToTreapNode y;
	switch(side)
	{/*Decide whether it is left or right rotation*/
		case RIGHT:/*Left rotation*/
				/*Turn y's left subtree into x's right subtree*/
				x=parent;
				y=current;
				x->right=y->left;
				if(y->left!=NULL)
				{
					y->left->parent=x;
				}
				y->parent=x->parent;
				if(x->parent==NULL)
				{/*y is now the root*/
					(*treapRoot)=y;
				}
				else if(x==(x->parent->left))
				{
					x->parent->left=y;
				}
				else
				{
					x->parent->right=y;
				}
				y->left=x;
				x->parent=y;
				break;
		case LEFT:/*Right rotation*/
				y=parent;
				x=current;
				y->left=x->right;
				if(x->right!=NULL)
				{
					x->right->parent=y;
				}
				x->parent=y->parent;
				if(y->parent==NULL)
				{/*y is now the root*/
					(*treapRoot)=x;
				}
				else if(y==(y->parent->left))
				{
					y->parent->left=x;
				}
				else
				{
					y->parent->right=x;
				}
				x->right=y;
				y->parent=x;
				break;
		default:
				break;		
	}
}

void deleteTreap(pointerToTreapNode node)
{/*Post order*/
	if(node==NULL)
	{
		return;
	}
	deleteTreap(node->left);
	deleteTreap(node->right);
	/*Deletion routine*/
	free(node->keyWord);
	free(node->left);
	free(node->right);
	node->left=NULL;
	node->right=NULL;	
	node->keyWord=NULL;
}

void printTreapIn(pointerToTreapNode node)
{/*In order*/
	if(node==NULL)
	{
		return;
	}
	printTreapIn(node->left);
	printf("Key:%s\n",node->keyWord );
	printf("Priority:%d\n",node->priority );
	printTreapIn(node->right);
}

void printTreapPre(pointerToTreapNode node)
{/*Pre order*/
	if(node==NULL)
	{
		return;
	}
	printf("Key:%s\n",node->keyWord );
	printf("Priority:%d\n",node->priority );
	printTreapPre(node->left);
	printTreapPre(node->right);
}

void printTreapPost(pointerToTreapNode node)
{/*Pre order*/
	if(node==NULL)
	{
		return;
	}
	printTreapPost(node->left);
	printTreapPost(node->right);
	printf("Key:%s\n",node->keyWord );
	printf("Priority:%d\n",node->priority );
}


void prettyPrint(pointerToTreapNode node)
{
	if(node==NULL)
	{
		return;
	}
	printf("My name is %s and my priority is:%d\n",node->keyWord,node->priority );
	if(node->left!=NULL)
	{
		printf("My left child is:%s,%d\n",node->left->keyWord,node->left->priority );
	}
	else
	{
		printf("My left child is:(nil)\n" );
	}
	if(node->right!=NULL)
	{
		printf("My right child is:%s,%d\n",node->right->keyWord,node->right->priority);
	}
	else
	{
		printf("My right child is:(nil)\n" );
	}
	prettyPrint(node->left);
	prettyPrint(node->right);
}

void topK(pointerToTreapNode root, int* k)
{
	if(treapEmpty(root)==TRUE)
	{
		return;
	}
	int i;
	int priority=root->priority;
	int next=0;
	printf("Top: ");
	int firstPrint=TRUE;
	while((*k)>0)
	{
		printInOrderK(root,k, priority,&next,&firstPrint);
		priority=next;
		next=0;
	}
	printf("\n");
}

void printInOrderK(pointerToTreapNode node,int *k, int priority, int* next, int* firstPrint)
{
	if((node==NULL)||((*k)<=0)||(priority>(node->priority)))
	{
		return;
	}
	if((node->left!=NULL)&&((node->left->priority)>=priority))
	{
		printInOrderK(node->left,k,priority,next,firstPrint);
	}
	else if((node->left!=NULL)&&((node->left->priority)<priority))
	{
		if((node->left->priority)>(*next))
		{
			(*next)=node->left->priority;
		}
	}
	if(((*k)>0)&&(node->priority)==priority)
	{
		if((*firstPrint)==FALSE)
		{
			printf("|%s",node->keyWord );
		}
		else
		{
			printf("%s",node->keyWord );
			(*firstPrint)=FALSE;
		}
		--(*k);
	}
	if((node->right!=NULL)&&((node->right->priority)>=priority))
	{
		printInOrderK(node->right,k,priority,next,firstPrint);
	}
	else if((node->right!=NULL)&&((node->right->priority)<priority))
	{
		if((node->right->priority)>(*next))
		{
			(*next)=node->right->priority;
		}
	}
}



