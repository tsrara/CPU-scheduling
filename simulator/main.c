#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct process
{
    int cpu_burst;
    int io_burst;
    
    /* io_unit : I/O burst를 몇번에 나눠서 실행할지 결정.
     
     한번 실행할떄마다
     cpu_per : (CPU Burst time / unit)의 올림값,
     io_per : (IO Burst time / unit)의 올림값 만큼 실행!
     
     예를 들어 CPU burst time이 10이고 IO burst time이 5, mod가 3이라면
     cpu가 4->4->2 unit 진행될때마다 io 2->2->1만큼씩 실행.
    */
    
    int io_unit;
    int arrival;
    int turnaround;
    int waiting;
    int priority;
    int pid;
    
    int cpu_per;
    int io_per;
    
}process;

//Ready Queue
typedef struct RQ
{
    process pr[20];
    int init[20]; //initial arrival time
    int end[20]; //is it end?
    /* 처음 arrival time을 RQ에 따로 저장, 그 이후로는 Waiting queue 직접대신 새 프로세스의 arrival time을 증가시켜서 간접적 i/o 수행 */
}RQ;

//Arrival time for FCFS
void sortArrival(process p[], int n, RQ *r);
void FCFS(process p[], int n);

//CPU burst time for SJF
void sortBurst(process p[], int n, RQ*);
void SJF_NP(process p[],int n);
void SJF_P(process p[],int n);

//Priority for Priority
void sortPriority(process p[], int n, RQ*);
void Priority_NP(process p[],int n);
void Priority_P(process p[],int n);

void disp_table(process p[],int n);

void RR(process p[],int n, int QT);
float cal_avgwt(process p[],int a);
float cal_avgtat(process p[],int b);
void display_table(process p[], int n);

void display_table(process p[],int n)
{
    int i;
    printf("\n\nPID\tARR\tCPU\tI/O\tUNI\t PRI");
    for(i=0; i<n; i++)
    {
        printf("\n %d \t %d \t %d \t %d \t %d \t %d",p[i].pid, p[i].arrival,p[i].cpu_burst,p[i].io_burst,p[i].io_unit, p[i].priority);
    }
    printf("\n\n");
}

//arrival time으로 Ready Queue 정렬
void sortArrival(process p[],int n, RQ *r){
    int i,j;
    process temp;
    int tmp_end, tmp_init;
    
    //Ready Queue에 값 쓰기 및 초기화
    for(i=0; i<n; i++){
        r->pr[i] = p[i];
    }
    
    //Ready Queue에 initial Process를 Arrival Time 순서로 정렬
    for(i=0; i<n; i++){
        for(j=i+1; j<n; j++){
            if(r->pr[j].arrival < r->pr[i].arrival){
                temp = r->pr[j];
                tmp_end = r->end[j];
                tmp_init = r->init[j];
                
                r->pr[j] = r->pr[i];
                r->end[j] = r->end[i];
                r->init[j] = r->init[i];
                
                r->pr[i] = temp;
                r->end[i] = tmp_end;
                r->init[i] = tmp_init;
            }
        }
    }    //return 값 : Ready Queue
}

//CPU Burst time으로 Ready Queue 정렬
void sortBurst(process p[],int n, RQ *r){
    int i,j;
    process temp;
    int tmp_end, tmp_init;
    
    //Ready Queue에 값 쓰기 및 초기화
    for(i=0; i<n; i++){
        r->pr[i] = p[i];
    }
    
    //Ready Queue에 initial Process를 Arrival Time 순서로 정렬
    for(i=0; i<n; i++){
        for(j=i+1; j<n; j++){
            if(r->pr[j].cpu_burst < r->pr[i].cpu_burst){
                temp = r->pr[j];
                tmp_end = r->end[j];
                tmp_init = r->init[j];
                
                r->pr[j] = r->pr[i];
                r->end[j] = r->end[i];
                r->init[j] = r->init[i];
                
                r->pr[i] = temp;
                r->end[i] = tmp_end;
                r->init[i] = tmp_init;
            }
        }
    }    //return 값 : Ready Queue
}

