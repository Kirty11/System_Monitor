#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): processId_(pid){
    calculateCpuUsage();
    determineCommand();
    determineRam();
    determineUptime();
    determineUser();
}


int Process::Pid() { return processId_; }


float Process::CpuUtilization()const { return cpuUsage_;}


string Process::Command() { return command_; }


string Process::Ram() { return ram_; }


string Process::User() { return user_; }


long int Process::UpTime() { return uptime_; }


void Process::calculateCpuUsage(){
    long uptime=LinuxParser::UpTime();
    vector<float>val=LinuxParser::CpuUtilization(Pid());
    if(val.size()==5){
        float totaltime=val[kUtime_]+val[kStime_]+val[kCstime_];
        float sec=uptime-val[kStarttime_];
        cpuUsage_=totaltime/sec;

    }

else

    cpuUsage_=0;


}

void Process::determineUser(){user_=LinuxParser::User(Pid());}
void Process::determineCommand(){command_=LinuxParser::Command(Pid());}
void Process::determineRam(){
    string val=LinuxParser::Ram(Pid());
    try{
        long con=std::stol(val)/1000;
        ram_=std::to_string(con);

    }catch(const std::invalid_argument& arg)
   { ram_="0";}
}
void Process::determineUptime(){
    uptime_=LinuxParser::UpTime(Pid());
    
}


bool Process::operator>(Process const& a) const { 
    return (CpuUtilization()>a.CpuUtilization()) ?true : false; }