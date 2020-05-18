#include <string>

#include "format.h"

using std::string;



string Format::ElapsedTime(long seconds) { 
    int HH=seconds/3600;
    seconds=seconds%3600;
    int MM= seconds/60;
    seconds=seconds%60;
    int SS= seconds;
    return std::to_string(HH)+':'+std::to_string(MM)+':'+std::to_string(SS); }
std::string Format::TimeValueToString(long time){
    if(time<10)return "0"+std::to_string(time);
    else
    {
        return std::to_string(time);

    }
    
}