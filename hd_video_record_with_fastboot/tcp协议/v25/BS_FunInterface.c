#include "lib/BS_FunInterface.h"
#include "lib/BS_Client.h"

struct FrameStruct frame_struct_glob;

unsigned char buffer_response[MAXlENGTH];

int length_response;

int socket_server;

int socket_client;

/*******************************
    *函数名： getCRC
    *功能说明：CRC校验码生成
    *参数说明： buf --- 待校验的信息
              len --- 待校验信息长度
    *返回说明： crc --- 计算后的校验码
********************************/
unsigned short getCRC(unsigned char* buf,unsigned int len)
{
	unsigned int i_count;
	unsigned int j_count;
	unsigned short crc;
	unsigned short flag;
	crc = 0x0000;
	for(i_count = 0; i_count < len; i_count++)
	{
		crc^=(((unsigned short)buf[i_count]) << 8);
		for (j_count = 0; j_count < 8; j_count++)
		{
			flag = crc & 0x8000;
			crc <<= 1;
			if(flag)
			{
				crc &= 0xfffe;
				crc ^= 0x8005;
			}
		}
	}
	return crc;
}


/***********************************************
    *函数名： ServerInit
    *功能说明：初始化服务端（设备）
    *参数说明： void
    *返回说明： serv_socket --- 创建完成的服务端套接字
************************************************/
void* ServerInit()
{
    printf("read to create server\n");
    /*socket 三个参数：
    af：地址族群，ip4还是ip6
    type：数据传输方式，是面向连接还是无连接
    protocol：传输协议，tcp还是udp*/
    int socket_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /*用于打印收到的信息用*/
    int count = 0;

    unsigned char buffer[MAXlENGTH] = {0x00};

    int length_serv;

    /*将套接字和IP、端口绑定操作*/
    /*ser_addr,用于存放服务端IP和端口信息*/
    struct sockaddr_in serv_addr;
    
    /*里面全部用0填充*/
    memset(&serv_addr, 0, sizeof(serv_addr));  

    /*配置绑定参数*/
    /*sin_family:使用IPV4地址*/
    /*sin_addr.s_addr:具体IP地址*/
    /*sin_port:端口号*/
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    serv_addr.sin_port = htons(1234); 

    /*参数绑定*/
    bind(socket_server, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    printf("read to listen\n");

    /*进入监听状态，等待用户发起请求*/
    /*20表示请求队列最大的长度*/
    listen(socket_server, 20);

    /*接受客户请求*/
    /*clnt_addr用于存放客户端的IP和端口信息*/
    struct sockaddr_in clnt_addr;

    /*socklen_t 就是int类型*/
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    /*套接字处于监听状态时，接受客户端请求，返回客户端套接字标识*/
    int clnt_sock = accept(socket_server, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    /*这是单独测试的时候用的，整合之后不用*/
    /*接收客户端发送的数据*/
    /*写在一个死循环里，它就能一直接收发来的数据*/
    // while(1)
    // {   
    //     printf("ready to read message");

    //     char buffer[1024] = {};

    //     read(clnt_sock, buffer, sizeof(buffer) - 1);

    //     /*这里的长度，每次测试的时候要修改*/
    //     for(int count = 0; count < 16; ++count)
    //     {
    //         printf("Message from client: %02xh\n", buffer[count]);     
    //     }
      
    // }   

        /*单个报文测试*/
        printf("ready to read message\n");

        read(clnt_sock, buffer, sizeof(buffer) - 1);

        /*这里计算的是报文中的length标志里的数具体是多少*/
        length_serv = LengthAssemble(buffer[10], buffer[11]);

        /*收到的东西打印出来 15表示除了数据区域，其他数据的总长*/
        for(int count = 0; count < (15 + length_serv); ++count)
        {
            printf("Message from client: %02hX\n", buffer[count]);  
        }
        printf("\n");

        /*获取收到报文的data区域*/
        unsigned char buffer_recv_data_area[length_serv];

        GetDataArea(buffer, buffer_recv_data_area, length_serv);

        /*判断报文中的ti和control*/
        Judge(buffer, length_serv, buffer_recv_data_area);

        /*根据组好的帧，把帧放进buffer里，然后发出去*/
        BufferAssemble();

        write(clnt_sock, buffer_response, sizeof(buffer_response));
        
}


/***********************************************
    *函数名： GetDataArea
    *功能说明：获取接收报文的数据域
    *参数说明： buffer --- 接收到的报文
              buffer_recv_data_area --- 报文的数据区
              length_serv --- 数据域的长度
    *返回说明： void
************************************************/
void GetDataArea(unsigned char* buffer, unsigned char* buffer_recv_data_area, int length_serv)
{

    printf("ready to get data area\n");

    printf("data area length = %d\n", length_serv);

    int count = 0;

    for (count; count < length_serv; ++count)
    {
        buffer_recv_data_area[count] = buffer[12 + count];
        printf("buffer_recv_data_area[%d] = %02hX\n", count, buffer_recv_data_area[count]);
    }

    printf("\n");

}


/***********************************************
    *函数名： Judge
    *功能说明：分析收到的报文，根据收到的报文，判断回传报文的ti、
             control
    *参数说明： unsigned char* buffer --- 收到的缓冲区数组
              unsigned char* buffer_rev_data_area --- 缓冲区的数据区域
    *返回说明： void
************************************************/
void Judge(unsigned char* buffer, int length_serv, unsigned char* buffer_recv_data_area)
{
    /*获取收到的数据的control和ti*/
    unsigned char control_serv = buffer[9];
    printf("control_serv = %02hX\n", control_serv);

    unsigned char ti_serv = buffer_recv_data_area[0];
    printf("ti_serv = %02hX\n", ti_serv);

    /*根据收到的control和ti，判断回传报文里是什么control、ti*/
    unsigned char control_response = JudgeControl(control_serv);
    printf("control_response = %02hX\n", control_response);

    unsigned char ti_response = JudgeTi(ti_serv);
    printf("ti_response = %02hX\n", ti_response);

    /*根据ti、control、数据区选择组帧方式*/
    FrameAssembleWay(ti_response, control_response, length_serv, buffer_recv_data_area);

    /*计算组好的帧的长度*/
    length_response = GetResponseLength(frame_struct_glob.length);

    printf("length_response = %d\n", length_response);

}

/***********************************************
    *函数名： GetResponseLength
    *功能说明：计算回传报文的数据长度length这一项值是多少
             并转化成int类型输出
    *参数说明： unsigned short length --- 报文帧结构体里的length
    *返回说明： int --- 转换成int类型的length
************************************************/
int GetResponseLength(unsigned short length)
{

    unsigned char length_response_short_high = ((length >> 8) & 0xFF);

    unsigned char length_response_short_low = (length & 0xFF);

    printf("\n");
    printf("It's in the GetResponseLength\n");
    int length_response_int = LengthAssemble(length_response_short_high, length_response_short_low);

    return length_response_int;
}


/***********************************************
    *函数名： FrameAssembleWay
    *功能说明：根据返回的ti、control、和接收的数据区域进行组帧
    *参数说明： ti_response --- 返回标识符
              control_response --- 返回控制符
              buffer_rev_data_area --- 接收的数据区
    *返回说明： void
************************************************/
void FrameAssembleWay(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    FrameInit();

    switch (ti_response)
    {
        /*心跳应答组帧*/
        case TI_HEART_TEST_RESPONSE:
        {
            HeartTestResponse(ti_response, control_response, buffer_recv_data_area);
            break;
        }
        /*参数查询应答组帧*/
        case TI_PARAM_INQUIRE_RESPONSE:
        {
            ParamInquireResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }  
        /*参数设置应答组帧*/
        case  TI_PARAM_SET_RESPONSE:
        {
            ParamSetResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*实时信息查询应答组帧*/
        case TI_REALTIME_MESSAGE_INQUIRE_RESPONSE:
        {
            RealtimeMessageInquireResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*控制命令应答组帧*/
        case TI_CONTROL_COMMAND_RESPONSE:
        {
            ControlCommandResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*文件目录查询应答组帧*/
        case TI_FILE_CATALOGUE_INQUIRE_RESPONSE:
        {
            FileCatalogueInquireResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*目录信息传输应答组帧*/
        case TI_FILE_CATALOGUE_TRANSMITE_RESPONSE:
        {   
            FileCatalogueTransmiteResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*写文件请求应答组帧*/
        case TI_FILE_WRITE_INQUIRE_RESPONSE:
        {
            FileWriteInquireResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*写文件数据确认组帧*/
        case TI_WRITE_DATA_TO_FILE_RESPONSE:
        {
            WriteDataToFileResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*写文件激活应答组帧*/
        case TI_WRITE_DATA_ACTIVATE_RESPONSE:
        {
            WriteDataActivateResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*读文件请求应答组帧*/
        case TI_READ_FILE_INQUIRE_RESPONSE:
        {
            ReadFileInquireResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*读文件数据应答组帧*/
        case TI_READ_FILE_DATA_RESPONSE:
        {
            ReadFileDataResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*系统对时应答组帧*/
        case TI_SYSTEM_TIME_CONFIRM_RESPONSE:
        {
            SystemTimeConfirmResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        /*始终查询应答组帧*/
        case TI_SYSTEM_TIME_INQUIRE_RESPONSE:
        {
            SystemTimeInquireResponse(ti_response, control_response, length_serv, buffer_recv_data_area);
            break;
        }
        default:
        {
            printf("Analyze faile, No response\n");
            break;
        }
        
    }
}


/***********************************************
    *函数名： JudgeTi
    *功能说明： 根据收到的ti，判断返回报文中的ti
    *参数说明： unsigned char ti_serv --- 所收报文中的ti
    *返回说明： unsigned char --- 返回报文中的ti
************************************************/
unsigned char JudgeTi(unsigned char ti_serv)
{
    unsigned char ti_response = 0xff;

    switch (ti_serv)
    {
        /*心跳应答*/
        case TI_HEART_TEST:
        {
            ti_response = TI_HEART_TEST_RESPONSE;
            break;
        }
        /*参数查询应答*/
        case TI_PARAM_INQUIRE:
        {
            ti_response = TI_PARAM_INQUIRE_RESPONSE;
            break;
        }
        /*参数设置应答*/
        case TI_PARAM_SET:
        {
            ti_response = TI_PARAM_SET_RESPONSE;
            break;
        }
        /*实时信息查询应答*/
        case TI_REALTIME_MESSAGE_INQUIRE:
        {
            ti_response = TI_REALTIME_MESSAGE_INQUIRE_RESPONSE;
            break;
        }
        /*控制命令应答*/
        case TI_CONTROL_COMMAND:
        {
            ti_response = TI_CONTROL_COMMAND_RESPONSE;
            break;
        }
        /*文件目录查询应答*/
        case TI_FILE_CATALOGUE_INQUIRE:
        {
            ti_response = TI_FILE_CATALOGUE_INQUIRE_RESPONSE;
            break;
        }
        /*文件目录信息传输应答*/
        case TI_FILE_CATALOGUE_TRANSMITE:
        {
            ti_response = TI_FILE_CATALOGUE_TRANSMITE_RESPONSE;
            break;
        }
        /*写文件请求应答*/
        case TI_FILE_WRITE_INQUIRE:
        {
            ti_response = TI_FILE_WRITE_INQUIRE_RESPONSE;
            break;
        }
        /*写文件数据确认*/
        case TI_WRITE_DATA_TO_FILE:
        {
            ti_response = TI_WRITE_DATA_TO_FILE_RESPONSE;
            break;
        }
        /*写文件激活应答*/
        case TI_WRITE_DATA_ACTIVATE:
        {
            ti_response = TI_WRITE_DATA_ACTIVATE_RESPONSE;
            break;
        }
        /*读文件请求应答*/
        case TI_READ_FILE_INQUIRE:
        {
            ti_response = TI_READ_FILE_INQUIRE_RESPONSE;
            break;
        }
        /*读文件数据应答*/
        case TI_READ_FIEL_DATA:
        {
           ti_response = TI_READ_FILE_DATA_RESPONSE;
           break;
        }
        /*系统对时应答*/
        case TI_SYSTEM_TIME_CONFIRM:
        {
            ti_response = TI_SYSTEM_TIME_CONFIRM_RESPONSE;
            break;
        }
        /*时钟查询应答*/
        case TI_SYSTEM_TIME_INQUIRE:
        {
            ti_response = TI_SYSTEM_TIME_INQUIRE_RESPONSE;
            break;
        }
        /*默认0x00*/
        default:
        {
            break;
        }           
    }

    return ti_response;
}


/***********************************************
    *函数名： JudgeControl
    *功能说明： 根据收到的control，判断返回报文中的control
    *参数说明： unsigned char control_serv --- 所收报文中的control
    *返回说明： unsigned char --- 返回报文中的control
************************************************/
unsigned char JudgeControl(unsigned char control_serv)
{
    unsigned char control_response = 0xff;

    switch (control_serv)
    {
        /*心跳应答*/
        case CONTROL_HEART_TEST:
        {
            control_response = CONTROL_HEART_TEST_RESPONSE;
            break;
        }
        /*参数相关应答*/
        case (CONTROL_PARAM_INQUIRE | CONTROL_PARAM_SET ):
        {
            /*返回的control是一样的，写一个就行*/
            control_response = CONTROL_PARAM_INQUIRE_RESPONSE;
            break;
        }
        /*实时信息查询应答*/
        case CONTROL_REALTIME_MESSAGE_INQUIRE:
        {
            control_response = CONTROL_REALTIME_MESSAGE_INQUIRE_RESPONSE;
            break;
        }
        /*控制命令应答*/
        case CONTROL_CONTROL_COMMAND:
        {
            control_response = CONTROL_CONTROL_COMMAND_RESPONSE;
            break;
        }
        /*文件目录相关应答*/
        case (CONTROL_FILE_CATALOGUE_INQUIRE | CONTROL_FILE_CATALOGUE_TRANSMITE
             | CONTROL_FILE_WRITE_INQUIRE | CONTROL_WRITE_DATA_TO_FILE | CONTROL_WRITE_DATA_ACTIVATE
             | CONTROL_READ_FILE_INQUIRE | CONTROL_READ_FILE_DATA):
        {
            /*返回的control是一样的，写一个就行*/
            control_response = CONTROL_FILE_CATALOGUE_INQUIRE_RESPONSE;
            break;
        }
        /*时钟相关应答*/
        case (CONTROL_SYSTEM_TIME_CONFIRM | CONTROL_SYSTEM_TIME_INQUIRE):
        {
            control_response = CONTROL_SYSTEM_TIME_CONFIRM_RESPONSE;
            break;
        }        
       default:
        {
            break;
        }
        
    }

    return control_response;

}


/***********************************************
    *函数名： HeartTestResponse
    *功能说明：用来回复心跳报文
    *参数说明： ti_response --- 要回的报文的标识符
              control_response --- 回复报文的控制符
              buffer_rev_data_area --- 收到的报文的数据区
    *返回说明： void
************************************************/
void HeartTestResponse(unsigned char ti_response, unsigned char control_response, unsigned char* buffer_rev_data_area)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    printf("It's in the struct response, control = %02hX\n", frame_struct_glob.control);

    frame_struct_glob.length = 0x0002;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = 0x00;

    frame_struct_glob.crc = 0x0000;

    frame_struct_glob.tail = TAIL_SIGNAL;
}


/***********************************************
    *函数名： ParamInquireResponse
    *功能说明：参数查询应答帧
    *参数说明： ti_response --- 要回的报文的标识符
              control_response --- 回复报文的控制符
              length_serv --- 收到的报文的length区数值
              buffer_rev_data_area --- 收到的报文的数据区
    *返回说明： void
************************************************/
void ParamInquireResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_rev_data_area)
{
    /*计算n的值*/
    unsigned char pn_char = buffer_rev_data_area[1];

    int pn_int = CharToInit(pn_char);

    /*因为一个pid占两个字节，所pid数据的长度 = 2 * n*/
    unsigned char buffer_pid_char[pn_int * 2];

    /*根据我的bufer_pid_int,来判断我各个M的值，把他们放在一个数组里*/
    /*buffer_pl_int 放各个pid的长度，*/
    int buffer_pl_int[pn_int];

    /*为了结构体中数据部分的填充*/
    unsigned char buffer_pl_char[pn_int];

    /*计算我要放到结构体里length标签里的数字*/
    unsigned short length_struct_response = CalcuParamInquireResponseLength(buffer_rev_data_area, length_serv, 
                                                               buffer_pid_char, pn_int, buffer_pl_int, buffer_pl_char);

    /*创建buffer_pv_char[]数组，用来存放实际的参数*/
    /*使用pl_buffer里的数据，算出我整个pv的char型数组有多长*/
    int length_buffer_pv_char = SumBufferPL(buffer_pl_int, pn_int);

    unsigned char buffer_pv_char[length_buffer_pv_char];

    /*关于如何获取参数这个，我把这个接口留出来了，张总后续实现*/
    /*在我测试阶段，我里面所有的参数，全都赋值位0xff*/
    SetBufferPV(buffer_pv_char, length_buffer_pv_char);

    /*用于在赋值时计数，从而充buffer_pv_char里正确获取参数*/
    int buffer_pv_char_count = 0;

    /*一个大数组，用来把我pid、pl、pv组合起来的大数组拼接起来*/
    int p_response_length = CalcuParamInquireResponseLengthInsideFunc(buffer_pl_int, pn_int);

    int p_data_length = p_response_length - 3;

    unsigned char p_data[p_data_length];

    int index_p_data = 0;

    /*组装我pn个参数编号、参数值长度、参数值*/
    for (int count = 0; count < pn_int; ++count)
    {
        /*这个就是我要的每个参数对应的参数值长度*/
        int m_param_length = buffer_pl_int[count];

        /*因为我是先分组，再合并的写法，这个buffer_temp就相当于每次分组的buffer*/
        /*我每获得一个buffer值，就要把它放到我的拼接数组里面*/
        unsigned char buffer_temp[3 + m_param_length];
        
        buffer_temp[0] = buffer_pid_char[2 * count];
        p_data[index_p_data] = buffer_temp[0];      
        index_p_data++;

        buffer_temp[1] = buffer_pid_char[2 * count + 1];
        p_data[index_p_data] = buffer_temp[1];      
        index_p_data++;

        buffer_temp[2] = buffer_pl_char[count];
        p_data[index_p_data] = buffer_temp[2];      
        index_p_data++;


        for (int m_temp = 0; m_temp < m_param_length; ++m_temp)
        {
            buffer_temp[3 + m_temp] = buffer_pv_char[buffer_pv_char_count + m_temp];
            p_data[index_p_data] = buffer_temp[3 + m_temp];      
            index_p_data++;
        }

        buffer_pv_char_count += m_param_length;
    }

    ParamInquireResponseAssemble(ti_response, control_response, length_struct_response, pn_char, p_data_length, p_data);

}


/***********************************************
    *函数名：  ParamInquireResponseAssemble
    *功能说明：用来对参数查询应答帧组帧
    *参数说明：              
    *返回说明： void
************************************************/
void ParamInquireResponseAssemble(unsigned char ti_response, unsigned char control_response, 
                                  unsigned short length_struct_response, unsigned char pn_char, int p_data_length, unsigned char* p_data)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_struct_response;

    frame_struct_glob.data_struct.ti = ti_response;

    /*err*/
    frame_struct_glob.data_struct.data[0] = ErrJudgeOfParamInquire();

    /*pn*/
    frame_struct_glob.data_struct.data[1] = pn_char;

    /*p_data放进来*/
    for (int count = 0; count < p_data_length; ++count)
    {
        frame_struct_glob.data_struct.data[2 + count] = p_data[count];
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;

}




/***********************************************
    *函数名：  ParamSetResponse
    *功能说明: 参数设置应答帧参数设置
    *参数说明： ti_response --- 返回帧中的ti
              control_response --- 返回帧中的control
              length_serv --- 接收报文中length栏内容的int形式
              buffer_recv_data_area --- 接收报文中的数据区域（ti到crc之前的东西）
    *返回说明： void
************************************************/
void ParamSetResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    /*计算n的值*/
    unsigned char pn_char = buffer_recv_data_area[1];

    /*pn-参数数量的int形态*/
    int pn_int = CharToInit(pn_char);

    /*计算长度*/
    int length_response_int = 3 + 2 * pn_int;

    unsigned short length_response_short = IntToShort(length_response_int);

    /*计算err*/
    unsigned char err_param_set_response = ErrJudgeOfParamSet();

    /*获取recv_buffer中的pid、pl、pv组成的数据，把他们放到这个数组里*/
    unsigned char buffer_p_data_recv[length_serv - 2];

    for (int count = 0; count < (length_serv - 2); ++count)
    {
        buffer_p_data_recv[count] = buffer_recv_data_area[2 + count];
    }

    /*构建一个pid的char型数组，用于装载接收报文中的所有pid*/
    unsigned char buffer_pid_char[2 * pn_int];

    /*把recv_buffer中的pid，全部存到buffer_pid_char里*/
    GetPIDParam(buffer_p_data_recv, buffer_pid_char, pn_int);

    /*把我算出来的需要的参数放到结构体里*/
    ParamSetResponseAssemble(ti_response, control_response, length_response_short, length_response_int, err_param_set_response,
                            pn_char, buffer_pid_char);

}


/*把我算好的参数设置应答帧，放到结构体里*/
void ParamSetResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short, int length_response_int,
                              unsigned char err_param_set_response, unsigned char pn_char, unsigned char* buffer_pid_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_param_set_response;

    frame_struct_glob.data_struct.data[1] = pn_char;

    for (int count = 2; count < length_response_int; ++count)
    {
        frame_struct_glob.data_struct.data[count] = buffer_pid_char[count - 2];
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;
}


/*计算我实时信息查询应答帧里需要的参数*/
void RealtimeMessageInquireResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    /*计算err*/
    unsigned char err_response_char = ErrJudgeOfRealtimeMessageInquire();

    /*获取DT*/
    unsigned char dt_char = buffer_recv_data_area[1];

    /*用于存放读取到的数据集合*/
    /*除了状态信息，其余都是4个字节，所有只有状态信息是只读dp_data_char[4]
    其他的实时信息都读取整个数组，高位在前，低位在后*/
    unsigned char buffer_dp_data_char[4] = {0x00, 0x00, 0x00, 0x00};

    /*通过这个函数对实时信息进行获取*/
    JudgeDP(dt_char, buffer_dp_data_char);

    /*计算长度*/
    int length_response_int = GetRealtimeMessageInquireResponseLength(dt_char);

    unsigned short length_response_short = IntToShort(length_response_int);

    /*把我算出来的参数放到结构体里*/
    RealtimeMessageInquireResponseAssemble(ti_response, control_response, length_response_short, length_response_int,
                                          err_response_char, dt_char, buffer_dp_data_char);


}

/*把我算好的实时应答帧，放到结构体里*/
void RealtimeMessageInquireResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short,
                                            unsigned int length_response_int, unsigned char err_response_char, unsigned char dt_char, unsigned char* buffer_dp_data_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    frame_struct_glob.data_struct.data[1] = dt_char;

    if (dt_char == STATE)
    {
       frame_struct_glob.data_struct.data[2] = buffer_dp_data_char[3];
    }
    else
    {
        for (int count = 0; count < length_response_int; ++count)
        {
            frame_struct_glob.data_struct.data[3 + count] = buffer_dp_data_char[count];
        }
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;

}


/*计算我控制命令应答帧里需要的参数*/
void ControlCommandResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    /*用来存发来的2字节cn值*/
    unsigned char buffer_cn_char[2] = {0x00, 0x00};

    /*cn 高两位*/
    buffer_cn_char[0] = buffer_recv_data_area[1];

    /*cn 低两位*/
    buffer_cn_char[1] = buffer_recv_data_area[2];

    unsigned short length_response_short = 0x0004;

    unsigned char err_response_char = ErrJudgeOfControlCommand();

    ControlCommandResponseAssemble(ti_response, control_response, length_response_short, buffer_cn_char, err_response_char);

}

/*把我算好的控制应答帧，放到结构体里*/
void ControlCommandResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short,
                                    unsigned char* buffer_cn_char, unsigned char err_response_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    frame_struct_glob.data_struct.data[1] = buffer_cn_char[0];

    frame_struct_glob.data_struct.data[2] = buffer_cn_char[1];

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;
}


/*计算我文件目录查询应答帧里的参数*/
void FileCatalogueInquireResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{

    unsigned short length_response_short = 0x0006;

    unsigned char buffer_did_char[4] = {0x00, 0x00, 0x00, 0x00};

    for (int count = 0; count < 4; ++count)
    {
        buffer_did_char[count] = buffer_recv_data_area[count + 1];
    }

    unsigned char err_response_char = ErrJudgeOfFileCatalogueInquire();

    FileCatalogueInquireResponseAssemble(ti_response, control_response, length_response_short,
                                        buffer_did_char, err_response_char);

}


/*把我算好的文件目录查询应答帧，放到结构体里*/
void FileCatalogueInquireResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short,
                                        unsigned char* buffer_did_char, unsigned char err_response_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    for (int count = 0; count < 4; ++count)
    {
        frame_struct_glob.data_struct.data[1 + count] = buffer_did_char[count];
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;
}                                           


/*计算我文件目录信息传输应答帧里的参数*/
void FileCatalogueTransmiteResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    unsigned short length_response_short = 0x0006;

    unsigned char buffer_did_char[4] = {0x00, 0x00, 0x00, 0x00};

    for (int count = 0; count < 4; ++count)
    {
        buffer_did_char[count] = buffer_recv_data_area[count + 1];
    }

    unsigned char err_response_char = ErrJudgeOfFileCatalogueTramiteResponse();

    FileCatalogueTransmiteResponseAssemble(ti_response, control_response, length_response_short,
                                        buffer_did_char, err_response_char);
}

/*把我算好的文件目录信息传输应答帧，放到结构体里*/
void FileCatalogueTransmiteResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short,
                                            unsigned char* buffer_did_char, unsigned char err_response_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    for (int count = 0; count < 4; ++count)
    {
        frame_struct_glob.data_struct.data[1 + count] = buffer_did_char[count];
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;
}


/*计算我写文件请求应答里的参数*/
void FileWriteInquireResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    unsigned short length_response_short = 0x0007 + buffer_recv_data_area[1];

    unsigned char err_response_char = ErrJudgeOfFileWriteInquireResponse();

    unsigned char fnl_response_char = buffer_recv_data_area[1];

    int length_fnl_int = CharToInit(buffer_recv_data_area[1]);

    unsigned char buffer_response_fn_char[length_fnl_int]; 

    for (int count = 0; count < length_fnl_int; ++count)
    {
        buffer_response_fn_char[count] = buffer_recv_data_area[2 + count];
    }

    unsigned char buffer_response_fid_char[4] = {0x00, 0x00, 0x00, 0x00};

    for (int count = 0; count < 4; ++count)
    {
        buffer_response_fid_char[count] = buffer_recv_data_area[2 + length_fnl_int + count];
    }

    FileWriteInquireResponseAssemble(ti_response, control_response, length_response_short, err_response_char,
                                     fnl_response_char, buffer_response_fn_char, buffer_response_fid_char, length_fnl_int);
}


/*把我算好的写文件请求应答帧，放到结构体里*/
void FileWriteInquireResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short, unsigned char err_response_char,
                                     unsigned char fnl_response_char, unsigned char* buffer_response_fn_char, unsigned char* buffer_response_fid_char, int length_fnl_int)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    frame_struct_glob.data_struct.data[1] = fnl_response_char;

    for (int count = 0; count < length_fnl_int; ++count)
    {
        frame_struct_glob.data_struct.data[2 + count] = buffer_response_fn_char[count];
    }

    for (int count = 0; count < 4; ++count)
    {
        frame_struct_glob.data_struct.data[2 + length_fnl_int + count] = buffer_response_fid_char[count];
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;
}


/*计算写文件数据确认帧*/
void WriteDataToFileResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    unsigned short length_response_short = 0x0006;

    unsigned char err_response_char = ErrJudgeOfWriteDataToFileResponse();

    unsigned char buffer_fid_char[4] = {0x00, 0x00, 0x00, 0x00};

    for (int count = 0; count < 4; ++count)
    {
        buffer_fid_char[count] = buffer_recv_data_area[1 + count];
    }

    WriteDataToFileResponseAssemble(ti_response, control_response, length_response_short, err_response_char,
                                    buffer_fid_char);
    
}

/*把我算好的参数放到写文件数据确认帧*/
void WriteDataToFileResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short,
                                     unsigned char err_response_char, unsigned char* buffer_fid_char)                                     
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    for (int count = 0; count < 4; ++count)
    {
        frame_struct_glob.data_struct.data[count + 1] = buffer_fid_char[count];
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;

}

/*计算写文件激活应答帧*/
void WriteDataActivateResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    unsigned char fnl_response_char = buffer_recv_data_area[1];

    int fnl_response_int = CharToInit(fnl_response_char);

    int length_response_int = 3 + fnl_response_int;

    unsigned short length_response_short = IntToShort(length_response_int);

    unsigned char err_response_char = ErrJudgeOfWriteDataActivateResponse();

    unsigned char buffer_fn_char[fnl_response_int];

    for (int count = 0; count < fnl_response_int; ++count)
    {
        buffer_fn_char[count] = buffer_recv_data_area[2 + count];
    }

    WriteDataActivateResponseAssemble(ti_response, control_response, length_response_short, 
                                      err_response_char, fnl_response_char, fnl_response_int, buffer_fn_char);


}


/*把算好的写文件激活应答帧放到结构体里*/
void WriteDataActivateResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short, 
                                      unsigned char err_response_char, unsigned char fnl_response_char, int fnl_response_int, unsigned char* buffer_fn_char)
{

    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    frame_struct_glob.data_struct.data[1] = fnl_response_char;

    for (int count = 0; count < fnl_response_int; ++count)
    {
        frame_struct_glob.data_struct.data[2 + count] = buffer_fn_char[count];
    }

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;
}


