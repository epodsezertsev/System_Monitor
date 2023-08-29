#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"


using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value;
  float MemTotal, MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        MemTotal = std::stof(value);
      } else if (key == "MemFree:") {
        MemFree = std::stof(value);
      } 
    } return (MemTotal - MemFree)/MemTotal;
  }
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line, uptimes;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptimes;
    uptime = std::stol(uptimes);
    return uptime;
  }
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long jiffies;
  jiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  return jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, strpid, value;
  float utime, stime, cutime, cstime, total;
  vector<string> Stat;
  strpid = to_string(pid);
  std::ifstream stream(kProcDirectory + strpid + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream){
      linestream >> value;
      Stat.push_back(value);
    }
    utime = std::stol(Stat[13]);
    stime = std::stol(Stat[14]);
    cutime = std::stol(Stat[15]);
    cstime = std::stol(Stat[16]);
    total = (utime + stime + cutime + cstime);
    return total;
  }
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = LinuxParser::CpuUtilization();
  long ActiveJiffies = std::stol(jiffies[CPUStates::kUser_]) + std::stol(jiffies[CPUStates::kNice_]) + 
                       std::stol(jiffies[CPUStates::kSystem_]) + std::stol(jiffies[CPUStates::kIRQ_]) + 
                       std::stol(jiffies[CPUStates::kSoftIRQ_]) + std::stol(jiffies[CPUStates::kSteal_]);
  return ActiveJiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = LinuxParser::CpuUtilization();
  long IdleJiffies = std::stol(jiffies[CPUStates::kIdle_]) + std::stol(jiffies[CPUStates::kIOwait_]);
  return IdleJiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key, value;
  vector <string> utilization;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key;
      while (linestream >> value){
        utilization.push_back(value);
      }
    } return utilization;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  int processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = stoi(value);
      } 
    } return processes;
  }
}
  
// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        processes = stoi(value);
      } 
    } return processes;
  }
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line, strpid;
  strpid = to_string(pid);
  std::ifstream stream(kProcDirectory + strpid + kCmdlineFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      return line;
    }
  }
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value, strpid, mb{"0.00"};
  float megabytes{0}, kilobytes{0};
  strpid = to_string(pid);
  std::ifstream stream(kProcDirectory + strpid + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        kilobytes = stof(value);
        megabytes = kilobytes/1000;
        std::stringstream stream;
        stream <<std::fixed << std::setprecision(2) <<megabytes;
        mb = stream.str();
        return mb;
      } 
    }
  }
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, strpid, uid;
  strpid = to_string(pid);
  std::ifstream stream(kProcDirectory + strpid + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
        return uid;
      } 
    }
  }
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string PassLine, username, x, userid, uid;
  uid = LinuxParser::Uid(pid);
  std::ifstream PassStream(kPasswordPath);
  if (PassStream.is_open()) {
    while (std::getline(PassStream, PassLine)){
      std::replace(PassLine.begin(), PassLine.end(), ':', ' ');
      std::istringstream PassStream(PassLine);
      PassStream >> username >> x >> userid;
      if (userid == uid) {
        return username;
      }
    }
  }
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, strpid, value;
  long start_time, uptime;
  vector<string> Stat;
  strpid = to_string(pid);
  std::ifstream stream(kProcDirectory + strpid + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream){
      linestream >> value;
      Stat.push_back(value);
    }
    start_time = std::stol(Stat[21]);
    uptime = LinuxParser::UpTime() - (start_time / sysconf(_SC_CLK_TCK));
    return uptime;
  }
}
