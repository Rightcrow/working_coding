#include "tcp_thread.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "file_save_assign.h"
#include <sys/select.h>
#include <sys/time.h>
#include "global_value.h"
#include "ini.h"


#define RV_BUF_SIZE 1024
#define DEFAULT_PORT 54321

char    buffer[1024];
char    *video_name;

void ServerParamSolve(int fd, char data[], int length)
{
    printf("ServerParamSolve:%s\n", data);

    if (length==11)
    {
        if (data[0]=='x' && data[5]=='j' && data[7]=='m' && data[9]=='l')
        {
            char data_send[] = "xiangjimulusend";
            send(fd, data_send, strlen(data_send), 0);

            printf("xiangjimulusend:%s;  size:%d\n", data_send,(int)strlen(data_send));
        }
    }

    if (length==14)
    {
        if (data[0]=='x' && data[5]=='j' && data[7]=='m' && data[9]=='l' && data[11]=='r' && data[12]=='c' && data[13]=='v')
        {
            printf("xiangjimulurcv:%s\n", data);

            memset(buffer, '\0', 1024);
            int list_fd = open("./list.dat", O_RDONLY);
            if (list_fd > -1)
            {
                while(read(list_fd, buffer, 1024) > 0)
                {
                    send(fd, buffer, strlen(buffer), 0); 
                    printf("xiangjimulurcv_data:%s\n", buffer);
                    memset(buffer, '\0', 1024);
                    
                }
                
                strcpy(buffer, "afz");
                send(fd, buffer, strlen(buffer), 0);
                memset(buffer, '\0', 1024);
                
            }
            else 
            {
                printf("xiangjimulurcv:list.dat open failed!");
            }
        }
    }

    if (length > 14 && length != 17)
    {
        if (data[0]=='x' && data[5]=='j' && data[7]=='s' && data[10]=='p')
        {
            char *name = (char*)malloc(length-14+1);
            memset(name, '\0', length-14+1);
            memcpy(name, data+14, length-14);
            video_name = name;

            int video_fd = open(video_name, O_RDONLY);
            if (video_fd > -1)
            {
                send(fd, data, strlen(data), 0);
            }
            
            close(video_fd);

            printf("xiangjishiping:%s\n", data);
        }
    }

    if (length == 17)
    {
        if (data[0]=='x' && data[5]=='j' && data[7]=='s' && data[10]=='p' && data[14]=='r' && data[15]=='c' && data[16]=='v')
        {
            memset(buffer, '\0', 1024);
            int video_fd = open(video_name, O_RDONLY);
            if (video_fd > -1)
            {
                while(read(video_fd, buffer, 1024) > 0)
                {
                    send(fd, buffer, 1024, 0); 
                    memset(buffer, '\0', 1024);
                }
                
                strcpy(buffer, "afz");
                send(fd, buffer, strlen(buffer), 0);
                memset(buffer, '\0', 1024);

                close(video_fd);
                
            }
            else 
            {
                printf("xiangjishipingrcv:video_fd open failed!");
            }

            
            free(video_name);
            video_name = NULL;

            printf("xiangjishipingr was send!");
        }
    }

    if (length == 17)
    {
        if (data[0]=='x' && data[5]=='j' && data[7]=='s' && data[10]=='p' && data[14]=='c' && data[15]=='t' && data[16]=='l')
        {
            video_ctl = 1;
            memset(buffer, '\0', 1024);
            char filename[60];
            memset(filename, '\0', 1024);

            while (record_ctl_file_ok==0);
            record_ctl_file_ok = 0;
            GetLastFilename(filename);
            snprintf(buffer, 60, "xiangjishiping%s", filename);
            send(fd, buffer, strlen(buffer), 0);
            memset(buffer, '\0', 1024);

            int recv_len = recv(fd, buffer, RV_BUF_SIZE-1, 0);
            if (recv_len == -1 || recv_len == 0)
            {
                printf("recv timeout protocol\n");
                return;
            }

            if (buffer[0]=='x' && buffer[5]=='j' && buffer[7]=='s' && buffer[10]=='p' && buffer[14]=='r' && buffer[15]=='c' && buffer[16]=='v')
            {
                memset(buffer, '\0', 1024);
                int video_fd = open(filename, O_RDONLY);
                if (video_fd > -1)
                {
                    while(read(video_fd, buffer, 1024) > 0)
                    {
                        send(fd, buffer, 1024, 0); 
                        memset(buffer, '\0', 1024);
                    }
                    
                    strcpy(buffer, "afz");
                    send(fd, buffer, strlen(buffer), 0);
                    memset(buffer, '\0', 1024);

                    close(video_fd);
                    
                }
                else 
                {
                    printf("xiangjishipingrcv:video_fd open failed!");
                }

                printf("xiangjishipingr was send!");
            }

        }
    }

    if (length == 17)
    {
        if (data[0]=='x' && data[5]=='j' && data[7]=='p' && data[10]=='z' && data[14]=='c' && data[15]=='t' && data[16]=='l')
        {
            photo_ctl = 1;
            memset(buffer, '\0', 1024);
            char filename[60];
            memset(filename, '\0', 1024);

            while (record_ctl_file_ok==0);
            record_ctl_file_ok = 0;
            GetLastFilename(filename);
            snprintf(buffer, 60, "xiangjipaizhao%s", filename);
            send(fd, buffer, strlen(buffer), 0);
            memset(buffer, '\0', 1024);

            int recv_len = recv(fd, buffer, RV_BUF_SIZE-1, 0);
            if (recv_len == -1 || recv_len == 0)
            {
                printf("recv timeout protocol\n");
                return;
            }

            if (buffer[0]=='x' && buffer[5]=='j' && buffer[7]=='s' && buffer[10]=='p' && buffer[14]=='r' && buffer[15]=='c' && buffer[16]=='v')
            {
                memset(buffer, '\0', 1024);
                int video_fd = open(filename, O_RDONLY);
                if (video_fd > -1)
                {
                    while(read(video_fd, buffer, 1024) > 0)
                    {
                        send(fd, buffer, 1024, 0); 
                        memset(buffer, '\0', 1024);
                    }
                    
                    strcpy(buffer, "afz");
                    send(fd, buffer, strlen(buffer), 0);
                    memset(buffer, '\0', 1024);

                    close(video_fd);
                    
                }
                else 
                {
                    printf("xiangjipaizhaorcv:video_fd open failed!");
                }

                printf("xiangjishipingr was send!");
            }

        }
    }

    /*
    "mode		= 2 ;\n"
	"picturesize = 1 ;\n"
	"videosize 	= 1 ;\n"
	"pictimes	= 1 ;\n"
	"videotimes	= 5 ;\n"
	"gaptimes	= 10;\n"
	"sensitivity= 3 ;\n"
	"timestamp 	= 1 ;\n"
    */
    if (length >= 14)
    {
        if (buffer[0]=='x' && buffer[5]=='j' && buffer[7]=='p' && buffer[11]=='m' && buffer[12]=='m' && buffer[13]=='e')
        {
            char *value = buffer + 14;
            save_CamPIR_int("mode", value);
        }

        if (buffer[0]=='x' && buffer[5]=='j' && buffer[7]=='p' && buffer[11]=='m' && buffer[12]=='p' && buffer[13]=='s')
        {
            char *value = buffer + 14;
            save_CamPIR_int("picturetimes", value);
        }

        if (buffer[0]=='x' && buffer[5]=='j' && buffer[7]=='p' && buffer[11]=='m' && buffer[12]=='s' && buffer[13]=='n')
        {
            char *value = buffer + 14;
            save_CamPIR_int("solution", value);
        }

        if (buffer[0]=='x' && buffer[5]=='j' && buffer[7]=='p' && buffer[11]=='m' && buffer[12]=='v' && buffer[13]=='s')
        {
            char *value = buffer + 14;
            save_CamPIR_int("videotimes", value);
        }

        if (buffer[0]=='x' && buffer[5]=='j' && buffer[7]=='p' && buffer[11]=='m' && buffer[12]=='s' && buffer[13]=='y')
        {
            char *value = buffer + 14;
            save_CamPIR_int("sensitivity", value);
        }
    }
}