/*计算读文件请求应答帧里需要的参数*/
void ReadFileInquireResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    /*文件名长度 n*/
    unsigned char fnl_response_char = buffer_recv_data_area[1];

    /*算应答帧长度*/
    int fnl_response_int = CharToInit(fnl_response_char);

    int length_response_int = 13 + fnl_response_int;

    unsigned short length_response_short = IntToShort(length_response_int);

    /*fn：文件名*/
    unsigned char buffer_fn_char[fnl_response_int];

    for (int count = 0; count < fnl_response_int; ++count)
    {
        buffer_fn_char[count] = buffer_recv_data_area[2 + count];
    }

    /*算err*/
    unsigned char err_response_char = ErrJudgeOfReadFileInquireResponse();

    /*算标识序号fid*/
    int fid_response_int = JudgeFileID(buffer_fn_char, fnl_response_int);

    unsigned char buffer_fid_response_char[4] = {0x00, 0x00, 0x00, 0x00};

    // IntToChar(fid_response_int, buffer_fid_response_char);

    /*算fl 文件大小*/
    unsigned char buffer_fl_response_char[4] = {0x00, 0x00, 0x00, 0x00};

    int fl_response_int = GetFileSize(buffer_fn_char);

    // IntToChar(fl_response_int, buffer_fl_response_char);

    /*计算文件的校验码*/
    unsigned short fv_response_short = GetValidateOfFile(buffer_fn_char);


    ReadFileInquireResponseAssemble(ti_response, control_response, length_response_short, err_response_char,
                                    fnl_response_char, fnl_response_int, buffer_fn_char, fid_response_int, fl_response_int, fv_response_short);
}

