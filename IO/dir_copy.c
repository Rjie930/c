#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

void reCopy(char *src, char *dest)
{
    mkdir(dest, 0777);
    printf("mk : %s\n", dest);

    DIR *dp = opendir(src);
    printf("cd %s\n", src);
    chdir(src);

    struct dirent *ep;
    struct stat entry_desfo;
    char entry_src[100];
    char entry_des[100];
    bzero(entry_src, 100);
    bzero(entry_des, 100);

    ep = readdir(dp);
    while (ep != NULL)
    {
        while (ep != NULL && (!strcmp(ep->d_name, ".") | !strcmp(ep->d_name, "..")))
            ep = readdir(dp);

        if (ep == NULL)
        {
            break;
        }

        stat(ep->d_name, &entry_desfo);
        getcwd(entry_src, 100);
        strcpy(entry_des, dest);

        sprintf(entry_src, "%s/%s", src, ep->d_name);
        sprintf(entry_des, "%s/%s", dest, ep->d_name);

        if (S_ISDIR(entry_desfo.st_mode))
        {
            printf("entry dir: %s\n", ep->d_name);
            reCopy(entry_src, entry_des);
            chdir("..");
        }
        else
        {
            printf("copy: %s -> %s\n", entry_src, entry_des);
            int size = 0;
            char buf[512];
            bzero(buf, 512);
            int fd_src = open(entry_src, O_RDONLY);
            int fd_dest = open(entry_des, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            while (size = read(fd_src, buf, 512))
            {
                write(fd_dest, buf, size);
                bzero(buf, 512);
            }
            close(fd_src);
            close(fd_dest);
        }
        ep = readdir(dp);
    }
    printf("cd %s\n", "..");
    closedir(dp);
}
int main(int argc, char const *argv[])
{
    reCopy("/home/zz/c", "/home/zz/c1");
}