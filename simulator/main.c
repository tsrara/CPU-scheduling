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
    //Ready Queue에 initial Process를 end 순서로 정렬
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

void FCFS(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int i, k = 0, flag = 1, end_count = 0;
    int delete = 0, dr = 0;
    float turnaround = 0, waiting = 0;
    Queue r,w,c; //ready, waiting, current
    r.num = -1, w.num = -1, c.num = -1; //초기화
    
    //c에 initial process 저장 :D
    for(i=0; i<n; i++){
        c.num++;
        c.pr[i] = p[i];
    }
    
    printf("FCFS: ");
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
        
        //CPU에 새로운 프로세스를 꺼낼 차례일 때
        if(flag == 1){
            //ready queue에 있던 프로세스 삭제
            arrange(&r);
            r.num -= dr;
            dr = 0;
            
            //새로 sorting
            sortArrival(&r);
            k = r.num;
            flag = 0;
        }
        
        //3. ready queue -> waiting queue
        if(k == -1 || r.pr[k].arrival > time){
            printf("z");
            flag = 1;
        }
        else{
            r.pr[k].cpu ++;
            r.pr[k].cpu_burst --;
            r.pr[k].arrival ++;
            printf("%d", r.pr[k].pid);
            flag = 0;
            
            //끝났을때 k번째 process를 삭제해야함
            if(r.pr[k].cpu_burst == 0){
                //printf("\n%d가 종료되었습니다.\n", r.pr[i].pid);
                end_count ++;
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
                
                turnaround += (time - c.pr[r.pr[k].pid].waiting);
                waiting += (time - c.pr[r.pr[k].pid].waiting - c.pr[r.pr[k].pid].cpu_burst);
            }
            else if(r.pr[k].cpu == r.pr[k].cpu_per){
                r.pr[k].cpu = 0;
                //printf("\n%d가 waiting queue로 이동합니다.\n", r.pr[i].pid);
                w.pr[++w.num] = r.pr[k];
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
            }
        }
        time++;
    }
    printf("\naverage turnaround : %lf", turnaround/n);
    printf("\naverage waiting time : %lf", waiting/n);
    printf("\n\n");
}

void SJF_P(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int i, end_count = 0;
    int delete = 0;
    float turnaround = 0, waiting = 0;
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
                turnaround += (time - c.pr[r.pr[i].pid].waiting);
                waiting += (time - c.pr[r.pr[i].pid].waiting - c.pr[r.pr[i].pid].cpu_burst);
                
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
    printf("\naverage turnaround : %lf", turnaround/n);
    printf("\naverage waiting time : %lf", waiting/n);
    printf("\n\n");
}

void SJF_NP(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int i, k = 0, flag = 1, end_count = 0;
    int delete = 0, dr = 0;
    float turnaround = 0, waiting = 0;
    Queue r,w,c; //ready, waiting, current
    r.num = -1, w.num = -1, c.num = -1; //초기화
    
    //c에 initial process 저장 :D
    for(i=0; i<n; i++){
        c.num++;
        c.pr[i] = p[i];
    }
    
    printf("SJF Non-Preemptive: ");
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
        
        //CPU에 새로운 프로세스를 꺼낼 차례일 때
        if(flag == 1){
            //ready queue에 있던 프로세스 삭제
            arrange(&r);
            r.num -= dr;
            dr = 0;
            
            //새로 sorting
            sortBurst(&r);
            k = r.num;
            flag = 0;
        }
        
        //3. ready queue -> waiting queue
        if(k == -1 || r.pr[k].arrival > time){
            printf("z");
            flag = 1;
        }
        else{
            r.pr[k].cpu ++;
            r.pr[k].cpu_burst --;
            r.pr[k].arrival ++;
            printf("%d", r.pr[k].pid);
            flag = 0;
            
            //끝났을때 k번째 process를 삭제해야함
            if(r.pr[k].cpu_burst == 0){
                //printf("\n%d가 종료되었습니다.\n", r.pr[i].pid);
                end_count ++;
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
                
                turnaround += (time - c.pr[r.pr[k].pid].waiting);
                waiting += (time - c.pr[r.pr[k].pid].waiting - c.pr[r.pr[k].pid].cpu_burst);
            }
            else if(r.pr[k].cpu == r.pr[k].cpu_per){
                r.pr[k].cpu = 0;
                //printf("\n%d가 waiting queue로 이동합니다.\n", r.pr[i].pid);
                w.pr[++w.num] = r.pr[k];
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
            }
        }
        time++;
    }
    printf("\naverage turnaround : %lf", turnaround/n);
    printf("\naverage waiting time : %lf", waiting/n);
    printf("\n\n");
}

void Priority_P(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int i, end_count = 0;
    int delete = 0;
    float waiting = 0, turnaround = 0;
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
                turnaround += (time - c.pr[r.pr[i].pid].waiting);
                waiting += (time - c.pr[r.pr[i].pid].waiting - c.pr[r.pr[i].pid].cpu_burst);
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
    printf("\naverage turnaround : %lf", turnaround/n);
    printf("\naverage waiting time : %lf", waiting/n);
    printf("\n\n");
}

