#define STATION_MAX_DEV_NUM 100
typedef struct dev_t {
	int device_id;
	int position_id;
} ;

typedef struct dev_l{
	struct dev_t dev[STATION_MAX_DEV_NUM];
	int num;
} DEV_LIST;


int get_dev_list(DEV_LIST *p_dev_list, int station_id);

