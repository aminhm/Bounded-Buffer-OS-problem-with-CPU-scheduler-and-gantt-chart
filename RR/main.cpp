#include "Semaphore.cpp"
#include "Process.cpp"
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <list>
//#include <windows.h>
#include <pthread.h>
#include <sstream>
#include "string"



class Buffer {
private:
    Process* buffer;
    int max_size;
    int count;
public:

    Buffer() {
        this->max_size = 1;
        this->buffer = (Process*)malloc(count*sizeof(Process));
        this->count = 0;

    }

    void add(Process p) {
        if (this->count < this->max_size) {
            this->buffer[this->count] = p;
            this->count++;
        }

        else {
            cout << "Buffer is full..." << endl;
        }
    }

    Process pop() {
        if (count > 0) {
            this->count--;
            return this->buffer[this->count];
        }
        else cout << "Buffer is empty..." << endl;
    }
};

Semaphore empty_(10, 10);
Semaphore full_(0, 10);
Semaphore mutex_(1, 1);
Buffer buffer_;


int _count=0;


bool flag= true;

double tq_RR=3;

double works=0.0;

int min_arrival_time=0;

double none_work_for_cpu_utilization=0.0;

int previous_process_number=-2;

double art=0.0;

int process_nums[200][200];



void* producer(void *arg) {
    int i=100;
    while (i) {
        int minimum_dist_to_next_process = 100;
        int counter_for_none_cpu_work = 0;
        list<string> _process;
        stringstream read;
        string count_;
        read << _count;
        read >> count_;
        string line;
        ifstream myfile("/Users/aminhasanzadehmoghadam/Desktop/q2/process" + count_ + ".txt");
        int min_burst_time_sjf = 100;
        Process bestChoice(0, 0, 0);
        if (myfile.is_open()) {
            while (myfile.good()) {
                getline(myfile, line);
                if (line == "") {
                    mutex_.wait();
                    flag = false;
                    i = 0;
                    mutex_.notify();
                }
                    _process.push_back(line);
                    int process_number;
                    int arrival_time;
                    int burst_time;
                    stringstream ss;
                    stringstream tt;
                    stringstream dd;
                    int index = 0;
                    ss << line[index];
                    ss >> process_number;
                    index++;
                    if (line[index] == ' ') { index++; }
                    else {
                        while (line[index] != EOF && line[index] != ' ' && line[index] != '\n' && line[index]!='\0') {
                            double rest;
                            stringstream _rest;
                            _rest << line[index];
                            _rest >> rest;
                            process_number = process_number * 10 + rest;
                            index++;
                        }
                        if (line[index] == ' ') { index++; }
                    }
                    tt << line[index];
                    tt >> arrival_time;
                    index++;
                    if (line[index] == ' ') { index++; }
                    else {
                        while (line[index] != EOF && line[index] != ' ' && line[index] != '\n' && line[index]!='\0') {
                            double rest;
                            stringstream _rest;
                            _rest << line[index];
                            _rest >> rest;
                            arrival_time = arrival_time * 10 + rest;
                            index++;
                        }
                        if (line[index] == ' ') { index++; }
                    }
                    dd << line[index];
                    dd >> burst_time;
                    index++;
                    if (line[index] == '\0' || line[index] == ' ' || line[index] == EOF) { index++; }
                    else {
                        while (line[index] != EOF && line[index] != ' ' && line[index] != '\n' && line[index]!='\0') {
                            double rest;
                            stringstream _rest;
                            _rest << line[index];
                            _rest >> rest;
                            burst_time = burst_time * 10 + rest;
                            index++;
                        }
                    }
                    Process p(process_number, arrival_time, burst_time);
                    if (p.arrival_time <= min_arrival_time && p.burst_time < min_burst_time_sjf) {
                        bestChoice = Process(p.process_number, p.arrival_time, p.burst_time);
                        min_burst_time_sjf = p.burst_time;
                    } else if (p.arrival_time > min_arrival_time) {
                        counter_for_none_cpu_work++;
                        if (p.arrival_time - min_arrival_time < minimum_dist_to_next_process) {
                            minimum_dist_to_next_process = p.arrival_time - min_arrival_time;
                        }
                    }
                }

                myfile.close();
            } else cout << "Unable to open file";
            myfile.close();
            if (counter_for_none_cpu_work == _process.size() && counter_for_none_cpu_work != 0) {
                bestChoice = Process(-1, min_arrival_time, minimum_dist_to_next_process);
                none_work_for_cpu_utilization+=minimum_dist_to_next_process;
                empty_.wait();
                mutex_.wait();

                buffer_.add(bestChoice);  // put to the buffer

                mutex_.notify();
                full_.notify();
            } else if (counter_for_none_cpu_work != _process.size()) {
                if (bestChoice.process_number != 0 && bestChoice.burst_time<=tq_RR) {
                    works++;
                }

                _count++;

                string new_file;

                stringstream write;

                write << _count;

                write >> new_file;


                ofstream newfile("/Users/aminhasanzadehmoghadam/Desktop/q2/process" + new_file + ".txt");
                stringstream ss;
                stringstream tt;
                stringstream dd;
                string remove_process_number;
                string remove_arrival_time;
                string remove_burst_time;
                ss << bestChoice.process_number;
                ss >> remove_process_number;
                tt << bestChoice.arrival_time;
                tt >> remove_arrival_time;
                dd << bestChoice.burst_time;
                dd >> remove_burst_time;
                int len = _process.size();
                string remove = remove_process_number + ' ' + remove_arrival_time + ' ' + remove_burst_time;
                int buzz = 0;
                if(process_nums[bestChoice.process_number][1]!=2 && i!=0) {
                    process_nums[bestChoice.process_number][1] = 1;
                    process_nums[bestChoice.process_number][0]=min_arrival_time;
                }
                if(bestChoice.burst_time<=tq_RR) {
                    for (int k = 0; k < len; k++) {

                        if (k == len - 2 && buzz == 0 && _process.front() != remove) {
                            newfile << _process.front();
                        } else if (_process.front() != remove) {
                            if (k == len - 1) {
                                newfile << _process.front();
                            } else { newfile << _process.front() << '\n'; }
                        } else {
                            buzz = 1;
                        }

                        _process.pop_front();
                    }
                }
                else {
                    stringstream change;
                    change << bestChoice.burst_time-tq_RR;
                    change >> remove_burst_time;
                    string remove2=remove_process_number + ' ' + remove_arrival_time + ' ' + remove_burst_time;
                    for (int k = 0; k < len; k++) {

                        if(k==len-1){
                            if(_process.front()==remove){
                                newfile<<remove2;
                            }
                            else{
                                newfile << _process.front();
                            }
                        }
                        else if(_process.front()==remove){
                            newfile<<remove2<<'\n';
                        }else{
                            newfile << _process.front()<<'\n';
                        }
                        _process.pop_front();
                    }
                }

                newfile.close();

                empty_.wait();
                mutex_.wait();

                buffer_.add(bestChoice);  // put to the buffer

                mutex_.notify();
                full_.notify();
            }
        }
//        i--;

}

