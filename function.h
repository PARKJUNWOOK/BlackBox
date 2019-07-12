#pragma once

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mntent.h>
#include <unistd.h>
#include <dirent.h>     // diropen(), readdir()
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <termios.h>

// #ifndef FUNCTION_H
// #define FUNCTION_H
// #endif

#define NUM_THREADS     3

/*
    * 구조체 정의부
*/
typedef enum _boolean {
    FALSE,
    TRUE
} boolean;

struct f_size
{
    long blocks;
    long avail;
    long percent;
};

typedef struct _mountinfo 
{
    FILE *fp;               // 파일 스트림 포인터    
    char devname[80];       // 장치 이름
    char mountdir[80];      // 마운트 디렉토리 이름
    char fstype[12];        // 파일 시스템 타입
    struct f_size size;     // 파일 시스템의 총크기/사용율 
} MOUNTP;


/*
    * 함수 선언부
*/

char* getCurrentTime(char typeFlag);
boolean createFolder(char* folderName);
boolean deleteFolder(char* folderName); // only Dir delete
int delDirFile(const char* folder);     // file/dir delete
int getRepoSize(char* targetPath);
unsigned long getFsSize(char* targetPath, char* osType);
MOUNTP *getDirSize();
int dfclose(MOUNTP *MP);
MOUNTP *dfget(MOUNTP *MP);
MOUNTP *dfopen();
int searchDir(char *targetPath);
int rmdirs(const char *path, int is_error_stop);
char* searchOldDir(char* targetPath);
char* getModiTime(char *targetPath);
char* itoa(int integer);
int getch(void);
void deleteProc(void* data);
