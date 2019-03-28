#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<curl/curl.h>

#include "queue.h"
//#define REPORT_TIME (15*60) //every 15minutes  report
#define REPORT_TIME (2) //every 2s  report only for test
#define TIMER_CMD_START 0xff000000
#define TIMER_CMD_MAX 65535
t_cmd_q *q = NULL;
t_u32 timer_cmd_id = 0;





int parse_json(char data) 
{
	/*************
	    json的解析
	***************************/
	char data[] = "{\"name\":\"邱于涵\",\"age\":20}";
	//json是json对象指针,json_name是 name对象的指针,json_age是age对象的指针
	cJSON *json=0, *json_name=0, *json_age=0;
	//解析数据包
	json = cJSON_Parse(data);
	//如果解析失败
	if (!json)
	{
		printf("Error Before:", cJSON_GetErrorPtr());
	}
	else 
	{
		json_age = cJSON_GetObjectItem(json, "age");
		//如果类型是 数字
		if(json_age->type==cJSON_Number)
		{
			
			printf("年龄:%d\n", json_age->valueint);
		}
		json_name = cJSON_GetObjectItem(json, "name");
		//如果类型是 字符串
		if (json_name->type == cJSON_String)
		{
			printf("姓名：%s\n", json_name->valuestring);
		}
		//释放内存
		cJSON_Delete(json);
	}
 
}





size_t get_data(void *ptr, size_t size, size_t nmemeb,  void *stream) {
			
	if(stream == NULL) {
	
			return 0;
	}	

	printf("get data !\n");	
	memcpy(stream, ptr, size*nmemeb);
	
	return size*nmemeb;
}


int get_url(char *url_str, char *out) {
	if( NULL == out) {
		printf(" out put ptr null \n");
		return -1;
	}
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
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	pars_json(out);		
	return 0;
}



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
	int ret ;
	char buf[1024];
	t_modcmd cmd;
	memset(&cmd, 0, sizeof(t_modcmd));
	cmd.cmd_id = 0x003;	

		
	while(1) {
		memset(buf, 0, 1024);
    	ret = get_url(cmd_url, &buf);

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
		}
		sleep(1);
		
		print_queue(q);
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
