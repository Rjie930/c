#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "kernel_list.h"

// 设计包含内核标准结构体的用户节点
typedef struct node // 大结构体
{
    int data;
    struct list_head list; // 小结构体
}listnode, *linklist;

linklist init_list()
{
    linklist head = calloc(1, sizeof(listnode));
    if(head != NULL)
    {
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

linklist new_node(int data)
{
    linklist new = calloc(1, sizeof(listnode));
    if(new != NULL)
    {
        new->data = data;
        INIT_LIST_HEAD(&new->list);
    }
    return new;
}

void show(linklist head)
{
    struct node *pos;
    list_for_each_entry(pos, &head->list, list)
    {
        printf("%d ", pos->data);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    // 1. 搞一个空链表（带头节点）
    linklist head = init_list();

    // 2. 将n个自然数依次插入链表末尾 1 2 3 4 5...n
    int n;
    scanf("%d", &n);

    for(int i=1; i<=n; i++)
    {
        // 2.1 搞一个新节点
        linklist new = new_node(i);

        // 2.2 将新节点插入链表末尾
        list_add_tail(&new->list, &head->list);
    }
    show(head);

    // 3. 奇偶数重排
    struct list_head *pos, *q = head->list.prev; // 让q指向链表的末尾节点中的小结构体;
    struct node *p;
    list_for_each_prev(pos, &head->list)
    {
        // 大结构体指针  小结构体指针  大结构体类型  小结构体在用户节点中的名字
              p = list_entry(pos,      struct node,          list);

        if(p->data%2 == 0) // 偶数，一律移动到链表的末尾
        {
            list_move_tail(pos, &head->list);
            pos = q;
        }

        else // 奇数，记录下当前位置
        {
            q = pos;
        }
    }
    show(head);


    return 0;
}