void TCP_Data_Send(int fd)
{
    
    /*get a addr*/
    //
    /*end*/
    char a[] = "./a.dat";
    int in;
    in = open(a,O_RDWR);
    char buff[1024];
    memset(buff, '\0',1024);
    
    while(1)
    {
        int ret=read(in, buff, 1024);
        if (ret==0)
            break;
        send(fd, buff, ret,0);
    }
    char buf[3] = "867";
    send(fd, buf, 3,0);
}

char protocol_cache[1024] = {0};
int cache_size = 0;
int cache_flag = 0;
void ProtocolReceive(char* buff)
{
    printf("867\n");
    printf("size:%d\n", strlen(buff));
    int first = 0;
    int size = strlen(buff);
    for(unsigned int i=0; i<strlen(buff)-2; i++)
    {
        if (cache_flag == 1)
        {
            if (buff[i]=='8' && buff[i+1]=='6' && buff[i+2]=='7')
            {
                char tmp[cache_size+i+2+1+1];
                memset(tmp, 0, cache_size+i+2+1+1);
                memcpy(tmp, protocol_cache, cache_size);
                memcpy(tmp+cache_size, buff, i+2+1);

                int first_=0;
                for (unsigned int j=0; j<strlen(tmp)-2; j++)
                {
                    
                    if (tmp[j]=='8' && tmp[j+1]=='6' && tmp[j+2]=='7')
                    {
                        char tmp2[j+2+1-first_+1];
                        memset(tmp2, 0, j+2+1-first_+1);
                        memcpy(tmp2, tmp+first_, j+2+1-first_);
                        printf("xbuff_size:%d\n", (int)strlen(tmp2));
                        printf("xbuff_times:%s\n",tmp);
                        printf("xframe:%s\n", tmp2);
                        first_ = j+2+1;
                    }
                }

                first = i+2+1;
                cache_flag = 0;
            }
        }
        else if (buff[i]=='8' && buff[i+1]=='6' && buff[i+2]=='7')
        {
            char tmp_ok[i+2+1-first+1];
            memset(tmp_ok, 0, i+2+1-first+1);
            memcpy(tmp_ok, buff+first, i+2+1-first);
            printf("buff_size:%d\n", i+2+1-first);
            printf("buff_times:%s\n",buff+first);
            first = i+2+1;
            printf("frame:%s\n", tmp_ok);
        }
        
    }
    if (buff[size-3]!='8'&& buff[size-2]!='6' && buff[size-1]!='7')
    {
        memcpy(protocol_cache, buff+first, size-first);
        cache_flag = 1;
        cache_size = size-first;
    }
}