list <string> _executed;
list <string> times;


double awt=0;

double atat=0;

void* consumer(void *arg) {
    mutex_.wait();
    while (flag) {
        mutex_.notify();
        list<string> _helper;
        list<string> times_helper;
        full_.wait();
        mutex_.wait();

        Process out_process = buffer_.pop();// take producct from buffer

        if (out_process.process_number != 0) {

            if (min_arrival_time >= out_process.arrival_time) {
                if (out_process.process_number != -1 && out_process.process_number!=previous_process_number) {
                    awt += min_arrival_time - out_process.arrival_time;
                }
            }
            int arrival_ = min_arrival_time;
            string arrival_time_for_printing;
            stringstream tt;
            tt << arrival_;
            tt >> arrival_time_for_printing;
            if(out_process.burst_time>=tq_RR) {
                min_arrival_time = min_arrival_time + tq_RR;
            } else{min_arrival_time = min_arrival_time + out_process.burst_time;}
            if (out_process.process_number != -1 && out_process.burst_time<=tq_RR) {
                atat += min_arrival_time-out_process.arrival_time;
            }
            if(process_nums[out_process.process_number][1]!=2 && out_process.process_number!=-1){
                art+=process_nums[out_process.process_number][0]-out_process.arrival_time;
                process_nums[out_process.process_number][1]=2;
            }
            stringstream hh;
            string endtimePrinting;
            hh << min_arrival_time;
            hh >> endtimePrinting;
            //sleep(out_process.burst_time);
            stringstream ss;
            string _process_number;
            ss << out_process.process_number;
            ss >> _process_number;
            _executed.push_back("P" + _process_number + "     ");
            if (_executed.size() == 1) {
                times.push_back(arrival_time_for_printing + " ---> " + endtimePrinting);
            } else { times.push_back(" ---> " + endtimePrinting); }
            _helper = _executed;
            times_helper = times;
            int len = _executed.size();
            string write_executes;
            stringstream write;

            write << len;

            write >> write_executes;
            ofstream executes("/Users/aminhasanzadehmoghadam/Desktop/q2/executed" + write_executes + ".txt");
            for (int k = 0; k < len; k++) {
                executes << _helper.front();
                _helper.pop_front();
            }
            executes << '\n';
            for (int k = 0; k < len; k++) {
                executes << times_helper.front();
                times_helper.pop_front();
            }
            executes.close();
            previous_process_number=out_process.process_number;
            cout << "consumer thread : out =" << out_process.process_number << endl;
            mutex_.notify();
            empty_.notify();
            //cout << "consumer thread - Empty: " << empty_.get_count() << endl;

            //out_ = out_ * 2;
        }
    }
}


int main() {
    pthread_t _consumer,_producer;
    pthread_create(&_producer,NULL,producer,NULL);
    pthread_create(&_consumer,NULL,consumer,NULL);
    pthread_join(_producer,NULL);
    pthread_join(_consumer,NULL);
    ofstream res("/Users/aminhasanzadehmoghadam/Desktop/q2/result.txt");
    res<<"AWT : "<<awt/works<<'\n';
    res<<"ART : "<<art/works<<'\n';
    res<<"ATAT : "<<atat/works<<'\n';
    res<<"Throghput : "<<works/min_arrival_time<<'\n';
    res<<"CPU Utilization : "<<((min_arrival_time-none_work_for_cpu_utilization)/min_arrival_time)*100<<'%';
    res.close();
    return 0;
}
