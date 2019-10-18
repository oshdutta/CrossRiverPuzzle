/*solving the wolf sheep cabbage Farmer problem*/
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<math.h>

//create statelist-apply rules to define transition costs
/*or define (0000)as start state (1111) as end state->check safeornot(not eaten)
//run search algorithm to get different paths to the goal from the source*/

typedef struct _state
{
	int st[4];//example- [0,0,0,0] meaning all are on the left bank; (1,1,1,1) meaning all are on the right bank
	bool visited;
	struct _state *prev;
	struct _state *next;
}STATE;

typedef struct _pathlist
{
	STATE *start;
}PATH;

bool isitsafe(STATE *);
assignNextState(STATE *);
swapPosition(STATE *)