

#define LEN_RECORD 48
#define BLOCK_RECORD 100
#define NUM_BLOCK 16
#define  LEN_ERROR 6

typdef struct {
	int amount;
 	unsigned char record[LEN_RECORD];
	unsigned char error[LEN_ERROR];
} t_node;


typedef struct {
	t_node node[BLOCK_RECORD]; //BLOCK_RECOD*node is  a block;
} t_block;

typedef struct{
 unsigned char head;
 unsigned char tail;
 t_block block[NUM_BLOCK]; // our queue has NUM_BLOCK  blocks; 
 int len;
} t_queue;


