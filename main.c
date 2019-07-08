#include "function.h"

int main()
{

    /*
        1. check device
        2. getCurrentTime > createFolder(char* currTime)
        3. 
    */
    
    // 1. check device
    /*
        * 
    */

    // 2. getCurrentTime > createFolder(char* currTime)
    // boolean flag = FALSE;       // Dir Create Flag   
    // while(flag != TRUE)
    // {
    //     // 1. getCurrentTime() param(D:yyyymmdd, T:HH(hour), S:YYYYMMDD_HHMMSS)
    //     char* currTime = getCurrentTime('T');
    //     // 2. createDir
    //     flag = createFolder(currTime);

    //     if(flag == FALSE)       // flag == FALSE
    //     {
    //         printf("hhh\n");
    //         /*
    //             * NOT COMPLETE  
    //             * DELETE USED DIR 190702
    //         */

    //         // printf("##### This Folder Used... delete Folder and new Create Folder \n");
            
    //         // // folder delete and new Create Folder
    //         // int chkFlag = 0;
    //         // chkFlag = delDirFile(currTime);
    //         // printf("chkFlag :: %d\n", chkFlag);

    //         // if(chkFlag != 1)
    //         // {
    //         //     printf("##### Dir Delete Error!! \n");
    //         //     exit(0);
    //         // }
    //     }
    //     else        // flag == TRUE
    //     {
    //         printf("##### CreateFolder Success!!\n");
    //     }   
    // }

    // 3. getRepoSize -> chkRepo
    // 지금 환경에서는 마운트된 SD카드의 사이즈를 검색이 아니고 특정 파일시스템의 용량을 확인한다.
    //int repoSize = getRepoSize("test");
    unsigned long mntSize = getDirSize();     // statfs를 이용하여 마운트된 드라이브 사이즈 출력
    printf("size :: %d", mntSize);

    // 4. REC   

    return 0;
}


#include "function.c"