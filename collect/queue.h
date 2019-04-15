#include"typehead.h"
#include<pthread.h>
#include<semaphore.h>

#define MAX_PARAM_BYTE 4
#define MAX_CMD_NUM 128

//下发给modbus 设备的指令结构体
typedef struct {
	t_u32 cmd_id;
	t_u32 dev_id;
	t_u8 cmd;
	t_i32 param; //操作参数，如设定温度命令中的温度值
} t_modcmd;

//环形队列结构体，要求多线程操作安全
typedef struct {
	t_u32 head;
	t_u32 tail;
	t_modcmd cmd_list[MAX_CMD_NUM]; //环形队列最大存放命令数 128
	t_u32 len;
	pthread_mutex_t mutex;  //互斥操作锁
	//sem_t empty;
	//sem_t full;

} t_cmd_q;


t_cmd_q* init_queue(void) ;  //创建环形队列
int en_queue(t_cmd_q *q, t_modcmd *p_cmd); //进入队列操作
int de_queue(t_cmd_q *q, t_modcmd *pcmd); //出队列操作
int reset_queue(t_cmd_q *q); //重置环形队列,指针从0开始，清空所有内容
int queue_is_empty(t_cmd_q *q); //判断队列是不是空
int queue_is_full(t_cmd_q *q) ; //判断队列是不是满
int len_queue(t_cmd_q *q) ; //队列长度
int free_queue(t_cmd_q *q); //释放队列

	


