#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "report.h"
#include "cJSON.h"
#include "queue.h"
#include "queue_r.h"

#define CHARS_LEN 30
#define POSTURL "http://120.77.168.74/phpweb/htprt.php"

char *item[] = { "power","timer", "set_temp","set_humid",
		"env_tempe", "env_humid", "onload", "error", 
		"tempe_1",  "tempe_2", "tempe_3", "tempe_4"
};
char *bit[] = { "Bit0", "Bit1", "Bit2", "Bit3", "Bit4", 
                "Bit5", "Bit6", "Bit7", "Bit8", "Bit9",
   "Bit10", "Bit11", "Bit12", "Bit13", "Bit14", "Bit15" };

char *bools[] = {"0", "1"};


int json_item_power(unsigned char *ptr, char* json_name,  char *json_value) {
    if(NULL == ptr || NULL == json_value) {
    	return -1;
    }
    
    strcpy(json_name, "power");
    
    if ((*ptr == 0x00) && (*(ptr+1) == 0xff)) {
        strcpy(json_value, "1");
        return 0;
    } 

     if ((*ptr == 0xff) && (*(ptr+1) == 0x00)) {
        strcpy(json_value, "0");
        return 0;
    } 


    return -2;
}

int json_item_timer(unsigned char *ptr, char *json_name, char *json_value) {

     int ret = 0;
     int value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     
    
     strcpy(json_name, "timer");
     value = *(ptr+1); 
     if (value > 24 || value < 0) {
         value = 0;
     }
     sprintf(json_value, "%d", value);
     return 0;

}

int json_item_set_temp(unsigned char *ptr, char *json_name,char *json_value) {
    int ret = 0;
    float value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "set_tempe");
     value =(float)(256*(*ptr) + *(ptr+1));
     value = value/10;
     sprintf(json_value, "%3.1f", value);
     return 0;
}

int json_item_set_humid(unsigned char *ptr, char *json_name, char *json_value) {
    int ret = 0;
    float value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "set_humid");
     value =(float)(256*(*ptr) + *(ptr+1));
     value = value/10;
     sprintf(json_value, "%3.1f", value);

     return 0;
}


int json_item_env_tempe(unsigned char* ptr, char *json_name, char *json_value) {
    int ret = 0;
    int value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "env_tempe");
     value = (int)(*(ptr+1));
    // value =  (int)(((int)(*ptr))<<8 +  *(ptr + 1)) ;
     sprintf(json_value, "%d", value);

     return 0;
}

int json_item_env_humid(unsigned char *ptr, char *json_name, char *json_value) {
    int ret = 0;
    float value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     


     strcpy(json_name, "env_humid");
     value =(float)(256*(*ptr) + *(ptr+1));
     value = value/10;
     sprintf(json_value, "%3.1f", value);

     return 0;

  }

/*故障状态 2字节 */
int json_obj_error(unsigned char *ptr,  cJSON* json_err) {
    int ret = 0;
    unsigned short a= 0;
    char value;
    
    if(NULL == json_err) {
         return -1;
     }     
     /* 解释error,bit0-bit15有意义*/
     a = *(ptr + 1) + (unsigned short)(*ptr) << 8;

     for(int i = 0; i < 16; i++) {

          if( a & 0x01) {
		value = '1';
          }  else {
		value = '0';
          }

          cJSON_AddItemToObject(json_err,bit[i], cJSON_CreateString(value));
          a = a >> 1;
          
     }

     return 0;
}

int json_item__tempe_A2D(unsigned char* ptr, char *json_name, char *json_value) {
    int ret = 0;
    int value = 0;
    
     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "env_tempe");
     value = (int)(*(ptr + 1));
     sprintf(json_value, "%d", value);

     return 0;
}

