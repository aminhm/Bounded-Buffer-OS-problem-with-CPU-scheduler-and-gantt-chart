#include <iostream>
#include <stdio.h>


using namespace std;

class Process {

public:
    int process_number;
    int arrival_time,burst_time;


public:
    Process(int _process_number, int _arrival_time,int _burst_time)
            : process_number(_process_number) , arrival_time(_arrival_time) ,burst_time(_burst_time){}

//    int get_process_number() {
//        return this->process_number;
//    }
//
//

};




