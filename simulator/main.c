#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//define MAX 20


typedef struct process
{
    int cpu_burst;
    int io_burst;
    int arrival;
    int turnaround;
    int waiting;
    int priority;
    int pid;
    
///////////////additional
    
    /* unit : I/O burst를 몇번에 나눠서 실행할지 결정.
     
     한번 실행할떄마다
     cpu_per : (CPU Burst time / unit)의 올림값,
     io_per : (IO Burst time / unit)의 올림값 만큼 실행!
     
     예를 들어 CPU burst time이 10이고 IO burst time이 5, mod가 3이라면
     cpu가 4->4->2 unit 진행될때마다 io 2->2->1만큼씩 실행.
    
    */
    int io_unit, cpu_unit;
    int cpu_per;
    int io_per;
    int end;
    
}process;

//Ready Queue
typedef struct Queue
{
    process pr[20];
    int num;
}Queue;

process temp;

//Arrival time for FCFS
void sortArrival(Queue *r);
//CPU Burst time for SJF
void sortBurst(Queue *r);
//Priority for Priority
void sortArrival(Queue *r);

void display_table(process p[], int n);

void FCFS(process p[], int n);
void SJF_NP(process p[], int n);
void SJF_P(process p[], int n);
void Priority_NP(process p[], int n);
void Priority_P(process p[], int n);
void RR(process p[], int n);



void display_table(process p[], int n){
    int i;
    printf("\n\nPID\tARR\tCPU\tI/O\tUNI\t PRI");
    for(i=0; i<n; i++)
    {
        printf("\n %d \t %d \t %d \t %d \t %d \t %d",p[i].pid, p[i].arrival,p[i].cpu_burst,p[i].io_burst,p[i].io_unit, p[i].priority);
    }
    printf("\n\n");
}

//arrival time으로 Ready Queue 정렬. 제일 작은애가 제일 뒤로 :D
void sortArrival(Queue *r){
    int i,j;
    process temp;
    
    //Ready Queue에 initial Process를 Arrival Time 순서로 정렬
    for(i = 0; i <= r->num; i++){
        for(j = i+1; j <= r->num; j++){
            if(r->pr[j].arrival > r->pr[i].arrival){
                temp = r->pr[j];
                r->pr[j] = r->pr[i];
                r->pr[i] = temp;
            }
        }
    }    //return 값 : Ready Queue
}

//CPU Burst time으로 Ready Queue 정렬
void sortBurst(Queue *r){
    int i,j;
    process temp;
    
    //Ready Queue에 initial Process를 Arrival Time 순서로 정렬
    for(i=0; i <= r->num; i++){
        for(j=i+1; j <= r->num; j++){
            if(r->pr[j].cpu_burst > r->pr[i].cpu_burst){
                temp = r->pr[j];
                r->pr[j] = r->pr[i];
                r->pr[i] = temp;
            }
        }
    }
    //return 값 : Ready Queue
}

//Waiting Queue arrange
void arrange(Queue *r){
    int i,j;
    int k = 0;
    process temp;
    
    //Ready Queue에 initial Process를 end 순서로 정렬
    for(i=0; i <= r->num; i++){
        k++;
        for(j=i+1; j<= r->num; j++){
            if(r->pr[j].end > r->pr[i].end){
                temp = r->pr[j];
                r->pr[j] = r->pr[i];
                r->pr[i] = temp;
            }
        }
    }
    
    //종료된 프로세스(맨 뒤에 몰려있음)가 있으면 제거
    for(i=0; i<=k; i++){
        if(r->pr[i].end == 0){
        r->num --;
        }
    }
    //return 값 : Ready Queue
}

//Priority으로 Ready Queue 정렬
void sortPriority(Queue *r){
    int i,j;
    process temp;
    
    //Ready Queue에 initial Process를 Arrival Time 순서로 정렬
    for(i=0; i <= r->num; i++){
        for(j=i+1; j<= r->num; j++){
            if(r->pr[j].priority > r->pr[i].priority){
                temp = r->pr[j];
                r->pr[j] = r->pr[i];
                r->pr[i] = temp;
            }
        }
    }    //return 값 : Ready Queue
}

