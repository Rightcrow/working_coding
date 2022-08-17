#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/statfs.h>
#include "file_save_assign.h"
#include <time.h>

time_t now;
struct tm *time_now;

void FileListDelet(void)
{
    FILE *fd, *fd_copy;
    char buff[1024] = {0};
    char remove_name[40] = {0};
    char remove_name_data[60] = {0};
    fd = fopen("./list.dat", "a+" );
    fd_copy = fopen("./list_copy.dat", "a+");
    int save_list_count = 0;

    GetLastFilename(remove_name);
    printf("\n############\nbuff:%s\nremovename:%s\n###########\n", buff, remove_name);
    while (fgets(buff, 1024,fd)!=NULL)
    {
        save_list_count++;
    }

    fclose(fd);
    fd = fopen("./list.dat", "a+" );

    for (int i=0; i<save_list_count-1; i++)
    {
        fgets(buff, 1024,fd);
        fputs(buff,fd_copy);
        memset(buff, 0, 1024);
    }

    memset(remove_name_data, '\0', 40);
    sprintf(remove_name_data, "/mnt/sd/%s", remove_name);
    if (remove(remove_name_data) != 0)
    {
        printf("\n remove failed:%s \n", remove_name);
    }

    fclose(fd);
    fclose(fd_copy);
    remove("./list.dat");
    rename("./list_copy.dat", "./list.dat");
    
}

void FileListReplaceAdd(char* name)
{
    FILE *fd, *fd_copy;
    char buff[1024];
    char remove_name[40];
    fd = fopen("./list.dat", "a+" );
    fd_copy = fopen("./list_copy.dat", "a+");

    char filename[strlen(name)+2];
    memset(filename, '\0', strlen(name)+2);
    memcpy(filename, name, strlen(name));
    printf("\r\nbase sizeof:%d\r\n",sizeof(name));
    filename[strlen(name)] = '\n';
    
    /*
    fwrite("5\n", 1,1, fd);
    
    fwrite("1\n", 2,1, fd);
    fwrite("2\n", 2,1, fd);
    fwrite("3\n", 2,1, fd);*/
    fputs(filename,fd);
    printf("base filename:%s\n", filename);

    fseek(fd, 0, SEEK_SET);
    while(1)
    {
        memset(buff,0,1024);
        if (fgets(buff, 1024, fd) == NULL)
        {
            break;
        }
        printf("buff size:%d", strlen(buff));
        buff[strlen(buff)-1] = '\0';
        memset(remove_name, '\0', 40);
        sprintf(remove_name, "/mnt/sd/%s", buff);
        if (remove(remove_name) == 0)
        {
            break;
        }
        else
        {
            printf("\n remove failed:%s \n", remove_name);
        }
    }
    

    memset(buff,0,1024);
    while(fgets(buff, 1024,fd)!=NULL)
    {
        fputs(buff,fd_copy);
        memset(buff, 0, 1024);
    }
    fclose(fd);
    fclose(fd_copy);
    remove("./list.dat");
    rename("./list_copy.dat", "./list.dat");
    
    /*
    //lseek(fd, 0, SEEK_SET);
    fwrite("kd\n", 3,1, fd);
    //lseek(fd, 0, SEEK_SET);
    fwrite("ba\n", 3,1, fd);
    
    fseek(fd, 0, SEEK_SET);
    fgets(buff, 22,fd);
    fgets(buff, 22, fd);
    printf("dfe:%s", buff);
    fwrite(buff, 22, 1,ffd);
    fwrite("xx\n", 3,1, ffd);*/

}

void ListAdd(char* filename)
{
    FILE *fd;
    fd = fopen("./list.dat", "a+" );
    fwrite(filename, 1, strlen(filename), fd);
    printf("filename:%s", filename);
    fclose(fd);
}

void FileListAdd(char* filename)
{
    /*add space*/
    int size = strlen(filename);
    char name[size+2];
    memset(name, '\0', size+2);
    strcpy(name, filename);
    printf("name size:%d", strlen(name));
    name[size]='\n';
    ListAdd(name);
}

