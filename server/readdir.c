#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define DIR_SIZE 8192

read_dir(dir)
char    *dir;   /* anticipate char dir[DIR_SIZE] */
{
        DIR * dirp;
        struct dirent *d;

        /* open directory */
        dirp = opendir(dir);
        if (dirp == NULL)
                return(0);

        /* stuff filenames into dir buffer */
        dir[0] = '\0';
        while (d = readdir(dirp))
                sprintf(dir, "%s%s\n", dir, d->d_name);

        /* return the result */
        closedir(dirp);
}

 

