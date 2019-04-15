#include<errno.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>

#include "mod_cmd.h"
#include "queue.h"
#include "queue_r.h"

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
		modbus_strerror(errno);
	}
	return rc;
}




int write_register(modbus_t *mb, unsigned int addr, unsigned int value ){

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
	
	rc = modbus_write_register(mb, addr,value);
	if(rc < 0 ){
		printf(" write register error\n");
		modbus_strerror(errno);
	}
	return rc;
}




int open_modbus(int id, modbus_t **ret_mb ) {
	modbus_t *mb;
	int rc;
	
	struct timeval response_timeout;
	uint32_t tv_sec = 0;
	uint32_t tv_usec = 0;
	response_timeout.tv_sec = 3;
	response_timeout.tv_usec = 0;
	
	mb= modbus_new_rtu("/dev/ttyS0", 9600, 'N', 8, 1);
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
		
		unsigned int addr = START_REG_ADDR;
		
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

int run_query_cmd(t_modcmd *pcmd, t_mod_ret *pret){

	int ret = 0;
	unsigned short a[12];
	modbus_t *mb;


	memset((unsigned char*)a, 0, 24);


	//open serial port
    	ret = open_modbus(1, &mb);
	if(ret < 0 ) {
		printf(" open mod bus error :%d\n", ret);	
		return -1;
    }
	
	int num = 1;
	unsigned int addr = START_REG_ADDR + pcmd->cmd -1;
	if(pcmd->cmd == 13) {
		num = 12;
		addr = START_REG_ADDR;
	}

	if(pcmd->cmd == 14) {
		addr = START_REG_ADDR;
		num = 1;
	
	}	
	ret = read_register(mb, addr, num, a );
	if(ret < 0 ) {

		return -1;
	} else {

		pret->cmd_id = pcmd->cmd_id;
		pret->dev_id = pcmd->dev_id;
		pret->cmd = pcmd->cmd;
		memset(pret->param, 0, RET_PARAM_BYTES);
		for( int i = 0; i < 12; i++ ) {
				int j =2*i;
				pret->param[j] =(unsigned char) ((a[i]>>8)&0x00ff);
				pret->param[j+1] = (unsigned char)(a[i]&0xFF );
				printf("%02x ", pret->param[j]);
				printf("%02x ", pret->param[j+1]);
		}
  	}
	printf("\n");
	//close serial port
	//close write file
	modbus_close(mb);
    modbus_free(mb);

	return 0;
}



int run_set_cmd(t_modcmd *pcmd, t_mod_ret *pret){

	//FILE *fout;
	int ret = 0;
	unsigned short a[12];
	modbus_t *mb;


	memset((char*)a, 0, 24);

	//open serial port
        ret = open_modbus(1, &mb);
	if(ret < 0 ) {
		printf(" open mod bus error :%d\n", ret);	
		return -1;
    }
	
	int num = 1;
	unsigned int addr = START_REG_ADDR + pcmd->cmd -1;
	int value = pcmd->param;

	ret = write_register(mb, addr, value);
	if(ret < 0 ) {
		printf(" set mb command error\n");
	}

	//close serial port
	//close write file
	modbus_close(mb);
        modbus_free(mb);

	return 0;
}


int run_cmd(t_modcmd *pcmd, t_mod_ret *pret) {
	int ret;

	if (NULL == pcmd) {

		return -1;
	}
	
	if ( pcmd->cmd == 0x00) {
		return -2;
	}
	if( pcmd->cmd < 0x05) {
		ret = run_set_cmd(pcmd,pret );	

	} else {
		ret = run_query_cmd(pcmd,pret);
		return ret;
	}

	return 0;

}
