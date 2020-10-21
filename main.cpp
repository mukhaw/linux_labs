#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std;

int fd = -1;

static void display_sched_attr(int policy, struct sched_param *param) {
    printf("    policy=%s\n    priority=%d\n    max priority=%d\n    min priority=%d\n",
           (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
           (policy == SCHED_RR)    ? "SCHED_RR" :
           (policy == SCHED_OTHER) ? "SCHED_OTHER" :
           "???",
           param->sched_priority,
           sched_get_priority_max(policy),
           sched_get_priority_min(policy)
    );
}

static void display_thread_sched_attr(const char* msg) {
    int policy;
    struct sched_param param;
    printf("%s\n", msg);
    pthread_getschedparam(pthread_self(), &policy, &param);
    display_sched_attr(policy, &param);
}

int fd_is_valid(int fd)
{
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

static void * thread_start(void *arg) {
    display_thread_sched_attr("[лог]Параметры нового потока");
    int fd = *((int *) arg);

    char buf[1024];
    size_t nbytes;
    ssize_t bytes_read;
    nbytes = sizeof(buf);
    bytes_read = read(fd, buf, nbytes);
    cout<<"[Текст из файла]:\n"<<buf;
    //close(fd);
    if(fd_is_valid(fd) == 0){
        cout<<"[New thread]Поток закрыл файл\n";
    }
    pthread_exit(0);
}
int task4_1(){
    int policy;
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;
    fd = open("/home/mukha/4 курс/linux_labs/lab4/example.txt", O_RDONLY);
    if(fd == -1){
        cout<<"[лог]Файл не открылся\n";
        return -1;
    }else{
        cout<<"[лог]Файл открылся, дескриптор = "<< fd<<"\n";
    }

    pthread_attr_init(&attr);

    pthread_create(&thread, &attr, &thread_start, &fd);

    pthread_join(thread, NULL);
    display_thread_sched_attr("\n[лог]Параметры главного потока");
    if(fd_is_valid(fd) == 1){
        cout<<"[Main thread]Поток не закрыл файл, пытаемся закрыть\n";
        close(fd);
        if(fd_is_valid(fd)){
            cout<<"[Main thread]Всё равно не удалось закрыть файл\n";
        }
        else{
            cout<<"[Main thread]Теперь файл закрыт";
        }
    }
    return 0;
}
int main() {
    int i;

    cout<<"Добро пожаловать выберите номер задания, который хотите просмотреть\n";
    cout<<"i = ";
    cin>>i;
    switch(i){
        case(1):
            task4_1();
        case(2):
            cout<<"Good bye";

    }

    return 0;
}
