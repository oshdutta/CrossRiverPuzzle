


/***********************************************************
	Program: dfs_bfs.c
	Peter Clote

	This program uses depth first search of a state diagram of
	a logic puzzle to find solution. As in most similar AI
	applications, the principal difficulty is in finding an
	efficient representation of the problem.

	Problem: how to transport a man, wolf, goat, cabbage from
	one side of the river to the other in a boat which can hold
	at most 2 items, and which requires the man to pilot the boat.
	If wolf-goat or goat-cabbage are left alone without the presence
	of the man, then one is devoured. Thus such states are "forbidden".

	The representation of the problem is given by an element of
		{0,1}^4
	where, for example 0101 means that the man and goat are on the
	left bank and the wolf and cabbage are on the right (0 = left,
	1 = right). There are thus 16 possible states, given by
	0,...,15 where the 4-tuple of 0's and 1's is obtained by
	finding the 0,1,2,3 bits of an integer representing the state.


***********************************************************/

#include<stdlib.h>
#include <stdio.h>
#include <limits.h>     /* definition of INT_MAX */
#include "qlist.c"      /* header file for queue routines */

#define START 0         /* start state is 0 */
#define FINISH 15       /* finish state is 15 */
#define NIL -1          /* nil state used for predecessor function in dfs */
#define INF INT_MAX     /* infinity distance used in bfs */
#define NUMSTATES 16    /* number states in state diagram */
#define BIT(x,i)  (( (x) >> (i) ) & 1)  /* coeff of 2^i in bin repr of x */
#define XOR(x,y,i) ( BIT(x,i) ^ BIT(y,i) )      /* xor of i-th bit of x,y */



enum colors { WHITE, GRAY, BLACK };

typedef struct node
	{
	int val;
	struct node *link;
	} NODE;



main()
{
	NODE **a;           /* array list of adjacencies */
	enum colors *color; /* array of colors */
	int *pred;          /* predecessor array */
	int *disc;          /* discovery time array (for dfs) */
	int *fini;          /* finish time array (for dfs) */
	int *dist;           /* distance array (for bfs) */

	/******************** function prototypes **************/
	NODE **adjacencylist();
	void dfs(NODE **a, enum colors *color, int *pred,int *disc, int *fini);
	void printMWGC (int i, int j, int *pred);
	void bfs(int s, NODE **a, enum colors *color, int *pred,int *dist);


	/************** creation of arrays *************************/
	color = (enum colors *) calloc(NUMSTATES,sizeof(enum colors));
	pred = (int *) calloc(NUMSTATES,sizeof(int));
	disc = (int *) calloc(NUMSTATES,sizeof(int));
	fini = (int *) calloc(NUMSTATES,sizeof(int));
	dist = (int *) calloc(NUMSTATES,sizeof(int));


	a = adjacencylist();
	printgraph ( a );
	dfs(a,color,pred,disc,fini);
	printf("\nDepth first search\n\n");
	printMWGC(START,FINISH,pred);
	printf("\nBreadth first search\n\n");
	bfs(START,a,color,pred,dist);
	printMWGC(START,FINISH,pred);
}


void bfs(int s, NODE **a, enum colors *color, int *pred, int *dist)
{
	/* breadth first search beginning at vertex s to compute
	   the color, predecessor and distance arrays.
	   External functions whose prototypes are in queue.h
	   and which are used in bfs() are:

			extern int  empty(void);   -- test if list empty
			extern void create(void);  -- create list with tail = NULL
			extern void move(void);    -- tail = tail-> link
			extern void delete(void);  -- delete head, tail remains same
			extern void join(int);     -- join new node at end, tail changes
			extern int  leave(void);   -- leave at head, tail remains same */



	int i,u,v;
	NODE *p;        /* p points to adjacency list of vertex u */

	for (i=0;i<NUMSTATES;i++)
		{
		color[i] = WHITE;
		dist[i] = INF;
		pred[i] = NIL;
		}
	color[s] = GRAY;
	dist[s] = 0;
	pred[s] = NIL;
	create();           /* create queue */
	join(s);            /* enqueue s */
	while ( !empty() )
		{
		u = leave();    /* u = head of queue */
		p = a[u];
		while ( p != NULL )
			{
			v = p->val;
			if (color[v] == WHITE)
				{
				color[v] = GRAY;
				dist[v]++;
				pred[v] = u;
				join(v);       /* enqueue v */
				}
			p = p->link;
			}
		color[u] = BLACK;       /* finished processing u */
		}
}

void dfs(NODE **a, enum colors *color, int *pred,
		 int *disc, int *fini)
{
	int i,time;         /* i loop control, time for timestamp in dfs */
	void dfsvisit(int i, int time, NODE **a,
				  enum colors *color, int *pred, int *disc, int *fini);

	/***************initializations***************************/
	time = 0;       /* initialize timestamp */
	for (i=0;i<NUMSTATES;i++)
		{
		color[i] = WHITE;
		pred[i] = NIL;
		}


	for (i=0;i<NUMSTATES;i++)
		if ( color[i] == WHITE )
			dfsvisit(i,time,a,color,pred,disc,fini);
}

void dfsvisit(int i, int time, NODE **a,
			  enum colors *color, int *pred, int *disc, int *fini)
{
	NODE *p;
	int j;

