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

            if (buffer->st_mode & S_IFDIR) printf("d");
            else printf("-");
            if (buffer->st_mode & S_IRUSR) printf("r");
            else printf("-");
            if (buffer->st_mode & S_IWUSR) printf("w");
            else printf("-");
            if (buffer->st_mode & S_IXUSR) printf("x");
            else printf("-");
            if (buffer->st_mode & S_IRGRP) printf("r");
            else printf("-");
            if (buffer->st_mode & S_IWGRP) printf("w");
            else printf("-");
            if (buffer->st_mode & S_IXGRP) printf("x");
            else printf("-");
            if (buffer->st_mode & S_IROTH) printf("r");
            else printf("-");
            if (buffer->st_mode & S_IWOTH) printf("w");
            else printf("-");
            if (buffer->st_mode & S_IXOTH) printf("x");
            else printf("-");
            
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