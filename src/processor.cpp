#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long Idle = LinuxParser::IdleJiffies();
  long NonIdle = LinuxParser::ActiveJiffies();
  long PrevTotal = PrevIdle_ + PrevNonIdle_;
  long Total = Idle + NonIdle;
  
  float TotalDelta = Total - PrevTotal;
  float IdleDelta = Idle - PrevIdle_;
  
  float CPU_Percentage = ((TotalDelta - IdleDelta)/TotalDelta);
  PrevIdle_ = Idle;
  PrevNonIdle_ = NonIdle;
  return CPU_Percentage;
}