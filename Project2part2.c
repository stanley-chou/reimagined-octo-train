#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include<stdbool.h>  

int size;
int team1;
int team2;
int team3;
int team4;
int i;
int j;
int teamInterval;
int intraTeamInterval;
int team1dif, team2dif, team3dif, team4dif;
int *arrayRand;
int *bulletin1, *bulletin2, *bulletin3, *bulletin4;

void *team1work(void *arg);
void *team2work(void *arg);
void *team3work(void *arg);
void *team4work(void *arg);
void handler1(int signum);
void handler2(int signum);
void handler3(int signum);
void handler4(int signum);
bool isprime(int number);
void calculate(int number, int team);
void sieve1(int number);
void sieve2(int number);
void sieve3(int number);
void sieve4(int number);

int main (int argc, char *argv[])
{
    pthread_t *tid[4];
    pthread_attr_t attr;

    int *startpoints[4];
    
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

    /*random Array of size 100000 */
    arrayRand = (int*)malloc(100000 * sizeof(int));
    
    teamInterval = 100000/4;

    team1dif = teamInterval/team1;
    team2dif = teamInterval/team2;
    team3dif = teamInterval/team3;
    team4dif = teamInterval/team4;

    for (i = 0; i < 100000; i++){
        arrayRand[i] = rand()%49001 + 1000;    /*Numbers can be between 1000 and 50000 inclusive*/
    }

    

    startpoints[0] = malloc(team1 * sizeof(int)); /* array of thread ids*/
    startpoints[1] = malloc(team2 * sizeof(int)); 
    startpoints[2] = malloc(team3 * sizeof(int)); 
    startpoints[3] = malloc(team4 * sizeof(int)); 

    /*Initialize Array of the correct sequence size */
    tid[0] = malloc(team1 * sizeof(pthread_t)); /* array of thread ids*/
    tid[1] = malloc(team2 * sizeof(pthread_t)); 
    tid[2] = malloc(team3 * sizeof(pthread_t)); 
    tid[3] = malloc(team4 * sizeof(pthread_t));  

    /*Initialize Bulletins, first row is prime number, second is number of primes before it, and third is number of occurrences */
    

    bulletin1 = malloc(10000 * sizeof(int)); /* there are 10000 primes before 100000, */

    bulletin2 = malloc(10000 * sizeof(int)); /* there are 10000 primes before 100000*/

    bulletin3 = malloc(10000 * sizeof(int)); /* there are 10000 primes before 100000*/

    bulletin4 = malloc(10000 * sizeof(int)); /* there are 10000 primes before 100000*/

    /*Create Threads for team1 */
    pthread_attr_init(&attr);
    
    for (i = 0; i < team1 ; i++){
        startpoints[0][i] = i*(teamInterval/team1);
        pthread_create(&tid[0][i], &attr, team1work, (void *)&startpoints[0][i]);
    }

 /*Create Threads for team2 */
    for (i = 0; i < team2 ; i++){
        startpoints[1][i] = teamInterval + i*(teamInterval/team2);
        pthread_create(&tid[1][i], &attr, team2work, (void *)&startpoints[1][i]);
    }

 /*Create Threads for team3 */
    for (i = 0; i < team3 ; i++){
        startpoints[2][i] =2*teamInterval + i*(teamInterval/team3);
        pthread_create(&tid[2][i], &attr, team3work, (void *)&startpoints[2][i]);
    }

 /*Create Threads for team4 */
    for (i = 0; i < team4 ; i++){
        startpoints[3][i] = 3*teamInterval + i*(teamInterval/team4);
        pthread_create(&tid[3][i], &attr, team4work, (void *)&startpoints[3][i]);
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
    int start = * (int *) param;  
    int end = start + team1dif; 
    int j;
    for( j = start; j < end; j++){
        if(isprime(arrayRand[j])){
            printf("%d ", arrayRand[j]);
            continue;
        }
        else{
            printf("%d ", arrayRand[j]);
            calculate(arrayRand[j], 1);
        }
    }
    pthread_exit(0);
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
    int start = * (int *) param; 
    int end = start + team2dif;  
    for(int i = start; i < end; i++){
        if(isprime(arrayRand[i])){
            continue;
        }
        else{
            calculate(arrayRand[i], 2);
        }
    }
    pthread_exit(0);
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
    int start = * (int *) param; 
    int end = start + team3dif; 
    for(int i = start; i < end; i++){
        if(isprime(arrayRand[i])){
            continue;
        }
        else{
            calculate(arrayRand[i], 3);
        }
    }
    pthread_exit(0);   
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
    int start = * (int *) param;
    int end = start + team4dif;  
    for(int i = start; i < end; i++){
        if(isprime(arrayRand[i])){
            continue;
        }
        else{
            calculate(arrayRand[i], 4);
        }
    } 
    pthread_exit(0);
    return NULL;
}


bool isprime(int number)
{
    int flag = 0;
    int j = 0;
    if (number == 0 || number == 1){
        flag = 1;
    }
    for (j = 2; j <= number/2; ++i)
    {
        if(number%j == 0){
            flag = 1;
            break;
        }
    }
    if(flag == 0){
        return false;
    }
    else if (flag == 1){
        return true;
    }
    return false;
}

/*adds prime to the array*/
void calculate(int number,  int team)
{
    int current;
    int numbehind;
    int largestprime;
    if(team == 1){
    /*check if prime exists already*/
        for(current = 0; current < 100000; current++){
            if (number < bulletin1[current])
            {
               largestprime = bulletin1[current-1];
               numbehind = current -2;
               return;
            }
        }
    /*check if not update the list of primes*/
        sieve1(number);
    }
    else if (team == 2){
        for(current = 0; current < 100000; current++){
            if (number < bulletin2[current])
            {
               largestprime = bulletin2[current-1];
               numbehind = current -2;
               return;
            }
        }
        sieve2(number);
    }
    else if (team == 3){
        for(current = 0; current < 100000; current++){
            if (number < bulletin3[current])
            {
               largestprime = bulletin3[current-1];
               numbehind = current -2;
               return;
            }
        }
        sieve3(number);
    }
    else if (team == 4){
        for(current = 0; current < 100000; current++){
            if (number < bulletin4[current])
            {
               largestprime = bulletin4[current-1];
               numbehind = current -2;
               return;
            }
        }
        sieve4(number);
    }
    return;
}

void sieve1(int number)
{
    bool prime[number + 1];
    memset(prime, true, sizeof(prime));
    int index = 0;
    for (int p = 2; p * p <= number; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.
            for (int i = p * p; i <= number; i += p)
                prime[i] = false;
        }
    }
    for (int p = 2; p <= number; p++)
        if (prime[p]){
            bulletin1[index] = p;
            index = index + 1;
        }
}

void sieve2(int number)
{
    bool prime[number + 1];
    memset(prime, true, sizeof(prime));
    int index = 0;
    for (int p = 2; p * p <= number; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.
            for (int i = p * p; i <= number; i += p)
                prime[i] = false;
        }
    }
    for (int p = 2; p <= number; p++)
        if (prime[p]){
            bulletin2[index] = p;
            index = index + 1;
        }
}

void sieve3(int number)
{
    bool prime[number + 1];
    memset(prime, true, sizeof(prime));
    int index = 0;
    for (int p = 2; p * p <= number; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.
            for (int i = p * p; i <= number; i += p)
                prime[i] = false;
        }
    }
    for (int p = 2; p <= number; p++)
        if (prime[p]){
            bulletin3[index] = p;
            index = index + 1;
        }
}

void sieve4(int number)
{
    bool prime[number + 1];
    memset(prime, true, sizeof(prime));
    int index = 0;
    for (int p = 2; p * p <= number; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.
            for (int i = p * p; i <= number; i += p)
                prime[i] = false;
        }
    }
    for (int p = 2; p <= number; p++)
        if (prime[p]){
            bulletin4[index] = p;
            index = index + 1;
        }
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