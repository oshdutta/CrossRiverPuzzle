#include<stdlib.h>
#include <stdio.h>
#include "qlist.c"
#include <limits.h>
#include<stdbool.h>

#define STARTST 0         // start state is 0 
#define ENDST 15   /* end state is 15 */
#define TOTSTATES 16
typedef struct node
{
	int val;
	bool is_visited;
	struct node *link;
} NODE;

typedef struct directed
{
	int val_1;
	int val_2;
	struct directed *link;
} DIRECT;

int validornot(int j);
int validedges(int i, int j);
NODE** adjacencylist();

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
	// returns 1 if i-->j is edge (valid move), 0 else 
	int s;
	int Fmoves, moveswithF, k;
	Fmoves = (( (i) >> (0) ) & 1) ^ (( (j) >> (0) ) & 1);
	moveswithF = 1; 
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
	int forbstartstate= validornot(i);
	return(Fmoves && !forbidden && s < 3 && moveswithF && !forbstartstate);
}

NODE** adjacencylist( )
{
	int i,j, temp_states =TOTSTATES, temp_i = 0, queue[TOTSTATES], front = -1, rear = -1;
	NODE **a;   
	NODE *p;   
	int validedges(int i, int j); 
	a = (NODE **) calloc(TOTSTATES,sizeof(NODE*));
	for (i=0;i<TOTSTATES;i++)
        a[i] = NULL;
	rear++;
	queue[rear] = 0;
    while (temp_states)
	{
		i = temp_i;
		for (j=0;j<TOTSTATES;j++)
		{
            p = (NODE *) calloc(TOTSTATES,sizeof(NODE));
			if ( !validedges(i,j) )
				continue;  /* go on to next j if i-->j not edge */
            if(a[j]!=NULL)
            {
                printf("a[j]->val = %d, i = %d\n", a[j]->val, i);
                if(a[j]->val == i)
                    continue;
            }
            p->val = j;
			p->is_visited = false;
			p->link = a[i]; /* if adj list empty then p->link is NULL */
			a[i] = p;
            temp_i = j;   
		}
        temp_states--;
	}
	return a;
}

void dfs(int source, int destination, NODE **adj_list, int stack[TOTSTATES],int stack_index,int path[], int index)
{
	path[index]= source;
	index++;
	
	if(source == destination)
	{
		printf("\n visited nodes are===================================================\n");
	    for (int i = 0; i < index; i++)
	    {
		    printf("%d \t",path[i]);
	    }
	    printf("\n =====================================================================\n");
	}
	else
	{
        NODE *temp;
        temp = adj_list[source];
		while (temp!= NULL)
        {
            stack[stack_index] = temp->val;
            stack_index++;
            temp = temp->link;   		
	    }
        for (int i = 0; i < index; i++)
        {
            if(path[i] == stack[stack_index-1])
            {
                stack_index = stack_index-1;
            }
        }
        dfs(stack[stack_index-1], destination, adj_list, stack, stack_index,path, index);
    }
}
void print_paths(NODE **adj_list)
{
	int stack[TOTSTATES],stack_index = 0, source = 0, destination = 15,index = 0, path[TOTSTATES];
	dfs(source, destination, adj_list, stack, stack_index,path, index);
	
}
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



int main()
{
	NODE **adjlist;	
	adjlist = adjacencylist();
	print_paths(adjlist);
    printgraph(adjlist);
	return 0;
}