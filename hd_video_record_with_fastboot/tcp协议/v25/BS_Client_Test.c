#include "lib/BS_FunInterface.h"
#include "lib/BS_Client.h"

// extern int length_response;

/*这个单纯是用来测试的*/
void main()
{
    /*创建套接字*/
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /*用于测试时的打印*/
    int count = 0;

    int length = 0;

    /*向服务器（特定的IP和端口）发送请求*/
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);

    connect(sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr));

    /*发送缓存*/
    unsigned char buffer_temp[MAXlENGTH] = {0x00};

    /*接收缓存*/
    unsigned char buffer_response[MAXlENGTH] = {0x00};

    /*组报文帧*/
    // ClientHeartTest();
    // ClientParamInquire();
    // ClientParamSet();
    // ClientRealTimeMessageInquire();
    // ClientControlCommand();
    // ClientFileCatalogueInquire();
    // ClientFileCatalogueTransmite();
    // ClientFileWriteInquire();
    // ClientWriteDataToFile();
    // ClientWriteDataActivate();
    // ClientReadFileInquire();
    // ClientReadFileData();
    // ClientSystemTimeConfirm();
    ClientSystemTimeInquire();

    /*获取报文长度*/
    length = GetFrameLength();

    /*把报文帧放到buffer缓存里*/
    PutFrameToClientBuffer(buffer_temp, length);

    /*把要发的东西打印出来，15 + length = 报文帧的总长度*/
    for (count; count < (15 + length); ++count)
    {
        printf("client buffer[%d] = %02hX\n", count, buffer_temp[count]);
    }

    /*发送报文*/
    write(sock, buffer_temp, sizeof(buffer_temp));

    printf("has sent to sock\n");

    printf("\n");

    /*开始准备接收服务端处理之后回传的报文*/
    printf("ready to recevice response\n");

    /*这里获取response报文的length*/
    int length_response_client = 0;

    read(sock, buffer_response, sizeof(buffer_response));

    length_response_client = LengthAssemble(buffer_response[10], buffer_response[11]);

    printf("in the client, length_response_client = %d\n", length_response_client);
   
    /*把我收到的response打印出来*/
    for (count = 0; count < (15 + length_response_client); ++count)
    {
        printf("response buffer[%d] = %02hX\n", count, buffer_response[count]);
    }

    
    // //关闭套接字
    // close(sock);

    exit(0);

}


