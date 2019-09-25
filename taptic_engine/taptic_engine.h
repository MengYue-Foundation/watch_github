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

/*高电平150ms, 低电平100ms， delay 1s中继续*/
#define MODE1	(1)
/*高电平1000ms， 低电平800， delay 0ms*/
#define MODE2	(2)

/*一直高电平*/
#define MODE3	(3)
extern int taptic_engine_module_init(void);
extern int start_taptic_engine(taptic_engine_info_t *p_taptic_engine_info);
extern int do_taptic_engine(int iMode, int iTimes);

#endif