void sortPriority(process p[],int n, RQ *r){
    int i,j;
    process temp;
    int tmp_end, tmp_init;
    
    //Ready Queue에 값 쓰기 및 초기화
    for(i=0; i<n; i++){
        r->pr[i] = p[i];
    }
    
    //Ready Queue에 initial Process를 Priority 순서로 정렬
    for(i=0; i<n; i++){
        for(j=i+1; j<n; j++){
            if(r->pr[j].priority < r->pr[i].priority){
                temp = r->pr[j];
                tmp_end = r->end[j];
                tmp_init = r->init[j];
                
                r->pr[j] = r->pr[i];
                r->end[j] = r->end[i];
                r->init[j] = r->init[i];
                
                r->pr[i] = temp;
                r->end[i] = tmp_end;
                r->init[i] = tmp_init;
            }
        }
    }    //return 값 : Ready Queue

}

void FCFS(process p[],int n){
    int i = 0;
    int j = 0;
    int count = 0;
    RQ r;
    int sum = 0; //현재 진행상황. Finish Time Check for Turnaround time
    
    
    //Ready Queue에 initial Process를 Arrival Time 순서로 정렬
    sortArrival(p, n, &r);
    
    //Ready Queue & unit initialize
    for(j = 0; j < n; j++){
        r.init[j] = p[j].arrival;
        r.end[j] = 0;
    }
    
    
    printf("FCFS : ");
    //이제부터 Ready Queue(r.pr[i]) 사용
    
    i = 0;
    while(count != n){
        //이 프로세스가 이미 종료된 프로세스가 아니라면
        if(r.end[i] != 1){
            //process가 아직 도착하지 않았을떄, 가장 앞에 도착한 애가 올때까지 기다림
            if(sum < r.pr[i].arrival){
                for(j = 0; j < r.pr[i].arrival - sum; j++){
                    printf("z");
                }
            }
            //CPU burst time이 딱 나누어떨어지지 않을때 : cpu_unit의 마지막(나머지)값
            if(r.pr[i].cpu_per > r.pr[i].cpu_burst){
                r.pr[i].cpu_per = r.pr[i].cpu_burst;
            }
            
            
            //현재 프로세스의 CPU burst를 화면에 출력
            for(j = 0; j < r.pr[i].cpu_per; j++){
                printf("%d", r.pr[i].pid);
            }
            
            //시간 증가, for turnaround time
            sum += r.pr[i].cpu_per;
            
            //remaining burst로 update
            r.pr[i].cpu_burst -= r.pr[i].cpu_per;
            
////////////////////waiting queue//////////
            //IO burst time이 딱 나누어떨어지지 않을때 : io_unit의 마지막(나머지)값
            if(r.pr[i].io_per > r.pr[i].io_burst){
                r.pr[i].io_per = r.pr[i].io_burst;
            }
            
            r.pr[i].io_burst -= r.pr[i].io_per;
            
            //업데이트 된 새로운 프로세스가 진행된 프로세스인 RQ[i]를 대체
            r.pr[i].arrival = sum + r.pr[i].io_per;
            
////////////////////waiting queue//////////
            
            
            //process terminate
            if((r.pr[i].cpu_burst == 0) && (r.pr[i].io_burst == 0)){
                //종료 flag
                r.end[i] = 1;
                r.pr[i].turnaround = sum - r.init[i];
                count ++;
            }
            
        }//r.end[i] != 1
        
        i = (i+1)%n;
        
        //한바퀴 다 돌았으면 다시 sorting (Waiting Queue 구현용)
        if (i == 0){
            sortArrival(r.pr, n, &r);
        }
    }//while
    
    printf("\n\n");
    
}

