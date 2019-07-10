#include "function.h"

/*
* define list
*/
#define TIME_BUFF_SIZE  1024

//char* ROOT_FOLDER_PATH = "/workspace/blackbox/";
char* ROOT_FOLDER_PATH = "/home/jk/workspace/blackbox/";
char* ROOT_FOLDER_PATH2 = "/home/jk/workspace/blackbox";
char REAL_TIME_FOLDER_PATH[256] = "";


const char *MMOUNT = "/proc/mounts";    // 프로세스 마운트 디바이스 고정주소
//const char *MMOUNT = "/etc/mtab";  // 현재 시스템의 마운트 정보

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
        strftime(buff, TIME_BUFF_SIZE, "%Y%m%d%H", st_time);
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
    * stat 를 이용한 파일 사이즈 체크
    * param : repoPath > path
    * return : int(size)
*/
int getRepoSize(char* targetPath)
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
        
        // TODO : 해당 경로 내 존재하는 파일들의 전체 사이즈 합산체크
    }
}

/*
    * statfs를 이용한 파일시스템(마운트된 드라이브) 사이즈 체크
    * param : fsPath, osType(LINUX / WINDOWS / UNIX / SOLALIS)
    * return : size(uLong)
*/
unsigned long getFsSize(char* targetPath, char* osType)
{
    FILE *mountFile;
    struct mntent *sMntent;
    struct statfs *sStatfs;

    unsigned long fSize = 0;

    printf("11\n");
    // /proc/mounts :: proc 가상 파일 시스템의 일부 (리눅스 커널이 시스템 상태를 파일 형식으로 출력)
    // /etc/mtab :: 현재 마운트 된 정보
    // /etc/fstab :: 부팅시 마운트 할 정보
    if(strcmp("LINUX", osType) == 0)
    {
        printf("22\n");
        mountFile = setmntent("/proc/mounts", MNTOPT_RO);
    }
    else if(strcmp("WINDOWS", osType) == 0)
    {
        printf("33\n");
        mountFile = setmntent("/proc/filesystems", MNTOPT_RO);
    }
    else if(strcmp("SOLALIS", osType) == 0)
    {
        printf("44\n");
        mountFile = setmntent("/usr/sbin/mount", MNTOPT_RO);
    }
    else
    {
        printf("해당 OS Type은 지원하지 않습니다. :: %s\n", osType);
        return -1;
    }

    // 마운트한 파일 객체가 0 이 아닐 경우
    if(mountFile != ((FILE*)0))
    {
        printf("55\n");
        // 조건을 떠나 한번은 수행을 시키기 위해 do-while 사용
        do {
            printf("66\n");
            sMntent = getmntent(mountFile);

            if(sMntent == ((struct mntent *)0)) 
            {
                printf("77\n");
                break;
            }
            if((sMntent->mnt_dir == ((char *)0)) || (sMntent->mnt_fsname == ((char*)0))){
                printf("88\n");
                continue;
            }
            if(statfs((char*)sMntent->mnt_dir, (struct statfs * ) (&sStatfs)) != 0){
                printf("99\n");
                continue;
            }
            if(sStatfs->f_blocks < 0) {
                printf("000\n");
                continue;
            }
            
            printf("checkSize :: \n");
            fSize = (((unsigned long) sStatfs->f_bavail)/1024) * (((unsigned long)sStatfs->f_bsize)/1024);
            printf("fSize :: %d\n", fSize);
            
        } while(1);
        (void)endmntent(sMntent);
    }
    return fSize;
}

/*
    * 마운트 경로 정보 파일 오픈
*/
MOUNTP *dfopen()
{
    MOUNTP *MP;

    // /proc/mounts 파일을 연다.
    MP = (MOUNTP *)malloc(sizeof(MOUNTP));
    if(!(MP->fp = fopen(MMOUNT, "r")))
    {
        return NULL;
    }
    else
        return MP;
}

/*
    * 마운트 된 파일 시스템 정보 추출
*/
MOUNTP *dfget(MOUNTP *MP)
{
    char buf[256];
    char *bname;
    char null[16];
    struct statfs lstatfs;
    struct stat lstat; 
    int is_root = 0;

    while(fgets(buf, 255, MP->fp))
    {
        is_root = 0;
        sscanf(buf, "%s%s%s",MP->devname, MP->mountdir, MP->fstype);
         if (strcmp(MP->mountdir,"/") == 0) is_root=1;
        if (stat(MP->devname, &lstat) == 0 || is_root)
        {
            if (strstr(buf, MP->mountdir) && S_ISBLK(lstat.st_mode) || is_root)
            {
                // 파일시스템의 총 할당된 크기(blocks)와 사용량(avail)을 구한다.        
                statfs(MP->mountdir, &lstatfs);
                MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024); 
                MP->size.avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024); 
                MP->size.percent = ((double)MP->size.avail / (double)MP->size.blocks) * 100;
                return MP;
            }
        }
    }
    rewind(MP->fp);
    return NULL;
}

