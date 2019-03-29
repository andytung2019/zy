#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h>


#include "queue.h"
//#define REPORT_TIME (15*60) //every 15minutes  report
#define REPORT_TIME (2) //every 2s  report only for test
#define TIMER_CMD_START 0xff000000
#define TIMER_CMD_MAX 65535
t_cmd_q *q = NULL;
t_u32 timer_cmd_id = 0;



void* timer_cmd(void *arg) {
	int ret;
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));	
	
	cmd.dev_id = 0x01;
	cmd.cmd = 0x03;
	cmd.param[0] = 0xfc;
	cmd.param[1] = 0x00;
	cmd.param[2] = 0x00;
	cmd.param[3] = 0x0B;
 	
    while(1) {
		timer_cmd_id = (timer_cmd_id + 1) % TIMER_CMD_MAX;
		cmd.cmd_id = TIMER_CMD_START + timer_cmd_id;
	
		ret = en_queue(q, &cmd);
		  	if (ret < 0) {
				printf("add cmd:%04x to queue error !\n", cmd.cmd_id);			
			 } else{
				
				printf("add cmd:%04x to queue ok !\n", cmd.cmd_id);			
			}
			
			sleep(REPORT_TIME);
	}	
}



void* server_cmd(void *arg) {
	char *cmd_url="https://cn.bing.com";
	int ret1,ret2 ;
	char buf[1024];
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));
	cmd.cmd_id = 0x003;	

		
	while(1) {
		memset(buf, 0, 1024);
    	ret1 = get_url(cmd_url, &buf);
		ret2 = parse_json_cmd(&buf, &cmd);	
     	if( ret2 == 0) {
          en_queue(q, &cmd);
     	}	
		sleep(5);
	//print_queue(q);
	}	
}

void* read_cmd(void *arg) {
	int ret; 
    t_modcmd cmd;

	while(1) {
		memset(&cmd, 0, sizeof(t_modcmd));
		ret = de_queue(q, &cmd);
	    
		if(ret <= 0 ) {
			printf("queue de null \n");
		}	else {
			printf("de queue cmd:%04x \n", cmd.cmd_id);
			//run the cmd , and get return 
		}
		sleep(1);
		
	}	
}




int main(void) {
	printf("start test queue \n");
	pthread_t th_server_cmd;
    pthread_t th_timer_cmd; 	
    pthread_t th_read_cmd; 	

	q = init_queue();
	printf("start queue: %p\n",q);

        //print_queue(q);
	t_modcmd cmd;
	
	int ret1, ret2, ret3;
	ret1 = pthread_create(&th_server_cmd, NULL, server_cmd, NULL);
	ret2 = pthread_create(&th_timer_cmd, NULL,timer_cmd, NULL);
	ret3 = pthread_create(&th_read_cmd, NULL, read_cmd, NULL);
	while(1) {

       
		sleep(10);

	}	

}
