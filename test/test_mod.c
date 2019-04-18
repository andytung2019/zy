#include <stdio.h>
#include<errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <modbus/modbus.h>


 int main(void)
    {
          modbus_t *mb;
          uint8_t bit_status[2] = {0};
          uint16_t bit_reg[2] = {0};


          mb = modbus_new_rtu("/dev/ttyS0",9600,'N',8,1);//open port
          modbus_set_slave(mb, 1); //set slave address
          modbus_set_debug(mb, true);

          modbus_connect(mb);

          struct timeval t;
          t.tv_sec=0;
          t.tv_usec=1000000;        //set modbus time out 1000ms
          t.tv_usec=5000000;        //set modbus time out 1000ms
          modbus_set_response_timeout(mb,&t);


	  unsigned addr = 0xfc02;
	  int value = 230;
	  int rc;
	  rc = modbus_write_register(mb, addr,value);
	  if(rc < 0 ){
		printf(" read register error\n");
		modbus_strerror(errno);
	  }


          modbus_close(mb);
          modbus_free(mb);

          return 0;
        }
