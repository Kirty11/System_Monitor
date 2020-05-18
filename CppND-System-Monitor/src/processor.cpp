#include "processor.h"
#include<string>
#include<vector>
#include "linux_parser.h"

using std::string;
using std::vector;

Processor::Processor() : prevTotalCpuTime(0.0),prevIdleCpuTime(0.0){};


float Processor::Utilization() { 
    vector<long> cpu_val=ConvertToLong(LinuxParser::CpuUtilization());
    float totalcputime=cpu_val[LinuxParser::kUser_]+cpu_val[LinuxParser::kNice_]+
    cpu_val[LinuxParser::kSystem_]+ cpu_val[LinuxParser::kIdle_]+ cpu_val[LinuxParser::kSystem_]+ cpu_val[LinuxParser::kIOwait_]
    +  cpu_val[LinuxParser::kSoftIRQ_]+ cpu_val[LinuxParser::kSteal_];
    float idlecput=cpu_val[LinuxParser::kIdle_]+cpu_val[LinuxParser::kIOwait_];
    float diffidle=idlecput-prevIdleCpuTime;
    float difftotal=totalcputime-prevTotalCpuTime;
    float diffUsage=(difftotal-diffidle)/difftotal;
    prevIdleCpuTime=idlecput;
    prevTotalCpuTime=totalcputime;
    return diffUsage;


 }

 vector<long> Processor::ConvertToLong(vector<string>val){
     vector<long>convertedValues{};
     for(int i=0;i<(int)val.size();i++)
     {
         try{
             convertedValues.push_back(std::stol(val[i]));

         }catch(const std::invalid_argument& arg){
             convertedValues.push_back((long)0);

         }
     }
     return convertedValues;
 }