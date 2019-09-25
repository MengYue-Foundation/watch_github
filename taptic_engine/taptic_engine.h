#ifndef __TAPTIC_ENGINE__H_

#define __TAPTIC_ENGINE__H_
#include <stdbool.h>

#define PER_INC_TIME_NS     (10*1000*1000)

#define VIBRATE_PIN	(21)

#define TAPTIC_ENGINE_PIN	(26)
#define LENGTH 	(5)
typedef struct taptic_engine_info {
	int iMode;
	int iTimes;
	int iHave_data;
}taptic_engine_info_t;

/*�ߵ�ƽ150ms, �͵�ƽ100ms�� delay 1s�м���*/
#define MODE1	(1)
/*�ߵ�ƽ1000ms�� �͵�ƽ800�� delay 0ms*/
#define MODE2	(2)

/*һֱ�ߵ�ƽ*/
#define MODE3	(3)
extern int taptic_engine_module_init(void);
extern int start_taptic_engine(taptic_engine_info_t *p_taptic_engine_info);
extern int do_taptic_engine(int iMode, int iTimes);

#endif
