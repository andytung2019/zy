#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include <curl/curl.h>

#include "queue.h"
#include "queue_r.h"
#include "get_device.h"

//#define REPORT_TIME (15*60) //every 15minutes  report
#define REPORT_TIME (30) //every 2s  report only for test
#define GET_SERV_TIME (20)
#define TIMER_CMD_START 0x10000000
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

    	printf(" dev_list.num: %d\n", dev_list.num); 
 	if( dev_list.num > 0) {
    
	  while(1) {
  		for( int i = 0; i < dev_list.num; i++ ) {
				memset(&cmd, 0, sizeof(t_modcmd));		
				cmd.dev_id = dev_list.dev[i].position_id; //this position id is modbus id in fact;
				cmd.cmd = 13;

				timer_cmd_id = (timer_cmd_id + 1) % TIMER_CMD_MAX;
				cmd.cmd_id = TIMER_CMD_START + timer_cmd_id;
			
				ret = en_queue(q, &cmd);
		  		if (ret < 0) {
					printf("add cmd:%04x to queue error !\n", cmd.cmd_id);			
			 	} else{
						printf("add cmd:%04x to queue ok !\n", cmd.cmd_id);			
				}
			}
	
		sleep(REPORT_TIME);
	 }	
     }
}

void* server_cmd(void *arg) {
	char *cmd_url = "http://132.232.25.130:8080/station/control/getcmd?stationId=20001";
	int dev_id = 0;

	int ret, ret1,ret2 ;
	char buf[1024];
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));
	
		
	while(1) {
		memset(buf, 0, 1024);
    		ret1 = get_url(cmd_url, (char*)&buf[0]);
		ret2 = parse_json_cmd((char *)&buf[0], &cmd);	
     		if( ret2 == 0) {
  			// change device id to positon 
 			dev_id = get_position(&dev_list, cmd.dev_id); //this position id is modbus id in fact;
			if (dev_id < 0) {
				printf(" run server cmd, get error device id\n");
			
			} else {
			
				cmd.dev_id = dev_id;
				ret = en_queue(q, &cmd);
		  		if (ret < 0) {
					printf("add server cmd:%04x , dev_id:%d to queue error !\n", cmd.cmd_id , cmd.dev_id);			
			 	} else{
				
					printf("add server cmd:%04x, dev_id:%d to queue ok !\n", cmd.cmd_id, cmd.dev_id);			
				}	
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
			if(cmd.cmd_id < TIMER_CMD_START) {	
				printf("send cmd return: %04x, %d\n", cmd.cmd_id, cmd.cmd);
				rt_serv = send_ret(&cmd);		
			}
			//run the cmd , and get return
			rt_cmd = run_cmd(&cmd, &ret);
			printf("run cmd: %04x, %d\n", cmd.cmd_id, cmd.cmd);

			if (rt_cmd == 0) {
	
				send_report(&ret);
		
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


int main(void) {
	printf("start test queue \n");
	pthread_t th_server_cmd;
  pthread_t th_timer_cmd; 	
  pthread_t th_read_cmd; 
  pthread_t th_report;	

  int num_dev = 0;

	//init curl 
	curl_global_init(CURL_GLOBAL_ALL);
	// get devices list
	num_dev = get_dev_list(&dev_list,station_id);
	printf("\nget dev num:%d\n", dev_list.num);
 	for(int i = 0; i < dev_list.num; i++) {
		printf("device: %d, position_id: %d\n", dev_list.dev[i].device_id, dev_list.dev[i].position_id);
	}
	
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
