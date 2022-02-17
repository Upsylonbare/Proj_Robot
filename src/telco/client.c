#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "commun.h"
#include "config.h"

#ifdef LOG_ENABLE
#define LOG_CLIENT(...) do{printf(CYAN);printf(__VA_ARGS__);printf(DEFAUT);}while(0)
#else
#define LOG_CLIENT(...)
#endif

static int a_socket;

void Client_start()
{
    LOG_CLIENT("CLIENT START\r\n");

    struct sockaddr_in server_address;
    a_socket = socket(AF_INET, SOCK_STREAM, 0);
    /* AF_INET = famille TCP/IP */
    server_address.sin_family = AF_INET;
    /* port du serveur auquel se connecter */
    server_address.sin_port = htons (SERVER_PORT);
    /* adresse IP (ou nom de domaine) du serveur auquel se connecter */
    server_address.sin_addr = *((struct in_addr *)gethostbyname ("localhost")->h_addr_list[0]);
    /* On demande la connexion auprès du serveur */
    connect(a_socket, (struct sockaddr *)&server_address, sizeof(server_address));
}

void Client_stop()
{
    LOG_CLIENT("CLIENT STOP\r\n");
    close(a_socket);
}

void Client_sendMsg(Comm_datas_s datas_to_be_send)
{
    LOG_CLIENT("CLIENT SEND MSG\r\n");
    datas_to_be_send.comm_dir = htonl(datas_to_be_send.comm_dir);
    write(a_socket, &datas_to_be_send, sizeof(datas_to_be_send));
}

Comm_datas_s Client_readMsg()
{
    LOG_CLIENT("CLIENT READ MESSAGE\r\n");
    Comm_datas_s datas_to_be_receive_by_client;
    read(a_socket, &datas_to_be_receive_by_client, sizeof(datas_to_be_receive_by_client));
    datas_to_be_receive_by_client.comm_logs = ntohl(datas_to_be_receive_by_client.comm_logs);
    return datas_to_be_receive_by_client;
}