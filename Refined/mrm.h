#include<iostream>
#include<fstream>
#include<math.h>
#include <bits/stdc++.h>
using namespace std;

struct curr_process
{
    int reg0,reg1,isLW;
};
class MRM{


    public:
    	vector<vector<int>>queue_op;
    	MRM(int rDelay,int cDelay);
    	int Mem_Req_Order(int rowbuffer,vector<vector<int>>current_queue_op,vector<int> current_op,int lineno, int size, int safe);
		static bool sortVec(const vector<int>& a,const vector<int>& b);
		int checkSafe_op(int r1,int r2,int r3);
		int checkSafe_addi(int r1,int r2);
		int check_beq_bne(int r1,int r2);
		int stored_value=-1;
		int check_sw_lw(int r1,int r2,int address,int cur_ins);
		map<string,int>memoryupdate;
		vector<int>dram;
		curr_process current;
		vector<int> rowbuffer;
		int clock_cycle = 0;
		int safe=0;
		vector<int> rowbufferUpdate;
		int rowdelay,coldelay;
		
		
};