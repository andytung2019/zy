#include<stdio.h>
#include<stdlib.h>
#include<curl/curl.h>

#include "cJSON.h"
#include "mod_cmd.h"
#include "queue.h"
#include "server_cmd.h"

int parse_json_cmd(char *p_data, t_modcmd *pcmd){
    int param ;

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
    pcmd->cmd_id = item->valueint;

    item = cJSON_GetObjectItem(root, "device_id");
    if( NULL == item) {
         printf("server command has no device_id\n");
         return -2;
    }
    pcmd->dev_id = item->valueint;

    item = cJSON_GetObjectItem(root, "contorl_type_id");
    if( NULL == item) {
         printf("server command has no control type_id\n");
         return -2;
    }
    pcmd->cmd = item->valueint;

    item = cJSON_GetObjectItem(root, "contorl_code");
    if( NULL == item) {
         printf("server command has no control code\n");
         return -2;
    }
    param = item->valueint;
	pcmd->param[0] = (param>>8) &0xff;
    pcmd->param[1] = param &0xff;	
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

	return 0;
}