int count = 2;
void VideoFilenameAssign(char *filename)
{
    time(&now);
    time_now = localtime(&now);

    snprintf(filename, 32, "video_%d%02d%02d_%02d_%02d_%02d.dat", time_now->tm_year+1900, time_now->tm_mon+1, time_now->tm_mday, time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
}

void PictureFilenameAssign(char *filename)
{
    time(&now);
    time_now = localtime(&now);

    snprintf(filename, 34, "picture_%d%02d%02d_%02d_%02d_%02d.dat", time_now->tm_year+1900, time_now->tm_mon+1, time_now->tm_mday, time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
    
}

void DoVideoFileListAdd(char *filename)
{
    VideoFilenameAssign(filename);
    FileListAdd(filename);
}

void DoPictureFileListAdd(char *filename)
{
    PictureFilenameAssign(filename);
    FileListAdd(filename);
}

void DoVideoFileListReplaceAdd(char *filename)
{
    VideoFilenameAssign(filename);
    FileListReplaceAdd(filename);
}

void DoPictureFileListReplaceAdd(char *filename)
{
    PictureFilenameAssign(filename);
    FileListReplaceAdd(filename);
}

void GetFilename(char *filename)
{
    FILE *fd;
    //char buffer[20] = {0};
    fd=fopen("./list.dat", "r" );
    if( NULL == fd)
    {
        printf("\r\nget filename failed!\r\n");
        return;
    }

    if (NULL == fgets(filename, 40, fd))
    {
        printf("\r\nfget filename failed!\r\n");
        return;
    }
    int size = strlen(filename);
    filename[size-1] = '\0';

    fclose(fd);
}

void GetLastFilename(char *filename)
{
    FILE *fd;
    //char buffer[20] = {0};
    fd=fopen("./list.dat", "r" );
    
    if( NULL == fd)
    {
        printf("\r\nget filename failed!\r\n");
        return;
    }

    while (NULL != fgets(filename, 40, fd));
    int size = strlen(filename);
    filename[size-1] = '\0';

    fclose(fd);
}

void JudgeDoPicture(char *filename)
{
    struct statfs disk_info;
    statfs("/mnt/sd", &disk_info);
    
    unsigned long free_disk = disk_info.f_bfree * disk_info.f_bsize;
    printf("sd card free size:%u\n", free_disk);

    if (free_disk > 0)
    {
        DoPictureFileListAdd(filename);
    }
    else
    {
        DoPictureFileListReplaceAdd(filename);
    }
    count++;
}

void JudgeDoVideo(char *filename)
{
    struct statfs disk_info;
    statfs("/mnt/sd", &disk_info);
    
    unsigned long free_disk = disk_info.f_bfree * disk_info.f_bsize;
    printf("sd card free size:%u\n", free_disk);

    if (free_disk > 20000)
    {
        DoVideoFileListAdd(filename);
    }
    else
    {
        DoVideoFileListReplaceAdd(filename);
    }

    count++;
}

/*
int main(void)
{
	char filename[40];
    char file_path_name[40];
    memset(file_path_name, '\0', 40);
	memset(filename, '\0', 40);
	GetLastFilename(filename);
    snprintf(file_path_name, 40, "./%s",filename);
    fopen(filename, "wb");
	printf("filename:%s\n",filename);

	return;
}
*/

/*
int main(void)
{
    
    FILE *f;
    f = fopen("./list.dat", "a+" );
    fputs("1\n",f);
    fputs("2\n",f);
    fputs("3\n",f);
    fputs("4\n",f);
    fclose(f);

    for (int i=0; i<5; i++)
    {
        char filename1[7] = {0};
        char filename2[9] = {0};
        printf("filename2:%d\n", sizeof(filename2));

        //DoVideoFileListAdd(filename1);
        DoVideoFileListReplaceAdd(filename2);    
    }
    char filename[20];
    JudgeDoPicture(filename);
    printf("filename_size:%d\n", strlen(filename));
    printf("value:%s\n", filename);
    printf("value:%s\n", filename);
    char path[30];
    memset(path, '\0', 30);
    sprintf(path, "/mnt/sd/%s", filename);
    open(path, O_RDWR|O_CREAT);

}
*/
