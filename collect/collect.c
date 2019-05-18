#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include <curl/curl.h>

#include "queue.h"
#include "queue_r.h"
#include "collect.h"

//#define REPORT_TIME (15*60) //every 15minutes  report
#define REPORT_TIME (30) //every 2s  report only for test
#define GET_SERV_TIME (20)
#define TIMER_CMD_START 0xff000000
#define TIMER_CMD_MAX 65535

//very important!! station_id is the only id for 1 station and 1 collector
int station_id = 10001;
//very import! get device list for the current station


t_cmd_q *q = NULL;
t_ret_q *rq = NULL;

t_u32 timer_cmd_id = 0;

DEV_LIST  dev_list;


void* timer_cmd(void *arg) {
	int ret;
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));	
	
	cmd.dev_id = 0x01;
	cmd.cmd = 13;
 	
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
	//char *cmd_url="http://149.28.67.234:8080/ZyTest/send";
	char *cmd_url = "http://120.77.168.74/phpweb/htpst.php?station=20001";

	int ret, ret1,ret2 ;
	char buf[1024];
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));
	
		
	while(1) {
		memset(buf, 0, 1024);
    	ret1 = get_url(cmd_url, (char*)&buf[0]);
		ret2 = parse_json_cmd((char *)&buf[0], &cmd);	
     	if( ret2 == 0) {
   
			ret = en_queue(q, &cmd);
		  	if (ret < 0) {
				printf("add server cmd:%04x to queue error !\n", cmd.cmd_id);			
			 } else{
				
				printf("add server cmd:%04x to queue ok !\n", cmd.cmd_id);			
			}

     	} else {
			printf(" parse server command error :%d\n", ret2);
		}	
		sleep(GET_SERV_TIME);
	//print_queue(q);
	}	
}

void* read_cmd(void *arg) {
	int rt;
	int rt_cmd;
	int rt_serv;
    	t_modcmd cmd;
	t_mod_ret ret;

       	while(1) {
		memset(&cmd, 0, sizeof(t_modcmd));
		memset(&ret, 0, sizeof(t_mod_ret));
		rt = de_queue(q, &cmd);
	    
		if(rt <= 0 ) {
			printf("queue de null \n");
		}
		else {
			printf("de queue cmd:%04x \n", cmd.cmd_id);
		
			//cmd come from server, send return to server 
	//		rt_serv = send_ret(&cmd);		
			
			//run the cmd , and get return
			rt_cmd = run_cmd(&cmd, &ret);
			printf("run cmd: %04x, %d\n", cmd.cmd_id, cmd.cmd);

			if (rt_cmd == 0) {
	
				send_report(&ret);
			//	rt = en_rqueue(rq, &ret);
		  //		if (rt < 0) {
		  //			printf("add return of cmd:%04x to rqueue error !\n", ret.cmd_id);			
		  //	 	}	 else{
				
		//		printf("add return of cmd:%04x to rqueue ok !\n", ret.cmd_id);			
		//		}
			}
			

		}
		sleep(1);
		
	}	
}


void* report(void *arg) {
	int rt; 
	t_mod_ret ret;

       	while(1) {
		memset(&ret, 0, sizeof(t_mod_ret));
		rt = de_rqueue(rq, &ret);
		if(rt <= 0 ) {
			printf("rqueue de null \n");
		}
		else {
			printf("rqueue de ............\n");
			send_report(&ret, station_id);
		}
		sleep(1);
		
	}	
}


int get_dev_list(int station_id){
 char *url="http://132.232.25.130:8080/station/getdev?station=10001";
 DEV_LIST dev_list;
 dev_list.dev[0].device_id=2;
 dev_list.dev[0].position_id = 2; 
 dev_list.num = 1; 
}

int main(void) {
	printf("start test queue \n");
	pthread_t th_server_cmd;
    	pthread_t th_timer_cmd; 	
    	pthread_t th_read_cmd; 
        pthread_t th_report;	

	//init curl 
	curl_global_init(CURL_GLOBAL_ALL);
	q = init_queue();
	printf("start queue: %p\n",q);
	rq = init_rqueue();
        //print_queue(q);
	t_modcmd cmd;
	
	int ret1, ret2, ret3,ret4;
	ret1 = pthread_create(&th_server_cmd, NULL, server_cmd, NULL);
	ret2 = pthread_create(&th_timer_cmd, NULL,timer_cmd, NULL);
	ret3 = pthread_create(&th_read_cmd, NULL, read_cmd, NULL);
//	ret4 = pthread_create(&th_report, NULL, report, NULL);
	while(1) {

       
		sleep(10);

	}	

}
