
#define ALL_REG_NUM  12

int open_modbus(int id, modbus_t **ret_mb ); 

int read_register(modbus_t *mb, unsigned int addr, int len, unsigned short *ret);

int read_multi_regs( modbus_t *mb, unsigned char addr, int num, unsigned char arr_ret[] );

int read_regs( modbus_t *mb, unsigned char addr, int num, unsigned char arr_ret[] );

int close_modbus(int id);
