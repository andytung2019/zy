#include"typehead.h"
#include<pthread.h>
#include<semaphore.h>

#define MAX_PARAM_BYTE 4
#define MAX_CMD_NUM 128

typedef struct {
	t_u32 cmd_id;
	t_u32 dev_id;
	t_u8 cmd;
	t_u8 param[MAX_PARAM_BYTE];
} t_modcmd;

typedef struct {
	t_u32 head;
	t_u32 tail;
	t_modcmd cmd_list[MAX_CMD_NUM];
	t_u32 len;
	pthread_mutex_t mutex;
	sem_t empty;
	sem_t full;

} t_cmd_q;



	


