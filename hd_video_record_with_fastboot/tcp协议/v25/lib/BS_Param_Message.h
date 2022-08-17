#ifndef __BS_PARAM_MESSAGE_H_
#define __BS_PARAM_MESSAGE_H_


/*用于宏定义专用地址*/
/*设备初始地址*/
#define DEVICE_ADDRESS_INITIAL                  0X00000000

/*地址设置通配地址*/
#define DEVICE_ADDRESS_GENERAL_DISTRIBUTION     0XAAAAAAAA

/*维护软件地址*/
#define DEVICE_ADDRESS_MAINTAIN                 0XEEEEEEEE

/*广播地址*/
#define DEVICE_ADDRESS_BROADCAST                0XFFFFFFFF


/*运行参数编号及参数*/
/*拍摄模式，默认为0*/
#define SHOOT_MODE                        0X0001
#define SHOOT_MODE_INT                    1
#define SHOOT_MODE_PARAM_LENGTH           0X01
#define SHOOT_MODE_PARAM_LENGTH_INT       1
#define SHOOT_MODE_PHOTO_VIDEO            0x00
#define SHOOT_MODE_PHOTO                  0X01
#define SHOOT_MODE_VIDEO                  0X02


/*照片尺寸，默认为0*/
#define PHOTO_SIZE                        0X0002
#define PHOTO_SIZE_INT                    2
#define PHOTO_SIZE_PARAM_LENGTH           0X01
#define PHOTO_SIZE_PARAM_LENGTH_INT       1
#define PHOTO_SIZE_12MP                   0X00
#define PHOTO_SIZE_5MP                    0X01
#define PHOTO_SIZE_2MP                    0X02

/*录像尺寸,默认为0*/
#define VIDEO_SIZE                        0X0003
#define VIDEO_SIZE_INT                    3
#define VIDEO_SIZE_PARAM_LENGTH           0X01
#define VIDEO_SIZE_PARAM_LENGTH_INT       1
#define VIDEO_SIZE_1440                   0X00
#define VIDEO_SIZE_1280                   0X01
#define VIDEO_SIEZE_640                   0X02

/*录像时长,默认为10s*/
#define VIDEO_DURATION                    0X0004
#define VIDEO_DURATION_INT                4
#define VIDEO_DURATION_PARAM_LENGTH       0X01
#define VIDEO_DURATION_PARAM_LENGTH_INT   1
#define VIDEO_DURATION_TIME               0X0A

/*PIR感应灵敏度，默认为高*/
#define PIR_SENSITIVITY                   0X0005
#define PIR_SENSITIVITY_INT               5
#define PIR_SENSITIVITY_PARAM_LENGTH      0X01
#define PIR_SENSITIVITY_PARAM_LENGTH_INT  1
#define PIR_SENSITIVITY_HIGH              0X00
#define PIR_SENSITIVITY_MIDDLE            0X01
#define PIR_SENSITIVITY_LOW               0X02
#define PIR_SENSITIVITY_CLOSE             0X03

/*触发时间间隔，默认为60s*/
#define TRIGGER_TIME_INTERVAL                   0X0006
#define TRIGGER_TIME_INTERVAL_INT               6
#define TRIGGER_TIME_INTERVAL_PARAM_LENGTH      0X02
#define TRIGGER_TIME_INTERVAL_PARAM_LENGTH_INT  2
#define TRIGGER_TIME_INIERVAL_TIME              0X003C

/*连拍张数，默认关闭*/
#define CONSECUTIVE_PHOTO                  0X0007
#define CONSECUTIVE_PHOTO_INT              7
#define CONSECUTIVE_PHOTO_PARAM_LENGTH     0X01
#define CONSECUTIVE_PHOTO_PARAM_LENGTH_INT 1
#define CONSECUTIVE_PHOTO_CLOSE            0X01
#define CONSECUTIVE_PHOTO_2                0X02
#define CONSECUTIVE_PHOTO_3                0X03

/*定时拍照闹钟，默认为0,关闭*/
#define TIME_PHOTO_ALARM_CLOCK                  0X0008
#define TIME_PHOTO_ALARM_CLOCK_INT              8
#define TIME_PHOTO_ALARM_CLOCK_PARAM_LENGTH     0X03
#define TIME_PHOTO_ALARM_CLOCK_PARAM_LENGTH_INT 3
#define TIME_PHOTO_ALARM_CLOCK_TIME             0X000000

/*连拍模式，默认关闭*/
#define BURST_MODE                        0X0009
#define BURST_MODE_INT                    9
#define BURST_MODE_PARAM_LENGTH           0X01
#define BURST_MODE_PARAM_LENGTH_INT       1
#define BURST_MODE_CLOSE                  0X00
#define BURST_MODE_OPEN                   0X01

/*经度*/
#define LONGITUDE                         0X000A
#define LONGITUDE_INT                     10
#define LONGITUDE_PARAM_LENGTH            0X04
#define LONGITUDE_PARAM_LENGTH_INT        4
#define LONGITUDE_LOCATION                0X00000000

/*纬度*/
#define LATITUDE                          0X000B
#define LATITUDE_INT                      11
#define LATITUDE_PARAM_LENGTH             0X04
#define LATITUDE_PARAM_LENGTH_INT         4
#define LATITUDE_LOCATION                 0X00000000

/*软件版本号*/
#define SOFTWARE_VERSION                  0X000C
#define SOFTWARE_VERSION_INT              12
#define SOFTWARE_VERSION_PARAM_LENGTH     0X23
#define SOFTWARE_VERSION_PARAM_LENGTH_INT 32

/*硬件版本号*/
#define HARDWARE_VERSION                  0X000D
#define HARDWARE_VERSION_INT              13
#define HARDWARE_VERSION_PARAM_LENGTH     0X23
#define HARDWARE_VERSION_PARAM_LENGTH_INT 32

/*控制命令字*/
/*设备软件重启*/
#define CONTROL_REBOOT                            0X0001

/*设备拍照*/
#define CONTROL_TAKE_PHOTO                        0X0002

/*设备录制视频*/
#define CONTROL_TAKE_VEDIO                        0X0003

/*SD卡格式化*/
#define CONTROL_SD_CARD_FORMAT                    0X0004


/*文件目录序号和对应的目录名*/
#define FILE_DIRECTION_ID_1                       0X00000001
#define FILE_ID_1_NAME_8_CHAR                     UPGRADE/

#define FILE_DIRECTION_ID_2                       0X00000002
#define FILE_ID_2_NAME_4_CHAR                     PAR/

#define FILE_DIRECTION_3                          0X00000003
#define FILE_ID_3_NAME_12_CHAR                    HISTORY/SOE/
#define FILE_ID_3_NAME_11_CHAR                    HISTORY/CO
#define FILE_ID_3_NAME_13_CHAR                    HISTORY/ULOG

#endif

