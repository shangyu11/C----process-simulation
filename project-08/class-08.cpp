#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<stdlib.h>
#include<cmath>
#include<queue>
#include<vector>
#include<stack>
#include<set>
#include<cmath>
#include<sstream>
#include<fstream>
#include"class-08.h"
using namespace std;

const string PRINT = "print";
const string WAIT = "waiting";
const string RUN = "running";
const string LOCK = "lock";
const string UNLOCK = "unlock";
const string END = "end";
const string FIN = "finished";
const string ERROR = "error";

Program::Program(string file_name)//��ʼ��һ������
{
    ifstream fin(file_name.c_str());//���ļ��ж����ַ���
    char line[1024];
    string str_line;
    while(fin.getline(line,sizeof(line)))
    {
        str_line.clear();
        for(int i= 0 ; i < (int)strlen(line) ; i++)
            str_line+=line[i];
        instructions_.push_back(str_line);
    }
    status_ = RUN;
    instruction_index_ = 0;
}

string Program::get_instruction(bool lock_flag)//ȡ�������е�ָ��
{
    if(instruction_index_>=instructions_.size()) return "";
    if(lock_flag&&instructions_[instruction_index_]==LOCK)
    {
       // cout<<LOCK<<endl;
        return WAIT;
    }
    //cout<<instructions_[instruction_index_]<<endl;
    return instructions_[instruction_index_++];
}

CPU::CPU(vector<Program>p_vc)//cpu�ĳ�ʼ��
{
    programs_ = p_vc;
    lock_flag_ = false;
    program_index_ = 0;
    memory_.clear();
    current_instruction_ = "";
    instruction_fields_.clear();
}

bool CPU::finished()//cpu����
{
    int i;
    for(i = 0 ; i < (int)programs_.size() ; i++)
    {
        if(programs_[i].get_status()==RUN||programs_[i].get_status()==WAIT) return false;
    }
    return true;
}

void CPU::run()//cpu��ʼ����
{
    while(1)
    {
        if(CPU::finished()) break;//�Ƿ����н��̶��Ѿ����
        //cout<<"program"<<program_index_<<" "<<"start instructing: ";
        current_instruction_ = programs_[program_index_].get_instruction(lock_flag_);//��ǰ����ʹ��CPUִ��ָ��
        if(current_instruction_==""||current_instruction_==WAIT)//locked
        {

            if(current_instruction_==WAIT)
            {
                programs_[program_index_].set_status(WAIT);
               // printf("program%d is waiting!\n",program_index_);
            }
            else
            {
                programs_[program_index_].set_status(FIN);
               // printf("program%d is finished!\n",program_index_);
            }
            program_index_ ^= 1;
            continue;
        }
        programs_[program_index_].set_status(RUN);
        execute_instruction();
        program_index_ ^= 1;//ʱ��Ƭ��ת
    }
}

void CPU::split_instruction()//����һ��ָ��
{
    string sp_instruction;
    sp_instruction.clear();//�洢ָ���ÿһ��С����
    instruction_fields_.clear();
    for(int i = 0; i < (int)current_instruction_.length() ; i++)//����ָ��
    {
        if(current_instruction_[i]==' ')
        {
            if(!sp_instruction.empty())
            {
                instruction_fields_.push_back(sp_instruction);//����ָ������
                sp_instruction.clear();
            }
            continue;
        }
        sp_instruction+=current_instruction_[i];
    }

    if(!sp_instruction.empty())
    {
        instruction_fields_.push_back(sp_instruction);
    }
}

bool CPU::is_print()//�Ƿ������ָ��
{
    if(instruction_fields_[0]==PRINT&&instruction_fields_.size()==2)
        return true;
    return false;
}

bool CPU::is_assignment()//�Ƿ����ָ��
{
    if(instruction_fields_.size()==3&&instruction_fields_[1]=="=")
        return true;
    return false;
}

bool CPU::is_end()//ָ���Ƿ����
{
    if(instruction_fields_.size()==1&&instruction_fields_[0]==END)
        return true;
    return false;
}

bool CPU::is_lock()//�Ƿ��Ǽ���ָ��
{
    if(instruction_fields_.size()==1&&instruction_fields_[0]==LOCK)
        return true;
    return false;
}
bool CPU::is_unlock()//�Ƿ��ǽ���ָ��
{
    if(instruction_fields_.size()==1&&instruction_fields_[0]==UNLOCK)
        return true;
    return false;
}

void CPU::execute_instruction()//ִ��һ���������ָ��
{
    split_instruction();//���Ƚ���ָ��
    if(is_print())
    {
        string exe_instr = instruction_fields_[1];
        map<string,long>::iterator it;
        it = memory_.find(exe_instr);
        if(it==memory_.end())//�Ҳ���ʱ����Ϊ����
        {
            programs_[program_index_].set_status(ERROR);
            return ;
        }
        long var = memory_[exe_instr];
        cout<<exe_instr<<" = "<<var<<endl;
    }
    else if(is_assignment())
    {
        string exe_instr = instruction_fields_[0];
        memory_[exe_instr] = atoi(instruction_fields_[2].c_str());
       // printf("program%d is assigning now!\n",program_index_);
    }
    else if(is_end())
    {
        programs_[program_index_].set_status(FIN);
    }
    else if(is_lock())
    {
        lock_flag_ = true;
      //  printf("i am locked by program%d!\n",program_index_);
    }
    else if(is_unlock())
    {
        lock_flag_ = false;
       // printf("i am unlocked by program%d!\n",program_index_);
    }
    else
    {
        //printf("error\n");
        programs_[program_index_].set_status(ERROR);
    }
}

