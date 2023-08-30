#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>

int main(int argc, char **argv)
{
    DIR *dp = opendir(argv[1]);

    struct stat info;
    struct dirent *ep;
    struct passwd *pd;
    struct group *gp;
    time_t ctim;
    char mode[10];
    char rwx[]= {'r','w','x'};
    ep = readdir(dp);
    while (ep != NULL)
    {

        stat(ep->d_name, &info);
        pd = getpwuid(info.st_uid);
        gp = getgrgid(info.st_gid);
        ctim = info.st_ctime;

        switch (info.st_mode & S_IFMT)
        {
        case S_IFSOCK:mode[0] = 's';
            break;
        case S_IFLNK:mode[0] = 'l';
            break;
        case S_IFDIR:mode[0] = 'd';
            break;
        case S_IFREG:mode[0] = '-';
            break;
        case S_IFBLK:mode[0] = 'b';
            break;
        case S_IFCHR:mode[0] = 'c';
            break;
        case S_IFIFO:mode[0] = 'p';
            break;
        default:
            break;
        }

        for (int i = 0; i < 9; i++)
        {
            snprintf(mode+i+1,2,"%c",(info.st_mode&(0400>>i))?rwx[i%3]:'-');
        }
        
        printf("%6s", mode);
        printf("%4ld", info.st_nlink);
        printf("%5s", pd->pw_name);
        printf("%5s", gp->gr_name);
        printf("%6ld  ", info.st_size);
        printf("%24s", strtok(ctime(&ctim), "\n"));
        printf("%10s\n", ep->d_name);
        ep = readdir(dp);
    }
    closedir(dp);
}