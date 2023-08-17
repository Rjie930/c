#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "data_struct/kernel_list/kernel_list.h"

typedef struct node
{
    char name[20];
    int time;
    struct list_head list;
} listnode, *linklist;

linklist init_list()
{
    linklist head = calloc(1, sizeof(listnode));
    if (head != NULL)
    {
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

linklist new_node(int time, char *name)
{
    linklist new = calloc(1, sizeof(listnode));
    if (new != NULL)
    {
        new->time = time;
        strcpy(new->name, name);

        INIT_LIST_HEAD(&new->list);
    }
    return new;
}

void show(linklist head)
{
    struct node *pos;
    list_for_each_entry(pos, &head->list, list)
    {
        printf("%s %d\n",pos->name, pos->time);
    }
}

void swap(int *a, int *b)
{

    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubbleSort(int data[], int len)
{
    int k=0;
    while(1)
    {
        bool done = true;

        int i;
        for(i=0; i<len-1-k; i++)
        {

            if(data[i] <= data[i+1])
            {
                continue;
            }
        
            swap(&data[i], &data[i+1]);
            done = false;
        }

        if(done)
            break;
        k++;
    }
}

int main()
{
    int sum=0;
    FILE *fp = fopen("record.txt", "r");
    char buf[sizeof(struct node)];
    linklist p = (struct node *)malloc(sizeof(struct node));

    linklist head = init_list();

    while (fgets(buf, sizeof(struct node), fp))
    {
        sscanf(buf, "%s %d", p->name, &p->time);
        linklist new = new_node(p->time,p->name);
        list_add_tail(&new->list, &head->list);
        sum++;
    }
    int number[sum];

    struct node *pos;
    int i=0;
    list_for_each_entry(pos, &head->list, list)
    {
         number[i]=pos->time;
         i++;
    }
    bubbleSort(number,sum);
    for (int i = 0; i < sum; i++)
    {
        printf("%d\n",number[i]);
    }
    
    printf("sum:%d\n",sum);
    printf("请输入第K名:\n");

    int k;
    scanf("%d",&k);
        list_for_each_entry(pos, &head->list, list)
    {
         if(pos->time==number[k-1])
         {
         printf("第K名选手为:%s,时间:%d\n",pos->name,pos->time);
         break;
         }
    }
    // show(head);
}
