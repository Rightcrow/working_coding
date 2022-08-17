#ifndef __UART_THREAD__
#define __UART_THREAD__

void test(void);

#define UART_PATH	"/dev/ttyS0"

typedef struct UART_PROTOCOL 
{
	char head;
	char len;
	char control;
	char param;
	char data[3];
	char tail;
}__attribute__((aligned(8)))uart_protocol;

int Uart_Main(void);
void uart_write(char* data);
void SleepSet(void);




#endif
