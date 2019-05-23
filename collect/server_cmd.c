#include<stdio.h>
#include<stdlib.h>
#include<curl/curl.h>

#include "cJSON.h"
#include "mod_cmd.h"
#include "queue.h"
#include "server_cmd.h"

int parse_json_cmd(char *p_data, t_modcmd *pcmd){
    
	char cmd_type[32];
	char dev_id[32];
	char cmd_id[32];
	char param[32];
    memset(cmd_type, 0, 32);
    memset(dev_id, 0, 32);
    memset(cmd_id, 0, 32);
    memset(param, 0, 32);

    if(NULL == p_data || NULL == pcmd) {
        return -1;
    }

    cJSON *root = NULL;
    cJSON *item = NULL;
	root = cJSON_Parse(p_data);
    if(NULL == root) {
        printf("server command parse error");
        return -1;
    }
	
    item = cJSON_GetObjectItem(root, "cmd_id");
    if( NULL == item) {
         printf("server command has no cmd_id\n");
         return -2;
    }
    strcpy(cmd_id,item->valuestring);

    item = cJSON_GetObjectItem(root, "device_id");
    if( NULL == item) {
         printf("server command has no device_id\n");
         return -3;
    }
    strcpy(dev_id ,item->valuestring);

    item = cJSON_GetObjectItem(root, "control_type_id");
    if( NULL == item) {
         printf("server command has no control type_id\n");
         return -4;
    }
    strcpy(cmd_type,item->valuestring);

    item = cJSON_GetObjectItem(root, "control_code");
    if( NULL == item) {
         printf("server command has no control code\n");
         return -5;
    }
    strcpy(param,item->valuestring);
	
	// set modbus cmd
	pcmd->cmd_id = atoi(cmd_id); 
	pcmd->dev_id = atoi(dev_id);
	pcmd->cmd= atoi(cmd_type);
   	pcmd->param =(int)atof(param)*10;
//	pcmd->isServ = 1;
	printf(" cmd from server:%d, dev:%d,cmd:%d param:%d\n",
			 pcmd->cmd_id,pcmd->dev_id,pcmd->cmd,pcmd->param);
	return 0;
}




size_t get_data(void *ptr, size_t size, size_t nmemeb,  void *stream) {
			
	if(stream == NULL) {
	
			return 0;
	}	

	memcpy(stream, ptr, size*nmemeb);
	
	return size*nmemeb;
}


int get_url(char *url_str, char *out) {
	if( NULL == out) {
		printf(" out put ptr null \n");
		return -1;
	}
	CURL *curl = NULL;

//	curl_global_init(CURL_GLOBAL_ALL);
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

	return 0;
}
