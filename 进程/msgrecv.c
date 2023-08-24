#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

#define PATH "."
#define ID 1

#define rec 1
#define snd 2

struct msgbuf
{
    long id;
    char data[20];
};

int main(int argc, char const *argv[])
{
    key_t key = ftok(PATH, ID);
    int id = msgget(key, IPC_CREAT  | 0666);
    struct msgbuf buf;

    printf("ret:%ld\n",msgrcv(id, &buf, sizeof(buf), 1, IPC_NOWAIT));
    printf("data:%s\n", buf.data);

    msgctl(id,IPC_RMID,NULL);
    return 0;
}
