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
		int checkSafe_op(vector<vector<int>>queue_op,int r1,int r2,int r3);
		int checkSafe_addi(vector<vector<int>>queue_op,int r1,int r2);
		int check_beq_bne(vector<vector<int>>queue_op,int r1,int r2);
		int check_sw_lw(vector<vector<int>>queue_op,int r1,int r2);
		map<string,int>memoryupdate;
		vector<int>dram;
		curr_process current;
		int rowbuffer = -1;
		int clock_cycle = 0;
		vector<int> rowbufferUpdate;
		int rowdelay,coldelay;
		
		
};