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

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  cpu_utilization_ = (LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK)) / (float)LinuxParser::UpTime(pid_);
  return cpu_utilization_;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() {
  ram_ = std::stof(LinuxParser::Ram(pid_));
  return LinuxParser::Ram(pid_);
}

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  return cpu_utilization_ > a.cpu_utilization_;
}
