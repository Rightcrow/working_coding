#ifndef __BS_CLIENT_H_
#define __BS_CLIENT_H_

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include "BS_Protocol_Signal.h"
#include "BS_Protocol_Struct.h"
#include "BS_Param_Message.h"
#include "BS_RealTime_Message.h"


extern void ResetClientBuffer();

extern int GetFrameLength();

extern void PutFrameToClientBuffer(unsigned char* buffer, int length);

extern void ClientHeartTest();

extern void ClientParamInquire();

extern void ClientRealTimeMessageInquire();

// extern unsigned char buffer_client[MAXlENGTH];

#endif