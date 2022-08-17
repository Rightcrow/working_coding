#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include "uart_thread.h"
#include "global_value.h"
#include <termios.h>

void uart_write(char* data)
{
	struct termios opt;
	int fd = open(UART_PATH, O_RDWR | O_NOCTTY | O_NDELAY);

	tcgetattr(fd, &opt);
	opt.c_oflag |= ~ICANON;
	opt.c_iflag &= ~ICRNL;
	opt.c_iflag &= ~IXON;
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &opt);

    write(fd, data, strlen(data));
	close(fd);

	return;
}


void protocol_init(uart_protocol* pro)
{
	pro->head = 0x86;
	pro->len = 0x08;
	pro->control = 0x0b;
	pro->tail= 0x16;

	return;
}

//protocol reframe
/*
void DataSet(int year, char month, char data, char hour, char minute, char second)
{
	uart_protocol pro;
	memset(&pro, 0, sizeof(pro));
	protocol_init(&pro);
	pro.param = 0x03;
	
	char year_h = (char)(year / 100);
	char year_l = (char)(year % 100);
	
	{
		pro.data[0] = year_h;
		pro.data[1] = year_l; 
		pro.data[2] = month;
		pro.data[3] = data; 
		pro.data[4] = hour;
		pro.data[5] = minute; 
		pro.data[6] = second;
	}

	uart_write((char*)&pro);
	return;
}
*/

//protocol reframe
/*HIGH 0X01 MIDDLE 0X02 LOW 0X03*/
void SensitivitySet(char data)
{
	uart_protocol pro;
	memset(&pro, 0, sizeof(pro));
	protocol_init(&pro);
	pro.param = 0x01;

	pro.data[0] = data;
	
	uart_write((char*)&pro);
	
	return;
}

//protocol reframe
/*
void TimeWakeupSet(char day, char hour, char minute)
{
	uart_protocol pro;
	memset(&pro, 0, sizeof(pro));
	protocol_init(&pro);
	pro.param = 0x02;
	
	{
		strcpy(&pro.data[0], &day);
		strcpy(&pro.data[1], &hour);
		pro.data[6] = minute; 
	}
	
	uart_write((char*)&pro);

	return;
}
*/

//protocol reframe
void SleepSet(void)
{
	uart_protocol pro;
	memset(&pro, 0, sizeof(pro));
	protocol_init(&pro);
	pro.param = 0x04;
	memset(pro.data, 0xff, sizeof(pro.data));
	uart_write((char*)&pro);

	return;
}

/*
//change time-now function
void Check_Timer(char* data)
{
	int year = data[0]*100 + data[1];
	int month = data[2];
	int day = data[3];
	int hour = data[4];
	int minute = data[5];
	int second = data[6];

	//RTC
	uart_write((char*)&year);
	uart_write((char*)&month);
	uart_write((char*)&day);
	uart_write((char*)&hour);
	uart_write((char*)&minute);
	uart_write((char*)&second);
}
*/

//protocol reslove
int Protocol_Analysis(char* data)
{
	
	if (data[0]==0x86 && data[1]==0x0C && data[2]==0x0f)
	{
		switch (data[3])
		{
			case 0x01:
				return 1;
			case 0x02:
				return 0;
			case 0x03:
				//Check_Timer(data+4);
				return 0;
		}
	}
	
	/*
	if (data[0]=='a' && data[1]=='b' && data[2]=='c' && data[strlen(data)-2]=='e')
	{
		switch (data[3])
		{
			case 'd':
				return 1;
			case 0x02:
				return 0;
			case 0x03:
				Check_Timer(data+4);
				return 0;
		}
	}
	*/
	

	return 0;
}


int Uart_Main()
{
	struct termios opt;
	int fd = open(UART_PATH, O_RDWR | O_NOCTTY | O_NDELAY);

	tcgetattr(fd, &opt);
	opt.c_oflag |= ~ICANON;
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &opt);
	
	fd_set rset;
    int signal_back = 0;

	while(1)
	{
        char buff[100];
		memset(buff, 0, sizeof(buff));

		FD_ZERO(&rset);
		FD_SET(fd, &rset);
		
		int rv = select(fd+1, &rset,NULL,NULL,NULL);
		if (rv < 0)
		{
			printf("select() failed:%s\n",strerror(errno));
			return 0;
		}
		else if (rv == 0)
		{
			printf("select() time out!\n");
		}
        else
        {
		    rv = read(fd, buff, sizeof(buff));
		    if (rv < 0)
		    {
			    printf("read() error");
		    }
            else 
            {
                
                signal_back = Protocol_Analysis(buff);
                printf("read from tty: %s %d %d\n", buff, strlen(buff), signal_back);
                
            }
        }

		if (signal_back == 1)
		{
			printf("signal_back ok\n");

			return 1;
		}
		
	}

	return 0;
}


/*
int main(void)
{
	Uart_Main();

	return 1;
}
*/