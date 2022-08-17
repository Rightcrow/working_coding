#include "lib/BS_FunInterface.h"
#include "lib/BS_Client.h"


unsigned char buffer_client[MAXlENGTH];

/***********************************************
    *函数名： ResetClientBuffer
    *功能说明: 用来初始化和重置client用来发送地方buffer
    *参数说明： void
    *返回说明： void
************************************************/
void ResetClientBuffer()
{
    int count = 0;

    for (int count; count < MAXlENGTH; ++count)
    {
        buffer_client[count] = 0x00;
    }

}


/***********************************************
    *函数名： GetFrameLength
    *功能说明: 用来获取报文帧的长度
    *参数说明： void
    *返回说明： int --- 报文的长度length
************************************************/
int GetFrameLength()
{
    int count = 0;

    int length = 0;

    unsigned short a[4] = {0x00, 0x00, buffer_client[10], buffer_client[11]};

    for(count; count < 4; ++count)
    {
        length += (a[count] << ((3 - count) * 8));
    }

    printf("data length = %d\n", length);

    return length;

}

/***********************************************
    *函数名： PutFrameToClientBuffer
    *功能说明: 把报文帧放到要发送的buffer里
    *参数说明： buffer --- 我的发送缓存区
              length --- 我要放多少东西到缓存区里
    *返回说明： void
************************************************/
void PutFrameToClientBuffer(unsigned char* buffer, int length)
{
    int count = 0;

    int tail_num = 11 + length + 3;

    for(count; count <= tail_num; ++count)
    {
        buffer[count] = buffer_client[count];
    }
}



/***********************************************
    *函数名： ClientHeartTest
    *功能说明: 用来组成心跳测试的报文帧
    *参数说明： void
    *返回说明： void
    *备注:在你发的时候，要注意，你怎么发的，服务端就会怎么收
         要想避免大端字和小端字的问题，发的时候，就要高位在前
         低位在后
************************************************/
void ClientHeartTest()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;
  
    buffer_client[9] = CONTROL_HEART_TEST;

    buffer_client[10] = 0x00;

    buffer_client[11] = 0x01;

    buffer_client[12] = TI_HEART_TEST;

    buffer_client[13] = 0x00;

    buffer_client[14] = 0x00;

    buffer_client[15] = TAIL_SIGNAL;

}


/***********************************************
    *函数名： ClientParamInquire
    *功能说明: 用来组成参数查询报文
    *参数说明： void
    *返回说明： void
    *备注:在你发的时候，要注意，你怎么发的，服务端就会怎么收
         要想避免大端字和小端字的问题，发的时候，就要高位在前
         低位在后
************************************************/
void ClientParamInquire()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    buffer_client[9] = CONTROL_PARAM_INQUIRE;

    buffer_client[10] = 0x00;

    /*legnth*/
    buffer_client[11] = 0x06;

    buffer_client[12] = TI_PARAM_INQUIRE;

    /*pn*/
    buffer_client[13] = 0x02;

    // /*两个pid， 0x0001、0x0002*/
    // buffer_client[14] = 0x00;

    // buffer_client[15] = 0x01;

    // buffer_client[16] = 0x00;

    // buffer_client[17] = 0x02;

    /*两个pid， 0x0006、0x000A*/
    buffer_client[14] = 0x00;

    buffer_client[15] = 0x06;

    buffer_client[16] = 0x00;

    buffer_client[17] = 0x0A;


    /*crc*/
    buffer_client[18] = 0x00;

    buffer_client[19] = 0x00;

    /*tail*/
    buffer_client[20] = TAIL_SIGNAL;

}


/***********************************************
    *函数名： ClientParamSet
    *功能说明: 用来组成设置报文
    *参数说明： void
    *返回说明： void
    *备注:在你发的时候，要注意，你怎么发的，服务端就会怎么收
         要想避免大端字和小端字的问题，发的时候，就要高位在前
         低位在后
************************************************/
void ClientParamSet()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    buffer_client[9] = CONTROL_PARAM_SET;

    /*length*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x0E;

    /*ti*/
    buffer_client[12] = TI_PARAM_SET;

    /*pn*/
    buffer_client[13] = 0x02;

    /*pid1 = 6*/
    buffer_client[14] = 0x00;

    buffer_client[15] = 0x06;

    /*pl1 = 2*/
    buffer_client[16] = 0x02;

    /*pv1 = 0xFFFF*/
    buffer_client[17] = 0xFF;

    buffer_client[18] = 0xFF;

    /*pid2 = 10*/
    buffer_client[19] = 0x00;

    buffer_client[20] = 0x0A;

    /*pl2 = 04*/
    buffer_client[21] = 0x04;

    /*pv2 = 0xFFFFFFFF*/
    buffer_client[22] = 0xFF;

    buffer_client[23] = 0xFF;

    buffer_client[24] = 0xFF;

    buffer_client[25] = 0xFF;

    /*crc*/
    buffer_client[26] = 0x00;

    buffer_client[27] = 0x00;

    /*tail*/
    buffer_client[28] = TAIL_SIGNAL;

}