	color[i] = GRAY;
	disc[i] = ++time;   /* increment time and dist[i] */
	p = a[i];
	while (p != NULL)
		{
		j = p->val;     /* j is next in adjacency list of i */
		if (color[j] == WHITE)
			{
			pred[j] = i;    /* i-->j is tree edge in dfs tree */
			dfsvisit(j,time,a,color,pred,disc,fini);
			}
		p = p->link;    /* go to next in adjacency list of i */
		}
	color[i] = BLACK;
	fini[i] = ++time;   /* increment time and fini[i] */
}



void printpath (int i, int j, int *pred)
{
	/* prints path from node i to node j if such exists
	   this is a generic print function for any dfs  */

	if (i == j)
		printf("%d ",i);
	else if (pred[j] == NIL)
		printf("No path from %d to %d.\n",i,j);
	else
		{
		printpath(i,pred[j],pred);
		printf("%d ",j);
		}
}


void printstateMWGC(int i)
{
	/* instead of integer i, prints the occupants of both
	   sides of the river in man, wolf, goat, cabbage problem */
	if (BIT(i,3) == 0) printf("man ");
	if (BIT(i,2) == 0) printf("wolf ");
	if (BIT(i,1) == 0) printf("goat ");
	if (BIT(i,0) == 0) printf("cabbage ");
	printf(" || ");
	if (BIT(i,3) == 1) printf("man ");
	if (BIT(i,2) == 1) printf("wolf ");
	if (BIT(i,1) == 1) printf("goat ");
	if (BIT(i,0) == 1) printf("cabbage ");
	printf("\n");
}

void printMWGC (int i, int j, int *pred)
{
	/* prints the occupants of both river banks in the
	   man, wolf, goat, cabbage problem */

	void printstateMWGC(int i);

	if (i == j)
		printstateMWGC(i);
	else if (pred[j] == NIL)
		printf("No path from %d to %d.\n",i,j);
	else
		{
		printMWGC(i,pred[j],pred);
		printstateMWGC(j);
		}
}


int edge(int i, int j)
{
	/* returns 1 if i-->j is edge (valid move), 0 else */
	int hamming, wolfeatsgoat, goateatscabbage;
	int manmoves, moveswithman, forbidden, k;
		/* hamming distance is the number of different bits between x,y.
		   moveswithman checks whether the bits (items) which changed
		   between x,y moved with the man who is required to pilot the
		   boat. This outlaws the otherwise possible move
				1010 to 0001, i.e.
				wolf cabbage || man goat TO man wolf goat || cabbage.
		   forbidden node is if wolfeatsgoat or goateatscabbage. */

	/*************** check whether man moves **********************/
	manmoves = XOR(i,j,3);


	/**************** check whether state j corresponds to someone eaten */
	wolfeatsgoat = BIT(j,3)^BIT(j,2) && BIT(j,3)^BIT(j,1);
	goateatscabbage = BIT(j,3)^BIT(j,1) && BIT(j,3)^BIT(j,0);
	forbidden = wolfeatsgoat || goateatscabbage;
			/**** state j is forbidden if someone gets eaten ******/


	/*********check that items which moved, indeed moved with man */
	moveswithman = 1;    /* initially assume correct */
	for (k=0;k<4;k++)
		if ( BIT(i,k)^BIT(j,k) && BIT(j,3)^BIT(j,k) )
			{
			moveswithman = 0;
			break;
			}

	/*********** compute hamming distance to count number items moved */
	hamming = XOR(i,j,0) + XOR(i,j,1) + XOR(i,j,2) + XOR(i,j,3);

	forbidden = wolfeatsgoat || goateatscabbage;
	return(manmoves && !forbidden && hamming < 3 && moveswithman);


}



NODE** adjacencylist ( )
{
	/* adjacencylist() returns a pointer to NODE*; i.e. an
	   array of pointers to NODE, where a[i] points to an
	   adjacency list for i */
	int i,j;
	NODE **a;   /* a is array of pointers to NODE */
	NODE *p;    /* pointer p used for creating new node */
	int edge(int i, int j); /* prototype */

	a = (NODE **) calloc(NUMSTATES,sizeof(NODE*));
	for (i=0;i<NUMSTATES;i++)
		{
		a[i] = NULL;    /* initialize adjacency list for i to be empty */
		for (j=NUMSTATES-1;j>=0;j--)
			{
			if ( !edge(i,j) )
				continue;  /* go on to next j if i-->j not edge */
			p = (NODE *) malloc(sizeof(NODE));
			p->val = j;
			p->link = a[i]; /* if adj list empty then p->link is NULL */
			a[i] = p;
			}
		}
	return(a);
		/* though not important, the inner loop is of form
				for (j=NUMSTATES-1;j>=0;j--)
		   rather than
				for (j=0;j<NUMSTATES;j++)
		   because our "quick and dirty" insertion into the
		   adjacency list for i is at the head of the list,
		   rather than at the tail (which would be the case
		   for the linked list structure -- but then we would
		   need an extra pointer for the tail of the list, or
		   instead use a circular list, etc. This extra complication
		   is not really merited by the problem at hand. */

}






/******************** routines used for debugging *************/

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

	for (i=0;i<NUMSTATES;i++)
		{
		printf("%d:\t",i);
		traverse(a[i]);
		printf("\n");
		}
}