/*把算好的读文件请求应答帧放到结构体里*/
void ReadFileInquireResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short, 
                                    unsigned char err_response_char, unsigned char fnl_response_char, int fnl_response_int, unsigned char* buffer_fn_char,
                                    int fid_response_int, int fl_response_int, unsigned short fv_response_short)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    frame_struct_glob.data_struct.data[1] = fnl_response_char;

    /*fn*/
    for (int count = 0; count < fnl_response_int; ++count)
    {
        frame_struct_glob.data_struct.data[2 + count] = buffer_fn_char[count];
    }

    /*fid*/
    frame_struct_glob.data_struct.data[2 + fnl_response_int] = ((fid_response_int >> 24) & 0xFF);

    frame_struct_glob.data_struct.data[2 + fnl_response_int + 1] = ((fid_response_int >> 16) & 0xFF);

    frame_struct_glob.data_struct.data[2 + fnl_response_int + 2] = ((fid_response_int >> 8) & 0xFF);

    frame_struct_glob.data_struct.data[2 + fnl_response_int + 3] = ((fid_response_int) & 0xFF);

    /*fl*/
    frame_struct_glob.data_struct.data[2 + fnl_response_int + 4] = ((fl_response_int >> 24) & 0xFF);

    frame_struct_glob.data_struct.data[2 + fnl_response_int + 5] = ((fl_response_int >> 16) & 0xFF);

    frame_struct_glob.data_struct.data[2 + fnl_response_int + 6] = ((fl_response_int >> 8) & 0xFF);

    frame_struct_glob.data_struct.data[2 + fnl_response_int + 7] = ((fl_response_int) & 0xFF);

    /*fv*/
    frame_struct_glob.data_struct.data[2 + fnl_response_int + 8] = ((fv_response_short >> 8) & 0xFF);

    frame_struct_glob.data_struct.data[2 + fnl_response_int + 9] = ((fv_response_short) & 0xFF);

    /*crc*/
    frame_struct_glob.crc = CRC;

    /*tail*/
    frame_struct_glob.tail = TAIL_SIGNAL;

}


