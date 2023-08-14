/*
 * @Author: vincent
 * @Date: 2023-08-11 09:31:07
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-14 11:30:15
 * @Description: 
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct student
{
    int age;
    char *name;
};

// 思路一：将数据类型的定义权交给用户
#define DATATYPE struct student // 该语句必须放在以下头文件之前
#include "linklist.h"

// 根据用户自己指定的特定数据，去给链表提供以下与数据操作有关的接口
void print(struct student *pdata)
{
    printf("姓名：%s\t", pdata->name);
    printf("年龄：%d\n", pdata->age);
}

bool compare(const struct student *p1, const struct student *p2)
{
    return strcmp(p1->name, p2->name)==0;
}

// 测试用例，组织各种数据类型
int main(int argc, char const *argv[])
{
    // 1. 搞一个空链表
    node *head = init_linklist();

    // 2. 键盘输入任意节点
    struct student Jack = {21, "Jack"};
    struct student Bill = {22, "Bill"};

    list_add_tail(head, new_node(Jack));
    list_add_tail(head, new_node(Bill));

    // 打印出所有学生的信息
    list_for_each(head, print);


    printf("请输入你要删除的学生的姓名：");
    char name[20];
    fgets(name, 20, stdin);
    strtok(name, "\n");
    struct student del = {0, name};

    // 试图找到学生节点p
    node *p = list_find(head, del, compare);
    if(p == NULL)
    {
        fprintf(stderr, "查无此人!\n");
    }
    else
    {
        // 将节点p剔除出链表，并释放其内存
        p = list_del(p);
        free(p);
    }
    list_for_each(head, print);


    // 3. 销毁链表
    destroy_list(head, NULL);
    head = NULL;

    return 0;
}
