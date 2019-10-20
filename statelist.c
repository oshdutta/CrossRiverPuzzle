#include<stdlib.h>
#include <stdio.h>
#include "qlist.c"
#include <limits.h>
/* (WCSF)
0000-0: 1111:15
WCSF- 0000 and 1111 - state 0 and 15
WC|SF- 0011 and 1100- state 3 and 12
W|CSF- 0111 and 1000- state 7 and 8
WSF|C- 0100 and 1011- state 4 and 11
WCF|S- 0010 and 1101- state 2 and 13
other states defy rules of the puzzle
valid or finite state moves: 0,3; 12,15; 7,4; 8,11; 3,2; 12,13; 7,2; 8,13; 4,13; 2,11; 
*/
#define STARTST 0         // start state is 0 
#define ENDST 15   /* end state is 15 */
#define TOTSTATES 16
typedef struct node
	{
	int val;
	struct node *link;
	} NODE;

void printallstates (int s, int e, int *prd);
void printstate(int s);
void traverse ( NODE *p );
void printgraph ( NODE **a );
int validornot(int j);
int validedges(int i, int j);
NODE** adjacencylist( );

main()
{
	NODE **adjlist; //adj list
	//int *prd;
	//prd = (int *) calloc(TOTSTATES,sizeof(int));
	
	adjlist = adjacencylist();
	printgraph ( adjlist );
	//printallstates(STARTST,ENDST,prd);
}

int validornot(int j)
{
	//check wheather someone is eaten or not
	int wolfeatsgoat = (( (j) >> (0) ) & 1)^(( (j) >> (3) ) & 1) && (( (j) >> (0) ) & 1)^(( (j) >> (1) ) & 1);
	int goateatscabbage = (( (j) >> (0) ) & 1)^(( (j) >> (1) ) & 1) && (( (j) >> (0) ) & 1)^(( (j) >> (2) ) & 1);
	int forbidden = wolfeatsgoat || goateatscabbage;
	return forbidden;
}
int validedges(int i, int j)
{
	/* returns 1 if i-->j is edge (valid move), 0 else */
	int s;
	int Fmoves, moveswithF, k;
		/* hamming distance is the number of different bits between x,y.
		   moveswithF checks whether the bits (items) which changed
		   between x,y moved with the farmer who is required to pilot the
		   boat. This outlaws the otherwise possible move
				1010 to 0001, i.e.
				wolf cabbage || farmer goat TO farmer wolf goat || cabbage.
		   forbidden node is if wolfeatsgoat or goateatscabbage. */

	//check if farmer moves
	Fmoves = (( (i) >> (0) ) & 1) ^ (( (j) >> (0) ) & 1);


		// check if farmer moves with items
	moveswithF = 1;    /* initially assume correct */
	for (k=0;k<4;k++)
		if ( (( (i) >> (k) ) & 1)^(( (j) >> (k) ) & 1)&& (( (j) >> (0) ) & 1)^(( (j) >> (k) ) & 1))
			{
			moveswithF = 0;
			break;
			}

	//count number items moved
	int r=i ^ j;
	 s= (( (r) >> (0) ) & 1) +(( (r) >> (1) ) & 1) +(( (r) >> (2) ) & 1) +(( (r) >> (3) ) & 1) ;

	int forbidden = validornot(j);
	return(Fmoves && !forbidden && s < 3 && moveswithF);


}
NODE** adjacencylist( )
{
	/* adjacencylist() returns a pointer to NODE*; i.e. an
	   array of pointers to NODE, where a[i] points to an
	   adjacency list for i */
	int i,j;
	NODE **a;   /* a is array of pointers to NODE */
	NODE *p;    /* pointer p used for creating new node */
	int validedges(int i, int j); /* prototype */

	a = (NODE **) calloc(TOTSTATES,sizeof(NODE*));
	for (i=0;i<TOTSTATES;i++)
		{
		a[i] = NULL;    /* initialize adjacency list for i to be empty */
		for (j=TOTSTATES-1;j>=0;j--)
			{
			if ( !validedges(i,j) )
				continue;  /* go on to next j if i-->j not edge */
			p = (NODE *) malloc(sizeof(NODE));
			p->val = j;
			p->link = a[i]; /* if adj list empty then p->link is NULL */
			a[i] = p;
			}
		}
	return(a);
}
void printstate(int x)
{
	/* instead of integer i, prints the occupants of both
	   sides of the river in farmer, wolf, goat, cabbage problem */
	if ((( (x) >> (3) ) & 1)  == 0) printf("Wolf ");
	if ((( (x) >> (2) ) & 1)  == 0) printf("Cabbage ");
	if ((( (x) >> (1) ) & 1)  == 0) printf("Sheep ");
	if ((( (x) >> (0) ) & 1)  == 0) printf("farmer");
	printf(" || ");
	if ((( (x) >> (3) ) & 1)  == 1) printf("Wolf ");
	if ((( (x) >> (2) ) & 1)  == 1) printf("Cabbage ");
	if ((( (x) >> (1) ) & 1)  == 1) printf("Sheep ");
	if ((( (x) >> (0) ) & 1)  == 1) printf("farmer");
	printf("\n");
}
	void printallstates (int s, int e, int *prd)
{
	//void printstate(int s);

	if (s == e)
		printstate(s);
	else if (prd[e] == -1)
		printf("No path from %d to %d \n",s,e);
	else
		{
		printallstates(s,prd[e],prd);
		printstate(e);
		}
}
//check graph
void traverse ( NODE *p )
{
	/* traverses adjacency list pointed to by p */
	if (p != NULL)
		{
		printf("%d, ",p->val);
		traverse(p->link);
		}
}
void printgraph ( NODE **a )
{
	/* for each vertex i of the graph G prints adjacency list for i */
	int i;

	for (i=0;i<TOTSTATES;i++)
		{
		printf("%d:\t",i);
		traverse(a[i]);
		printf("\n");
		}
}


