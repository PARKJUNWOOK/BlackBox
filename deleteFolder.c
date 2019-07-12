#include "function.h"

#define CHECK_PATH  "/home/jk/workspace/blackbox/"

int main(int argc, char* argv[])
{
    /*
        ************************************
        1. 마운트 디바이스 검사 (연결체크, 용량체크)
        - 별도 쓰레드로 러닝하는 방법
        - 쓰레드로인하여 전체프로세스가 죽는것을 방지하기위해서는 삭제.c를 따로 배치러닝시킨다
        ************************************
     */

    char* pathStr;
    // 옵션 지정하지 않았을 때 에러 출력하고 종료
    if (argc == 1) {
        fputs("옵션 미 입력 시 정의된 경로를 이용합니다. %s\n", CHECK_PATH);
    }
    else
    {
        // 옵션 배열의 요소들을 하나씩 입력
        for (int i = 1; i < argc; i++)
        {
            strcat(pathStr, argv[i]);
        }
    }

    do
    {
        MOUNTP *mntSize = getDirSize();     
        printf("현재 마운트 드라이브의 사용량은 %d %% 입니다.\n", mntSize->size.percent);

        // 마운트 드라이버의 유휴공간이 10% 이하일 경우 오래된 영상폴더&파일 삭제처리
        if(mntSize->size.percent < 10)
        {   
            char *oldDir;
            if(strlen(pathStr) == 0)
                oldDir = searchOldDir(CHECK_PATH);
            else
                strcpy(oldDir, pathStr);
            
            
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

    return 0;
}

#include "function.c"