void SJF_NP(process p[],int n){
    int i = 0;
    int j = 0;
    int count = 0;
    RQ r;
    int sum = 0; //현재 진행상황. Finish Time Check for Turnaround time
    
    
    //Ready Queue에 initial Process를 CPU Burst 순서로 정렬
    sortBurst(p, n, &r);
    
    //Ready Queue & unit initialize
    for(j = 0; j < n; j++){
        r.init[j] = p[j].arrival;
        r.end[j] = 0;
    }
    
    
    printf("FCFS : ");
    //이제부터 Ready Queue(r.pr[i]) 사용
    
    i = 0;
    while(count != n){
        //이 프로세스가 이미 종료된 프로세스가 아니라면
        if(r.end[i] != 1){
            
            
            
            //process가 아직 도착하지 않았을떄, 가장 앞에 도착한 애가 올때까지 기다림
            //가장 앞에 도착한 애 찾는 코드
            
            if(sum < r.pr[i].arrival){
                for(j = 0; j < r.pr[i].arrival - sum; j++){
                    printf("z");
                }
            }
            
            
            
            //CPU burst time이 딱 나누어떨어지지 않을때 : cpu_unit의 마지막(나머지)값
            if(r.pr[i].cpu_per > r.pr[i].cpu_burst){
                r.pr[i].cpu_per = r.pr[i].cpu_burst;
            }
            
            
            //현재 프로세스의 CPU burst를 화면에 출력
            for(j = 0; j < r.pr[i].cpu_per; j++){
                printf("%d", r.pr[i].pid);
            }
            
            //시간 증가, for turnaround time
            sum += r.pr[i].cpu_per;
            
            //remaining burst로 update
            r.pr[i].cpu_burst -= r.pr[i].cpu_per;
            
            ////////////////////waiting queue//////////
            //IO burst time이 딱 나누어떨어지지 않을때 : io_unit의 마지막(나머지)값
            if(r.pr[i].io_per > r.pr[i].io_burst){
                r.pr[i].io_per = r.pr[i].io_burst;
            }
            
            r.pr[i].io_burst -= r.pr[i].io_per;
            
            //업데이트 된 새로운 프로세스가 진행된 프로세스인 RQ[i]를 대체
            r.pr[i].arrival = sum + r.pr[i].io_per;
            
            ////////////////////waiting queue//////////
            
            
            //process terminate
            if((r.pr[i].cpu_burst == 0) && (r.pr[i].io_burst == 0)){
                //종료 flag
                r.end[i] = 1;
                r.pr[i].turnaround = sum - r.init[i];
                count ++;
            }
            
        }//r.end[i] != 1
        
        i = (i+1)%n;
        
        //한바퀴 다 돌았으면 다시 sorting (Waiting Queue 구현용)
        if (i == 0){
            sortBurst(r.pr, n, &r);
        }
    }//while
    
    printf("\n\n");
}

int main(){
    process p[20];
    //float avg_WT, avg_TAT;

    int i = 0;
    int n;
    srand((unsigned int)time(NULL));
    
    printf("\nEnter total number of process : ");
    scanf("%d", &n);
    
////////Configuration
    for(i = 0; i < n; i++)
    {
        //arrival : 0~5
        p[i].arrival = rand()%6;
        
        //cpu_burst : 5~9
        p[i].cpu_burst = rand()%5 + 5;
        
        //io_burst : 0~9
        p[i].io_burst = rand()%5 + 5;
        
        //io_unit : 1~3
        p[i].io_unit = rand()%3 + 1;
        
        //초기화
        p[i].turnaround = 0;
        p[i].waiting = 0;
        
        //한번 실행때 진행되는 unit
        p[i].cpu_per = ((p[i].cpu_burst + p[i].io_unit - 1)/p[i].io_unit);
        p[i].io_per = ((p[i].io_burst + p[i].io_unit - 1)/p[i].io_unit); //올림값
        
        p[i].pid = i;
        
        //priority : 1~99 (숫자가 겹치는걸 최대한 방지하기 위해서)
        p[i].priority = rand()%100;
    }
    
    display_table(p, n);
    ///Fisrt Come First Served
    FCFS(p, n);
    
    return 0;
}
