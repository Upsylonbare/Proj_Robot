#ifndef CLIENT_H
#define CLIENT_H

#include "commun.h"

void Client_start();
void Client_stop();
void Client_sendMsg(Comm_order_e order);
Comm_logs_s Client_readMsg();

#endif /* CLIENT_H */