#include <dirent.h>
#include <unistd.h>
#include<iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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


string LinuxParser::Kernel() {
  string os, kernel,hlp;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os>>hlp >> kernel;
  }
  return kernel;
}


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

float LinuxParser::MemoryUtilization() {
  float total_mem=0.0;
  float free_mem=0.0;
  string line;
  string key;
  string val;

  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
  std::remove(line.begin(),line.end(),' ');
  std::replace(line.begin(),line.end(),':',' ');
  std::istringstream linestream(line);
  while(linestream>>key>>val){
    if(key=="MemTotal"){
    	total_mem=std::stof(val);}
    else if (key=="MemFree"){
    	free_mem=std::stof(val);
    	break;}
    
   }
  }  
 }
return ((total_mem-free_mem)/total_mem);
}




long LinuxParser::UpTime() { 
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  string line,val{""};
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>val; 
  }
  return std::stol(val);
}



long LinuxParser::Jiffies() { return 0; }


long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }


long LinuxParser::ActiveJiffies() { return 0; }


long LinuxParser::IdleJiffies() { return 0; }



vector<string> LinuxParser::CpuUtilization() { 
  string line,val;
  vector <string> cpuVector;
  std::ifstream file(kProcDirectory+kStatFilename);
  if(file.is_open()){
    std::getline(file,line);
    std::istringstream stream(line);
    while(stream >>val){
      if(val!="cpu"){
        cpuVector.emplace_back(val);
      }
    };
  }
  return cpuVector;
}


int LinuxParser::TotalProcesses() { 
  std::string line,key,val;
  std::ifstream file(kProcDirectory+kStatFilename);
  if(file.is_open()){
    while(std::getline(file,line)){
      std::istringstream stream(line);
      stream>>key>>val;
      if(key=="processes"){ return std::stoi(val);}
    }
  }
  return 0; }



int LinuxParser::RunningProcesses() { 
  std::string line,key,val;
  std::ifstream file(kProcDirectory+kStatFilename);
  if(file.is_open()){
    while(std::getline(file,line)){
      std::istringstream stream(line);
      stream>>key>>val;
      if(key=="procs_running"){ return std::stoi(val);}
    }
  }
  return 0; }


string LinuxParser::Command(int pid) { 
  string val=""
;
std::ifstream filestream (kProcDirectory+"/"+std::to_string(pid)+
kCmdlineFilename);
if(filestream.is_open()){
  std::getline(filestream,val);
   return val;

  
}
return val;
 }


vector<float>LinuxParser::CpuUtilization(int pid){
  vector<float> cpu_val{};
  string line;
  float time=0;
  string val;
  std::ifstream filestream(kProcDirectory+"/"+std::to_string(pid)+
  kStatFilename);
  if(filestream.is_open())
{
  while(std::getline(filestream,line)){
    std::istringstream linestream(line);
    for(int i=1;i<=kStarttime_;i++){
      linestream>>val;
      if(i==kUtime_||i==kStime_||i==kCutime_||i==kCstime_||
      i==kStarttime_){
        time=std::stof(val)/sysconf(_SC_CLK_TCK);
        cpu_val.push_back(time);
      }
    }
  }
}
return cpu_val;
}



string LinuxParser::Ram(int pid) { 
  string line,key,value="";
  std::ifstream file(kProcDirectory+"/"+to_string(pid)+kStatusFilename);
  if(file.is_open()){
    while(std::getline(file,line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream stream(line);
      while(stream >>key>>value){
        if(key=="VmSize"){
          return value;
        }
      }
    }
  }
  return value;
   }



string LinuxParser::Uid(int pid) { 
  string line;
  string val="";
  string key;
  std::ifstream filestream(kProcDirectory+"/"+std::to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      //std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      linestream>>key>>val;
        if(key=="Uid"){
          break;
        }
    }
  }
  return val;
 }



string LinuxParser::User(int pid) {
  string line;
  string search_token = "x:"+to_string(pid);
  std::ifstream file(kPasswordPath);
  if(file.is_open()){
    while(std::getline(file,line)){
      auto pos = line.find(search_token);
      if(pos!=string::npos)
        return line.substr(0,pos-1);
    }
  }
  return "root"; }


long LinuxParser::UpTime(int pid) { 
  string line;
  long uptime=0;
  string value;
  std::ifstream filestream(kProcDirectory+"/"+std::to_string(pid)+
  kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      for(int i=1;i<=kStarttime_;i++){
        linestream>>value;
        if(i==kStarttime_){
          try{
            uptime=std::stol(value)/sysconf(_SC_CLK_TCK);
            return uptime;
          }
          catch(const std::invalid_argument& arg){
            return 0;
          }
        }
      }
    }}
    return uptime;
  }
  
 /*string line,hlp;
  std::ifstream file(kProcDirectory+to_string(pid)+kStatFilename);
  if(file.is_open()){
    std::getline(file,line);
    std::istringstream stream(line);
    for(int i=0;i<14;i++)
      stream>> hlp;
    return std::stol(hlp);*/