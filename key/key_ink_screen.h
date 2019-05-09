#ifndef __KEY_INK_SCREEN__H

#define __KEY_INK_SCREEN__H

#include <stdbool.h>

extern int key_module_init();
#define TTP229_I2C_BUS_NAME     "/dev/i2c-1"
#define TTP229_ADDR     0x57

#define KEY_NUM     4
#define UP          0x1
#define DOWN        0x2

#define YES         0x3
#define NO          0x6



#define PER_INC_TIME_NS     (10*1000*1000)



//å•ä½æ˜¯1ms
#define PER_INC_TIME_MS     (10)
#define LONG_PRESS_TIME     (1000 / PER_INC_TIME_MS)

#define ACTION_NUM_PER_KEY  (4)
typedef void * (key_func_callback_t)(void *p_parameter);




//#define 
enum Key_index {
    KEY0 = 0,
    KEY1,
    KEY2,
    KEY3,
    NO_KEY_PRESS,
};

enum Key_action_offset {
    FIRST_PRESS_DOWN = 0,
    SHORT_PRESS,
    LONG_PRESS,
    PRESS_UP,
};


enum key_sys_reg_func_index {
    KEY0_FIRST_PRESS_DOWN,
    KEY0_SHORT_PRESS,
    KEY0_LONG_PRESS,
    KEY0_UP,

    KEY1_FIRST_PRESS_DOWN,
    KEY1_SHORT_PRESS,
    KEY1_LONG_PRESS,
    KEY1_UP,
    
    KEY2_FIRST_PRESS_DOWN,
    KEY2_SHORT_PRESS,
    KEY2_LONG_PRESS,
    KEY2_UP,
    
    KEY3_FIRST_PRESS_DOWN,
    KEY3_SHORT_PRESS,
    KEY3_LONG_PRESS,
    KEY3_UP,
    KEY_SYS_REG_FUNC_MAX,
};
typedef struct per_key_data_record {
    int iPress_Status;//æŒ‰ä¸‹è¿˜æ˜¯æ¾å¼€
}per_key_data_record_t;

/*è¦æ³¨å†Œçš„dataæ˜¯åŒ…æ‹¬ è¦æ³¨å†Œçš„funcçš„,å¯èƒ½è¿˜ä¼šæœ‰åˆ«çš„ä¸œè¥¿éœ€è¦æ³¨å†Œ*/
typedef struct key_need_reg_data {
    bool is_reg[KEY_SYS_REG_FUNC_MAX];       
    key_func_callback_t * p_key_reg_data[KEY_SYS_REG_FUNC_MAX]; //æ³¨å†Œçš„å‡½æ•°
    void *p_szParameter[KEY_SYS_REG_FUNC_MAX];
}key_need_reg_data_t;

typedef struct key_data_need_transfer {
	int iNeed_process;//ÓÃÀ´Çø·ÖÊÇ²»ÊÇ³¬Ê±»¹ÊÇÕæµÄÓĞÊı¾İÒª´¦Àí
    int iCurrent_key_index;//µ±Ç°ÕıÔÚ´¦ÀíµÄ°´¼ü±àºÅ¡£
    per_key_data_record_t sz_per_key_data[KEY_NUM];
    
}key_data_need_transfer_t;

typedef struct key_read_i2c_data {
    int iPress_Status;//æŒ‰ä¸‹è¿˜æ˜¯æ¾å¼€
    int iKey_index;//å“ªä¸€ä¸ªæŒ‰é”®æŒ‰ä¸‹
    int iStatus_change; //æŒ‰é”®çš„çŠ¶æ€æ˜¯å¦å‘ç”Ÿå˜åŒ–
}key_read_i2c_data_t;


void key_sys_register(int iAction_id, void *(*start_routine) (void *), void *p_parameter); 
void key_sys_deregister(int iAction_id);
int find_action_id(int iCurrent_key_index, int iActionOffset);
int check_key_call_back(int iCurrent_key_index, int iActionOffset);


extern key_need_reg_data_t key_need_reg_data_cb;

#endif
