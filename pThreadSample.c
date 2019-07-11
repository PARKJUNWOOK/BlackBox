#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>  

#define NUM_THREADS     3

void *t_function(void *data);
int getch(void);

int main()
{
    /*
    int key;
    int count = 1;
    int retCode;
    printf("##### %d 회차 녹화를 시작합니다. #####\n", count);
    retCode = system("python /home/jk/workspace/blackbox/hello.py");

    printf("retCode :: %d\n", retCode);

    do
    {
        int ch;  
        for(; !(ch=='\n');){  
            ch = getch();  
            printf("키 입력을 감지했습니다. 프로그램을 종료합니다. \n", ch);
            exit(0);
        }
    } while (1);
     */

    pthread_t p_thread[2];
    int thr_id;
    int status;
    char p1[] = "thread_1";   // 1번 쓰레드 이름
    char p2[] = "thread_2";   // 2번 쓰레드 이름
    char pM[] = "thread_m";   // 메인 쓰레드 이름
 
 
    sleep(1);  // 2초 대기후 쓰레드 생성
 
    // ① 1번 쓰레드 생성
    // 쓰레드 생성시 함수는 t_function
    // t_function 의 매개변수로 p1 을 넘긴다.  
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)p1);
 
    // pthread_create() 으로 성공적으로 쓰레드가 생성되면 0 이 리턴됩니다
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
 
    // ② 2번 쓰레드 생성
    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)p2);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
 
    // ③ main() 함수에서도 쓰레드에서 돌아가고 있는 동일한 함수 실행
    t_function((void *)pM);
 
    // 쓰레드 종료를 기다린다. 
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
 
    printf("언제 종료 될까요?\n");
 
    return 0;
}

/*
    * Thread
 */
void *t_function(void *data)
{
    pid_t pid;            // process id
    pthread_t tid;        // thread id
 
    pid = getpid();
    tid = pthread_self();
 
    char* thread_name = (char*)data;
    int i = 0;
 
    while (i<3)   // 0,1,2 까지만 loop 돌립니다.
    {
        // 넘겨받은 쓰레드 이름과 
        // 현재 process id 와 thread id 를 함께 출력
        printf("[%s] pid:%u, tid:%x --- %d\n", 
            thread_name, (unsigned int)pid, (unsigned int)tid, i);
        i++;
        sleep(1);  // 1초간 대기

    }
}

/*
    * 키보드 입력 감지 getch 구현
 */
int getch(void)
{
    int ch;
    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= (ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;  
    tcsetattr(0, TCSAFLUSH, &buf);  
    ch = getchar();  
    tcsetattr(0, TCSAFLUSH, &save);  

    return ch;  
}

