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
        - 별도 쓰레드로 러닝하는 방법
        - 쓰레드로인하여 전체프로세스가 죽는것을 방지하기위해서는 삭제.c를 따로 배치러닝시킨다
        ************************************
     */

    /*
        * Thread 버전
        * 용량 체크 후 삭제 로직 수행 판별 
     */
    pthread_t pThread[NUM_THREADS];
    int thrId;
    int status;
    char th1[] = "##### Delete Thread #####";
    char th2[] = "##### Video Rec Thread #####";

    //void deleteProc(void* data)
    thrId = pthread_create(&pThread[0], NULL, deleteProc, (void*)th1);
    if(thrId < 0)
    {
        perror("pThread[0] Thread 생성에 실패하였습니다!!\n");
        exit(0);
    }
    pthread_join(pThread[0], (void **)&status);


    /*
        * 일반 함수 버전
        * 해당 함수를 별도의 파일로 생성하여 crontab 을 이용하여 매시간 실행시킨다
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
        - 폴더생성 및 REC는 폴더생성이 일어난 뒤 REC를 수행해야함
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

            printf("##### This Folder Used... delete Folder and new Create Folder \n");
            
            // folder delete and new Create Folder
            int chkFlag = 0;
            chkFlag = delDirFile(currTime);
            printf("chkFlag :: %d\n", chkFlag);

            if(chkFlag != 1)
            {
                printf("##### Dir Delete Error!! \n");
                exit(0);
            }
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
           
           int key;
           int count = 1;
           int retCode;
           do
           {
                // int ch;  
                // for(; !(ch=='\n');){  
                //     ch = getch();  
                //     printf("키 입력을 감지했습니다. 프로그램을 종료합니다. \n", ch);
                //     exit(0);
                // }

                printf("##### %d 회차 녹화를 시작합니다. #####\n", count);
                count++;
                retCode = system("python /home/nvidia/test/rec_video.py");
                if(retCode != 0)
                {
                    perror(".py systemCall Error!!");
                    exit(0);
                }

                sleep(600);
           } while (1);
           
        }   
    }
    return 0;
}


#include "function.c"
