#include "lib/BS_FunInterface.h"



void main()
{

    PthreadCreate(ServerInit());

    SocketClose(socket_server);

    exit(0);

}