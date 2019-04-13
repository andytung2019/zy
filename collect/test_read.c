#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<modbus/modbus.h>

int read_register(modbus_t *mb, unsigned int addr, int len, unsigned short *ret){

	int rc;

	if(NULL == mb){
		printf(" modbus is null\n");
		return -1;
	}

    rc = modbus_connect(mb);
        printf("modbus_connect: %d \n",rc);

        if (rc == -1) {
                        printf("Connection failed\n");
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


int main(int argc, char** argv){

	//FILE *fout;
	int ret = 0;
	unsigned short a[12];
	char out[24];
    char line[2] ={"\n"};
	modbus_t *mb;


	memset((char*)a, 0, 24);
	memset(out, 0, 24);

    int start = 0;
	int num = 0;
	unsigned int addr = 0xFC00;

	if(argv[1][0] == 'A') {
		start = 10;
	} else if( argv[1][0] == 'B' ) {
		start = 11;
	} else if(argv[1][0] == 'C') {
		start = 12;
	} else {
		start = (int)argv[1][0]  - 48;
	}
	
    if(argv[2][0] == 'A') {
		num = 10;
	} else if( argv[2][0] == 'B' ) {
		num = 11;
	} else if(argv[2][0] == 'C') {
		num = 12;
	} else {
		num = (int)argv[2][0]  - 48;
	}
	start = start + 0xFC00;
	printf("start:%04x, num:%d\n", start, num);

    FILE *fout;
	fout = fopen("read1.txt", "a");
	if(fout == NULL ){
		printf(" error , can not open file");
		return -1;
	}


	//open serial port
    ret = open_modbus(1, &mb);
    printf("mb:%p", mb);

	while(1) {

		ret = read_register(mb,1 , 2, a );
		//ret = read_register(mb, start, num, a );
		printf("%04x %04x\n", a[0], a[1]);

		for( int i = 0; i < 12; i++ ) {
				int j =2*i;
				out[j] =(unsigned char) ((a[i]>>8)&0x00ff);
				out[j+1] = (unsigned char)(a[i]&0x00FF);
				fprintf(fout, "%02x", out[j]);
				fprintf(fout, "%02x", out[j+1]);
		}
		fprintf(fout, "%s", "\n");

  	//sleep 10s
	    sleep(5);
		
	}



	//close serial port
	//close write file
	modbus_close(mb);
    modbus_free(mb);

}
