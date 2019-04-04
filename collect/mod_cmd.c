#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include "mod_cmd.h"

int read_register(modbus_t *mb, unsigned int addr, int len, unsigned short *ret){

	int rc;

	if(NULL == mb){
		printf(" modbus is null\n");
		return -1;
	}

    rc = modbus_connect(mb);
    printf("modbus_connect: %d \n",rc);

    if (rc == -1) {
            printf("Connection modbus host failed\n");
            return -1;
     }

	rc = modbus_read_registers(mb, addr, len, ret);
	if(rc < 0 ){
		printf(" read register error\n");
	}
	return rc;
}

int open_modbus(int id, modbus_t **ret_mb ) {
	modbus_t *mb;
	int rc;
	
	struct timeval response_timeout;
	uint32_t tv_sec = 0;
	uint32_t tv_usec = 0;
	response_timeout.tv_sec = 5;
	response_timeout.tv_usec = 0;
	
	mb= modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
	if(NULL == mb) {
		printf("open dev ttyUSB0 failed\n");
		return -1;
	}
	else {
		printf("get mb :%p", mb);
	}
		
	modbus_set_debug(mb, TRUE);

    rc = modbus_set_slave(mb, 1);
	if(rc != 0) {
		printf(" modbus set slave error\n");
		return -2;
	}

	modbus_get_response_timeout(mb, &response_timeout); 
    printf("Default response timeout:%ld sec %ld usec \n", response_timeout.tv_sec, response_timeout.tv_usec );

    response_timeout.tv_sec = 5;
    response_timeout.tv_usec = 0;

    modbus_set_response_timeout(mb, &response_timeout); 
    modbus_get_response_timeout(mb, &response_timeout); 
    printf("Set response timeout:%ld sec %ld usec \n", response_timeout.tv_sec, response_timeout.tv_usec );


	*ret_mb = mb;
	return 0;
      
}



int close_modbus(modbus_t *mb) {

   if(NULL == mb) {
		return -1;
	}

	modbus_close(mb);
    modbus_free(mb);
	
	return 0;
}


int read_all_regs( modbus_t *mb, unsigned char  arr_ret[]) {

		int ret;

		if (NULL == mb || NULL == arr_ret) {
			return  -1;
		}
		
		unsigned int addr = 0xFC00;
		
		memset(arr_ret, 0, ALL_REG_NUM*2);
		
		ret = read_register(mb, addr , ALL_REG_NUM, arr_ret);

		for( int i = 0; i <  2*ALL_REG_NUM; i++ )  		{
				printf("%02x ", arr_ret[i]);
		}
		printf("\n");

		return ALL_REG_NUM*2;

}


int read_multi_regs( modbus_t *mb, unsigned char addr, int num, unsigned char arr_ret[] ){

		int ret;

		if (NULL == mb || NULL == arr_ret) {
			return  -1;
		}
		
		memset(arr_ret, 0, 2*num);
		
		ret = read_register(mb, addr, num, arr_ret);

		for( int i = 0; i <  2*num; i++ )  		{
				printf("%02x ", arr_ret[i]);
		}
		printf("\n");
		
		return num*2;


}



int run_set_cmd(t_modcmd *pcmd){

	//FILE *fout;
	int ret = 0;
	unsigned short a[12];
	char out[24];
    char line[2] ={"\n"};
	modbus_t *mb;


	memset((char*)a, 0, 24);
	memset(out, 0, 24);

    int start = 0;
	int num = 2;



	//open serial port
    ret = open_modbus(1, &mb);
	if(ret < 0 ) {
		printf(" open mod bus error :%d\n", ret);	
		return -1;
    }

	//close serial port
	//close write file
	modbus_close(mb);
    modbus_free(mb);

	return 0;
}



int run_query_cmd(t_modcmd *pcmd){

	//FILE *fout;
	int ret = 0;
	unsigned short a[12];
	char out[24];
    char line[2] ={"\n"};
	modbus_t *mb;


	memset((char*)a, 0, 24);
	memset(out, 0, 24);

	int num = 0;
	unsigned int addr = pcmd->param[0] ;
      addr = addr << 16;
      addr  += pcmd->param[1];

	//open serial port
    ret = open_modbus(1, &mb);
	if(ret < 0 ) {
		printf(" open mod bus error :%d\n", ret);	
		return -1;
    }
	
	start = addr;
	num = 2;
	if(pcmd->cmd == 8) {
		num = 24;
		start = 0xfc000000;
	}

	if(pcmd->cmd == 9) {
	}	
	ret = read_register(mb, start, num, a );

	for( int i = 0; i < 12; i++ ) {
				int j =2*i;
				out[j] =(unsigned char) ((a[i]>>8)&0x00ff);
				out[j+1] = (unsigned char)(a[i]&0x00FF);
				printf(fout, "%02x", out[j]);
				printf(fout, "%02x", out[j+1]);
		}
  

	//close serial port
	//close write file
	modbus_close(mb);
    modbus_free(mb);

	return 0;
}


int run_cmd(t_modcmd *pcmd) {
	if (NULL == pcmd) {

		return -1;
	}

	if( pcmd->cmd < 0x05) {
		run_set_cmd(pcmd);	

	} else {
		run_query_cmd(pcmd);
	}


}
