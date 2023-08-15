#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>

typedef struct node
{
    int data;
    struct node *next;
}node;

node * new_node(int data)
{
    node *new = calloc(1, sizeof(node));
    if(new != NULL)
    {
        new->data = data;
        new->next = NULL; 
    }
    return new;
}

bool list_add(node *head, int data)
{
    node *new = new_node(data);
    if(new == NULL)
        return false;

    new->next = head->next;
    head->next = new;
    return true;
}

node *init_linklist()
{
    node *new = calloc(1, sizeof(node));
    if(new != NULL)
    {
        new->next = NULL;
    }
    return new;
}

bool is_empty(node *head)
{
    return head == NULL;
}

void show(node *head)
{
    if(is_empty(head))
        return;

    node *p;
    for(p=head->next; p!=NULL; p=p->next)
    {
        printf("%d\t", p->data);
    }
    printf("\n");
}

void revert(node *head)
{
	node * p = head->next, *q;

	head->next = NULL;

	while(p){
		q = p;
		p = p->next;
		q->next = head->next;
		head->next = q;
	}
}

void main(int argc, char **argv)
{
    node *head=init_linklist();
    for (int i = 0; i < 6; i++)
    {
        list_add(head,i);
    }
    show(head);

    printf("逆转：");
    revert(head);

    show(head);
}
