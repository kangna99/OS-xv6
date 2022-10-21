#include "types.h"
#include "stat.h"
#include "user.h"
#define PNUM 5 // Process 개수
#define PRINT_CYCLE 10000000 // Process가 정보를 출력하는 주기
#define TOTAL_COUNTER 500000000 // Process가 종료할 때 counter 값

void sdebug_func(void)
{
    // counter는 fork()로 생성된 프로세스가 생성 이후 스케줄되어 수행되는 시간
    int pid, counter = 0;


    printf(1, "start sdebug command\n");

    for(int i = 1; i <= PNUM; i++) { // PNUM 값 만큼 프로세스 생성
        pid = fork(); // sdebug가 부모 프로세스가 되고 생성된 프로세스가 자식 프로세스가 됨
        if(pid < 0) break;
        if(pid == 0) {
            int start = uptime();
            weightset(i);
            while(1) {
                counter++; // counter는 프로세스가 수행될 때, 매 clock_tick마다 1씩 증가
                if(counter == PRINT_CYCLE) { // 출력 주기가 되면 해당 프로세스의 정보를 출력
                    int end = uptime();
                    printf(1, "PID: %d, WEIGHT: %d, TIMES: %d ms\n", getpid(), i, (end - start)*10);
                }
                if(counter == TOTAL_COUNTER) { // TOTAL_COUNTER을 전부 소모하면 프로세스 종료
                    printf(1, "PID: %d terminated\n", getpid());
                    exit();
                }
            }
        }
    }

    // sdebug에서 생성한 모든 자식 프로세스의 종료를 기다림
    for(int i = 1; i <= PNUM; i++) {
        if(wait() < 0){
            printf(1, "wait stopped early\n");
            exit();
        }
    }

    if(wait() != -1){
        printf(1, "wait got too many\n");
        exit();
    }
    printf(1, "end of sdebug command\n");
}

int main(void)
{
    sdebug_func();
    exit();
}