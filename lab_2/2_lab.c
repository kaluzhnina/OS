#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>


int main() {
    DIR *dir;
    struct dirent *entry = (struct dirent*)calloc(sizeof(struct dirent), 1);
    struct stat *buffer = (struct stat*)calloc(sizeof(struct stat), 1);
    int number_of_rec = 0;

    time_t last_time_modif;

    struct dirent **namelist;
    number_of_rec = scandir(".", &namelist, 0, alphasort); 
    if (number_of_rec > 0) 
    {
        dir = opendir("./");
        if (!dir) {
            perror("diropen");
            exit(1);
        };
        int i = 0;
        while ( (entry = readdir(dir)) != NULL) 
        {
            entry = namelist[i];
            stat (entry->d_name,buffer);

            printf((buffer->st_mode & S_IFDIR)?"d":"-");
            printf((buffer->st_mode & S_IRUSR)?"r":"-");
            printf((buffer->st_mode & S_IWUSR)?"w":"-");
            printf((buffer->st_mode & S_IXUSR)?"x":"-");
            printf((buffer->st_mode & S_IRGRP)?"r":"-");
            printf((buffer->st_mode & S_IWGRP)?"w":"-");
            printf((buffer->st_mode & S_IXGRP)?"x":"-");
            printf((buffer->st_mode & S_IROTH)?"r":"-");
            printf((buffer->st_mode & S_IWOTH)?"w":"-");
            printf((buffer->st_mode & S_IXOTH)?"x":"-");
            
            printf("\t%5lld", buffer->st_size );

            char mtime[100];
            last_time_modif = buffer->st_mtime;
            struct tm time;
            localtime_r(&last_time_modif, &time);
            strftime(mtime, sizeof(mtime), "%d %b %Y %T", &time);
            printf("\t%s", mtime);
            printf("   %s\n", entry->d_name);
            free (namelist[number_of_rec]);
            i++;
        }
    }
    closedir(dir);
    return 0;
};