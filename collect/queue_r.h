#include"typehead.h"
#include<pthread.h>

#define RET_PARAM_BYTES 24 
#define MAX_RET_NUM 1024

typedef struct {
	t_u32 cmd_id;
	t_u32 dev_id;
	t_u8 cmd;
	t_u8 param[RET_PARAM_BYTES];
} t_mod_ret;

typedef struct {
	t_u32 head;
	t_u32 tail;
	t_mod_ret ret_list[RET_PARAM_BYTES];
	t_u32 len;
	pthread_mutex_t mutex;

} t_ret_q;



	


t_ret_q* init_rqueue(void);

int de_rqueue(t_ret_q *q, t_mod_ret *pcmd);
int reset_rqueue(t_ret_q *q);
int en_rqueue(t_ret_q *q, t_mod_ret *p_cmd);


