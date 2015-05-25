#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//define MAX 20


typedef struct process
{
    int cpu_burst;
    int arrival;
    int turnaround;
    int waiting;
    int priority;
    int pid;
    
    //프로그램들은 cpu_per마다 io_per씩 진행
    int cpu_per;
    int io_per;
    
    int io;
    int cpu;
    
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
    printf("\n\nPID\tARR\tCPU\tCUT\tI.O\tPRI");
    for(i=0; i<n; i++)
    {
        printf("\n %d \t %d \t %d \t %d \t %d \t %d",p[i].pid, p[i].arrival,p[i].cpu_burst,p[i].cpu_per,p[i].io_per,p[i].priority);
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

void arrange(Queue *r){
    int i,j;
    process temp;
    //Ready Queue에 initial Process를 Arrival Time 순서로 정렬
    for(i=0; i <= r->num; i++){
        for(j=i+1; j<= r->num; j++){
            if(r->pr[j].end > r->pr[i].end){
                temp = r->pr[j];
                r->pr[j] = r->pr[i];
                r->pr[i] = temp;
            }
        }
    }    //return 값 : Ready Queue
}

void SJF_P(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int i, end_count = 0;
    int delete = 0;
    Queue r,w,c; //ready, waiting, current
    r.num = -1, w.num = -1, c.num = -1; //초기화
    
    //c에 initial process 저장 :D
    for(i=0; i<n; i++){
        c.num++;
        c.pr[i] = p[i];
    }
    
    printf("SJF Preemptive: ");
    while(n != end_count){
        
    //1. waiting queue -> ready queue
        for (i = 0; i <= w.num; i++){
            if(w.pr[i].io == w.pr[i].io_per){
                w.pr[i].io = 0;
                //printf("\n%d가 ready queue로 이동합니다.\n", w.pr[i].pid);
                r.pr[++r.num] = w.pr[i];
                
                //큐에서 삭제
                w.pr[i].end = 0;
                delete++;
            }
            else{
                w.pr[i].io++;
                w.pr[i].arrival++;
            }
        }
        arrange(&w);
        w.num -= delete;
        delete = 0;
        
    //2. 현재 time에 도착한 프로세스를 ready queue로
        for (i = 0; i <= c.num; i++){
            if (time == c.pr[i].arrival)
                r.pr[++r.num] = c.pr[i];
        }
        
        i = r.num;
    //3. ready queue -> waiting queue
        if(i == -1 || r.pr[i].arrival > time)
            printf("z");
        else{
            sortBurst(&r);
            
            r.pr[i].cpu ++;
            r.pr[i].cpu_burst --;
            r.pr[i].arrival ++;
            printf("%d", r.pr[i].pid);
            
            if(r.pr[i].cpu_burst == 0){
                //printf("\n%d가 종료되었습니다.\n", r.pr[i].pid);
                end_count ++;
                r.num--;
            }
            else if(r.pr[i].cpu == r.pr[i].cpu_per){
                r.pr[i].cpu = 0;
                //printf("\n%d가 waiting queue로 이동합니다.\n", r.pr[i].pid);
                w.pr[++w.num] = r.pr[r.num--];
            }
        }
        time++;
    }
    printf("\n\n");
}


void Priority_P(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int i, end_count = 0;
    int delete = 0;
    Queue r,w,c; //ready, waiting, current
    r.num = -1, w.num = -1, c.num = -1; //초기화
    
    //c에 initial process 저장 :D
    for(i=0; i<n; i++){
        c.num++;
        c.pr[i] = p[i];
    }
    
    printf("Priority Preemptive: ");
    while(n != end_count){
        
        //1. waiting queue -> ready queue
        for (i = 0; i <= w.num; i++){
            if(w.pr[i].io == w.pr[i].io_per){
                w.pr[i].io = 0;
                //printf("\n%d가 ready queue로 이동합니다.\n", w.pr[i].pid);
                r.pr[++r.num] = w.pr[i];
                
                //큐에서 삭제
                w.pr[i].end = 0;
                delete++;
            }
            else{
                w.pr[i].io++;
                w.pr[i].arrival++;
            }
        }
        arrange(&w);
        w.num -= delete;
        delete = 0;
        
        //2. 현재 time에 도착한 프로세스를 ready queue로
        for (i = 0; i <= c.num; i++){
            if (time == c.pr[i].arrival)
                r.pr[++r.num] = c.pr[i];
        }
        
        i = r.num;
        //3. ready queue -> waiting queue
        if(i == -1 || r.pr[i].arrival > time)
            printf("z");
        else{
            sortPriority(&r);
            
            r.pr[i].cpu ++;
            r.pr[i].cpu_burst --;
            r.pr[i].arrival ++;
            printf("%d", r.pr[i].pid);
            
            if(r.pr[i].cpu_burst == 0){
                //printf("\n%d가 종료되었습니다.\n", r.pr[i].pid);
                end_count ++;
                r.num--;
            }
            else if(r.pr[i].cpu == r.pr[i].cpu_per){
                r.pr[i].cpu = 0;
                //printf("\n%d가 waiting queue로 이동합니다.\n", r.pr[i].pid);
                w.pr[++w.num] = r.pr[r.num--];
            }
        }
        time++;
    }
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
        
        //cpu_burst : 1~9
        p[i].cpu_burst = rand()%9 + 1;
    
        //priority : 1~20
        p[i].priority = rand()%20 + 1;
        
        p[i].cpu_per = rand()%p[i].cpu_burst + 1;
        p[i].io_per = rand()%5;
        //io_per가 0이면 cpu bound process
        
        //initialize
        p[i].turnaround = 0;
        p[i].waiting = 0;
        p[i].io = 0;
        p[i].cpu = 0;
        p[i].end = 1;
    }
    
    display_table(p,n);
    SJF_P(p,n);
    Priority_P(p,n);
    return 0;
}