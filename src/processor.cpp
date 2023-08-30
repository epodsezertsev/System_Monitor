#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>

float Processor::Utilization() {
  long PrevIdle = LinuxParser::IdleJiffies();
  long PrevNonIdle = LinuxParser::ActiveJiffies();
  long PrevTotal = PrevIdle + PrevNonIdle;
  
  sleep(1);
  
  long Idle = LinuxParser::IdleJiffies();
  long NonIdle = LinuxParser::ActiveJiffies();
  long Total = Idle + NonIdle;
  
  float TotalDelta = Total - PrevTotal;
  float IdleDelta = Idle - PrevIdle;
  
  float CPU_Percentage = ((TotalDelta - IdleDelta)/TotalDelta);

  return CPU_Percentage;
}
