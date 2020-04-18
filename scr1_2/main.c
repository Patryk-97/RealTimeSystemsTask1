#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define ERROR -1
#define NO_ERROR 0
#define CONVERT_ERROR 0

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        printf("Should be three parameters\n");
        return EXIT_FAILURE;
    }
    
	pid_t pid = atoi(argv[1]);
    int sig_no = atoi(argv[2]);
    union sigval val;
    val.sival_int = atoi(argv[3]);
    
    if(pid == CONVERT_ERROR)
    {
        printf("PID is invalid\n");
        return EXIT_FAILURE;
    }
    if(sig_no == CONVERT_ERROR)
    {
        printf("Signal no is invalid\n");
        return EXIT_FAILURE;
    }
    if(val.sival_int == CONVERT_ERROR)
    {
        printf("Value is not a integer\n");
        return EXIT_FAILURE;
    }
    
    if(sigqueue(pid, sig_no, val) == NO_ERROR)
    {
        printf("Succesfully sent signal\n");
    }
    else
    {
        printf("Failure occurred while sending signal\n");
        return EXIT_FAILURE;
    }
    
	return EXIT_SUCCESS;
}
