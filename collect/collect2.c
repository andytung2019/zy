#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<curl/curl.h>

#include "queue.h"
#define REPORT_TIMER 10   // define 10s for report  test . 
#define CMD_TIMER_START 0xff000000;
static t_u32 timer_cmd_id = CMD_TIMER_START;

t_cmd_q *p_qcmd = NULL;	

size_t get_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
//	for( int = 0; i <nmemb; i++ ) {
		
//	}
 	printf("size : %d, %d \n", size, nmemb);  	
}


// read the config file ,get command url 
int get_url(char *url_str) {
	CURL *curl = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	
	if(NULL == curl) {
		printf("error easy init \n");
		curl_easy_cleanup(curl);
		return 0;
	}
	
	curl_easy_setopt(curl, CURLOPT_URL, url_str);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	return 0;
}



//read the command  thread func
void* server_cmd(void* pv) {
	char *cmd_url = "https://cn.bing.com";
	while(1) {
		int ret;

//		ret = get_url(cmd_url);
		sleep(10);
	}	
	
	return NULL;
}



void* timer_cmd(void *arg) {
	t_modcmd cmd;
	int ret = 0;

	memset(&cmd, 0, sizeof(t_modcmd));
	cmd.cmd_id = timer_cmd_id;
	timer_cmd_id ++;
	cmd.dev_id = 0x01;
	cmd.cmd = 0x03;
	cmd.param[0] = 0xFC;
	cmd.param[1] = 0x00;
	cmd.param[2] = 0x00;
	cmd.param[3] = 0x0B;


	while(1) {
		ret = en_queue(p_qcmd, &cmd);
		if(ret < 0 ) {
			printf("add cmd:%d to queue error !\n", cmd.cmd_id);
		}
	
		printf("add cmd:%d to queue  !\n", cmd.cmd_id);
		sleep(REPORT_TIMER);

	}
}


// report the data thread func
void* read_cmd(void *arg) {
	int ret;	
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));

	while(1) {
		ret = de_queue(p_qcmd, &cmd);
		printf(" report thread: \n");
		printf(" cmd id:%d \n", cmd.cmd_id);
		sleep(1);
	}	
}


int main(void) {
	printf("start running collecting data \n");
	pthread_t thread_add_cmd; // add cmd to queue
        pthread_t thread_timer_cmd; // add report cmd to queue 	
	pthread_t thread_read_cmd; // read cmd


	int ret1, ret2, ret3;

	// init queue
	p_qcmd = init_queue();
	if(NULL ==  p_qcmd) {
		return -1;
	}


//	ret1 = pthread_create(&thread_add_cmd, NULL, server_cmd, NULL);
	ret2 = pthread_create(&thread_timer_cmd, NULL, timer_cmd, NULL);
//	ret3 = pthread_create(&thread_read_cmd, NULL, read_cmd, NULL);

	while (1) {
		sleep(100);

	}	

	return 0;

}
