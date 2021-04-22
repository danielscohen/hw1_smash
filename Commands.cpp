#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"

using namespace std;

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

vector<string> _cmdLineToParams(const string& cmd_line){
    vector<string> params;
    std::istringstream iss(_trim(string(cmd_line)));
    for(string s; iss >> s; ) {
        params.push_back(s);
    }
    params.erase(params.begin());
    return params;
}

int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
  const string str(cmd_line);
  return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
  const string str(cmd_line);
  // find last character other than spaces
  unsigned int idx = str.find_last_not_of(WHITESPACE);
  // if all characters are spaces then return
  if (idx == string::npos) {
    return;
  }
  // if the command line does not end with & then return
  if (cmd_line[idx] != '&') {
    return;
  }
  // replace the & (background sign) with space and then remove all tailing spaces.
  cmd_line[idx] = ' ';
  // truncate the command line string up to the last non-space character
  cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h 

SmallShell::SmallShell() : prompt("smash") {
// TODO: add your implementation
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
	// For example:
    string cmd_s = _trim(string(cmd_line));
    string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));
    if (firstWord.compare("chprompt") == 0) {
        return new CHPromptCommand(cmd_line, getInstance());
    }
/*
  string cmd_s = _trim(string(cmd_line));
  string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

  if (firstWord.compare("pwd") == 0) {
    return new GetCurrDirCommand(cmd_line);
  }
  else if (firstWord.compare("showpid") == 0) {
    return new ShowPidCommand(cmd_line);
  }
  else if ...
  .....
  else {
    return new ExternalCommand(cmd_line);
  }
  */
  return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
  // TODO: Add your implementation here
  Command* cmd = CreateCommand(cmd_line);
  cmd->execute();
  // Please note that you must fork smash process for some commands (e.g., external commands....)
}

string SmallShell::getPrompt() const {
    return prompt;
}

void SmallShell::setPrompt(string prompt) {
    SmallShell::prompt = prompt;
}

CHPromptCommand::CHPromptCommand(const char *cmd_line, SmallShell &smash)
        : BuiltInCommand(cmd_line), smash(smash) {}

void CHPromptCommand::execute() {
    if(!cmd_params.empty()){
        smash.setPrompt(*cmd_params.begin()); //isn't bad playing with iterator?
    } else {

        smash.setPrompt("smash");
    }


}

Command::Command(const string& cmd_line) {
    cmd_params = _cmdLineToParams(cmd_line);
}

BuiltInCommand::BuiltInCommand(const char *cmd_line) : Command(cmd_line) {
   if(cmd_params.back() == "&") cmd_params.pop_back();
}

ShowPidCommand::ShowPidCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {
 }

void ShowPidCommand::execute() {
    cout << "smash pid is " << getpid() << endl;
}

GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {

}

void GetCurrDirCommand::execute() {
    char buf[COMMAND_ARGS_MAX_LENGTH + 1]; // +1 needed?
    cout << getcwd(buf, sizeof(buf)) << endl;
}

ChangeDirCommand::ChangeDirCommand(const char *cmd_line, char **plastPwd) : BuiltInCommand(cmd_line), plastPwd(plastPwd)  {

}

void ChangeDirCommand::execute() {
    if (cmd_params.size() == 0) { //What happens if size==0?
        return;
    }
    if (cmd_params.size() > 1) {
        cerr << "smash error: cd: too many arguments" << endl;
        return;
    }
    if (cmd_params.front() == "-") {
        if (plastPwd != NULL) {
            if(chdir(*plastPwd) == -1){
                perror("smash error: chdir failed");
            }
            return;
        }
        else {
            cerr << "smash error: cd: OLDPWD not set" << endl;  // if null
            return;
        }
    }
    else {
        char* path = new char [cmd_params.front().length()+1];
        strcpy(path, cmd_params.front().c_str());
        //should null terminator be removed?
        if(chdir(path) == -1){
            perror("smash error: chdir failed");
        }
        return;
    }
}
