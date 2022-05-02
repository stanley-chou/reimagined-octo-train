#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>

int size;
int team1;
int team2;
int team3;
int team4;
int i;
int j;

void *team1work(void *arg);
void *team2work(void *arg);
void *team3work(void *arg);
void *team4work(void *arg);
void handler1(int signum);
void handler2(int signum);
void handler3(int signum);
void handler4(int signum);

int main (int argc, char *argv[])
{
    pthread_t *tid[4];
    pthread_attr_t attr;

    /*First check input parameters*/
    if (argc != 5){
        fprintf(stderr, "usage: a.out <integer value> <integer value> <integer value> <integer value>\n" );
        fprintf(stderr, "usage: each integer value is the number of threads you want for team 1,2,3, and 4 in order\n" );
        exit(0);
    }

    /*sizes of each team*/
    team1 = atoi(argv[1]);
    team2 = atoi(argv[2]);
    team3 = atoi(argv[3]);
    team4 = atoi(argv[4]);



    if(team1 <= 0 || team2 <= 0 || team3 <= 0 || team4 <= 0){
        fprintf(stderr, "usage: provided integer value for the number of threads in each team must be greater than 0\n" );
        exit(0);
    }

    size = team1 + team2 + team3 + team4;

    /*Initialize Array of the correct sequence size */
    tid[0] = malloc(team1 * sizeof(pthread_t)); /* array of thread ids*/
    tid[1] = malloc(team2 * sizeof(pthread_t)); 
    tid[2] = malloc(team3 * sizeof(pthread_t)); 
    tid[3] = malloc(team4 * sizeof(pthread_t)); 
    /*Create Threads for team1 */
    pthread_attr_init(&attr);
    
    for (i = 0; i < team1 ; i++){
        pthread_create(&tid[0][i], &attr, team1work, NULL);
    }

 /*Create Threads for team2 */
    for (i = 0; i < team2 ; i++){
        pthread_create(&tid[1][i], &attr, team2work, NULL);
    }

 /*Create Threads for team3 */
    for (i = 0; i < team3 ; i++){
        pthread_create(&tid[2][i], &attr, team3work, NULL);
    }

 /*Create Threads for team4 */
    for (i = 0; i < team4 ; i++){
        pthread_create(&tid[3][i], &attr, team4work, NULL);
    }


 /*Join threads for team1 */
    for(i = 0; i < team1; i++){
        pthread_join(tid[0][i], NULL);
    }
     /*Join threads for team2 */
    for(i = 0; i < team2; i++){
        pthread_join(tid[1][i], NULL);
    }
     /*Join threads for team3 */
    for(i = 0; i < team3; i++){
        pthread_join(tid[2][i], NULL);
    }
     /*Join threads for team4 */
    for(i = 0; i < team4; i++){
        pthread_join(tid[3][i], NULL);
    }

    return 0;

}

void *team1work(void *param){
     /*team 1 handles SIGINT SIGSEGV and SIGSTOP */
    signal(SIGINT, handler1);
    signal(SIGABRT, SIG_IGN);
    signal(SIGILL, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGSEGV, handler1);
    signal(SIGFPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGSTOP, handler1);
    sleep(30);
    return NULL;
}
void *team2work(void *param){
    /*team 2 handles SIGINT SIGABRT and SIGCHLD */
    signal(SIGINT, handler2);
    signal(SIGABRT, handler2);
    signal(SIGILL, SIG_IGN);
    signal(SIGCHLD, handler2);
    signal(SIGSEGV, SIG_IGN);
    signal(SIGFPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    sleep(30);
    return NULL;
}
void *team3work(void *param){
    /*team 3 handles SIGStopT SIGHUP and SIGFPE */
    signal(SIGINT, SIG_IGN);
    signal(SIGABRT, SIG_IGN);
    signal(SIGILL, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGSEGV, SIG_IGN);
    signal(SIGFPE, handler3);
    signal(SIGHUP, handler3);
    signal(SIGSTOP, handler3);
    sleep(30);
    return NULL;
}
void *team4work(void *param){
    /*team 4 handles SIGILL SIGCHLD and SIGSEGV*/
    signal(SIGINT, SIG_IGN);
    signal(SIGABRT, SIG_IGN);
    signal(SIGILL,  handler4);
    signal(SIGCHLD,  handler4);
    signal(SIGSEGV,  handler4);
    signal(SIGFPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    sleep(30);
    return NULL;
}


 /*pthread exit so each thread only catches the signal and reports its number once*/
void handler1(int signum)
{
    printf("Signal Number %d Caught by Thread Number: %ld\n", signum, pthread_self());
    pthread_exit(0); 
}

void handler2(int signum)
{
    printf("Signal Number %d Caught by Thread Number: %ld\n", signum, pthread_self());
    pthread_exit(0);
}

void handler3(int signum)
{
    printf("Signal Number %d Caught by Thread Number: %ld\n", signum, pthread_self());
    pthread_exit(0);
}

void handler4(int signum)
{
    printf("Signal Number %d Caught by Thread Number: %ld\n", signum, pthread_self());
    pthread_exit(0);
}