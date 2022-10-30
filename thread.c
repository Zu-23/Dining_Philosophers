# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>


typedef struct var
{
    int a;
    int b;
}arg;

void    *testing(void *param)
{
    arg *t;

    t = (arg *)param;
    t->a = 5;
    t->b = 10;
    printf("t->a %d t->b %d\n",t->a, t->b);
    return (NULL);
}

void    *func(arg *param, pid_t id)
{
    param->a++;
    param->b++;
    printf("i am proc num %d\n",id);
    printf("param->a %d param->b %d\n",param->a, param->b);
    return(NULL);
}
int main()
{
    int i; 
    pid_t id;
    arg t;


    i = 0;
    t.a =1;
    t.b =2;
    while (i < 3)
    {
        id = fork();
        if (id == 0)
        {
            func(&t, id);
        }
        i++;
    }
}