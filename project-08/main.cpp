#include<string>
#include<iostream>
#include<vector>
#include<cstdio>
#include<fstream>
#include<sstream>
#include "class-08.h"
using namespace std;
int main ()
{

    Program p1("first.txt");
    Program p2("second.txt");

    vector<Program>p_vc;
    p_vc.push_back(p1);
    p_vc.push_back(p2);

    CPU my_cpu(p_vc);
    my_cpu.run();
}
