#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
//#include "commun.h"
#include "pilot.h"
#include "server.h"
#include "config.h"

static void Server_sendMsg();
static void Server_readMsg();


void Server_start()
{
    int socket_listen;
    int socket_data;
    struct sockaddr_in server_address;
    Pilot_start();
    socket_listen = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    bind(socket_listen, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(socket_listen, MAX_PENDING_CONNECTIONS);
    while (1)
    {
        socket_data = accept(socket_listen, NULL, 0);
        Server_readMsg(socket_data);

    }
    
}

void Server_stop()
{

}

static void Server_sendMsg()
{

}

static void Server_readMsg()
{

}