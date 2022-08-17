#ifndef __BS_FUNINTERFACE_H_
#define __BS_FUNINTERFACE_H_

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

extern unsigned short getCRC(unsigned char* buf,unsigned int len);

extern void FrameAssemble( unsigned char ti_response, unsigned char control_response, int length_serv, unsigned char* buffer_rev_data_area);

extern void BufferAssemble();

extern unsigned char ErrJudgeOfHeartTest(int socket_client);

extern int LengthAssemble(unsigned char length_high, unsigned char length_low);

extern void Judge(unsigned char* buffer, int length_serv, unsigned char* buffer_recv_data_area);

extern unsigned char JudgeTi(unsigned char ti_serv);

extern unsigned char JudgeControl(unsigned char control_serv);

extern int GetResponseLength(unsigned short length);

extern int GetRealtimeMessageInquireResponseLength(unsigned char dt_char);

extern unsigned char ErrJudgeOfRealtimeMessageInquire();

extern unsigned char ErrJudgeOfControlCommand();

extern unsigned char ErrJudgeOfFileCatalogueInquire();

extern unsigned char ErrJudgeOfFileCatalogueTramiteResponse();

extern unsigned char ErrJudgeOfFileWriteInquireResponse();

extern unsigned char ErrJudgeOfWriteDataToFileResponse();

extern unsigned char ErrJudgeOfWriteDataActivateResponse();

extern unsigned char ErrJudgeOfReadFileInquireResponse();

extern unsigned char ErrJudgeOfReadFileDataResponse();

extern unsigned short GetValidateOfFile(unsigned char* buffer_fn_char);

extern unsigned char ErrJudgeOfSystemTimeConfirmResponse();

extern unsigned char ErrJudgeOfSystemTimeInquireResponse();

extern int CharToInit(unsigned char pn_char);

extern void FrameInit();

extern void* ServerInit();

extern unsigned char ErrJudgeOfParamInquire();

extern unsigned short CalcuParamInquireResponseLength(unsigned char* buffer_rev_data_area, int length_serv, 
                                               unsigned char* buffer_pid_char, int pn_int, int* buffer_pl_int, unsigned char* buffer_pl_char);


extern void ParamSetResponse(unsigned char ti_response, unsigned char control_response, int length_serv, 
                             unsigned char* buffer_recv_data_area);


extern void RealtimeMessageInquireResponse(unsigned char ti_response, unsigned char control_response,
                                           int length_serv, unsigned char* buffer_recv_data_area);

extern unsigned char ErrJudgeOfParamSet();

extern unsigned short IntToShort(int length_int);

extern int GetPNToInt(unsigned char pn_char);

extern void FrameTx(int socket_temp, unsigned char buffer_temp);

extern void FrameRecv(int socket_temp, unsigned char buffer_temp);

extern void SocketClose(int socket);

extern void PthreadCreate();

extern struct FrameStruct frame_struct_glob;

extern unsigned char buffer_response[MAXlENGTH];

extern int length_response;

extern int socket_server;

extern int socket_client;


#endif