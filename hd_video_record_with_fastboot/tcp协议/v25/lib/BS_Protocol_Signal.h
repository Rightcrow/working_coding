#ifndef __BS_PROTOCOL_SIGNAL_H_
#define __BS_PROTOCOL_SIGNAL_H_

/*设置所有报文控制码和标识类型的宏定义*/
/*心跳测试宏*/
#define CONTROL_HEART_TEST              0X80
#define TI_HEART_TEST                   0X01

/*心跳应答宏*/
#define CONTROL_HEART_TEST_RESPONSE     0X00
#define TI_HEART_TEST_RESPONSE          0X02

/*参数查询宏*/
#define CONTROL_PARAM_INQUIRE           0X81
#define TI_PARAM_INQUIRE                0X05

/*参数查询应答宏*/
#define CONTROL_PARAM_INQUIRE_RESPONSE  0X01
#define TI_PARAM_INQUIRE_RESPONSE       0X06

/*参数设置宏*/
#define CONTROL_PARAM_SET               0X81
#define TI_PARAM_SET                    0X07

/*参数设置应答宏*/
#define CONTROL_PARAM_SET_RESPONSE      0X01
#define TI_PARAM_SET_RESPONSE           0X08

/*实时信息查询宏*/
#define CONTROL_REALTIME_MESSAGE_INQUIRE   0X82
#define TI_REALTIME_MESSAGE_INQUIRE        0X09

/*实时信息查询应答宏*/
#define CONTROL_REALTIME_MESSAGE_INQUIRE_RESPONSE   0X02
#define TI_REALTIME_MESSAGE_INQUIRE_RESPONSE        0X0A

/*控制命令宏*/
#define CONTROL_CONTROL_COMMAND         0X83
#define TI_CONTROL_COMMAND              0X0B

/*控制命令应答宏*/
#define CONTROL_CONTROL_COMMAND_RESPONSE   0X03
#define TI_CONTROL_COMMAND_RESPONSE        0X0C

/*文件目录查询宏*/
#define CONTROL_FILE_CATALOGUE_INQUIRE     0X84
#define TI_FILE_CATALOGUE_INQUIRE          0X0D

/*文件目录查询应答宏*/
#define CONTROL_FILE_CATALOGUE_INQUIRE_RESPONSE 0X04
#define TI_FILE_CATALOGUE_INQUIRE_RESPONSE      0X0E

/*文件目录信息传输宏*/
#define CONTROL_FILE_CATALOGUE_TRANSMITE   0X84
#define TI_FILE_CATALOGUE_TRANSMITE        0X0F

/*文件目录信息传输应答宏*/
#define CONTROL_FILE_CATALOGUE_TRANSMITE_RESPONSE   0X04
#define TI_FILE_CATALOGUE_TRANSMITE_RESPONSE        0X10

/*写文件请求宏*/
#define CONTROL_FILE_WRITE_INQUIRE         0X84
#define TI_FILE_WRITE_INQUIRE              0x11

/*写文件请求应答宏*/
#define CONTROL_FILE_WRITE_INQUIRE_RESPONSE 0X04
#define TI_FILE_WRITE_INQUIRE_RESPONSE      0X12

/*写文件数据宏*/
#define CONTROL_WRITE_DATA_TO_FILE         0X84
#define TI_WRITE_DATA_TO_FILE              0X13

/*写文件数据确认宏*/
#define CONTROL_WRITE_DATA_TO_FILE_RESPONSE 0X04
#define TI_WRITE_DATA_TO_FILE_RESPONSE      0X14

/*写文件激活宏*/
#define CONTROL_WRITE_DATA_ACTIVATE        0X84
#define TI_WRITE_DATA_ACTIVATE             0X15

/*写文件激活应答宏*/
#define CONTROL_WRITE_DATA_ACTIVATE_RESPONSE    0X04
#define TI_WRITE_DATA_ACTIVATE_RESPONSE         0X16

/*读文件请求宏*/
#define CONTROL_READ_FILE_INQUIRE          0X84
#define TI_READ_FILE_INQUIRE               0X17

/*读文件请求应答宏*/
#define CONTROL_READ_FILE_INQUIRE_RESPONSE  0X04
#define TI_READ_FILE_INQUIRE_RESPONSE       0x18

/*读文件数据帧*/
#define CONTROL_READ_FILE_DATA              0X84
#define TI_READ_FIEL_DATA                   0X19

/*读文件数据应答帧*/
#define CONTROL_READ_FILE_DATA_RESPONSE     0X04
#define TI_READ_FILE_DATA_RESPONSE          0X1A

/*系统对时宏*/
#define CONTROL_SYSTEM_TIME_CONFIRM         0X85
#define TI_SYSTEM_TIME_CONFIRM              0X1B

/*系统对时应答宏*/
#define CONTROL_SYSTEM_TIME_CONFIRM_RESPONSE    0X05
#define TI_SYSTEM_TIME_CONFIRM_RESPONSE         0X1C

/*时钟查询宏*/
#define CONTROL_SYSTEM_TIME_INQUIRE         0X85
#define TI_SYSTEM_TIME_INQUIRE              0X1D

/*时钟查询应答宏*/
#define CONTROL_SYSTEM_TIME_INQUIRE_RESPONSE    0X05
#define TI_SYSTEM_TIME_INQUIRE_RESPONSE         0X1E

/*标识符宏*/
#define HEAD_SIGNAL                        0XEA
#define TAIL_SIGNAL                        0X16

/*心跳err宏*/
#define ERR_HEART_RESPONSE_SUCCESS          0X00
#define ERR_HEART_RESPONSE_FAIL             0XFF

/*一般err宏*/
#define ERR_SUCCESS                         0X00
#define ERR_INVALID                         0X01
#define ERR_UNKNOW_ERR                      0X02

/*基本参数宏定义,暂时这么用，后面会改成可修改的*/
/*SA = 192.168.1.220*/
/*DA = 192.168.1.110*/
#define CRC                                 0x0000
#define SA                                  0XC0A801DC
#define DA                                  0XC0A8016E  

#define SA_RESPONSE                         0XC0A8016E   
#define DA_RESPONSE                         0XC0A801DC


#endif