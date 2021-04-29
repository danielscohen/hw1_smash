#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"
#include <unistd.h>

using namespace std;

void ctrlZHandler(int sig_num) {
    cout<<"smash: got ctrl-Z"<<endl;
    SmallShell &smash = SmallShell::getInstance();
    if (smash.getIsConcurrentForegound()) {
        JobsList& jobsList = smash.getJobslist();
        pid_t stopped_pid = getpid();
        jobsList.removeJobByPID(stopped_pid);
        kill(stopped_pid, SIGSTOP);
        cout<<" smash: process " <<stopped_pid<<"was stopped"<<endl;
    }
    return;
}

void ctrlCHandler(int sig_num) {
    cout<<"smash: got ctrl-C"<<endl;
    SmallShell &smash = SmallShell::getInstance();
    if (smash.getIsConcurrentForegound()) {
        pid_t killed_pid = getpid();
        kill(killed_pid, SIGKILL);
        cout<<" smash: process " <<killed_pid<<"was killed"<<endl;
    }
    return;
}

void alarmHandler(int sig_num) {
    cout<<"smash got an alarm"<<endl;
}

