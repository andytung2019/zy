#include "queue.h"

t_cmd_q *q = NULL;

void* en_q2(void *arg) {
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));	
	while(1) {

		printf(" en queue2  thread>>>>>>");
		sleep(3);
		for (int i = 0; i < 50; i++ ) {
			en_queue(q, &cmd);
		}
		//print_queue(q);
	}	
}



void* en_q(void *arg) {
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));	
	while(1) {

		printf(" en queue  thread-----");
		sleep(2);
		for (int i = 0; i < 50; i++ ) {
			en_queue(q, &cmd);
		}
		//print_queue(q);
	}	
}

void* de_q(void *arg) {
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));
	while(1) {

		printf(" de queue  thread.... ");
		sleep(1);
		de_queue(q, &cmd);
		
		//print_queue(q);
	}	
}




int main(void) {
	printf("start test queue \n");
	pthread_t de_th;
        pthread_t en_th; 	
        pthread_t en_th2; 	

	q = init_queue();
	printf("start queue: %p\n",q);

        //print_queue(q);
	t_modcmd cmd;
	
	int ret1, ret2, ret3;
	ret1 = pthread_create(&de_th, NULL, de_q, NULL);
	ret2 = pthread_create(&en_th, NULL, en_q, NULL);
	ret3 = pthread_create(&en_th2, NULL, en_q2, NULL);
	while(1) {

       
		sleep(10);

	}	

}
