#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "kernel_list.h"

// 大结构体
struct student
{
    char *name;
    int age;

    struct list_head list; // 小结构体
};

struct student *init_list()
{
    struct student *head = calloc(1, sizeof(struct student));
    if (head != NULL)
    {
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

int main(int argc, char const *argv[])
{
    // 1. 搞一个空链表
    struct student *head = init_list();

    // 2. 搞几个学生实例
    struct student Jack = {"Jack", 21};
    struct student Bill = {"Bill", 23};
    struct student Rose = {"Rose", 18};

    // 3. 将以上结点依次放入链表表尾
    list_add_tail(&Jack.list, &head->list);
    list_add_tail(&Bill.list, &head->list);
    list_add_tail(&Rose.list, &head->list);

    // 4. 遍历链表
    struct list_head *pos;
    struct list_head *n;
    list_for_each(pos, &head->list)
    {
        printf("姓名：%s\t", list_entry(pos, struct student, list)->name);
        printf("年龄：%d\n", list_entry(pos, struct student, list)->age);
    }

    list_for_each_safe(pos,n, &head->list)
    {
        if (strcmp(list_entry(pos, struct student, list)->name, "Bill") == 0)
        list_del(pos);
    }

    list_for_each_prev(pos, &head->list)
    {
        printf("姓名：%s\t", list_entry(pos, struct student, list)->name);
        printf("年龄：%d\n", list_entry(pos, struct student, list)->age);
    }
    return 0;
}
