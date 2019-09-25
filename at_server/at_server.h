#ifndef __AT_SERVER__H

#define __AT_SERVER__H

#include <stdbool.h>


#define PHONE_RING	("RING")
#define SGY_DEBUG

typedef void * (request_callback_t)(void *p_parameter);

enum CMD_STATE {
	CMD_READY = 1, //æÕ≤Ó∑¢ÀÕ\r\n¡À
	CMD_SEND_END,
};

enum AT_REQUEST {
	REQUEST_PHONE = 1,
	REQUEST_RING,
	CALLING_HANG_UP,
	NO_CALLING_HANG_UP,
	PHONE_CALL_CONNECT,
	REQUEST_MAX,
};

enum PHONE_STATE {
	IDLE_PHONE,
	RING,
	RING_READY,
	PHONE_CALLING,
	PHONE_OVER,
};

enum PROCESS_TYPE {
	NO_CMD = 1,
	CMD,
};
typedef struct st_Modem_reg_info {
    bool is_reg[REQUEST_MAX];       
	request_callback_t *p_sz_request_callback[REQUEST_MAX];
    void *p_szParameter[REQUEST_MAX];
}st_modem_reg_info_t;
int On_Request(int iRequest, void *(*start_routine) (void *), void *p_parameter);
int execute_callback(int iRequest, char *tmp_buf);
int at_send_command(char *p_cmd, int *p_iResult);
int at_server_module_init();



#define OK 		(1)
#define ERROR	(2)

#define SIZE 	(1024)

#endif
