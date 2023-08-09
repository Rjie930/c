/*******************************************************************
 * @Descripttion   : Ctrl+C -> Ctrl+V
 * @version        : 3.14
 * @Author         : Rjie
 * @Date           : 2023-08-01 16:16
 * @LastEditTime   : 2023-08-01 16:16
*******************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    struct stat fileInfo;
    
    struct stat *p=malloc(sizeof(struct stat));
    stat(argv[1], p);

    // printf("%d,%d\n", major(fileInfo.st_dev), minor(fileInfo.st_dev));
    // printf("%ld,%d\n", fileInfo.st_size,fileInfo.st_mode);
     char perm[10];
     bzero(perm,10);
     int a=0400;
     char mode[]={'r','w','x'};
     for(int i=0;i<9;i++)
        perm[i]=(p->st_mode&(a>>i))?mode[i%3]:'-';
    printf("%s\n",perm);
}