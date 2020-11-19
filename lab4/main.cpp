#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>

using namespace std;

int fd = -1;
ofstream out_1, out_2;
ifstream in;
static void * start_thread_1(void *arg);
static void * start_thread_2(void *arg);
string line1, line2;

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
static void * thread_start2(void *arg) {
    display_thread_sched_attr("\n[лог]Параметры нового потока");
    int fd = *((int *) arg);

    char buf[1024];
    size_t nbytes;
    ssize_t bytes_read;
    nbytes = sizeof(buf);
    bytes_read = read(fd, buf, nbytes);
    printf("[Текст из файла]:\n%s",buf);
    pthread_exit(0);
}

static void * thread_start(void *arg) {
    display_thread_sched_attr("\n[лог]Параметры нового потока");
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
int task4_2(){
    int policy;
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;
    fd = open("/home/mukha/4 курс/linux_labs/lab4/example.txt", O_RDONLY);
    if(fd == -1){
        printf("[лог]Файл не открылся\n");
        return -1;
    }else{
        printf("[лог]Файл открылся, дескриптор = %d\n", fd);
    }

    pthread_attr_init(&attr);

    pthread_create(&thread, &attr, &thread_start, &fd);

    pthread_join(thread, NULL);

    pthread_create(&thread, &attr, &thread_start2, &fd);

    pthread_join(thread, NULL);
    display_thread_sched_attr("[лог]Параметры главного потока");
    if(fd_is_valid(fd) == 1){
        printf("[Main thread]Поток не закрыл файл, пытаемся закрыть\n");
        close(fd);
        if(fd_is_valid(fd)){
            printf("[Main thread]Всё равно не удалось закрыть файл\n");
        }
        else{
            printf("[Main thread]Теперь файл закрыт");
        }
    }

    return 0;
}
int task4_3(){

        in.open("/home/mukha/4 курс/linux_labs/lab4/example.txt");
        out_1.open("/home/mukha/4 курс/linux_labs/lab4/ex.txt");
        out_2.open("/home/mukha/4 курс/linux_labs/lab4/ex_2.txt");

        int linesCount = 0;

        if(!(in.is_open() && out_1.is_open() && out_2.is_open()))
            perror("Не удалось открыть файлы");

        pthread_t thread_1, thread_2;
        pthread_attr_t attr;

        pthread_attr_init(&attr);

        while(true){
            bool first = true, second = true;

            if(getline(in, line1)){ // считываем строку для 1 потока
                pthread_create(&thread_1, &attr, &start_thread_1, &line1);
            }else{
                first = false;
            }
            if(getline(in, line2)){ // считываем строку для 2 потока
                pthread_create(&thread_2, &attr, &start_thread_2, &line2);
            }else{
                second = false;
            }

            pthread_join(thread_1, NULL); // ждем 1 поток
            pthread_join(thread_2, NULL); // ждем 2 поток

            if(!(first && second)){ // если читать больше нечего, выходим из цикла
                break;
            }
        }

        in.close();
        out_1.close();
        out_2.close();

        return 0;
    }
    static void * start_thread_1(void *arg){
        string line = *((string *) arg);
        out_1 << line << endl;
        pthread_exit(0);
    }

    static void * start_thread_2(void *arg){
        string line = *((string *) arg);
        out_2 << line << endl;
        pthread_exit(0);
    }


int main() {
    int i;

    cout<<"Добро пожаловать!\nВыберите номер задания, который хотите просмотреть\n";
    cout<<"i = ";
    cin>>i;
        switch (i) {
            case (1):
                task4_1();
            case (2):
                task4_2();
            case (3):
                task4_3();


        };

    return 0;
}
