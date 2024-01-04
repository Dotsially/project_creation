#include "depedencies.h"
#include "WinSock2.h"

class SocketObject{
private:



public:
    SOCKET skSocket;        
    SocketObject();
    ~SocketObject();
    i32 Bind(i32 port);



};