/*
    * 열어둔 파일객체 close 처리
*/
int dfclose(MOUNTP *MP)
{
    fclose(MP->fp);
}

/*
    * mntent를 사용하지 않은 파일시스템 객체 리턴
*/
MOUNTP *getDirSize()
{
    MOUNTP *MP;
    if ((MP=dfopen()) == NULL)
    {
        perror("error");
        return 1;
    }
    else if(dfget(MP))
        return MP;                                
}

/*
    * 디렉토리 / 하위 파일 삭제 함수
    * param : 폴더 경로, 타입(1:디렉토리/파일삭제, 0:오류발생시 중단)
    * 레퍼런스URL : https://downman.tistory.com/150
    
    * COMMENT
    * path 를 이용하여 DIR 객체 추출
    * 최초 조회 DIR이 파일이면 삭제 후 종료
    * 아닐 경우 DIR 의 readdir 함수를 이용하여 하나씩 읽기처리
      파일이면 삭제, 디렉토리이면 재귀호출로 해당 디렉토리를 타켓으로 다시 처리
*/
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
        } 
        else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) { // �Ϲ����� �Ǵ� symbolic link �̸�
            if(unlink(filename) == -1 && is_error_stop) {
                return -1;
            }
        }
    }
    closedir(dir_ptr);
    return rmdir(path);
}

/*
    * 하위 폴더 중 가장 오래된 폴더를 검색한다.
    * parma : targetPath(대상 루트경로)
    * return : 가장 오래된 폴더 path
*/
char* searchOldDir(char* targetPath)
{
    DIR *dirPtr = NULL;
    struct dirent *file = NULL;
    int oldTime = 0;
    char *oldPath = malloc(sizeof(char)*256);
    char buff[256] = "";

    if((dirPtr = opendir(targetPath)) == NULL)
    {
        fprintf(stderr, "%s No Read Dir\n", targetPath);
        return -1;
    }
    printf("##### 오래된 폴더를 검색하고 하위파일과 폴더를 삭제합니다...\n");
    printf("기준 디렉토리 :: %s\n", targetPath);

    while((file = readdir(dirPtr)) != NULL)
    {
        if((strlen(file->d_name)) >= 4)
        {
            printf("##### 검사 대상 파일명 :: %s\n", file->d_name);
            strcpy(buff, targetPath);
            strcat(buff, "/");
            strcat(buff, file->d_name);
            printf("##### 생성일을 조회할 타겟 폴더경로 :: %s\n", buff);

            // 디렉토리 폴더의 생성 시각을 조회(YYYYMMDDHH)
            char *dBuff = malloc(sizeof(char) * TIME_BUFF_SIZE);
            dBuff = getModiTime(buff);
            printf("dBuff :: %s\n", dBuff);
            printf("strlen(oldPath) :: %d\n", strlen(oldPath));

            // 생성일 비교 기준을 위하여 첫번째 데이터 입력
            if(strlen(oldPath) <= 0)
            {
                strcpy(oldPath, buff);
                oldTime = atoi(dBuff);
                printf("생성일 비교 첫번째 데이터 입력 :: %d\n", oldTime);
            }
            else
            {
                /* 폴더 간 날자비교 */
                int timeOne = oldTime;
                int timeTwo = atoi(dBuff);

                printf("비교할 두개의 날자정보\n");
                printf("%d\n", timeOne);
                printf("%d\n", timeTwo);

                if(timeOne > timeTwo)
                {
                    strcpy(oldPath, buff);
                    oldTime = timeTwo;
                }
            }

            // malloc init
            free(dBuff);
        }
        
    }
    closedir(dirPtr);
    
    printf("기준 폴더 내 가장 오래된 폴더명 :: %s\n", oldPath);
    printf("기준 폴더 내 가장 오래된 파일시간 :: %d\n", oldTime);

    return oldPath;
}

/*
    * itoa 구현 (int to char)
    * param : int value
    * return : char
*/
char* itoa(int integer)
{
    return ((char*)integer);
}

/*
    * 해당 폴더의 생성시간정보를 리턴한다.
    * param : 생성시간정보를 조회할 폴더 경로
    * return : 생성시간정보(char*)
*/
char* getModiTime(char *targetPath)
{
    struct stat sb;
    struct tm *locTime;
    char* buff = malloc(sizeof(char)*TIME_BUFF_SIZE);

    if(stat(targetPath, &sb) != -1)
    {
        locTime = localtime(&sb.st_ctime);   
        strftime(buff, TIME_BUFF_SIZE, "%Y%m%d%H", locTime);
    }
    printf("생성일을 조회할 대상 폴더 :: %s\n", targetPath);
    printf("이 폴더의 생성날자는 다음과 같습니다.:: %s\n", buff);
    return buff;
}
