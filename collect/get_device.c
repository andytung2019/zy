#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<curl/curl.h>

#include "get_device.h"

#include "cJSON.h"

int get_position(DEV_LIST *p_dev_list, int dev_id) {
    int i = 0;

    if(NULL == p_dev_list) {
        return -1;
    }
    if (p_dev_list->num <=0 ) {
        return -1;
    }
    for( i = 0; i < p_dev_list->num; i++) {
        if( p_dev_list->dev[i].device_id == dev_id) {
            return p_dev_list->dev[i].position_id;
        }
    }
    return -1;

}

int get_device_id(DEV_LIST *p_dev_list, int position_id) {

    if(NULL == p_dev_list) {
        return -1;
    }
    if (p_dev_list->num <=0 ) {
        return -1;
    }
    for( int i = 0; i < p_dev_list->num; i++) {
        if( p_dev_list->dev[i].position_id == position_id) {
            return p_dev_list->dev[i].device_id;
        }
    }
    return -1;

}

int parse_json_device(char *text, DEV_LIST *p_dev_list) {
	cJSON *json, *arrayItem, *item, *object;
	int i;
	int size = 0;

	if (!json)
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}
	else
	{
		arrayItem=cJSON_GetObjectItem(json,"devices");
		if(arrayItem!=NULL)
		{
			size=cJSON_GetArraySize(arrayItem);
			printf("cJSON_GetArraySize: size=%d\n",size);
 
			for(i=0;i<size;i++)
			{
				printf("i=%d\n",i);
				object=cJSON_GetArrayItem(arrayItem,i);
 
				item=cJSON_GetObjectItem(object,"device_id");
				if(item!=NULL)
				{
					printf("cJSON_GetObjectItem: type=%d, string is %s\n",item->type,item->string);
					memcpy(p_dev_list->dev[i].device_id,item->valuestring,strlen(item->valuestring));
				}
 
				item=cJSON_GetObjectItem(object,"position_id");
				if(item!=NULL)
				{
					printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
					memcpy(p_dev_list->dev[i].position_id,item->valuestring,strlen(item->valuestring));
				}
 
			}
		}

		p_dev_list->num = size;
	
	}

	return size; 

}



int get_dev_list(DEV_LIST *p_dev_list, int station_id){

       
char *url="http://132.232.25.130:8080/station/getdev?station=%d";
int ret, ret1;
char url_addr[100];
 char buf[1024];
 memset(url_addr, 0, 100);
 memset(buf, 0, 1024);

if( NULL == p_dev_list ) {
    return -1;
}
 sprintf(url_addr,url, station_id);
 printf("get device from: %s", url_addr);
 //get dev list from url

 ret = get_dev_url(url, (char*)&buf[0]);
 ret1 = parse_json_device((char*)&buf[0], p_dev_list);
 if( ret1 <= 0 ) {
 
 	 return 0;
 }

 return p_dev_list->num; 
}



size_t get_list_data(void *ptr, size_t size, size_t nmemeb,  void *stream) {
			
	if(stream == NULL) {
	
			return 0;
	}	

	memcpy(stream, ptr, size*nmemeb);
	
	return size*nmemeb;
}


int get_dev_url(char *url_str, char *out) {
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
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_list_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return 0;
}
