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
#include "util.h"

static int a_socket;

char* orderAsText[O_ORDER_NB] = 
{
    "Quit",
    "Left",
    "Right",
    "Forward",
    "Backward",
    "No",
    "Logs"
};

void Client_start()
{
    LOG_CLIENT("\r\n");
    struct sockaddr_in server_address;
    a_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(a_socket == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    /* AF_INET = famille TCP/IP */
    server_address.sin_family = AF_INET;
    /* port du serveur auquel se connecter */
    server_address.sin_port = htons (SERVER_PORT);
    /* adresse IP (ou nom de domaine) du serveur auquel se connecter */
    server_address.sin_addr = *((struct in_addr *)gethostbyname ("localhost")->h_addr_list[0]);
    /* On demande la connexion auprès du serveur */
    if(connect(a_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    };

}

void Client_stop()
{
    LOG_CLIENT("\r\n");
    close(a_socket);
}

void Client_sendMsg(Comm_order_e order)
{
    LOG_CLIENT("Client send %s\r\n", orderAsText[order]);
    order = htonl(order);
    write(a_socket, &order, sizeof(order));
}

Comm_logs_s Client_readMsg()
{
    Comm_logs_s received_logs;
    read(a_socket, &received_logs, sizeof(received_logs));
    LOG_CLIENT("Just received raw       luminosity :%f speed: %d collision: %d\r\n",received_logs.luminosity, received_logs.speed, received_logs.collision);
    received_logs.collision = ntohl(received_logs.collision);
    received_logs.luminosity = ntohl(received_logs.luminosity);
    received_logs.speed = ntohl(received_logs.speed);
    LOG_CLIENT("Just received converted luminosity :%f speed: %d collision: %d\r\n",received_logs.luminosity, received_logs.speed, received_logs.collision);
    return received_logs;
}