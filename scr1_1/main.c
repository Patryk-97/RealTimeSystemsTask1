#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR -1
#define NO_ERROR 0
#define SIGNAL_IN_THREAD1 SIGRTMIN
#define SIGNAL_IN_THREAD2 (SIGRTMIN + 1)

void* thread1(void* arg)
{
    sigset_t signalSet;
    siginfo_t sigInfo;
    
    if(sigfillset(&signalSet) == ERROR)
    {
        printf("Cannot initialize the signal set in thread1\n");
        return NULL;
    }
    if(sigdelset(&signalSet, SIGNAL_IN_THREAD1) == ERROR)
    {
        printf("Cannot delete SIGRTMIN signal from signal set in thread1\n");
        return NULL;
    }
    
    //maskowanie, wszystkie sygnały z wyjątkiem SIGRTMIN zablokowane w tym wątku
    //SIG_BLOCK zamiast SIG_SETMASK, ponieważ na debianie SIG_SETMASK nie działa poprawnie
    if(pthread_sigmask(SIG_BLOCK, &signalSet, NULL) != NO_ERROR)
    {
        printf("Cannot mask signal set in thread1\n");
        return NULL;
    }
    
    if(sigemptyset(&signalSet) == ERROR)
    {
        printf("Cannot empty signal set in thread1\n");
        return NULL;
    }
    if(sigaddset(&signalSet, SIGNAL_IN_THREAD1) == ERROR)
    {
        printf("Cannot add SIGRTMIN signal to signal set in thread1\n");
        return NULL;
    }
    
    while(1)
    {
        //blokowanie wątku w oczekiwaniu na sygnał SIGRTMIN
        if(sigwaitinfo(&signalSet, &sigInfo) != ERROR)
        {
            printf("[Odblokowanie watku pierwszego] -> ");
            printf("pid: %d, received value: %d\n", sigInfo.si_pid, sigInfo.si_value.sival_int);
        }
        else
        {
            printf("Failure occurred inside thread1 after sigwaitinfo function\n");
        }
    }
    
    return NULL;
}

void* thread2(void* arg)
{
    sigset_t signalSet;
    siginfo_t sigInfo;
    
    if(sigfillset(&signalSet) == ERROR)
    {
        printf("Cannot initialize the signal set in thread2\n");
        return NULL;
    }
    if(sigdelset(&signalSet, SIGNAL_IN_THREAD2) == ERROR)
    {
        printf("Cannot delete (SIGRTMIN + 1) signal signal from set in thread2\n");
        return NULL;
    }
    
    //maskowanie, wszystkie sygnały z wyjątkiem (SIGRTMIN + 1) zablokowane w tym wątku
    //SIG_BLOCK zamiast SIG_SETMASK, ponieważ na debianie SIG_SETMASK nie działa poprawnie
    if(pthread_sigmask(SIG_BLOCK, &signalSet, NULL) != NO_ERROR)
    {
        printf("Cannot mask signal set in thread2\n");
        return NULL;
    }
    
    if(sigemptyset(&signalSet) == ERROR)
    {
        printf("Cannot empty signal set in thread2\n");
        return NULL;
    }
    if(sigaddset(&signalSet, SIGNAL_IN_THREAD2) == ERROR)
    {
        printf("Cannot add (SIGRTMIN + 1) signal to signal set in thread2\n");
        return NULL;
    }
    
    while(1)
    {
        //blokowanie wątku w oczekiwaniu na sygnał (SIGRTMIN + 1)
        if(sigwaitinfo(&signalSet, &sigInfo) != ERROR)
        {
            printf("[Odblokowanie watku drugiego] -> ");
            printf("pid: %d, received value: %d\n", sigInfo.si_pid, sigInfo.si_value.sival_int); 
        }
        else
        {
            printf("Failure occurred inside thread2 after sigwaitinfo function\n");
        }
    }
    
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t t1, t2;
    sigset_t signalSet;
    pid_t processId = getpid();
    
    printf("pid aktualnego procesu: %d\n", processId);
    
    if(sigfillset(&signalSet) == ERROR)
    {
        printf("Cannot initialize the signal set in main thread\n");
        return EXIT_FAILURE;
    }
    
    //maskowanie, wszystkie sygnały zablokowane w tym wątku
    //SIG_BLOCK zamiast SIG_SETMASK, ponieważ na debianie SIG_SETMASK nie działa poprawnie
    if(pthread_sigmask(SIG_BLOCK, &signalSet, NULL) != NO_ERROR)
    {
        printf("Cannot mask signal set in main thread\n");
        return EXIT_FAILURE;
    }
    
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
	return EXIT_SUCCESS;
}