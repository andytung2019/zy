typedef struct dev_t {
	int device_id;
	int position_id;
} ;

typedef struct dev_l{
	struct dev_t dev[100];
	int num;
} DEV_LIST;