/*计算读文件数据应答帧里需要的参数*/
void ReadFileDataResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    /*回传长度*/
    unsigned short length_response_short = 0x0006;

    /*fid*/
    unsigned char buffer_fid_response_char[4] = {0x00, 0x00, 0x00, 0x00};

    for (int count = 0; count < 4; ++count)
    {
        buffer_fid_response_char[count] = buffer_recv_data_area[1 + count];
    }

    unsigned char err_response_char = ErrJudgeOfReadFileDataResponse();

    ReadFileDataResponseAssemble(ti_response, control_response, length_response_short, err_response_char, buffer_fid_response_char);
}

/*把算好的读文件数据应答帧放到结构体里*/
void ReadFileDataResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short, 
                                  unsigned char err_response_char, unsigned char* buffer_fid_response_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    /*fid*/
    for (int count = 0; count < 4; ++count)
    {
        frame_struct_glob.data_struct.data[1 + count] = buffer_fid_response_char[count];
    }

    /*crc*/
    frame_struct_glob.crc = CRC;

    /*tail*/
    frame_struct_glob.tail = TAIL_SIGNAL;

}


/*计算对时应答帧里的参数*/
void SystemTimeConfirmResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    unsigned short length_response_short = 0x0002;

    unsigned char err_response_char = ErrJudgeOfSystemTimeConfirmResponse();

    SystemTimeConfirmResponseAssemble(ti_response, control_response, length_response_short, err_response_char);
}


