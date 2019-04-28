#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef __cplusplus

extern "C"{

#endif

    int watch_ehome_fan_open(void);
    int watch_ehome_fan_close(void);
    int watch_ehome_fan_larger(void);
    int watch_ehome_fan_smaller(void);
    int watch_ehome_fan_set(char *pcLevel);



#ifdef __cplusplus

};

#endif
