#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int rmdirs(const char *path, int is_error_stop)
{
    DIR *  dir_ptr      = NULL;
    struct dirent *file = NULL;
    struct stat   buf;
    char   filename[1024];

    if((dir_ptr = opendir(path)) == NULL) {
		return unlink(path);
    }

    while((file = readdir(dir_ptr)) != NULL) {
        if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
             continue;
        }

        sprintf(filename, "%s/%s", path, file->d_name);

        if(lstat(filename, &buf) == -1) {
            continue;
        }

        if(S_ISDIR(buf.st_mode)) {
            
            if(rmdirs(filename, is_error_stop) == -1 && is_error_stop) {
                return -1;
            }
        } else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) { // �Ϲ����� �Ǵ� symbolic link �̸�
            if(unlink(filename) == -1 && is_error_stop) {
                return -1;
            }
        }
    }

    closedir(dir_ptr);
    
    return rmdir(path);
}

int main(int argc, char *argv[])
{
    return rmdirs("/home/jk/workspace/blackbox/20190702-17", 1);
}