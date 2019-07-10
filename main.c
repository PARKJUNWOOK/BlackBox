#include "function.h"

int main()
{
    /* function.c 에 선언한 전역변수 참조 */
    extern char* ROOT_FOLDER_PATH;
    extern char* ROOT_FOLDER_PATH2;

    /*
        1. 마운트 디바이스 검사 (연결체크, 용량체크)
            1.1 용량 부족 시 오래된 폴더 및 하위 파일 삭제
            1.2 삭제 후 다시 용량 체크 반복
        2. 폴더생성(YYYYMMDD-HH(24))
        3. 영상 녹화 Call
    */
    
    //int repoSize = getRepoSize("test");
    /*
        ************************************
        1. 마운트 디바이스 검사 (연결체크, 용량체크)
        ************************************
     */
    // MOUNTP *mntSize = getDirSize();     
    // printf("현재 마운트 드라이브의 사용량은 %d %% 입니다.\n", mntSize->size.percent);

    // // 마운트 드라이버의 유휴공간이 10% 이하일 경우 오래된 영상폴더&파일 삭제처리
    // if(mntSize->size.percent < 10)
    // {
    //     printf("ROOT_FOLDER_PATH :: %s\n", ROOT_FOLDER_PATH);
    //     printf("ROOT_FOLDER_PATH2 :: %s\n", ROOT_FOLDER_PATH2);
        
    //     char *oldDir; 
    //     oldDir = searchOldDir(ROOT_FOLDER_PATH2);
        
    //     printf("##### 삭제대상 디렉토리명 :: %s\n", oldDir);

    //     printf("##### 드라이브 내 용량이 부족하여 삭제 프로세스를 수행합니다\n");
    //     printf("##### 삭제 대상 폴더경로 :: %s\n", oldDir);
        
    //     rmdirs(oldDir, 1);

    //     printf("##### %s 하위 파일 및 폴더삭제를 완료 하였습니다.\n", oldDir);
    // }
    // else
    //     printf("현재 마운트 드라이브의 유휴공간이 10 %% 이상입니다!!\n");
    
    /*
        ************************************
        2. 폴더 생성
        ************************************
     */
    boolean flag = FALSE;       // Dir Create Flag   
    while(flag != TRUE)
    {
        // 1. getCurrentTime() param(D:yyyymmdd, T:HH(hour), S:YYYYMMDD_HHMMSS)
        char* currTime = getCurrentTime('T');
        // 2. createDir
        flag = createFolder(currTime);
        
        if(flag == FALSE)       // 폴더 생성 실패 시
        {
            /*
                * NOT COMPLETE  
                * DELETE USED DIR 190702
            */

            // printf("##### This Folder Used... delete Folder and new Create Folder \n");
            
            // // folder delete and new Create Folder
            // int chkFlag = 0;
            // chkFlag = delDirFile(currTime);
            // printf("chkFlag :: %d\n", chkFlag);

            // if(chkFlag != 1)
            // {
            //     printf("##### Dir Delete Error!! \n");
            //     exit(0);
            // }
        }
        else        // 폴더 생성 성공 시
        {
            printf("##### CreateFolder Success!!\n");

            /*
                ************************************
                3. REC .py 외부 라이브러리 호출
                PARAM : 녹화시간(int/sec), 저장경로(--path)
                ************************************
            */
        }   
    }

    

    // 4. REC   
    // .py 로 만들어둔 영상 녹화 소스에 경로인자/촬영시간 정보를 인자로 줄것

    return 0;
}


#include "function.c"
