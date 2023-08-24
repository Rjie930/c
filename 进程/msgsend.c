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
    buf.id = 1;
    strcpy(buf.data, "6");

    int sum=1;
    // while (1)
    // {
            msgsnd(id, &buf, sizeof(buf.data), 0);
            sum++;
            printf("%d\n",sum);
            // usleep(1000*100);
    // }
    

    return 0;
}
