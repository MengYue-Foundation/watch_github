#ifndef __RASPBERRY_GPIO_ALPS__H

#define __RASPBERRY_GPIO_ALPS__H
#include <stdbool.h>


#define OFF (1)

#define ON (0)

#define CW (0)
#define CCW (1)
typedef void * (alps_func_callback_t)(void *p_parameter);

enum alps_sys_reg_func_index {
    CW_SYS_REG_INDEX = 0,
    CCW_SYS_REG_INDEX = 1,
    ROTATE_SYS_REG_MAX,
};

typedef struct alps_data_need_transfer {
    int iCurrent_state;//当前的状态是什么    
}alps_data_need_transfer_t;

enum alps_rotate_state {
    IDLE = 0,
    CW_JUDGE = 1,//这个表示CW预判断
    CW_KEEP =2,
    CCW_JUDGE = 3,
    CCW_KEEP = 4,
    ROTATE_CW_KEEP = 5,
    ROTATE_CCW_KEEP = 6,
};

typedef struct alps_need_reg_data {
    bool is_reg[ROTATE_SYS_REG_MAX];       /* 注册的函数是有效的还是无效的 */
    alps_func_callback_t * p_alps_reg_data[ROTATE_SYS_REG_MAX]; //注册的函数
    void *p_szParameter[ROTATE_SYS_REG_MAX];
}alps_need_reg_data_t;


extern int alps_rotate_module_init();
extern void alps_sys_register(int num_of_trigger, void *(*start_routine) (void *), void *p_parameter, int direction); 


#endif

