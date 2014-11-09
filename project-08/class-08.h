#ifndef SIMPLE_INTERPRETER
#define SIMPLE_INTERPRETER
#include<vector>
using std::vector;
#include<map>
using std::map;
#include<string>
using std::string;
#include<iostream>
using std::ostream;
class Program{
private:
    size_t instruction_index_;
    vector<string> instructions_;
    string status_;
public:
    Program(string file_name);
    Program(){}
    string get_instruction(bool lock_flag);
    string get_status() {return status_;}
    void set_status(string s) {status_ = s;}
};

class CPU {
private:
    string current_instruction_;
    vector<string> instruction_fields_;
    map<string,long> memory_;
    vector<Program> programs_;
    size_t program_index_;
    bool lock_flag_;
public:
    CPU( vector<Program> p_vc);
    bool finished();
    void run();
    bool is_assignment();
    bool is_print();
    bool is_end();
    bool is_lock();
    bool is_unlock();
    void split_instruction();
    void execute_instruction();
};

#endif
