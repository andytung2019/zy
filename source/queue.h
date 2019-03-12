

#define LEN_RECORD 48
#define LEN_QUEUE 100


typdef struct {
	int amount_record;
 	unsigned char record[LEN_RECORD];
	int error;
} t_queue;



