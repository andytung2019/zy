#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "get_device.h"

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


int get_dev_list(DEV_LIST *p_dev_list, int station_id){

 char *url="http://132.232.25.130:8080/station/getdev?station=%d";
 char url_addr[100];
 memset(url_addr, 0, 100);

if( NULL == p_dev_list ) {
    return -1;
}
 sprintf(url_addr,url, station_id);
 printf("get device from: %s", url_addr);
 //get dev list from url

 DEV_LIST dev_list;
 p_dev_list->dev[0].device_id=201;
 p_dev_list->dev[0].position_id = 3; 
 p_dev_list->dev[1].device_id = 202;
 p_dev_list->dev[1].position_id=4;
 p_dev_list->num = 2;
 return p_dev_list->num; 
}