/*实时信息查询发送的测试帧*/
void ClientRealTimeMessageInquire()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    buffer_client[9] = CONTROL_REALTIME_MESSAGE_INQUIRE;

    /*length*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x02;

    /*ti*/
    buffer_client[12] = TI_REALTIME_MESSAGE_INQUIRE;

    /*dt*/
    // buffer_client[13] = STATE;
    buffer_client[13] = CURRENT_VALUE;

    /*crc*/
    buffer_client[14] = 0x00;

    buffer_client[15] = 0x00;

    /*终止符*/
    buffer_client[16] = TAIL_SIGNAL;

}


void ClientControlCommand()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    buffer_client[9] = CONTROL_CONTROL_COMMAND;

    /*length*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x03;

    /*ti*/
    buffer_client[12] = TI_CONTROL_COMMAND;

    /*cn*/
    buffer_client[13] = 0x00;

    buffer_client[14] = 0x01;

    /*crc*/
    buffer_client[15] = 0x00;

    buffer_client[16] = 0x00;

    /*终止符*/
    buffer_client[17] = TAIL_SIGNAL;
}


void ClientFileCatalogueInquire()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    buffer_client[9] = CONTROL_FILE_CATALOGUE_INQUIRE;

    /*length 以序号1为例*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x0E;

    /*ti*/
    buffer_client[12] = TI_FILE_CATALOGUE_INQUIRE;

    /*did 已序号1为例*/
    buffer_client[13] = 0x00;

    buffer_client[14] = 0x00;

    buffer_client[15] = 0x00;

    buffer_client[16] = 0x01;

    /*dnl*/
    buffer_client[17] = 0x08;

    /*dir具体*/
    buffer_client[18] = 'U';

    buffer_client[19] = 'P';

    buffer_client[20] = 'G';

    buffer_client[21] = 'R';

    buffer_client[22] = 'A';

    buffer_client[23] = 'D';

    buffer_client[24] = 'E';

    buffer_client[25] = '/';

    /*crc*/
    buffer_client[26] = 0x00;

    buffer_client[27] = 0x00;

    /*终止符*/
    buffer_client[28] = TAIL_SIGNAL;
}


/*文件目录信息传输帧*/
void ClientFileCatalogueTransmite()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    buffer_client[9] = CONTROL_FILE_CATALOGUE_TRANSMITE;

    /*length,简单测试用，只写到did就完了，所以length只有2*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x05;

    /*ti*/
    buffer_client[12] = TI_FILE_CATALOGUE_TRANSMITE;

    /*did 已1为例*/
    buffer_client[13] = 0x00;

    buffer_client[14] = 0x00;

    buffer_client[15] = 0x00;

    buffer_client[16] = 0x01;

    /*crc*/
    buffer_client[17] = 0x00;

    buffer_client[18] = 0x00;

    /*终止符*/
    buffer_client[19] = TAIL_SIGNAL;
}


/*写文件请求帧*/
void ClientFileWriteInquire()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    /*control*/
    buffer_client[9] = CONTROL_FILE_WRITE_INQUIRE;

    /*length = 20 commm.par（8个字节）为例*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x14;

    /*ti*/
    buffer_client[12] = TI_FILE_WRITE_INQUIRE;

    /*文件名长度*/
    buffer_client[13] = 0x08;

    /*文件名*/
    buffer_client[14] = 'c';

    buffer_client[15] = 'o';

    buffer_client[16] = 'm';

    buffer_client[17] = 'm';

    buffer_client[18] = '.';

    buffer_client[19] = 'p';

    buffer_client[20] = 'a';

    buffer_client[21] = 'r';

    /*文件标识*/
    buffer_client[22] = 0x00;

    buffer_client[23] = 0x00;

    buffer_client[24] = 0x00;

    buffer_client[25] = 0x01;

    /*文件大小,先写作0x0000ffff*/
    buffer_client[26] = 0x00;

    buffer_client[27] = 0x00;

    buffer_client[28] = 0xff;

    buffer_client[29] = 0xff;

    /*文件校验码*/
    buffer_client[30] = 0x00;

    buffer_client[31] = 0x00;

    /*crc*/
    buffer_client[32] = 0x00;

    buffer_client[33] = 0x00;

    /*结尾*/
    buffer_client[34] = 0x16;
}

/*写文件数据帧*/
void ClientWriteDataToFile()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    /*control*/
    buffer_client[9] = CONTROL_WRITE_DATA_TO_FILE;

    /*length, 测试的时候，只写到了fid，后面没有写了，所以length = 5*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x05;

    /*ti*/
    buffer_client[12] = TI_WRITE_DATA_TO_FILE;

    /*fid*/
    buffer_client[13] = 0x00;

    buffer_client[14] = 0x00;

    buffer_client[15] = 0x00;

    buffer_client[16] = 0x01;

    /*crc*/
    buffer_client[17] = 0X00;

    buffer_client[18] = 0x00;

    /*tail*/
    buffer_client[19] = TAIL_SIGNAL;
    
}


