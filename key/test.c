#include <key_ink_screen.h>
#include <stdio.h>

void * test0(void *p_parameter)
{
    printf("第一次按�? para:%s\n", (char *)p_parameter);
}

void * test1(void *p_parameter)
{
    printf("短按, para:%s\n", (char *)p_parameter);
}

void * test2(void *p_parameter)
{
    printf("长按, para:%s\n", (char *)p_parameter);
}

void * test3(void *p_parameter)
{
    printf("抬起, para:%s\n", (char *)p_parameter);
}



int main () {
    key_module_init();
    char *str = "123";
    key_sys_register(KEY0_FIRST_PRESS_DOWN, test0, (void *)str);
    key_sys_register(KEY0_SHORT_PRESS, test1, (void *)str);
    key_sys_register(KEY0_LONG_PRESS, test2, (void *)str);    
    key_sys_register(KEY0_UP, test3, (void *)str);
    while (1);
    return 0;
}
