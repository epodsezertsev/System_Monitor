#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int id) : pid_(id){}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  float seconds;
  cpu_utilization_ = (LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK))/LinuxParser::UpTime(pid_);
  //cpu_utilization_ = ((LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK))/LinuxParser::UpTime(pid_));
  return cpu_utilization_;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() {
  ram_ = std::stoi(LinuxParser::Ram(pid_));
  return LinuxParser::Ram(pid_);
}

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  if (cpu_utilization_ == a.cpu_utilization_) {
    return pid_ < a.pid_;
  } else {
    return cpu_utilization_ < a.cpu_utilization_;
  }
  //return (cpu_utilization_ < a.cpu_utilization_);
}