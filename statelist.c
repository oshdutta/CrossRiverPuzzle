#include<stdlib.h>
#include <stdio.h>
#include "qlist.c"
#include <limits.h>
#include<stdbool.h>
/* (WCGM)
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
	bool visited;
	} NODE;

void printallstates (int s, int e, int *prd);
void printstate(int s);
void traverse ( NODE *p );
void printgraph ( NODE **a );
int validornot(int j);
int validedges(int i, int j);
NODE** adjacencylist( );
void printAllPaths(int , int , NODE**); 
void printAllPathsUtil(int u, int d, int path[], bool visited[], int path_index, NODE **adjlist);

int main()
{
	NODE **adjlist; //adj list
	adjlist = adjacencylist();
	//printgraph ( adjlist );

    printf("The paths from INITIAL STATE:All on the left bank of the river to FINAL STATE: All on the right bank \n");
    printAllPaths(STARTST, ENDST,adjlist); 
	return 0;
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
		/* moveswithF checks whether man moved with the boat. 
			This outlaws the otherwise possible move
				0000 to  1100, i.e.
				Goat man wolf cabbage ||  TO man goat || wolf cabbage.
		  	 forbidden moves are if start state or end state if such that wolfeatsgoat or goateatscabbage. */

	//check if man moves
	Fmoves = (( (i) >> (0) ) & 1) ^ (( (j) >> (0) ) & 1);


		// check if man moves with items
	moveswithF = 1;    /* initially assume correct */
	for (k=0;k<4;k++)
		if ( (( (i) >> (k) ) & 1)^(( (j) >> (k) ) & 1)&& (( (j) >> (0) ) & 1)^(( (j) >> (k) ) & 1))
			{
			moveswithF = 0;
			break;
			}

	//count items moved
	int r=i ^ j;
	 s= (( (r) >> (0) ) & 1) +(( (r) >> (1) ) & 1) +(( (r) >> (2) ) & 1) +(( (r) >> (3) ) & 1) ;

	int forbidden = validornot(j);
	int forbstartstate= validornot(i);
	return(Fmoves && !forbidden && s < 3 && moveswithF && !forbstartstate);


}
NODE** adjacencylist( )
{
	/* adjacencylist() return an array of pointers to NODE, */

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
			p->visited=false;//mark all as not visited
			a[i] = p;
			}

		}
	return(a);
}
void printstate(int x)
{
	/* prints the state with occupants  at both sides of the river*/
	if ((( (x) >> (3) ) & 1)  == 0) printf("Wolf ");
	if ((( (x) >> (2) ) & 1)  == 0) printf("Cabbage ");
	if ((( (x) >> (1) ) & 1)  == 0) printf("Goat ");
	if ((( (x) >> (0) ) & 1)  == 0) printf("man");
	printf(" || ");
	if ((( (x) >> (3) ) & 1)  == 1) printf("Wolf ");
	if ((( (x) >> (2) ) & 1)  == 1) printf("Cabbage ");
	if ((( (x) >> (1) ) & 1)  == 1) printf("Goat ");
	if ((( (x) >> (0) ) & 1)  == 1) printf("man");
	printf("\n");
}

/*//check graph- for inital debugging
void traverse ( NODE *p )
{
	//traverses adjacency list pointed to by p 
	if (p != NULL)
		{
		printf("%d, ",p->val);
		traverse(p->link);
		}
}*/
/*void printgraph ( NODE **a )
{
	// for each vertex i of the graph G prints adjacency list for i 
	int i;

	for (i=0;i<TOTSTATES;i++)
		{
		printf("%d:\t",i);
		traverse(a[i]);
		printf("\n");
		}
}*/
void printAllPaths(int s, int d, NODE ** adjlist) 
{ 
    // Mark all the vertices as not visited 
   bool *visited =(bool *) calloc(TOTSTATES,sizeof(bool));
  
    // array to store paths 
    int *path = (int *) calloc(TOTSTATES,sizeof(int));
    int path_index = 0; // Initialize path[] as empty 
  
    // Initialize all states as not visited 
   for (int i = 0; i < TOTSTATES; i++) 
        visited[i] = false; 
  
    // to print all paths from source to destination
    printAllPathsUtil(s, d, path, visited, path_index,adjlist); 
    
} 
void printAllPathsUtil(int u, int d, int path[], bool visited[], int path_index, NODE **adjlist) 
{ 
    // current node marked and stored in path[] 
   visited[u] = true; 
   
    path[path_index] = u; 
    path_index++; 
   // printf("till here t1 \n");
    // If current state is same as destination state, then print 
    // current path[] 
    if (u == d) 
    { 
        for (int i = 0; i<path_index; i++) 
        	{
           printf("%d ->", path[i]);
       		printstate(path[i]);
       			}
        
       	/*printf("%d", path[path_index-1]);
       	printstate(path_index-1);*/
         printf("\n"); 
         
          
    } 
    else // If current vertex is not destination 
    { 	NODE *i;
    	i=adjlist[u];
    	//printf("till here t2 \n");
        // Recur for all the vertices adjacent to current vertex 
        while(i!= NULL)
        {

        	//printf("till here t3 %d \n", i->val);
        	//i=i->link;
            if (!(visited[i->val])) 
                printAllPathsUtil(i->val, d, path, visited, path_index, adjlist); 
            i=i->link;
        }
    } 
  
    // Remove current vertex from path[] and mark it as unvisited 
    path_index--; 
    visited[u] = false; 
    
} 


