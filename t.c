#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char const *argv[])
{
    struct hostent *host;
    host = gethostbyname("www.baidu.com");

    if (host != NULL)
    {
        for(int i=0; host->h_addr_list[i] != NULL; i++)
        printf("%s\n",inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    

    return 0;
}