/*将对时应答帧里的参数放到结构体里*/
void SystemTimeConfirmResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short, 
                                       unsigned char err_response_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    frame_struct_glob.crc = CRC;

    frame_struct_glob.tail = TAIL_SIGNAL;
}


/*计算时钟查询应答里的参数*/
void SystemTimeInquireResponse(unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_recv_data_area)
{
    unsigned short length_response_short = 0x0008;

    unsigned char err_response_char = ErrJudgeOfSystemTimeInquireResponse();

    unsigned char buffer_time_response_char[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    GetTimeClock(buffer_time_response_char);

    SystemTimeInquireResponseAssemble(ti_response, control_response, length_response_short, buffer_time_response_char, err_response_char);
}

void SystemTimeInquireResponseAssemble(unsigned char ti_response, unsigned char control_response, unsigned short length_response_short,
                                      unsigned char* buffer_time_response_char, unsigned char err_response_char)
{
    frame_struct_glob.head = HEAD_SIGNAL;

    frame_struct_glob.source_address = SA_RESPONSE;

    frame_struct_glob.destin_address = DA_RESPONSE;

    frame_struct_glob.control = control_response;

    frame_struct_glob.length = length_response_short;

    frame_struct_glob.data_struct.ti = ti_response;

    frame_struct_glob.data_struct.data[0] = err_response_char;

    for (int count = 0; count < 6; ++count)
    {
        frame_struct_glob.data_struct.data[1 + count] = buffer_time_response_char[count];
    }

    frame_struct_glob.crc = CRC;
        
    frame_struct_glob.tail = TAIL_SIGNAL;
}



/*测试用的,这是留出来的接口，用的是固定的22年06月22日10时30分15秒*/
void GetTimeClock(unsigned char* buffer_time_response_char)
{   
    buffer_time_response_char[0] = 0x16;

    buffer_time_response_char[1] = 0x06;

    buffer_time_response_char[2] = 0x16;

    buffer_time_response_char[3] = 0x0A;

    buffer_time_response_char[4] = 0x1E;

    buffer_time_response_char[5] = 0x0F;
}

/*根据文件名，计算文件内容校验码，测试时暂时全部返回0x0000*/
unsigned short GetValidateOfFile(unsigned char* buffer_fn_char)
{
    return 0x0000;
}


/*根据文件名，获取文件大小，这是个接口，测试时暂时全部返回0x000000ff（255）*/
int  GetFileSize(unsigned char* buffer_fn_char)
{
    return 255;
}



/*把int换成4位的char型数组*/
void IntToChar(int int_num, unsigned char* buffer_char)
{
    for (int count = 0; count < 4; ++count)
    {
        buffer_char[count] = (int_num) & 0xFF;

        int_num = int_num >> 8;
    }
}


/*判断文件对应的标识*/
int JudgeFileID(unsigned char* buffer_fn_char, int fnl_response_int)
{
    /*倒数第一个*/
    unsigned char fn_char_last = buffer_fn_char[fnl_response_int - 1];

    /*倒数第二个*/
    unsigned char fn_char_second_last = buffer_fn_char[fnl_response_int - 2];

    /*倒数第三个*/
    unsigned char fn_char_third_last = buffer_fn_char[fnl_response_int - 3];

    if ((fn_char_third_last == 'b') && (fn_char_second_last == 'i') && (fn_char_last == 'n'))
    {
        return 1;
    }
    if ((fn_char_third_last == 'p') && (fn_char_second_last == 'a') && (fn_char_last == 'r'))
    {
        return 2;
    }
    if ((fn_char_third_last == 'm') && (fn_char_second_last == 's') && (fn_char_last == 'g'))
    {
        return 3;
    }
    
    return 0;

}




/*实时信息查询应答里根据dt的类型，返回length标签的值*/
int GetRealtimeMessageInquireResponseLength(unsigned char dt_char)
{
    if (dt_char == STATE)
    {
        return (3 + 1);
    }
    else if(dt_char == CURRENT_VALUE || dt_char == FAULT_CURRENT || dt_char == ELECTRIC_FIELD_INTENSITY
           || dt_char == CAPACITOR_VOLTAGE || dt_char == BATTERY_VOLTAGE || dt_char == TEMPERATURE ||
           dt_char == TERMINAL_MAIN_BATTERY_VOLTAGE || dt_char == TERMINAL_SOLAR_CT_VOLTAGE
           || dt_char == TERMINAL_TEMPERATURE || dt_char == TERMINAL_SIGNAL_STRENGTH)
    {
        return (3 + 4);
    }

}


/*实时信息应答里，获取dp的值*/
void JudgeDP(unsigned char dt_char, unsigned char* buffer_dp_data_char)
{
    switch (dt_char)
    {
    /*在state下，只用处理数组里的最后一个字节*/
    case STATE:
    {
        GetState(buffer_dp_data_char);
        break; 
    }
    case CURRENT_VALUE:
    {
        GetCurrentValue(buffer_dp_data_char);
        break;
    }
    case FAULT_CURRENT:
    {
        GetFaultCurrent(buffer_dp_data_char);
        break;
    }
    case ELECTRIC_FIELD_INTENSITY:
    {
        GetElectricFieldIntensity(buffer_dp_data_char);
        break;
    }
    case CAPACITOR_VOLTAGE:
    {
        GetCapacitorVol(buffer_dp_data_char);
        break;
    }
    case BATTERY_VOLTAGE:
    {
        GetBatteryVol(buffer_dp_data_char);
        break;
    }
    case TEMPERATURE:
    {
        GetTemperature(buffer_dp_data_char);
        break;
    }
    case TERMINAL_MAIN_BATTERY_VOLTAGE:
    {
        GetTerminalMainBatteryVol(buffer_dp_data_char);
        break;
    }
    case TERMINAL_SOLAR_CT_VOLTAGE:
    {
        GetTerminalSolarVol(buffer_dp_data_char);
        break;
    }
    case TERMINAL_TEMPERATURE:
    {
        GetTerminalTempture(buffer_dp_data_char);
        break;
    }
    case TERMINAL_SIGNAL_STRENGTH:
    {
        GetTerminalSignalStrength(buffer_dp_data_char);
        break;
    }      
    default:
    {
        break;
    }
        
    }
}


/*从这开始都是实时信息的dp值的获取接口，现在都做成了默认的0xff*/
void GetState(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetCurrentValue(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetFaultCurrent(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetElectricFieldIntensity(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetCapacitorVol(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetBatteryVol(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetTemperature(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetTerminalMainBatteryVol(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetTerminalSolarVol(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetTerminalTempture(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}

void GetTerminalSignalStrength(unsigned char* dp_data_char)
{
    memset(dp_data_char, 0xff, sizeof(dp_data_char));
}
/*上面实时信息的获取接口，现在都做成了默认的0xff*/


/*判断实时信息查询的err*/
unsigned char ErrJudgeOfRealtimeMessageInquire()
{
    return ERR_SUCCESS;
}

/*判断控制命令的err*/
unsigned char ErrJudgeOfControlCommand()
{
    return ERR_SUCCESS;
}

/*判断文件目录查询的err*/
unsigned char ErrJudgeOfFileCatalogueInquire()
{
    return ERR_SUCCESS;
}

unsigned char ErrJudgeOfFileCatalogueTramiteResponse()
{
    return ERR_SUCCESS;
}

/*判断文件请求应答的err*/
unsigned char ErrJudgeOfFileWriteInquireResponse()
{
    return ERR_SUCCESS;
}

/*写文件数据应答的err*/
unsigned char ErrJudgeOfWriteDataToFileResponse()
{
    return ERR_SUCCESS;
}

unsigned char ErrJudgeOfWriteDataActivateResponse()
{
    return ERR_SUCCESS;
}

unsigned char ErrJudgeOfReadFileInquireResponse()
{
    return ERR_SUCCESS;
}

unsigned char ErrJudgeOfReadFileDataResponse()
{
    return ERR_SUCCESS;
}

unsigned char ErrJudgeOfSystemTimeConfirmResponse()
{
    return ERR_SUCCESS;
}

unsigned char ErrJudgeOfSystemTimeInquireResponse()
{
    return ERR_SUCCESS;
}

/***********************************************
    *函数名：  GetPIDParam
    *功能说明: 把接收报文中的pid单独拎出来，放到buffer_pid_char里
    *参数说明： buffer_p_data_recv --- 接收报文中，包含有pid、pl、pv的部分
              buffer_pid_char --- 存放char型pid的数组
              pn_int --- pid数量的int型
    *返回说明： void
************************************************/
void GetPIDParam(unsigned char* buffer_p_data_recv, unsigned char* buffer_pid_char, int pn_int)
{
    /*获得pid1的数据长度*/
    unsigned char m_length = buffer_p_data_recv[2];

    /*pid1编号的高位*/
    buffer_pid_char[0] = buffer_p_data_recv[0];

    /*pid1编号的低位*/
    buffer_pid_char[1] = buffer_p_data_recv[1];

    for (int count = 1; count < pn_int; ++count)
    {
        buffer_pid_char[2 * count] = buffer_p_data_recv[3 * count + m_length];

        buffer_pid_char[2 * count + 1] = buffer_p_data_recv[3 * count + m_length + 1];

        int m_temp = buffer_p_data_recv[3 * count + m_length + 2];

        m_length += m_temp;
    }

}




/***********************************************
    *函数名：  ErrJudgeOfParamSet
    *功能说明: 判断参数设置应答报文里的err值
              这里暂时做成只能回传0x00
    *参数说明： 
    *返回说明： void
************************************************/
unsigned char ErrJudgeOfParamSet()
{
    return ERR_SUCCESS;
}


/***********************************************
    *函数名：  ErrJudgeOfParamInquire
    *功能说明: 判断参数查询应答报文里的err值
              这里暂时做成只能回传0x00
    *参数说明： 
    *返回说明： void
************************************************/
unsigned char ErrJudgeOfParamInquire()
{
    return ERR_SUCCESS;
}



/***********************************************
    *函数名： SetBufferPV
    *功能说明：用来放置我查询到的参数，这个是个接口函数，给张总开放出来
    *参数说明： buffer_pv_char --- 存放我所有参数值的数组
              length_buffer_pv_char  --- 这个数组有多长
    *返回说明： void
************************************************/
void SetBufferPV(unsigned char* buffer_pv_char, int length_buffer_pv_char)
{
    for (int count = 0; count < length_buffer_pv_char; ++count)
    {
        buffer_pv_char[count] = 0xFF;
    }
}


/***********************************************
    *函数名： SumBufferPL
    *功能说明：使用pl_buffer_int里的数据，算出我整个pv的char型数组有多长
    *参数说明： buffer_pl_int --- pl 内容的int型数组
               pn_int --- buffer_pl_int数组的长度，pn_int为pid个数，有多少pid，
               才有对应的pid数值长度
    *返回说明： void
************************************************/
int SumBufferPL(int* buffer_pl_int, int pn_int)
{
    int sum = 0;

    for (int count = 0; count < pn_int; ++count)
    {
        sum += buffer_pl_int[count];
    }

    return sum;
}


/***********************************************
    *函数名： CalcuParamInquireResponseLength
    *功能说明：计算我参数查询应答报文的长度
             L = 0X0003 + (3 + M1) + (3 + M2)......+ (3 + Mn)
    *参数说明： buffer_rev_data_area --- 接收报文的数据区
              length_serv      --- 接收报完的length位的填充值（int型）
    *返回说明： int --- 返回pn的int型
************************************************/
unsigned short CalcuParamInquireResponseLength(unsigned char* buffer_rev_data_area, int length_serv, 
                                               unsigned char* buffer_pid_char, int pn_int, int* buffer_pl_int, unsigned char* buffer_pl_char)
{

    printf("\n");
    printf("It's in the CalcuParamInquireResponseLength\n");
    
    /*用来存放转换成int型后的pid，为了后续判断用哪一个参数编号的时候更方便*/
    int buffer_pid_int[pn_int];
    printf("pn_int = %d\n", pn_int);


    printf("length_serv = %d\n", length_serv);
    /*把所有buffer_recv_data_area里的pid相关的，放到buffer_pid_char里面来*/
    for (int count = 2; count < length_serv; ++count)
    {
        buffer_pid_char[count - 2] = buffer_rev_data_area[count];
        printf("buffer_pid_char[%d] = %02hX\n", count - 2, buffer_pid_char[count - 2]);
    }

    
    /*把pid里面每两个char，组成一个我认识的int*/
    for (int count = 0; count < (pn_int * 2); count = count + 2)
    {   printf("count = %d\n", count);
        printf("buffer_pid_char[%d] = %02hX\n", count, buffer_pid_char[count]);
        printf("buffer_pid_char[%d] = %02hX\n", count + 1, buffer_pid_char[count + 1]);
        buffer_pid_int[count / 2] = LengthAssemble(buffer_pid_char[count], buffer_pid_char[count + 1]);
        printf("buffer_pid_int[%d] = %d\n", (count / 2), buffer_pid_int[count / 2]);
    }
    printf("\n");

    /*经过这个函数处理之后，buffer_pl_int里面装的就是我pid一一对应的参数长度*/
    GetPLBuffer(buffer_pid_int, buffer_pl_int, pn_int);

    /*把我的参数长度转换成char类型*/
    GetPLBufferChar(buffer_pl_int, buffer_pl_char, pn_int);

    /*获取长度的int型表示*/
    int length_int = CalcuParamInquireResponseLengthInsideFunc(buffer_pl_int, pn_int);

    /*把长度转换成unsigned short 填充进buffer里*/
    unsigned short length_short = IntToShort(length_int);

    return length_short;

}


/***********************************************
    *函数名： GetPLBufferChar
    *功能说明：把我的参数长度pl转换成char类型
    *参数说明： buffer_pl_int --- pl的int型数组
              buffer_pl_char --- pl的char型数组
              pn_int --- pl_int型数组的长度
    *返回说明： void
************************************************/
void GetPLBufferChar(int* buffer_pl_int, unsigned char* buffer_pl_char, int pn_int)
{
    for (int count = 0; count < pn_int; ++count)
    {
        buffer_pl_char[count] = buffer_pl_int[count] & 0xFF;
    }

}


/***********************************************
    *函数名： IntToShort
    *功能说明：将int转换成unsigned short
              在int <= 65534情况下该方法才有效
    *参数说明： length_int --- 需要转换的int值
    *返回说明： unsig short --- 转换结果
************************************************/
unsigned short IntToShort(int length_int)
{
    unsigned short length_short = 0x0000;

    unsigned char length_short_high = (length_int >> 8) & 0xFF;

    unsigned char length_short_low = (length_int & 0xFF);

    length_short = length_short | length_short_high;

    length_short = length_short << 8;

    length_short = length_short | length_short_low;

    return length_short;

}


/***********************************************
    *函数名： CalcuParamInquireResponseLengthInsideFunc
    *功能说明：得到参数查询应答帧length位的int值
    *参数说明： buffer_pl_int --- pl的int型buffer
              pn_int --- 两个buffer的长度
    *返回说明： int --- 计算得到的应答帧length位的int值
************************************************/
int CalcuParamInquireResponseLengthInsideFunc(int* buffer_pl_int, int pn_int)
{
    /*这里的3已经把ti、err、pn已经算进去了*/
    int length = 3;

    for (int count = 0; count < pn_int; ++count)
    {
        length += (3 + buffer_pl_int[count]);
    }

    return length;
}


/***********************************************
    *函数名： GetPLBuffer
    *功能说明：得到一个pl的int型的buffer
    *参数说明： buffer_pid_int --- pid的int型buffer
              buffer_pl_int  --- pl的int型buffer
              pn_int --- 两个buffer的长度
    *返回说明： void
************************************************/
void GetPLBuffer(int* buffer_pid_int, int* buffer_pl_int, int pn_int)
{
    for (int count = 0; count < pn_int; ++count)
    {
        int pl_int = GetPLInt(buffer_pid_int[count]);

        buffer_pl_int[count] = pl_int;
    }
}



/***********************************************
    *函数名： GetPLInt
    *功能说明: 根据pid号获取它对应的参数值长度pl
    *参数说明： buffer_pid_int --- pid号
    *返回说明： int --- 对应的参数值长度pl
************************************************/
int GetPLInt(int buffer_pid_int)
{
    int pl_int = 0;

    switch (buffer_pid_int)
    {
        case SHOOT_MODE_INT:
        {
            pl_int = SHOOT_MODE_PARAM_LENGTH_INT;
            break;
        }
        case PHOTO_SIZE_INT:
        {
            pl_int = PHOTO_SIZE_PARAM_LENGTH_INT;
            break;
        }
        case VIDEO_SIZE_INT:
        {
            pl_int = VIDEO_SIZE_PARAM_LENGTH_INT;
            break;
        }
        case VIDEO_DURATION_INT:
        {
            pl_int = VIDEO_DURATION_PARAM_LENGTH_INT;
            break;
        }
        case PIR_SENSITIVITY_INT:
        {
            pl_int = PIR_SENSITIVITY_PARAM_LENGTH_INT;
            break;
        }
        case TRIGGER_TIME_INTERVAL_INT:
        {
            pl_int = TRIGGER_TIME_INTERVAL_PARAM_LENGTH_INT;
            break;
        }
        case CONSECUTIVE_PHOTO_INT:
        {
            pl_int = CONSECUTIVE_PHOTO_PARAM_LENGTH_INT;
            break;
        }
        case TIME_PHOTO_ALARM_CLOCK_INT:
        {
            pl_int = TIME_PHOTO_ALARM_CLOCK_PARAM_LENGTH_INT;
            break;
        }
        case BURST_MODE_INT:
        {
            pl_int = BURST_MODE_PARAM_LENGTH_INT;
            break;
        }
        case LONGITUDE_INT:
        {
            pl_int = LONGITUDE_PARAM_LENGTH_INT;
            break;
        }
        case LATITUDE_INT:
        {
            pl_int = LATITUDE_PARAM_LENGTH_INT;
            break;
        }
        case SOFTWARE_VERSION_INT:
        {
            pl_int = SOFTWARE_VERSION_PARAM_LENGTH_INT;
            break;
        }
        case HARDWARE_VERSION_INT:
        {
            pl_int = HARDWARE_VERSION_PARAM_LENGTH_INT;
            break;
        }
        default:
        {
            pl_int = 99;
            break;
        }
        
    }

    return pl_int;
}




/***********************************************
    *函数名： CharToInit
    *功能说明：将收到的报文中的pn转换成int值
    *参数说明： pn_char --- char型的pn值         
    *返回说明： int --- 返回pn的int型
************************************************/
int CharToInit(unsigned char pn_char)
{
    int count = 0;

    int pn_int = 0;

    unsigned char a[4] = {0x00, 0x00, 0x00, pn_char};

    for(count; count < 4; ++count)
    {
        pn_int += (a[count] << ((3 - count) * 8));
    }

    printf("char to  = %d\n", pn_int);

    return pn_int;
}


/***********************************************
    *函数名： LengthAssemble
    *功能说明：判断传进来的缓冲区length这个变量是多少
    *参数说明： length_high --- length的高位
               length_low --- length的低位
    *返回说明： serv_socket --- 创建完成的服务端套接字
************************************************/
int LengthAssemble(unsigned char length_high, unsigned char length_low)
{
    int count = 0;

    int length = 0;

    unsigned char a[4] = {0x00, 0x00,length_high, length_low};

    for(count; count < 4; ++count)
    {
        length += (a[count] << ((3 - count) * 8));
    }

    sleep(2);
    printf("frame length = %d\n", length);

    return length;
}


/***********************************************
    *函数名： FrameInit
    *功能说明: 初始化我全局变量的整个结构体
    *参数说明： void
    *返回说明： void
************************************************/
void FrameInit()
{
    frame_struct_glob.head = 0x00;

    frame_struct_glob.source_address = 0x0000;

    frame_struct_glob.destin_address = 0x0000;

    frame_struct_glob.control = 0x00;

    frame_struct_glob.length = 0x0000;

    frame_struct_glob.data_struct.ti = 0x00;

    memset(frame_struct_glob.data_struct.data, 0xff, sizeof(frame_struct_glob.data_struct.data));

    frame_struct_glob.crc = 0x0000;

    frame_struct_glob.tail = 0x00;

}

void BufferInit()
{
    int count = 0;

    for (count; count < MAXlENGTH; ++count)
    {
        buffer_response[count] = 0xff;
    }
}


/***********************************************
    *函数名： FrameTx
    *功能说明: 发送报文帧
    *参数说明： socket_temp --- 发给谁
              buffer_temp --- 发什么
    *返回说明： void
************************************************/
void FrameTx(int socket_temp, unsigned char buffer_temp)
{
    write(socket_temp, buffer_temp, sizeof(buffer_temp));
}


/***********************************************
    *函数名： FrameRecv
    *功能说明: 接收报文帧
    *参数说明： socket_temp --- 谁接收
              buffer_temp --- 放在哪
    *返回说明： void
************************************************/
void FrameRecv(int socket_temp, unsigned char buffer_temp)
{
    read(socket_temp, buffer_temp, sizeof(buffer_temp) - 1);
}


/***********************************************
    *函数名： SocketClose
    *功能说明: 关闭客户端和服务端套接字
    *参数说明： void
    *返回说明： void
************************************************/
void SocketClose(int socket)
{
    close(socket);
    printf("socket has been closed\n");
}


/***********************************************
    *函数名：  PthreadCreate
    *功能说明: 创建一个函数
    *参数说明： void* Functionname --- 输入一个指针函数，符合线程创建的条件
    *返回说明： void
************************************************/
void PthreadCreate(void* FunctionName)
{
    pthread_t thread_temp;

    if (pthread_create(&thread_temp, NULL, (void*)FunctionName, NULL) != 0)
    {
        printf("new Thread create fail\n");
    }
    else
    {
        printf("new Thread has been create\n");
    }
   
}


/***********************************************
    *函数名： ErrJudgeOfHeartTest
    *功能说明：判断设备（服务端）是否收到心跳测试
    *参数说明： socket_serv --- 表示服务端套接字
    *返回说明： err --- 表示是是否收到心跳的应答值
************************************************/
unsigned char ErrJudgeOfHeartTest(int socket_serv)
{
    unsigned char err = 0xff;

	unsigned char buffer[17];

    read(socket_serv, buffer, sizeof(buffer) - 1);

    if (buffer[3] == 0x01)
    {
        /*表示成功接收*/
        err = 0x00;
    }
    else
    {
        /*接收失败，但是不会发出去*/
        err = 0xff;
    }

    return err;
    
}



void BufferAssemble()
{
    BufferInit();

    buffer_response[0] = frame_struct_glob.head;

    buffer_response[1] = ((frame_struct_glob.source_address >> 24) & 0xFF);

    buffer_response[2] = ((frame_struct_glob.source_address >> 16) & 0xFF);

    buffer_response[3] = ((frame_struct_glob.source_address >> 8) & 0xFF);

    buffer_response[4] = frame_struct_glob.source_address & 0xFF;

    buffer_response[5] = ((frame_struct_glob.destin_address >> 24) & 0xFF);

    buffer_response[6] = ((frame_struct_glob.destin_address >> 16) & 0xFF);

    buffer_response[7] = ((frame_struct_glob.destin_address >> 8) & 0xFF);

    buffer_response[8] = frame_struct_glob.destin_address & 0xFF;

    buffer_response[9] = frame_struct_glob.control;

    printf("\n");

    printf("It's in the buffer response, control = %02hX\n", frame_struct_glob.control);

    buffer_response[10] = ((frame_struct_glob.length >> 8) & 0xFF);

    buffer_response[11] = frame_struct_glob.length & 0xFF;

    buffer_response[12] = frame_struct_glob.data_struct.ti;

    for (int count = 0; count < length_response - 1; ++count)
    {
        buffer_response[13 + count] = frame_struct_glob.data_struct.data[count];
    }


    buffer_response[12 + length_response] = ((CRC >> 8) & 0XFF);

    buffer_response[12 + length_response + 1] = (CRC & 0XFF);

    buffer_response[12 + length_response + 2] = TAIL_SIGNAL;

    printf("the tail index number = %d\n", 12 + length_response + 2);

}