/*写文件激活帧*/
void ClientWriteDataActivate()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    /*control*/
    buffer_client[9] = CONTROL_WRITE_DATA_ACTIVATE;

    /*length,还是以comm.par为例*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x0A;

    /*ti*/
    buffer_client[12] = TI_WRITE_DATA_ACTIVATE;

    /*fnl*/
    buffer_client[13] = 0x08;

    /*文件名*/
    buffer_client[14] = 'c';

    buffer_client[15] = 'o';

    buffer_client[16] = 'm';

    buffer_client[17] = 'm';

    buffer_client[18] = '.';

    buffer_client[19] = 'p';

    buffer_client[20] = 'a';

    buffer_client[21] = 'r';

    /*crc*/
    buffer_client[22] = 0x00;

    buffer_client[23] = 0x00;

    /*tail*/
    buffer_client[24] = TAIL_SIGNAL;

}

/*读文件请求帧*/
void ClientReadFileInquire()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    /*control*/
    buffer_client[9] = CONTROL_READ_FILE_INQUIRE;

    /*length 已comm.par为例，length = 2 + 8*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x0A;

    /*ti*/
    buffer_client[12] = TI_READ_FILE_INQUIRE;

    /*文件名长度 fnl*/
    buffer_client[13] = 0x08;

    /*文件名*/
    buffer_client[14] = 'c';

    buffer_client[15] = 'o';

    buffer_client[16] = 'm';

    buffer_client[17] = 'm';

    buffer_client[18] = '.';

    buffer_client[19] = 'p';

    buffer_client[20] = 'a';

    buffer_client[21] = 'r';

    /*crc*/
    buffer_client[22] = 0x00;

    buffer_client[23] = 0x00;

    /*tail*/
    buffer_client[24] = TAIL_SIGNAL;
}

/*读文件数据请求帧*/
void ClientReadFileData()
{
    printf("It's in the ClientReadFileData\n");

    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    /*control*/
    buffer_client[9] = CONTROL_READ_FILE_DATA;
    

    /*length*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x12;

    /*ti*/
    buffer_client[12] = TI_READ_FIEL_DATA;

    /*fid*/
    buffer_client[13] = 0x00;

    buffer_client[14] = 0x00;

    buffer_client[15] = 0x00;

    buffer_client[16] = 0x02;

    /*后续标志sf*/
    buffer_client[17] = 0x00;

    /*文件内容偏移指针值*/
    buffer_client[18] = 0x00;

    buffer_client[19] = 0x00;

    buffer_client[20] = 0x00;

    buffer_client[21] = 0x00;

    /*数据个数*/
    buffer_client[22] = 0x00;

    buffer_client[23] = 0x06;

    /*数据内容，暂定为0xffffffffffff*/
    buffer_client[24] = 0xff;

    buffer_client[25] = 0xff;

    buffer_client[26] = 0xff;

    buffer_client[27] = 0xff;

    buffer_client[28] = 0xff;

    buffer_client[29] = 0xff;

    /*crc*/
    buffer_client[30] = 0x00;

    buffer_client[31] = 0x00;

    /*tail*/
    buffer_client[32] = TAIL_SIGNAL;
}

/*系统对时帧*/
void ClientSystemTimeConfirm()
{
    // printf("It's in the ClientSystemTimeConfirm\n");

    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    /*control*/
    buffer_client[9] = CONTROL_SYSTEM_TIME_CONFIRM;
    
    /*length,length = 0x0007*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x07;

    /*ti*/
    buffer_client[12] = TI_SYSTEM_TIME_CONFIRM;

    /*time 2002年6月22日10时30分15秒*/
    buffer_client[13] = 0x16;

    buffer_client[14] = 0x06;

    buffer_client[15] = 0x16;

    buffer_client[16] = 0x0A;

    buffer_client[17] = 0x1E;

    buffer_client[18] = 0x0F;

    /*crc*/
    buffer_client[19] = 0x00;

    buffer_client[20] = 0x00;

    /*tail*/
    buffer_client[21] = TAIL_SIGNAL;
}

/*时钟查询帧*/
void ClientSystemTimeInquire()
{
    ResetClientBuffer();

    buffer_client[0] = 0xEA;

    buffer_client[1] = ((SA >> 24) & 0XFF);

    buffer_client[2] = ((SA >> 16) & 0XFF);

    buffer_client[3] = ((SA >> 8) & 0XFF);

    buffer_client[4] = SA & 0xFF;
  
    buffer_client[5] = ((DA >> 24) & 0XFF);

    buffer_client[6] = ((DA >> 16) & 0XFF);

    buffer_client[7] = ((DA >> 8) & 0XFF);

    buffer_client[8] = DA & 0XFF;

    /*control*/
    buffer_client[9] = CONTROL_SYSTEM_TIME_INQUIRE;

    /*length = 0x0001定值*/
    buffer_client[10] = 0x00;

    buffer_client[11] = 0x01;

    /*ti*/
    buffer_client[12] = TI_SYSTEM_TIME_INQUIRE;

    /*crc*/
    buffer_client[13] = 0x00;

    buffer_client[14] = 0x00;

    /*tail*/
    buffer_client[15] = TAIL_SIGNAL;
}