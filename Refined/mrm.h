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
    	vector<vector<int>>queue_op[4];
    	MRM(int rDelay,int cDelay);
    	int Mem_Req_Order(int rowbuffer,vector<vector<int>>current_queue_op,vector<int> current_op,int lineno, int size, int safe);
		static bool sortVec(const vector<int>& a,const vector<int>& b);
		int checkSafe_op(int r1,int r2,int r3,vector<int>banks,int index);
		int checkSafe_addi(int r1,int r2,vector<int>banks,int index);
		int check_beq_bne(int r1,int r2,vector<int>banks,int index);
		int stored_value=-1;
		int currBank =-1;
		int check_sw_lw(int r1,int r2,int address,int cur_ins,vector<int>banks,int index,int clock_cycle);
		map<string,int>memoryupdate;
		vector<int>dram;
		vector<curr_process> current;
		vector<int> rowbuffer;
		vector<vector<int>>rowbufferBank;
		int clock_arr[4];
		vector<int> clock_core;
		int safe=0;
		vector<int> rowbufferUpdate;
		int rowdelay,coldelay;
		
		
};