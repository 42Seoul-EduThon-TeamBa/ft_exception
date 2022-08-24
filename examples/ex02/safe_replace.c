#include <setjmp.h>
#include <signal.h>

jmp_buf env;

static void on_signal(int sig)
{
    longjmp(env, sig);
}

int safe_replace(char* str, char oldchar, char newchar)
{
    int i;

    signal(SIGSEGV, on_signal);
    signal(SIGBUS, on_signal);
    if (setjmp(env) != 0)
    {
        return 0;
    }
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == oldchar)
        {
            str[i] = newchar;
        }
    }
    return 1;
}
