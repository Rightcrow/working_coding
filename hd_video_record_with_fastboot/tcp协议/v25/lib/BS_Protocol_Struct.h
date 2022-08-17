#ifndef __BS_PROTOCOL_STRUCT_H_
#define __BS_PROTOCOL_STRUCT_H_

#define MAXlENGTH 1024

/*这个结构体表示，从ti到ctc之前的所有东西*/
struct DataStruct
{
    unsigned char ti;

    unsigned char data[MAXlENGTH];
}__attribute__((packed));



/*更改之后的新报文结构体*/
struct FrameStruct
{
    
    /*启动字符*/
    unsigned char head;

    /*源地址*/
    unsigned int source_address;

    /*目标地址*/
    unsigned int destin_address;

    /*控制字*/
    unsigned char control;

    /*报文长度*/
    unsigned short length;

    /*中间的实际数据*/
    struct DataStruct data_struct;

    /*crc校验码*/
    unsigned short crc;

    unsigned char tail;

}__attribute__((packed));



#endif