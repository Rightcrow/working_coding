#ifndef __BS_REALTIME_MESSAGE_H_
#define __BS_REALTIME_MESSAGE_H_

/*实时信息编号*/
/*状态信息*/
#define STATE                           0x00

/*电流值*/
#define CURRENT_VALUE                   0X01

/*故障电流*/
#define FAULT_CURRENT                   0X02

/*电场强度*/
#define ELECTRIC_FIELD_INTENSITY        0X03

/*电容电压*/
#define CAPACITOR_VOLTAGE               0x04

/*电池电压*/
#define BATTERY_VOLTAGE                 0X05

/*温度*/
#define TEMPERATURE                     0X06

/*终端主电池电压*/
#define TERMINAL_MAIN_BATTERY_VOLTAGE   0X07

/*终端太阳能/CT电压*/
#define TERMINAL_SOLAR_CT_VOLTAGE       0X08

/*终端温度*/
#define TERMINAL_TEMPERATURE            0X09

/*终端信号强度*/
#define TERMINAL_SIGNAL_STRENGTH        0X0A


/*状态信息的位定义*/
struct BS_RealTime_STATE
{
    unsigned char earth_fault:1;
    unsigned char instantaneous_short_circuit:1;
    unsigned char permanent_short_circuit:1;
    unsigned char current_out_of_limit:1;
    unsigned char temperature_out_of_limit:1;
    unsigned char flash_light:1;
    unsigned char battery_undervoltage:1;
    unsigned char power_failure:1;
};



#endif