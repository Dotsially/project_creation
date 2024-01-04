#include "socket_object.h"



i32 SocketObject::Bind(int port){
    sockaddr_in saServerAdress;
    skSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(skSocket == INVALID_SOCKET){
        return false;
    }



}