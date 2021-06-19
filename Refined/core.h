#include <bits/stdc++.h>
#include "mrm.h"
using namespace std;

class Core{


    public:
    	vector<int> regesterFile;
    	MRM* mrm=nullptr;
		int stackpointer;
		int index;
		string const alpha = {"abcdefghijklmnopqrstuvwxyz"};
		string const digit = {"0123456789"};
		vector<int>repetion;
		int rowdelay,coldelay;
		int sequence_cycle = 0;
		int lineno = 0;
		int ins_size;
		vector<vector<string>> instruction_memory;
		map<string,int>labels;
		Core(string filename ,int idx, int rDelay , int cDelay , MRM* mrm);
        void print_reg(vector<int> regesterFile);
        int convertRegisters(string s);
        int getBankNum(int address);
        int addressnew(string s);
        int row_regester(string s);
		int rownumber(string s);
		int colnumber(string s);
		void parse_next();
		void queue_func(vector<vector<string>>tokens,map<string,int>labels,int safe,int button);
		bool isCompleted=false;
		bool isSafeOp(int reg1,int reg2,int reg3);
		bool isSafe_beq_bne(int reg1,int reg2);
		bool isSafeAddi(int reg1,int reg2);
		int beq(vector<string> tokens,int lineno,map<string,int>labels);
		int bne(vector<string> tokens,int lineno,map<string,int>labels);
		int j(vector<string> tokens,int lineno,map<string,int>labels);
		int lw(vector<string> tokens,int lineno,int ins_size,int abs_lineno,int change);
		int sw(vector<string> tokens,int lineno,int ins_size, int abs_lineno, int change);
		void parse(vector<vector<string>> tokens,map<string,int>labels);


};