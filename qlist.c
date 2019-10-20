
#include<stdio.h>
#include <stdlib.h>                    /* prototypes for malloc, NULL */


struct nodetype
   {
   int val;
   struct nodetype *link;
   };


typedef struct nodetype Nodetype;
typedef struct nodetype *Pointer;

#define NEW (Pointer) malloc(sizeof(Nodetype))
#define EMPTY()  (length == 0)

static Pointer tail = NULL;
static int length = 0;

void create()
{
   length = 0;
   tail = NULL;
}

int empty(void)
{
	return( EMPTY() );
}

int gethead(void)
{
	if ( !EMPTY() ) return(tail->link->val);
}


int gettail(void)
{
	if ( !EMPTY() ) return(tail->val);
}


void printlist(void)
{
	Pointer p,q;

	if ( !EMPTY() )
		{
		p = tail->link;
		printf("%d ",p->val);
		q = p->link;
		while ( q != p )
			{
			printf("%d ",q->val);
			q = q->link;
			}
		}
}




void join(int n)
{
   Pointer p;

   if (EMPTY())
      {
      tail = NEW;
      tail->val = n;
      tail->link = tail;
      }
   else
      {
      p = NEW;
      p -> val = n;
      p -> link = tail -> link;
      tail -> link = p;
      tail = p;
      }
   length++;
}  

void move()
{
   tail = tail -> link;
}

void delete()
{
	Pointer p;

	p = tail -> link;
	tail -> link = p -> link;
	free(p);
	length--;
}


int leave()
{
   int n;
   void delete(void);

   n = (tail->link)->val;
   delete();
   return n;
}
