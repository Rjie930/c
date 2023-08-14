#include "dlinklist.h"

// 1. 初始化空的双循环链表（带头节点）
node *init_dlinklist()
{
    node * head = calloc(1, sizeof(node));
    if(head != NULL)
    {
        head->next = head;
        head->prev = head;
    }
    return head;
}

node * new_node(int data)
{
    node *new = calloc(1, sizeof(node));
    if(new != NULL)
    {
        new->data = data;
        new->prev = new;
        new->next = new;
    }
    return new;
}

// 2. 尾插法添加新节点
bool list_add_tail(dlinklist head, node *new)
{
    new->prev = head->prev;
    new->next = head;
    head->prev->next = new;
    head->prev = new;
    return true;
}

bool is_empty(dlinklist head)
{
    return head->prev == head->next;
}

// 3. 向后遍历链表的所有结点
void show(dlinklist head)
{
    if(is_empty(head))
        return;
    
    node *p;
    for(p=head->next; p!=head; p=p->next)
    {
        printf("%d\t", p->data);
    }
    printf("\n");
}
