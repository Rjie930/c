/*
 * @Author: vincent
 * @Date: 2023-08-31 11:05:39
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-31 00:21:52
 * @Description: 
 */
#include "head.h"
#include "kernel_list.h"

// 用户节点
struct user
{
    int ID;
    struct sockaddr_in cliaddr;

    struct list_head list;
};

int totaluser = 0;

struct user *new_node(int ID, struct sockaddr_in cliaddr)
{
    struct user *new = calloc(1, sizeof(struct user));
    if(new != NULL)
    {
        new->ID = ID;
        new->cliaddr = cliaddr;
        INIT_LIST_HEAD(&new->list);
    }
    return new;
}

pthread_mutex_t m;
struct user *head;

// 等待客户端的登入（"|in|ID|"）、登出（"|out|ID|"），依次来更新链表
void *Log(void *arg)
{
    int logfd = *(int *)arg;
    pthread_detach(pthread_self());

    char buf[20];
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    while(1)
    {
        bzero(buf, 20);
        bzero(&cliaddr, len);
        recvfrom(logfd, buf, 20, 0, (struct sockaddr *)&cliaddr, &len);

        // 登入
        if(strncmp(buf, "|in|", 4) == 0)
        {
            // 加锁
            pthread_mutex_lock(&m);

            // 增加链表节点
            // 1. 获取客户端的ID、地址
            int ID = atoi(buf+4); // buf: "|in|12345|"
            // 2. 新增节点
            struct user *new = new_node(ID, cliaddr);
            // 3. 插入链表末尾
            list_add_tail(&new->list, &head->list);

            // 解锁
            pthread_mutex_unlock(&m);
            totaluser++;

            // 温馨提示
            printf("[%s:%hu][ID:%d]上线了！\n", inet_ntoa(cliaddr.sin_addr),
                                                ntohs(cliaddr.sin_port),
                                                ID);
        }

        // 登出
        if(strncmp(buf, "|out|", 5) == 0)
        {
            // 加锁
            pthread_mutex_lock(&m);

            // 剔除、释放链表节点
            struct user *pos, *n;
            list_for_each_entry_safe(pos, n, &head->list, list)
            {
                // 1. 获取客户端的ID
                int ID = atoi(buf+5); // buf: "|out|12345|"
                // 2. 找到相同的结点就剔除
                if(pos->ID == ID)
                {
                    list_del(&pos->list);
                    // 温馨提示
                    printf("[%s:%hu][ID:%d]下线了！\n", inet_ntoa(pos->cliaddr.sin_addr),
                                                        ntohs(pos->cliaddr.sin_port),
                                                        pos->ID);
                    free(pos);
                    break;
                }
            }

            // 解锁
            pthread_mutex_unlock(&m);
            totaluser--;
        }
        printf("当前在线人数：%d\n", totaluser);
    }
}

void *Msg(void *arg)
{
    int msgfd = *(int *)arg;
    pthread_detach(pthread_self());

    char *buf = calloc(1, MAXMSGLEN); // "ID|......"
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    while(1)
    {
        bzero(buf, MAXMSGLEN);
        bzero(&from, sizeof(from));
        recvfrom(msgfd, buf, MAXMSGLEN, 0, (struct sockaddr *)&from, &fromlen);


        // 遍历链表，群发（除了发送者之外）消息
        struct list_head *pos;
        list_for_each(pos, &head->list)
        {
            struct user *p = list_entry(pos, struct user, list);

            // 跳过发送者
            if(p->ID == atoi(buf))
                continue;
            
            char *msg = strchr(buf, '|')+1;
            sendto(msgfd, msg, strlen(msg), 0,
                  (struct sockaddr *)&p->cliaddr, sizeof(p->cliaddr));
        }
    }
}

struct user *init_list()
{
    struct user *head = calloc(1, sizeof(struct user));
    if(head != NULL)
    {
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

int main(void)
{
    // 准备一个空的客户链表
    head = init_list();

    // 初始化互斥锁
    pthread_mutex_init(&m, NULL);

    int logfd = socket(AF_INET, SOCK_DGRAM, 0);
    int msgfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 自动获取IP地址
    addr.sin_port = LOGPORT;
    bind(logfd, (struct sockaddr *)&addr, len);

    addr.sin_port = MSGPORT;
    bind(msgfd, (struct sockaddr *)&addr, len);

    pthread_t t;
    pthread_create(&t, NULL, Log, (void *)&logfd);
    pthread_create(&t, NULL, Msg, (void *)&msgfd);

    pthread_exit(NULL);
}