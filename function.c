#include "function.h"

/*
* define list
*/
#define TIME_BUFF_SIZE  1024

//char* ROOT_FOLDER_PATH = "/workspace/blackbox/";
char* ROOT_FOLDER_PATH = "/home/jk/workspace/blackbox/";
char REAL_TIME_FOLDER_PATH[256] = "";

// 마운트 디바이스 고정주소
const char *MMOUNT = "/proc/mounts";

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
    * 파일객체 오픈
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
    * 파일시스템 내 파티션 정보를 추출한다
*/
MOUNTP *dfget(MOUNTP *MP)
{
    char buf[256];
    char *bname;
    char null[16];
    struct statfs lstatfs;
    struct stat lstat; 
    int is_root = 0;

    // /proc/mounts로 부터 마운트된 파티션의 정보를 얻어온다.
    while(fgets(buf, 255, MP->fp))
    {
        is_root = 0;
        sscanf(buf, "%s%s%s",MP->devname, MP->mountdir, MP->fstype);
         if (strcmp(MP->mountdir,"/") == 0) is_root=1;
        if (stat(MP->devname, &lstat) == 0 || is_root)
        {
            if (strstr(buf, MP->mountdir) && S_ISBLK(lstat.st_mode) || is_root)
            {
                // 파일시스템의 총 할당된 크기와 사용량을 구한다.        
                statfs(MP->mountdir, &lstatfs);
                MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024); 
                MP->size.avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024); 
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
    * mntent를 사용하지 않은 파일시스템 사이즈 리턴
*/
unsigned long getDirSize()
{
    MOUNTP *MP;
    if ((MP=dfopen()) == NULL)
    {
        perror("error");
        return 1;
    }
    else if(dfget(MP))
    {
        printf("출력합니다 ::::: \n");
        printf("%-14s%-20s%10lu%10lu\n", MP->mountdir, MP->devname, MP->size.blocks, MP->size.avail);

        return MP->size.blocks;
                                
    }
    
    // while(1)
    // {
    //     while(dfget(MP))
    //     {
    //         printf("%-14s%-20s%10lu%10lu\n", MP->mountdir, MP->devname, 
    //                             MP->size.blocks,
    //                             MP->size.avail);
    //     }
    //     printf("=========================\n\n");
    //     sleep(1);
    // }
}
