#include <bits/stdc++.h>
#include "mrm.h"
using namespace std;

class Core{


    public:
    	MRM* mrm=nullptr;
		int totalCores;
		int stackpointer;
		int index;
		string const alpha = {"abcdefghijklmnopqrstuvwxyz"};
		string const digit = {"0123456789"};
		vector<int>repetion;
		int rowdelay,coldelay;
		int sequence_cycle = 0;
		int lineno = 0;
		int ins_size;
		vector<int>banks;
		string encode(string reg);
		void encode_memory(vector<vector<string>>instructions);
		string encode_ins(vector<string>arguments);
		vector<vector<string>> instruction_memory;
		vector<string> encoded_memory;
		map<string,int>labels;
		Core(string filename ,int idx, int rDelay , int cDelay , MRM* mrm,int total_cores);
        void print_reg(vector<int> regesterFile);
		vector<int> queueBank(int index);
		string getIns(vector<string>t);
        int convertRegisters(string s);
        int getBankNum(int address);
        int addressnew(string s);
		int coreAddress(int address,int index);
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
		int push_lw(vector<string> tokens,int lineno,int ins_size,int abs_lineno,int change);
		int push_sw(vector<string> tokens,int lineno,int ins_size,int abs_lineno,int change);
		int sw(vector<string> tokens,int lineno,int ins_size, int abs_lineno, int change);
		void parse(vector<vector<string>> tokens,map<string,int>labels);
		void increment();
		void setSeqCycle();


};