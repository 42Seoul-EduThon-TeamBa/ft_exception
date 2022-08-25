#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int safe_replace(char* str, char oldchar, char newchar);

static void custom_sighandler(int sig)
{
    (void)sig;
    exit(0);
}

int main(int ac, char *av[])
{

    if (ac < 2)
        return 2;

    int testNo = atoi(av[1]);

    if (testNo == 1)
    {
        // Segmentation fault가 올바르게 처리되지 않음.
        if (safe_replace(NULL, 'o', '?') != 0)
        {
            printf("Test1 (Expected Segfault handling) failed :(\n");
            return 1;
        }
        printf("Test1 OK :D\n");
    }

    if (testNo == 2)
    {
        // Bus error가 올바르게 처리되지 않음.
        if (safe_replace("Hello", 'o', '?') != 0)
        {
            printf("Test2 (Expected Bus error handling) failed :(\n");
            return 1;
        }
        printf("Test2 OK :D\n");
    }

    if (testNo == 3)
    {
        // Success
        char hello[] = "Hello world";

        // 문자열에서 문자 교체 반환 값이 올바르지 않음.
        if (safe_replace(hello, 'o', '?') == 0)
        {
            printf("Test3 (Expected Success) failed :(\n");
            return 1;
        }
        // 문자열에서 문자 교체가 올바르게 이루어지지 않음.
        if (strcmp(hello, "Hell? w?rld") != 0)
        {
            printf("Test3 (Expected Success) failed :(\n");
            return 1;
        }
        printf("Test3 OK :D\n");
    }

    if (testNo == 4)
    {
        // SIGSEGV, SIGBUS를 제외한 모든 시그널 핸들러를 등록.
        for (int i = 1; i < 32; i++)
        {
            if (i == SIGSEGV || i == SIGBUS)
                continue;
            signal(i, custom_sighandler);
        }

        char hello[] = "Hello world";
        // 문자열에서 문자 교체 반환 값이 올바르지 않음.
        if (safe_replace(hello, 'o', '?') == 0)
        {
            printf("Test4 (Expected unhandled another signals) failed :(\n");
            return 1;
        }
        // 문자열에서 문자 교체가 올바르게 이루어지지 않음.
        if (strcmp(hello, "Hell? w?rld") != 0)
        {
            printf("Test4 (Expected success) failed :(\n");
            return 1;
        }

        // SIGSEGV, SIGBUS를 제외한 모든 시그널 핸들러가 유지되는지 확인.
        for (int i = 1; i < 32; i++)
        {
            if (i == SIGSEGV || i == SIGBUS)
                continue;
            struct sigaction old;
            struct sigaction new;
            memset(&new, 0, sizeof(new));
            sigemptyset( &new.sa_mask );
            new.sa_handler = SIG_DFL;
            sigaction(i, &new, &old);
            if (old.sa_handler != custom_sighandler)
            {
                printf("Test4 (Expected not modifing signal handler (%d)) failed :( %p\n", i, old.sa_handler);
                return 1;
            }
        }

        printf("Test4 OK :D\n");
    }

    if (testNo == 5)
    {
        char *hello = malloc(12);
        hello[0] = 0;
        strcpy(hello, "Hello world");
        // 문자열에서 문자 교체 반환 값이 올바르지 않음.
        if (safe_replace(hello, 'o', '?') == 0)
        {
            printf("Test5 (Expected Success) failed :(\n");
            return 1;
        }
        // 문자열에서 문자 교체가 올바르게 이루어지지 않음.
        if (strcmp(hello, "Hell? w?rld") != 0)
        {
            printf("Test5 (Expected Success) failed :(\n");
            return 1;
        }
        free(hello);

        printf("Test5 OK :D\n");
    }

    if (testNo == 6)
    {
        char* test = malloc(1048576);
        memset(test, 'k', 1048576);
        test[1048575] = 0;
        // 긴 문자열에서 문자 교체 반환 값이 올바르지 않음.
        if (safe_replace(test, 'k', '!') == 0)
        {
            printf("Test6 (Expected long uccess) failed :(\n");
            return 1;
        }
        // 긴 문자열에서 문자 교체가 올바르게 이루어지지 않음.
        if (strrchr(test, '!') != &test[1048574])
        {
            printf("Test6 (Expected long all replaced) failed :(\n");
            return 1;
        }
        free(test);

        printf("Test6 OK :D\n");
    }

    // No action
    // if (safe_replace(NULL, 'd', 'd') != 0)
    // {
    //     printf("Test4 (Expected no action) failed :(\n");
    //     return 1;
    // }

    return 0;
}
