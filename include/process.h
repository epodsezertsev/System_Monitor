#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int id);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_;
  float cpu_utilization_{0.0};
  float ram_ {0.0};
};

#endif