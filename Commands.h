#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

const std::string WHITESPACE = " \n\r\t\f\v";

using namespace std;

class SmallShell;

class Command {
// TODO: Add your data members
 protected:
    vector<string> cmd_params;
    const string cmdText;
 public:
  Command(const string&);

    const string &getCmdText() const;

    virtual ~Command(){};
  virtual void execute() = 0;
  //virtual void prepare();
  //virtual void cleanup();
  // TODO: Add your extra methods if needed
  virtual bool isExternalCMD() = 0;
  bool isBackgroundCMD();
};

class BuiltInCommand : public Command {
 public:
  BuiltInCommand(const char* cmd_line);


    virtual ~BuiltInCommand() {}

    bool isExternalCMD() override;
};

class ExternalCommand : public Command {
protected:
    pid_t pid;
 public:
  ExternalCommand(const char* cmd_line, SmallShell &smash);
  virtual ~ExternalCommand() {}
  void execute() override;
  bool isExternalCMD() override;

    pid_t getPid() const;

    void setPid(pid_t pid);
};


class PipeCommand : public Command {
  // TODO: Add your data members
 public:
  PipeCommand(const char* cmd_line);
  virtual ~PipeCommand() {}
  void execute() override;
};

class RedirectionCommand : public Command {
 // TODO: Add your data members
 public:
  explicit RedirectionCommand(const char* cmd_line);
  virtual ~RedirectionCommand() {}
  void execute() override;
  //void prepare() override;
  //void cleanup() override;
};



class GetCurrDirCommand : public BuiltInCommand {
 public:
  GetCurrDirCommand(const char* cmd_line);
  virtual ~GetCurrDirCommand() {}
  void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
 public:
  ShowPidCommand(const char* cmd_line);
  virtual ~ShowPidCommand() {}
  void execute() override;
};

class JobsList;
class QuitCommand : public BuiltInCommand {
// TODO: Add your data members public:
  QuitCommand(const char* cmd_line, JobsList* jobs);
  virtual ~QuitCommand() {}
  void execute() override;
};



class JobsList {
 public:
  class JobEntry {
   // TODO: Add your data members
   int jobID;
   pid_t pid;
   int insertTime;
   string cmd;
   bool isStopped;

  public:
      JobEntry(int jobId, pid_t pid, int insertTime, const string &cmd, bool isStopped);

      int getInsertTime() const;

      const string &getCmd() const;

      bool getStopped() const;

      pid_t getPid() const;

      int getJobId() const;

  };
 // TODO: Add your data members
private:
 vector<JobEntry> jList = vector<JobEntry>();
    int maxJobID;
public:
  JobsList();
  ~JobsList();
  void addJob(Command *cmd, bool isStopped, pid_t pid);
  void printJobsList();
  void killAllJobs();
  void removeFinishedJobs();
  JobEntry & getJobById(int jobId);
  void removeJobById(int jobId);
  void removeJobByPID(pid_t pid);
  JobEntry & getLastJob();
  JobEntry *getLastStoppedJob(int *jobId);
  void findMaxJobID();
  static bool compareEntry(JobEntry entry1, JobEntry entry2);
  bool isJobIdInList(int jobId) const;
  bool empty() const;

  // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
 // TODO: Add your data members
    JobsList& jobslist;
 public:
  JobsCommand(const char* cmd_line, JobsList& jobs);
  virtual ~JobsCommand() {}
  void execute() override;
};

class KillCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  KillCommand(const char* cmd_line);
  virtual ~KillCommand() {}
  void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  ForegroundCommand(const char* cmd_line);
  virtual ~ForegroundCommand() {}
  void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  BackgroundCommand(const char* cmd_line, JobsList* jobs);
  virtual ~BackgroundCommand() {}
  void execute() override;
};

class CatCommand : public BuiltInCommand {
 public:
  CatCommand(const char* cmd_line);
  virtual ~CatCommand() {}
  void execute() override;
};


class SmallShell {
 private:
  // TODO: Add your data members
  string prompt;
    char* plastPwd;
    JobsList jobslist;
public:
    char * getPlastPwd() const;

    void setPlastPwd(char *plastPwd);
    //should be initiated to NULL?
public:
    string getPrompt() const;

    void setPrompt(string prompt);

    JobsList &getJobslist();

private:
    SmallShell();
 public:
  Command *CreateCommand(const char* cmd_line);
  SmallShell(SmallShell const&)      = delete; // disable copy ctor
  void operator=(SmallShell const&)  = delete; // disable = operator
  static SmallShell& getInstance() // make SmallShell singleton
  {
    static SmallShell instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
  }
  ~SmallShell();
  void executeCommand(const char* cmd_line);
  // TODO: add extra methods as needed
};

class CHPromptCommand : public BuiltInCommand {
    SmallShell &smash;
// TODO: Add your data members
 public:
    CHPromptCommand(const char *cmd_line, SmallShell &smash);
    virtual ~CHPromptCommand() {}
    void execute() override;
};

class ChangeDirCommand : public BuiltInCommand {
    SmallShell& smash;
// TODO: Add your data members public:
public:
    ChangeDirCommand(const char* cmd_line, SmallShell &smash);
    virtual ~ChangeDirCommand() {}
    void execute() override;
};
#endif //SMASH_COMMAND_H_
