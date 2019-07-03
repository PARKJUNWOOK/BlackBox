#include "function.h"

/*
* define list
*/
#define TIME_BUFF_SIZE  1024
//char* ROOT_FOLDER_PATH = "/workspace/blackbox/";
char* ROOT_FOLDER_PATH = "/home/jk/workspace/blackbox/";
char REAL_TIME_FOLDER_PATH[256] = "";

// boolean struct
typedef enum _boolean {
    FALSE,
    TRUE
} boolean;

// INNER Function
boolean checkDirectory(char* folderPath);

/*
    * getTime
    * param D(YYYYMMDD), T(HH:24), S(HHMMSS)
    
*/
char* getCurrentTime(char typeFlag)
{
    time_t tm_time;
    struct tm *st_time;
    char* buff = malloc(sizeof(TIME_BUFF_SIZE));

    time(&tm_time);
    st_time = localtime(&tm_time);
    if(typeFlag == 'D')     // Date (YYYYMMDD)
        strftime(buff, TIME_BUFF_SIZE, "%Y%m%d", st_time);
    else if(typeFlag == 'T') // TIME (HH)
        strftime(buff, TIME_BUFF_SIZE, "%Y%m%d-%H", st_time);
    else    // currentTime      strftime(buff, TIME_BUFF_SIZE, "%Y%m%d-S%H%M%S-E%H%M%S", st_time);
        strftime(buff, TIME_BUFF_SIZE, "%H%M%S", st_time);
        
    return buff;
}

/*
    * createFolder
    * return : TRUE/FALSE (ENUM)
*/
boolean createFolder(char* folderName)
{
    boolean isFlag = FALSE;

    char realPath[256] = {0};
    char fullPath[256];

    // getCwd
    getcwd(realPath, 255);
    strcpy(realPath, ROOT_FOLDER_PATH);
    strcat(realPath, folderName);

    // Directory State Check
    struct stat st;
    if(checkDirectory(realPath) == TRUE)
    {   
        int ret = mkdir(realPath, 0777);
        if(ret != -1)
        {
            printf("### Success Create Folder!!\n");
            isFlag = TRUE;
            strcpy(REAL_TIME_FOLDER_PATH, realPath);
            printf("REAL_TIME_FOLDER_PATH :: %s\n", REAL_TIME_FOLDER_PATH);
        } 
        else
        {
            printf("### Failed Create Folder!!\n");
            printf("error..NO.%d \n",errno);
            printf("errno check URL :: http://www.ioplex.com/~miallen/errcmpp.html \n");
            //isFlag = FALSE;
            isFlag = TRUE;   // imsi proc
        }
    }
    else
        isFlag = TRUE;  // isFlag = FALSE;
    
    return isFlag;
}

/*
    * Delete Folder/File
    * Param : folderName = folderPath+folderName
*/
boolean deleteFolder(char* folderName)
{
    boolean isFlag = FALSE;
    char fullPath[256] = {0};

    strcpy(fullPath, ROOT_FOLDER_PATH);
    strcat(fullPath, folderName);
    printf("##### delete fullPath :: %s\n", fullPath);

    if(rmdir(fullPath) == 0)
        isFlag = TRUE;
    else
    {
        perror( "폴더 삭제 실패 - 폴더 안에 파일이 있거나 사용중\n" );
		printf( "errorno : %d", errno );
    }
    
    return isFlag;
}

/*
    * checkDir
    * param : check stat folderPath
    * return : usedFolder(FALSE), unusedFolder(TRUE)
*/
boolean checkDirectory(char* folderPath)
{
    boolean isFlag = FALSE;
    struct stat st;
    if(stat(folderPath, &st) != 0)
        isFlag = TRUE;
    else
        isFlag = FALSE;

    return isFlag;
}

/*
    * DIR Checker And Delete
    * Reference : https://kldp.org/node/108793
*/
int delDirFile(const char* folder)
{
    DIR *dp;
    struct dirent *dirp;
    char targetfile[512];
    struct stat file_stat;

    strcpy(targetfile, ROOT_FOLDER_PATH);
    strcat(targetfile, folder);
    
    stat(folder, &file_stat);

    while(1)
    {
        // is Directorys
        if(S_ISDIR(file_stat.st_mode))
        {
            if (delDirFile(targetfile) == -1 ) // sub directory 삭제를 위해 자기 자신을 재귀 호출
            {
            return -1; // 실패 시 -1 반환
            }
            continue;
        }

        if ( unlink(targetfile) == -1 )
        {
            return -1; // 실패 시 -1 반환
        }
        continue;
    }

    // folder 디렉토리의 모든 항목들 제거후, folder를 제거
    int result = rmdir(folder);
    closedir(dp);
    return result == 0 ? 1:-1;
}

/*
    * repository size return
    * param : repoPath > path
    * return : int(size)
*/
int getRepoSize()
{
    struct stat targetInfo;
    //printf("path :: %s\n", REAL_TIME_FOLDER_PATH);
    if(stat("/home/jk/workspace/blackbox/20190702-17/Anaconda3-2018.12-Linux-x86_64.sh", &targetInfo) == -1)
    {
        printf("file not read \n");
    }
    else
    {
        mode_t fileMode;

        fileMode = targetInfo.st_mode;

        if(S_ISDIR(fileMode))
            printf("##### This is DIR\n");
        
        test1(&targetInfo);
    }
    

}

void test1(struct stat *buf)
{
    long long dSize = buf->st_size;
    printf("FILE SIZE IS22 : %d\n", dSize);
    printf("마지막 읽은 시간22 : %d\n", buf->st_atime);
    printf("마지막 수정 시간22 : %d\n", buf->st_mtime);
    printf("하드링크된 파일수22 : %d\n", buf->st_nlink);
}

// REC

// REC File Write