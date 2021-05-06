#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"
#include <unistd.h>

using namespace std;

void ctrlZHandler(int sig_num) {
    cout<<"smash: got ctrl-Z"<<endl;
    SmallShell &smash = SmallShell::getInstance();
    pid_t fgJobPid = smash.getFgJobPid();
    int fgJobId = smash.getFgJobId();
    string fgJobCMD = smash.getFgJobCmd();
    if (fgJobPid != 0) {
        JobsList& jobsList = smash.getJobslist();
        if(fgJobId != 0){
            jobsList.addJobAtJobId(fgJobCMD,fgJobId, fgJobPid);
        } else{
            jobsList.addJob(fgJobCMD, true, fgJobPid);
        }
        if(kill(fgJobPid, SIGTSTP) == -1){
            perror("smash error: kill failed");
        }
        cout << "smash: process " << fgJobPid <<" was stopped" << endl;
    }
}

void ctrlCHandler(int sig_num) {
    cout<<"smash: got ctrl-C"<<endl;
    SmallShell &smash = SmallShell::getInstance();
    pid_t fgJobPid = smash.getFgJobPid();
    string fgJobCMD = smash.getFgJobCmd();
    if (fgJobPid != 0) {
        if(kill(fgJobPid, SIGKILL) == -1){
            perror("smash error: kill failed");
        }
        smash.removeTimeOutJobByPID(fgJobPid);
        cout << "smash: process " << fgJobPid <<" was killed" << endl;
    }
}

void alarmHandler(int sig_num) {
    SmallShell &smash = SmallShell::getInstance();
    cout<<"smash: got an alarm"<<endl;
    smash.setNextAlarm();
    smash.getJobslist().removeFinishedJobs();
    smash.removeTimedOutJob();
}

