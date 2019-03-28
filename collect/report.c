#include "report.h"
#include "cJSON.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>


#define CHARS_LEN 30

char *bit[] = { "Bit0", "Bit1", "Bit2", "Bit3", "Bit4", 
                "Bit5", "Bit6", "Bit7", "Bit8", "Bit9",
   "Bit10", "Bit11", "Bit12", "Bit13", "Bit14", "Bit15" };

int report(unsigned char* ip, char* json)
{   

  return 0;
}


int json_item_power(unsigned char two_byte[], char *json_name, char *json_value) {
    cJSON* jsn_power = NULL;

    jsn_power = cJSON_CreateObject();
    if( NULL == jsn_power ) {
        return -1;
    }

    json_name = "power";
    
    if ((two_byte[0] == 0x00) && (two_byte[1] == 0xff)) {
        json_value="1";
        return 0;
    } 

     if ((two_byte[0] == 0xff) && (two_byte[1] == 0x00)) {
        json_value ="0";
        return 0;
    } 

    return -2;
}


int json_item_timer(unsigned char bytes[], char *json_name, int *json_value) {

     int ret = 0;
     int value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     
    
     strcpy(json_name, "timer");
     value =(int)bytes[0]*256 + (int)bytes[1]; 
     if (value > 24 || value < 0) {
         *json_value = 0;
     }
     *json_value = value;
     return 0;

}

int json_item_set_tem(unsigned char bytes[], char *json_name, float *json_value) {
    int ret = 0;
    float value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "set_tempe");
     value =((float)bytes[0]*256 + (float)bytes[1])/10;
     *json_value = value;
     return 0;
}

int json_item_set_hum(unsigned char bytes[], char *json_name, float *json_value) {
    int ret = 0;
    float value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "set_humid");
     value =((float)bytes[0]*256 + (float)bytes[1])/10;
     *json_value = value;
     return 0;
}


int json_item_env_tem(unsigned char bytes[], char *json_name, float *json_value) {
    int ret = 0;
    float value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "env_tempe");
     value =((int)bytes[0]*256 + (int)bytes[1])/10;
     *json_value = value;
     return 0;
}

int json_item_env_hum(unsigned char bytes[], char *json_name, float *json_value) {
    int ret = 0;
    float value = 0;

     if(NULL == json_name || NULL == json_value) {
         return -1;
     }     

     strcpy(json_name, "env_humid");
     value =((float)bytes[0]*256 + (float)bytes[1])/10;
     *json_value = value;
     return 0;
}

int json_obj_onload(unsigned char bytes[],  cJSON* json_onload) {
    int ret = 0;
    unsigned char a= 0;
    char value;

    
    if(NULL == json_onload) {
         return -1;
     }     
     // 解释负载,bit0-bit7有意义，其它均为0
     a = bytes[1];

     for(int i = 0; i < 8; i++) {

          if( a & 0x01) {
		value = '1';
          }  else {
                value = '0';
          }

          cJSON_AddItemToObject(json_onload,bit[i], cJSON_CreateString(value));
          a = a >> 1;
          
     }

     return 0;
}

/*故障状态 2字节 */
int json_obj_error(unsigned char bytes[],  cJSON* json_err) {
    int ret = 0;
    unsigned short a= 0;
    char value;
    
    if(NULL == json_err) {
         return -1;
     }     
     /* 解释error,bit0-bit15有意义*/
     a = bytes[1] + (unsigned short)bytes[0] << 8;

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

char* create_json( t_node *p_tnode ){   
    int ret = 0;
    char json_name[CHARS_LEN];
    char json_value[CHARS_LEN];

    cJSON* jsonroot = NULL;
    
    if(NULL == p_tnode) {
        return NULL;
    }

    jsonroot = cJSON_CreateObject();
    if( NULL == jsonroot ) {
        return NULL;
    }
   
    memset(json_name, 0, CHARS_LEN);
    memset(json_value, 0, CHARS_LEN);
    ret = json_item_power(&p_tnode->record[0], json_name, json_value);  

}




/*send json data by libcurl*/
void send_report(char *jsonObj){

  CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl == NULL) {
        return 128;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    /*curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");


       res = curl_easy_perform(curl);
*/
    curl_easy_setopt(curl, CURLOPT_URL, "http://http2bin.org/put");

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");

    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return res;

}
int main(void) {
	unsigned char t[2] = { 0x00, 0x44};
	char a[20];
	//float temp = 0.0;

    int ret = 0;
   
  
	
	 cJSON *json = cJSON_CreateObject();
     ret = json_obj_error(t, json);

    char* buf = cJSON_Print(json);
    printf("%s", buf);

}
