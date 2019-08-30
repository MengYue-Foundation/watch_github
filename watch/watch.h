#ifndef __WATCH_H__
#define __WATCH_H__

#include "../key/key_ink_screen.h"


#include "../time/watch_time.h"
#include "../screen/screen.h"
#include <taptic_engine.h>

#include <ink_screen_display.h>
#include "../time/watch_time.h"
#include "../key/raspberry_gpio_alps.h"

//include "../app/lark/watch_lark.h"
#include "../app/lark/watch_lark_client.h"
#include "../app/lark/watch_lark.h"
#include "../app/remind/watch_remind_client.h"
#include "../app/remind/watch_remind.h"
#include "../app/phone/watch_phone.h"
#include "../app/short_message/watch_short_message.h"
#include "../app/aiworld/watch_aiworld.h"
#include "../app/weather/watch_weather.h"


typedef struct watch{
	int iId;
	char cszIp[32];
	watch_mesg_t stWatchMesg;
}watch_t;

int watch_init(watch_t *pstWatch);
int watch_exec(watch_t *pstWatch);
int key3_second(watch_t *pstWatch);

#endif