void SJF_P(process p[], int n){
    Queue r, w; //ready queue, waiting queue
    int time = 0; //현재 시간
    int end_count = 0;
    int i;
    
    r.num = -1, w.num = -1; //위치 초기화.
    
    
//0. initiallize Ready Queue
    for(i = 0; i < n; i++){
        ++r.num;
        r.pr[i] = p[i];
    }
    
    printf("SJF Preemptive : ");
    while (end_count != n){
        
        
        printf("%d %d\n", r.num, w.num);
        printf("%d\n", end_count);
//1. Waiting queue 실행 및 Ready Queue로 이동
        //각각 1초씩 빼주고(time passing), 끝난 애를 ready queue에 넣기
        //각각 서로 다른 i/o device라고 가정(lock 구현 x)
        for (i = 0; i <= w.num; i++){
            w.pr[i].io_burst --;
            w.pr[i].io_per --;
            w.pr[i].arrival ++;
        
        //IO 할당량 채운 애(io_per = 0)을 찾아서!
            if(w.pr[i].io_per == 0){
                w.pr[i].io_unit --;
                
                //(1) I/O의 종료
                if(w.pr[i].io_unit == 0 || w.pr[i].io_burst == 0){
                    //(2) 프로세스의 종료
                    if(w.pr[i].cpu_unit == 0 || w.pr[i].cpu_burst == 0){
                        end_count ++;
                        w.pr[i].end = 0;
                    }
                    else{
                        //waiting queue에서 빠지고 ready queue로
                        r.pr[++r.num] = w.pr[i];
                        w.pr[i].end = 0;
                    }
                }
                else{//(3) 다음 I/O 대비
                    w.pr[i].io_per = (w.pr[i].io_burst + w.pr[i].io_unit - 1)/w.pr[i].io_unit;
                    r.pr[++r.num] = w.pr[i];
                    w.pr[i].end = 0;
                }
            }
        }
        arrange(&w);
        
//2. Ready Queue에서 나올 값 정하기
        //shortest job first로 정렬!
        
        sortBurst(&r);
        //shortest job이 아직 오지 않았다면
        if (time < r.pr[r.num].arrival){
            
            //arrival time으로 재정렬 -> 제일 빨리 온애를 일단 수행!
            sortArrival(&r);
            
            //arrival time이 가장 작은아이도 아직 오지 않았다면
            if (time < r.pr[r.num].arrival || r.num == -1){
                printf("z"); //wait 1 time
            }
            else{
                //2. arrival time으로 Process 실행
                printf("%d", r.pr[r.num].pid);
                r.pr[r.num].cpu_burst --;
                r.pr[r.num].cpu_per --;
                r.pr[r.num].arrival ++;
                
                //I/O 발생 체크
                if(r.pr[r.num].cpu_per == 0){
                    r.pr[r.num].cpu_unit --;
                    //(1) CPU burst의 종료
                    if(r.pr[r.num].cpu_unit == 0 || r.pr[r.num].cpu_burst == 0){
                        //(2) 프로세스의 종료
                        if(r.pr[r.num].io_unit == 0 || r.pr[r.num].io_burst == 0){
                            end_count ++;
                            r.pr[r.num].end = 0;
                            arrange(&r);
                        }
                        else{
                            //waiting queue에서 빠지고 ready queue로
                            w.pr[++w.num] = r.pr[r.num--];
                        }
                    }
                    else{//(3) 다음 execution 대비
                        r.pr[r.num].cpu_per = (r.pr[r.num].cpu_burst + r.pr[r.num].cpu_unit - 1)/r.pr[r.num].cpu_unit;
                        w.pr[++w.num] = r.pr[r.num--];
                    }
                }
            }//arrival time 끝
        }
        
        //shortest job 도착 -> 프로세스 실행
        else{
            //ready queue에서 shortest job 실행
            printf("%d", r.pr[r.num].pid);
            r.pr[r.num].cpu_burst --;
            r.pr[r.num].cpu_per --;
            r.pr[r.num].arrival ++;
            //I/O 발생 체크
            if(r.pr[r.num].cpu_per == 0){
                r.pr[r.num].cpu_unit --;
                
                //(1) CPU burst의 종료
                if(r.pr[r.num].cpu_unit == 0 || r.pr[r.num].cpu_burst == 0){
                    //(2) 프로세스의 종료
                    if(r.pr[r.num].io_unit == 0 || r.pr[r.num].io_burst == 0){
                        end_count ++;
                        r.pr[r.num].end = 0;
                        arrange(&r);
                    }
                    else{
                        //ready queue에서 빠지고 waiting queue로
                        w.pr[++w.num] = r.pr[r.num--];
                    }
                }
                else if (r.pr[r.num].io_unit ==0){//(3) 다음 execution 대비
                    r.pr[r.num].cpu_per = (r.pr[r.num].cpu_burst + r.pr[r.num].cpu_unit - 1)/r.pr[r.num].cpu_unit;
                }
                r.pr[r.num].cpu_per = (r.pr[r.num].cpu_burst + r.pr[r.num].cpu_unit - 1)/r.pr[r.num].cpu_unit;
                w.pr[++w.num] = r.pr[r.num--];
                
            }
        }
        time ++; //여기까지 1초동안 일어난 상황
    }//while문 끝
    
    printf("\n\n");
}

int main(){
    process p[20];
    int i = 0;
    int n;
    srand((unsigned int)time(NULL));
    
    printf("\nEnter total number of process : ");
    scanf("%d", &n);
    
////////Configuration
    for(i = 0; i < n; i++)
    {
        
        p[i].pid = i;
        
        //arrival : 0~5
        p[i].arrival = rand()%6;
        
        //cpu_burst : 3~9
        p[i].cpu_burst = rand()%7 + 3;
    
        //io_burst : 3~9
        p[i].io_burst = rand()%7 + 3;
        
        //io_unit : 1~3
        p[i].io_unit = rand()%3 +1;
        p[i].cpu_unit = p[i].io_unit;
        
        //priority : 1~20
        p[i].priority = rand()%20 + 1;
        
        //initialize
        p[i].turnaround = 0;
        p[i].waiting = 0;
        p[i].cpu_per = (p[i].cpu_burst + p[i].cpu_unit - 1)/p[i].cpu_unit;
        p[i].io_per = (p[i].io_burst + p[i].io_unit - 1)/p[i].io_unit;
        p[i].end = 1;

    }
    
    display_table(p,n);
    SJF_P(p,n);
    return 0;
}