
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

int inputInt(string message){
    int integer=0;
    cout << (message + " = ");
    cin >> integer;
    return integer;
}

string inputString(string message){
    string str = "";
    cout << (message + " = ");
    cin >> str;
    cout << endl;
    return str;
}

void writeAttributes(){
    pid_t pid = getpid();
    cout << "Идентификатор процесса " << pid << endl;
    cout << "Идентификатор предка " << getppid() << endl;
    cout << "Идентификатор сессии процесса " << getsid(pid) << endl;
    cout << "Идентификатор группы процессов " << getpgid(pid) << endl;
    cout << "Реальный дентификатор пользователя " << getuid() << endl;
    cout << "Эффективный идентификатор пользователя " << geteuid() << endl;
    cout << "Реальный групповой идентификатор " << getgid() << endl;
    cout << "Эффективный групповой идентификатор " << getegid() << endl;
    cout << endl;
}



int main() {

    int exec_sleep = inputInt("Exec sleep");
    int fork_sleep = inputInt("Fork sleep");
    int vfork_sleep = inputInt("Vfork sleep");

    if (fork() == 0) {
        sleep(fork_sleep);
        cout<<"FORK"<<endl;
        writeAttributes();
        exit(1);
    }

    if (vfork() == 0) {
        sleep(vfork_sleep);
        cout<<"VFORK"<<endl;
        writeAttributes();

        if (execl("/home/mukha/4 курс/linux_labs/lab3/lab3_txt.odt","","",(char *)0)) {
            exit(1);
        }
    } else {
        sleep(exec_sleep);
        cout<<"EXEC"<<endl;
        writeAttributes();

    }

    return 0;
}
