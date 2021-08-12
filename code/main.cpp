#include <bits/stdc++.h>
#include "core.h"
using namespace std;




int main(int argc,char* argv[]){

	int num_cores,rowdelay,coldelay,Max_cycles;
	int total_row_buffer_updates = 0;
	bool mrm_delay = true;
	if(argc<2){
		cerr<<"Error: Enter number of cores\n";
		return 0;
	}
	num_cores = stoi(argv[1]);
	Max_cycles = stoi(argv[2]);

	if(argc<num_cores+5){
		cerr<<"Error: Either the correct number of testcases aren't used or delay is not given\n";
		return 0;
	}
	rowdelay = stoi(argv[3+num_cores]);
	coldelay = stoi(argv[4+num_cores]);
	if(argc>5+num_cores){
		mrm_delay = false;
	}
	int max_clock=0;
	vector<Core*> cores(num_cores);
	vector<int> cycles(num_cores);
	MRM* mrm_universal=new MRM(rowdelay,coldelay);
	mrm_universal->max_cycle = Max_cycles;
	mrm_universal->rowbufferBank.resize(4);
	for (int i=0;i<num_cores;i++){
		string fileName = argv[i+3];
		cores[i]=new Core(fileName,i+1,rowdelay,coldelay,mrm_universal,num_cores);
	}

	int num_completed=0;
	vector<bool> completed(num_cores,false);
	int Cycle_complete=0;
	vector<bool> updatedBanks(4,false);
	while (num_cores!=num_completed && Cycle_complete<Max_cycles){
		for (int i=0;i<num_cores;i++){

			if (!completed[i]){
				cores[i]->parse_next();
				if (cores[i]->isCompleted){
					completed[i]=true;
					num_completed++;
				}
			}

		}
		Cycle_complete = *max_element(mrm_universal->clock_core.begin(),mrm_universal->clock_core.end());
		for (int i=0;i<4;i++){
			int log = mrm_universal->update(i,num_cores,mrm_delay);
				
		}


	}
	
	int numUpdated = 0;
	while (numUpdated != 4){

		for (int i=0;i<4;i++){
			int log = mrm_universal->update(i,num_cores,mrm_delay);
			mrm_universal->clock_core[i]++;
			if (log == -1){
				updatedBanks[i] = true;
			}
		}
		numUpdated = 0;
		for (int i=0;i<4;i++){
			if (updatedBanks[i]){
				numUpdated++;
			}
		}


	}
	for(int i=0;i<num_cores;i++){
		cores[i]->isCompleted = true;
		mrm_universal->indexCompleted[i] = true;
	}




	for(int i=1;i<=4;i++){
		if(mrm_universal->clock_core[i]>max_clock){
			max_clock = mrm_universal->clock_core[i];
		}
	}

	for(int i=0;i<4;i++){
		if(mrm_universal->rowbuffer[i]!=-1){
			if (mrm_universal->writeBackCycle[i]+10>mrm_universal->max_cycle){
				continue;
			}
			cout<<"Cycle "<<(mrm_universal->writeBackCycle[i]+1)<<"-";
			mrm_universal->writeBackCycle[i]+=10;

			mrm_universal->rowbuffer[i] = -1;
			cout<<(mrm_universal->writeBackCycle[i])<<" :DRAM Writeback rowbuffer number "<<i+1<<"\n";
		}
	}

	vector<string> reg_name = {"$0","$1","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9","$t10","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$s8","$k1","$gp","$sp","$fp","$ra"};
	
	cout<<"\n";
	for (int i=0;i<num_cores;i++){
		cout<<" Core "<<i+1<<":\n";
		cores[i]-> print_reg(mrm_universal->regesterFile[i]);
		cout<<"\n";

	}
	cout<<"\n DRAM Memory at the end of the execution:\n";
	for(auto i:mrm_universal->memoryupdate){
		cout<<i.first<<" : "<<i.second<<endl;
	}
	cout<<"\n Row Buffer Updates of each Banks are\n";
	for(int i=0;i<4;i++){
		cout<<"Bank Number: "<<i+1<<" : Row Buffer Updates are: "<<mrm_universal->rowbufferUpdate[i]<<"\n";
		total_row_buffer_updates+=mrm_universal->rowbufferUpdate[i];
	}
	cout<<"\nTotal Row Buffer Updates are: "<<total_row_buffer_updates<<"\n";
	cout<<"MRM Delay is: "<<mrm_universal->MRM_Delay<<"\n";
	cout<<"Total number of Instructions Executed:"<<mrm_universal->ins_executed<<"\n";
	cout<<"Throughput: "<<((float)mrm_universal->ins_executed/(float)Max_cycles)<<"\n";
return 0;


}