void TCP_Main(void)
{
    char rv_buffer[RV_BUF_SIZE];
    int socket_fd;
    struct sockaddr_in servaddr, clientaddr;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(DEFAULT_PORT);
    bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(socket_fd, 1);

    fd_set set;
    FD_ZERO(&set);
    FD_SET(socket_fd, &set);
    int rv;
    struct timeval select_timeout, recv_timeout;
    select_timeout.tv_sec = 60;
    select_timeout.tv_usec = 0;
    recv_timeout.tv_sec = 60;
    recv_timeout.tv_usec = 0;

    while(1)
    {
        rv = select(socket_fd + 1, &set, NULL, NULL, &select_timeout);
        if (rv == -1)
        {
            close(socket_fd);
            perror("select\n");
            return;
        }
        else if(rv == 0)
        {
            close(socket_fd);
            printf("timeout occurred (300 second) \n");
            return;
        }
        else
        {
            socklen_t clientaddr_len = sizeof(clientaddr);
            int connect_fd = accept(socket_fd, (struct sockaddr*)&clientaddr, &clientaddr_len);
            setsockopt(connect_fd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));
            
            memset(buffer, '\0', 1024);

            while(1)
            {
                memset(rv_buffer,'\0', RV_BUF_SIZE);
                int recv_len = recv(connect_fd, rv_buffer, RV_BUF_SIZE-1, 0);
                if (recv_len == -1 || recv_len == 0)
                {
                    printf("recv timeout protocol\n");
                    close(connect_fd);
                    break;
                }

                ServerParamSolve(connect_fd, rv_buffer, recv_len);
                /*
                rv_buffer[recv_len] = '\0';
                ProtocolReceive(rv_buffer);
                printf("protocol:%s\n", rv_buffer);
                //Protocol_Analysis(rv_buff);
                //Live_Tick_Send();
                int live_tick = recv(connect_fd, rv_buffer, RV_BUF_SIZE-1, 0);
                if (live_tick == -1 || live_tick == 0)
                {
                    printf("recv timeout live tick\n");
                    close(connect_fd);
                    break;
                }
                rv_buffer[recv_len] = '\0';
                printf("live_tick:%s\n", rv_buffer);
                */
            }
        }
    }

}


/*
int main(void)
{
    TCP_Main();
    
    return 1;
}
*/
