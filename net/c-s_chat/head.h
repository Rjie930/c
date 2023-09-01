/*
 * @Author: vincent
 * @Date: 2023-08-31 11:06:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-31 00:27:30
 * @Description: 
 */
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <strings.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#define LOGPORT 50001
#define MSGPORT 50002

#define MAXMSGLEN 1024