cJSON* create_json_all( t_mod_ret *pret ){  
#define  CHARS_LEN 24

	char name[CHARS_LEN];
	char out[CHARS_LEN];
	time_t t;
	int time_d;

	if ( NULL  == pret) {
		return NULL;
	}
	
	cJSON *root= cJSON_CreateObject();
	cJSON *json_onload;// = cJSON_CreateObject();
	cJSON *json_error; //= cJSON_CreateObject();

	//device_id:

	memset(out, 0, CHARS_LEN);
	sprintf(out, "%u", pret->dev_id);
	cJSON_AddStringToObject(root,"device_id", out);
     //time
	time_d = time(&t);
	memset(out, 0, CHARS_LEN);
	sprintf(out, "%u", time_d);
	cJSON_AddStringToObject(root,"timestamp", out);

	//0-1 power
	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_power(&(pret->param[0]),name, out);
	cJSON_AddStringToObject(root,name, out);

	//2-3 timer
	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_timer(&(pret->param[2]),name, out);
	cJSON_AddStringToObject(root,name, out);

	//4-5 set_temperature	
	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_set_temp(&(pret->param[4]),name, out);
	cJSON_AddStringToObject(root,name, out);
	
	//6-7 set_humidity
	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_set_humid(&(pret->param[6]),name, out);
	cJSON_AddStringToObject(root,name, out);



	//8-9 env_temp
	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_env_tempe(&(pret->param[8]),name, out);
	cJSON_AddStringToObject(root,name, out);

	//10-11 env_humid
	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_env_humid(&(pret->param[10]),name, out);
	cJSON_AddStringToObject(root,name, out);
	
	//12-13 onload
    	cJSON_AddItemToObject(root,"onload",json_onload=cJSON_CreateObject() );
        int load = pret->param[13];
        printf("%d , %s,  %s \n", load, bools[0], bools[1]);	
	for(int i = 0; i < 8; i++) {
          cJSON_AddStringToObject(json_onload,bit[i], bools[load&0x01]);
          load = load >> 1;
       }


	//14-15 error
   	cJSON_AddItemToObject(root,"error",json_error=cJSON_CreateObject() );
	//printf(" %02x , %02x \n", pret->param[15], pret->param[14]);
     	unsigned  short  err = (unsigned short)(pret->param[14])<<8 + (unsigned short)(pret->param[15]);
	for(int i = 0; i < 16; i++) {
          cJSON_AddStringToObject(json_error,bit[i], bools[err&0x0001]);
          err =err >> 1;
       }


	//16-23
	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_env_tempe(&(pret->param[16]),name, out);
	strcpy(name, "tempe_1");
	cJSON_AddStringToObject(root,name, out);

	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_env_tempe(&(pret->param[18]),name, out);
	strcpy(name, "tempe_2");
	cJSON_AddStringToObject(root,name, out);

	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_env_tempe(&(pret->param[20]),name, out);
	strcpy(name, "tempe_3");
	cJSON_AddStringToObject(root,name, out);

	memset(out, 0, CHARS_LEN);
	memset(name, 0, CHARS_LEN);
	json_item_env_tempe(&(pret->param[22]),name, out);
	strcpy(name, "tempe_4");
	cJSON_AddStringToObject(root,name, out);

//	printf("%s\n", cJSON_Print(root));
	return root;
}

#if 0
cJSON* create_json_all( t_mod_ret *pret ){  
    
    int len = 0;
    printf(" create json,  param :\n");
    {
	    for (int i = 0; i < 24; i++) {
	    	printf(" %02x ", pret->param[i]);
	    }
    }
    len  = pret->param[2];
    if( len != 24) {
    	return -1;
    }
    
    int ret = 0;
    char json_name[CHARS_LEN];
    char json_value[CHARS_LEN];

    cJSON* jsonroot = NULL;
    
    if(NULL == pret) {
        return NULL;
    }
    jsonroot = cJSON_CreateObject();
    if( NULL == jsonroot ) {
        return NULL;
    }

       
    memset(json_name, 0, CHARS_LEN);
    memset(json_value, 0, CHARS_LEN);
    ret = json_item_power(&pret->param[3], json_name, json_value); 
    cJSON_AddStringToObject(jsonroot, json_name, json_value);
   
 
    return jsonroot;
}

#endif 

int send_report(t_mod_ret *pret){
	int ret;
	if(pret == NULL) {
		return -1;
	}

	cJSON *json =  create_json_all(pret);
	if (json != NULL) {
     		char* buf = cJSON_Print(json);
     
      		printf("%s\n", buf);
			ret = send_json(buf);
			printf("send json ret:d\n", ret);
		    cJSON_free(json);	
	}
	return 0;


}

	
/*send json data by libcurl*/
void send_json(char *jsonObj){

  CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl == NULL) {
        return -10;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl_easy_setopt(curl, CURLOPT_URL, POSTURL);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");


    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return res;
} 


/*
int main(void) {
	
     t_mod_ret ret;
     printf(" start test\n");
     memset(&ret, 0, sizeof(t_mod_ret));
     ret.cmd_id = 0x01;
     ret.dev_id = 0x01;
     ret.cmd = 13;
     ret.param[0] = 0xFF;
     ret.param[1] = 0x00;
     
    cJSON* root = create_json(&ret);

    if (root != NULL) {
     	char* buf = cJSON_Print(root);
     printf("%s", buf);

    }

} */
