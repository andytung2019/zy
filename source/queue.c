#include<stdio.h>
#include <queue.h>


static t_queue *p = null;
static int start = 0;
static int end = 0;
static int cur = 0;

int init_queue(t_queue *p)
  if( null == p){
		p = (t_queue *)malloc(LEN_QUEUE*sizeof(t_queue)); 
    }
   
   if (null == p ) {
		printf(" malloc queue failed!\n");
		return -1;	
   }
     
  
}


int en_queue(t_queue *q , t_queue data) {



}

int  de_queue(t_queue *q, t_queue *recv) {


}

int clear_queue(t_queue *q ) {


}


int free_queue(t_queue *q) {