void Priority_NP(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int i, k = 0, flag = 1, end_count = 0;
    int delete = 0, dr = 0;
    float waiting =0, turnaround = 0;
    Queue r,w,c; //ready, waiting, current
    r.num = -1, w.num = -1, c.num = -1; //초기화
    
    //c에 initial process 저장 :D
    for(i=0; i<n; i++){
        c.num++;
        c.pr[i] = p[i];
    }
    
    printf("Priority Non-Preemptive: ");
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
        
        //CPU에 새로운 프로세스를 꺼낼 차례일 때
        if(flag == 1){
            //ready queue에 있던 프로세스 삭제
            arrange(&r);
            r.num -= dr;
            dr = 0;
            
            //새로 sorting
            sortPriority(&r);
            k = r.num;
            flag = 0;
        }
        
        //3. ready queue -> waiting queue
        if(k == -1 || r.pr[k].arrival > time){
            printf("z");
            flag = 1;
        }
        else{
            r.pr[k].cpu ++;
            r.pr[k].cpu_burst --;
            r.pr[k].arrival ++;
            printf("%d", r.pr[k].pid);
            flag = 0;
            
            //끝났을때 k번째 process를 삭제해야함
            if(r.pr[k].cpu_burst == 0){
                //printf("\n%d가 종료되었습니다.\n", r.pr[i].pid);
                end_count ++;
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
                
                turnaround += (time - c.pr[r.pr[k].pid].waiting);
                waiting += (time - c.pr[r.pr[k].pid].waiting - c.pr[r.pr[k].pid].cpu_burst);
            }
            else if(r.pr[k].cpu == r.pr[k].cpu_per){
                r.pr[k].cpu = 0;
                //printf("\n%d가 waiting queue로 이동합니다.\n", r.pr[i].pid);
                w.pr[++w.num] = r.pr[k];
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
            }
        }
        time++;
    }
    printf("\naverage turnaround : %lf", turnaround/n);
    printf("\naverage waiting time : %lf", waiting/n);
    printf("\n\n");
}

//RR은 cut 고려 x, FCFS + time quantum
void RR(process p[], int n){
    //preemptive는 매 시간 check
    int time = 0;
    int q;
    int i, k = 0, flag = 1, end_count = 0;
    int delete = 0, dr = 0;
    float waiting = 0, turnaround = 0;
    Queue r,w,c; //ready, waiting, current
    r.num = -1, w.num = -1, c.num = -1; //초기화
    
    printf("\ntime quantum : ");
    scanf("%d", &q);
    
    
    //c에 initial process 저장 :D
    for(i=0; i<n; i++){
        c.num++;
        c.pr[i] = p[i];
    }
    
    printf("Round Robin: ");
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
        
        //CPU에 새로운 프로세스를 꺼낼 차례일 때
        if(flag == 1){
            //ready queue에 있던 프로세스 삭제
            arrange(&r);
            r.num -= dr;
            dr = 0;
            
            //새로 sorting
            sortArrival(&r);
            k = r.num;
            flag = 0;
        }
        
        //3. ready queue -> waiting queue
        if(k == -1 || r.pr[k].arrival > time){
            printf("z");
            flag = 1;
        }
        else{
            r.pr[k].cpu ++;
            r.pr[k].cpu_burst --;
            r.pr[k].arrival ++;
            printf("%d", r.pr[k].pid);
            flag = 0;
            
            //끝났을때 k번째 process를 삭제해야함
            if(r.pr[k].cpu_burst == 0){
                //printf("\n%d가 종료되었습니다.\n", r.pr[i].pid);
                end_count ++;
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
                
                turnaround += (time - c.pr[r.pr[k].pid].waiting);
                waiting += (time - c.pr[r.pr[k].pid].waiting - c.pr[r.pr[k].pid].cpu_burst);
            }
            else if(r.pr[k].cpu == q){ //time quantum 이랑 비교
                r.pr[k].cpu = 0;
                //printf("\n%d가 waiting queue로 이동합니다.\n", r.pr[i].pid);
                w.pr[++w.num] = r.pr[k];
                r.pr[k].end = 0;
                dr ++;
                flag = 1;
            }
        }
        time++;
    }
    printf("\naverage turnaround : %lf", turnaround/n);
    printf("\naverage waiting time : %lf", waiting/n);
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
        
        //cpu_burst : 2~9
        p[i].cpu_burst = rand()%8 + 2;
    
        //priority : 1~99
        p[i].priority = rand()%99 + 1;
        
        p[i].cpu_per = rand()%(p[i].cpu_burst - 1) + 1;
        p[i].io_per = rand()%5;
        
        //io_per가 0이면 cpu bound process
        if(p[i].io_per == 0)
            p[i].cpu_per = p[i].cpu_burst;
            
        //initialize
        p[i].turnaround = 0;
        p[i].waiting = 0;
        p[i].io = 0;
        p[i].cpu = 0;
        p[i].end = 1;
    }
    
    display_table(p,n);
    FCFS(p,n);
    SJF_P(p,n);
    SJF_NP(p,n);
    Priority_P(p,n);
    Priority_NP(p,n);
    RR(p,n);
    return 0;
}