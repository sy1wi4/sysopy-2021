//
// Created by sylwia on 4/7/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handler_SIGINT(int sig, siginfo_t *info, void *ucontext){
    printf("\n\nReceived SIGINT signal (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("--------------- info ----------------\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    kill(info->si_pid, SIGINT);
}

void handler_SIGCHLD(int sig, siginfo_t *info, void *ucontext){
    printf("\n\nReceived SIGCHLD signal (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("--------------- info ----------------\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    exit(0);
}


void handler_SIGINT_as_child(int sig, siginfo_t *info, void *ucontext){
    printf("\n\nReceived SIGINT signal [as child] (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("--------------- info ----------------\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    exit(0);
}


void handler_NOCLDSTOP(int signum){
    printf("Received signal SIGCHLD (%d). PID: %d, PPID: %d\n", signum, getpid(), getppid());
}

void handler_RESETHAND(int signum){
    printf("Handler function here\n");
    printf("Received signal (%d). PID: %d, PPID: %d\n", signum, getpid(), getppid());
}


void test_SIGINFO(){
    printf("\n\n\n---------------- TEST SA_SIGINFO ----------------\n");

    // SIGINT
    struct sigaction act_SIGINT;
    sigemptyset(&act_SIGINT.sa_mask);
    act_SIGINT.sa_sigaction = handler_SIGINT;
    // SA_SIGINFO flag, the signals are queued, and they're all delivered
    act_SIGINT.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act_SIGINT, NULL);


    // SIGCHLD
    // When a child process stops or terminates, SIGCHLD is sent to the parent process
    struct sigaction act_SIGCHLD;
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_sigaction = handler_SIGCHLD;
    act_SIGCHLD.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &act_SIGCHLD, NULL);



    pid_t pid = fork();
    if(pid == 0){
        signal(SIGINT, (__sighandler_t) &handler_SIGINT_as_child);

        // send signal to parent process
        kill(getppid(), SIGINT);
        pause();

    }

    else while(1){}
}


void test_NOCLDSTOP(){
    printf("\n\n\n---------------- TEST SA_NOCLDSTOP ----------------\n");

    // SA_NOCLDSTOP - meaningful only for the SIGCHLD signal.
    // When the flag is set, the system delivers the signal for a terminated child process but not for one that is stopped.
    // Do not generate SIGCHLD when children stop or stopped children continue.

    struct sigaction act_SIGCHLD;
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_handler = &handler_NOCLDSTOP;

    sigaction(SIGCHLD, &act_SIGCHLD, NULL);

    // fork and stop child process without flag
    printf("\n~~~~ SA_NOCLDSTOP flag is not set ~~~~\n\n");

    pid_t pid = fork();
    if(pid == 0){
        printf("Child process here: %d\n", getpid());
        while(1){}
    }
    else{
        printf("Parent process here: %d\n", getpid());
        printf("Child process stopped, SIGCHLD should be sent...\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }


    printf("\n~~~~   SA_NOCLDSTOP flag is set   ~~~~\n\n");
    act_SIGCHLD.sa_flags = SA_NOCLDSTOP;

    pid = fork();
    if(pid == 0){
        printf("Child process here: %d\n", getpid());
        while(1){}
    }
    else{
        printf("Parent process here: %d\n", getpid());
        printf("Child process stopped, but SIGCHLD should NOT be sent...\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }
}

void test_RESETHAND() {
    printf("\n\n\n---------------- TEST SA_RESETHAND ----------------\n");

//    If set on, the disposition of the signal handler is automatically reset to
//    SIG_DFL, and the SA_SIGINFO flag is set off at entry to the signal handler.

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = &handler_RESETHAND;
    sigaction(SIGCHLD, &act, NULL);

    printf("\n~~~~ SA_RESETHAND flag is not set ~~~~\n\n");
    pid_t pid = fork();
    if(pid == 0){
        while(1){}
    }
    else{
        printf("Child process stopped, SIGCHLD sent, handler function should be called...\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }


    sleep(2);

    printf("\n~~~~ SA_RESETHAND flag is set ~~~~\n\n");
    act.sa_flags = SA_RESETHAND;

    pid = fork();
    if(pid == 0){
        while(1){}
    }
    else{
        printf("Child process stopped, SIGCHLD sent, the default disposition of SIGCHLD is [ignore]....\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }



}




int main(){
    //  sigaction() can block other signals until the current handler returns while
    //  signal() function does not (necessarily) block other signals from arriving

    test_NOCLDSTOP();
    test_RESETHAND();
    test_SIGINFO();

    return 0;
}

