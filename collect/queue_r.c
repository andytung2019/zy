#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
//#include<pthread.h>
//#include<semaphore.h>

#include "queue_r.h"


t_ret_q* init_queue(void) {
	t_ret_q *q = NULL;
	q = (t_ret_q *)malloc(sizeof(t_ret_q));

	if( NULL == q) {
		printf(" malloc queue failed!\n");
		return NULL;
	}	
	
	memset(q, 0, sizeof(t_ret_q));
	q->len = 0;
	q->head = 0;
	q->tail = 0;
	
	pthread_mutex_init(&q->mutex, NULL);
		return q;
}

int en_queue(t_ret_cmd *q, t_ret_cmd *p_cmd) {
	t_ret_cmd *p;
	if(NULL == q || NULL == p_cmd) {
		return -1;
	}

	if(q->len >= MAX_CMD_NUM ){
		return -2;
	}
//	sem_wait(&q->empty);
	pthread_mutex_lock(&q->mutex);
		
	p = &(q->cmd_list[q->tail]);
	memset(p, 0, sizeof(t_ret_cmd));	
	memcpy((t_u8*)p, (t_u8*)p_cmd, sizeof(t_ret_cmd));
	q->tail = (q->tail + 1) % MAX_CMD_NUM;
	q->len += 1;
	
	pthread_mutex_unlock(&q->mutex);
//	sem_post(&q->full);
	return q->len;

}	

//only one thread can de queue
int de_queue(t_cmd_q *q, t_ret_cmd *pcmd) {

	if(NULL == q || NULL == pcmd) {
		return -1;
	}

		
	if(q->len <= 0 ) {
		return 0;
	}
//	sem_wait(&q->full);
	pthread_mutex_lock(&q->mutex);
	memcpy((t_u8*)pcmd,(t_u8*)&q->ret_list[q->head], sizeof(t_ret_cmd));
	t_ret_cmd *p = &(q->ret_list[q->head]);
	
	q->head = (q->head + 1) % MAX_CMD_NUM;
	q->len -= 1;
	pthread_mutex_unlock(&q->mutex);
//	sem_post(&q->empty);
    return 1 ;	
	

}

int reset_queue(t_cmd_q *q) {
	if( NULL == q) {
		return -1;
	}

	if (NULL != q){
	    
		memset(q, 0, sizeof(t_cmd_q));
		q->len = 0;
		q->head = 0;
		q->tail = 0;
	}
	
        return 0;
	
}

int queue_is_empty(t_cmd_q *q) {

	if( NULL == q) {
		return 0;
	}

	if( q->len == 0) {
		return 1;
	}

	return 0;
}

int q_is_full(t_cmd_q *q) {
	if(NULL == q) {
		return 0;
	}

	if(q->len == MAX_CMD_NUM) {

		return 1;
	}
	return 0;
}

int len_queue(t_cmd_q *q) {
	if(NULL == q ) {
		return -1;
	}

	return q->len;
}

int free_queue(t_cmd_q *q){
	if( NULL == q) {
		return -1;
	}
	memset(q, 0, sizeof(t_cmd_q));
	free(q);
	
	pthread_mutex_destroy(&q->mutex);

		if(q->len <= 0 ) {
		//waiting_threads++;
		//while(1 == queue_is_empty(q)) {
		//	pthread_cond_wait(&cond, &mutex);
		//}
		//waiting_threads--;

		pthread_mutex_unlock(&q->mutex);
		return 0;
	}	

	printf("free the queue:%p\n", q);
	return 0;
}


/* ...............test code ................*/
void print_queue(t_cmd_q *q) {
     if(NULL == q) {
      printf(" null queue !\n");
     } 

     printf("queue:%p; size:%d, head:%d, tail:%d\n", q, q->len, q->head, q->tail);
}

void print_list(t_cmd_q *q) {

	if (NULL == q) {
		printf(" null queue \n");
	
	}
	for(int i = 0; i < 48; i++ ) {
		t_ret_cmd *p = &(q-ret_list[i]);
		printf(" %02x ", p->cmd_id);
	}
	printf("\n");
}

/*
int main(void) {
	t_cmd_q *q = NULL;
	q = init_queue();
	printf("start queue: %p\n",q);

        print_queue(q);
	t_ret_cmd cmd;
	int ret;
	ret = en_queue(q, &cmd);
	print_queue(q);

	ret = en_queue(q, &cmd);
	print_queue(q);


	return 0;
}*/


#if 0  
void* en_q2(void *arg) {
	t_ret_cmd cmd;
	memset(&cmd, 0, sizeof(t_ret_cmd));	
	while(1) {

		printf(" en queue2  thread>>>>>>");
		sleep(3);
		for (int i = 0; i < 50; i++ ) {
			en_queue(q, &cmd);
		}
		print_queue(q);
	}	
}



void* en_q(void *arg) {
	t_ret_cmd cmd;
	memset(&cmd, 0, sizeof(t_ret_cmd));	
	while(1) {

		printf(" en queue  thread-----");
		sleep(2);
		for (int i = 0; i < 50; i++ ) {
			en_queue(q, &cmd);
		}
		print_queue(q);
	}	
}

void* de_q(void *arg) {
	t_ret_cmd cmd;
	memset(&cmd, 0, sizeof(t_ret_cmd));
	while(1) {

		printf(" de queue  thread.... ");
		sleep(1);
		de_queue(q, &cmd);
		
		print_queue(q);
	}	
}



int main(void) {
	printf("start test queue \n");
	pthread_t de_th;
        pthread_t en_th; 	
        pthread_t en_th2; 	

	q = init_queue();
	printf("start queue: %p\n",q);

        print_queue(q);
	t_ret_cmd cmd;
	
	int ret1, ret2, ret3;
	ret1 = pthread_create(&de_th, NULL, de_q, NULL);
	ret2 = pthread_create(&en_th, NULL, en_q, NULL);
	ret3 = pthread_create(&en_th2, NULL, en_q2, NULL);
	while(1) {
	 sleep(10);

	}	

} 
#endif

