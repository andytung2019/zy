#include<stdio.h>
#include <queue.h>


t_queue* init_queue(void) {

  t_queue *p = null;

	p = (t_queue *)malloc(sizeof(t_queue)); 
 
   if (null == p ) {
		printf(" malloc queue failed!\n");
		return null;	
   }
  
  // init the mem in queue; 
  memset((unsigned char*)p->block, 0, NUM_BLOCK*BLOCK_RECORD*LEN_RECORD); //mem total len: 16*100*48
  p->len = 0;
  p->head = 0;
  p->tail = 0;
  return p;
}


int en_queue(t_queue *q , t_block data) {
  t_block *pBlock = null;

  if (null == q) {
    return -1;
  }
  
  // the queue full 
  if (q->len == NUM_BLOCK) {
    return -2;
  }

  if (q->len == 0) {
    memecpy(q->block[q->head], data, sizeof(t_block));
    q->tail = 1;
    q->len = q->len + 1;
    return q->len;  
  } 
  
  // queue is not empty and not full
  pBlock = q->tail;
  memcpy((unsigned char*)q->tail, data, sizeof(t_block));
  q->len = q->len + 1;
  q->tail = q->block[q->len];
  return q->len;
   

}

int  de_queue(t_queue *q, t_block *recv) {
   if( null == q) {
     return -1;
   }
   if( q->len == 0) {
    return 0;
   }

   memcpy(recv, q->head, sizeof(t_block));
      
      //head is the end of array;

   q->head = 

}



int free_queue(t_queue *q) {
  if ( null != q) {
    memset((unsigned char*)p->block, 0, NUM_BLOCK*BLOCK_RECORD*LEN_RECORD); //mem total len: 16*100*48
    free(q);
  }

  return 0;
}



