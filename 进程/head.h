/*
 * @Author: vincent
 * @Date: 2023-08-24 11:28:45
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-24 11:46:15
 * @Description: 
 */
#define PROJ_PATH "."
#define PROJ_ID   1

#define JACK2ROSE 1
#define ROSE2JACK 2

struct msgbuf
{
    // 规定必须有的long型数据，且一定在开头
    long type; 

    // 用户定义的任意数据
    char data[20];
};