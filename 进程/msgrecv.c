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
    int id = msgget(key, IPC_CREAT | 0666);
    struct msgbuf buf;

    if (fork() == 0)
    {
        while (1)
        {
            msgrcv(id, &buf, sizeof(buf), 0, 0);
            printf("接收:%s\n", buf.data);
        }
    }
    else
        while (1)
        {
            fgets(buf.data, 20, stdin);
            msgsnd(id, &buf, sizeof(buf.data), 0);
        }

    return 0;
}
