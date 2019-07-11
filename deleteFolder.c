#include "function.h"

int main()
{
    /*
        ************************************
        1. 마운트 디바이스 검사 (연결체크, 용량체크)
        - 별도 쓰레드로 러닝하는 방법
        - 쓰레드로인하여 전체프로세스가 죽는것을 방지하기위해서는 삭제.c를 따로 배치러닝시킨다
        ************************************
     */
    do
    {
        MOUNTP *mntSize = getDirSize();     
        printf("현재 마운트 드라이브의 사용량은 %d %% 입니다.\n", mntSize->size.percent);

        // 마운트 드라이버의 유휴공간이 10% 이하일 경우 오래된 영상폴더&파일 삭제처리
        if(mntSize->size.percent < 10)
        {
            printf("ROOT_FOLDER_PATH :: %s\n", ROOT_FOLDER_PATH);
            printf("ROOT_FOLDER_PATH2 :: %s\n", ROOT_FOLDER_PATH2);
            
            char *oldDir; 
            oldDir = searchOldDir(ROOT_FOLDER_PATH2);
            
            printf("##### 삭제대상 디렉토리명 :: %s\n", oldDir);

            printf("##### 드라이브 내 용량이 부족하여 삭제 프로세스를 수행합니다\n");
            printf("##### 삭제 대상 폴더경로 :: %s\n", oldDir);
            
            rmdirs(oldDir, 1);

            printf("##### %s 하위 파일 및 폴더삭제를 완료 하였습니다.\n", oldDir);
        }
        else
            printf("현재 마운트 드라이브의 유휴공간이 10 %% 이상입니다!!\n");

        sleep(600);
    } while (1);
}

#include "function.c"