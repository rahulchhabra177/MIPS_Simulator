#include <bits/stdc++.h>
#include "core.h"
using namespace std;




int main(){

	int num_cores;
	cout<<"Enter Number of Cores:";
	cin>>num_cores;
	int max_clock=0;
	vector<Core*> cores(num_cores);
	vector<int> cycles(num_cores);
	MRM* mrm_universal=new MRM(10,2);
	mrm_universal->rowbufferBank.resize(4);
	for (int i=0;i<num_cores;i++){
		cores[i]=new Core("./testcases/testcase"+to_string(i+1)+".txt",i+1,10,2,mrm_universal,num_cores);
	}

	int num_completed=0;
	vector<bool> completed(num_cores,false);
	int debug=0;
	vector<bool> updatedBanks(4,false);
	while (num_cores!=num_completed && debug<10000){

		debug++;
		for (int i=0;i<num_cores;i++){

			if (!completed[i]){
				cores[i]->parse_next();
				// cores[i]-> print_reg(mrm_universal->regesterFile[i]);
				//cout<<"comp "<<cores[i]->isCompleted<<"\n";
				if (cores[i]->isCompleted){
					// cout<<"core "<<i<<"\n";
					completed[i]=true;
					num_completed++;
				}
			}


		}
		
		for (int i=0;i<4;i++){
			int log = mrm_universal->update(i,num_cores);
				
		}


	}

	int numUpdated = 0;
	while (numUpdated != 4){

		for (int i=0;i<4;i++){
			int log = mrm_universal->update(i,num_cores);
			mrm_universal->clock_core[i]++;
			// cout<<i<<"::"<<log<<"\n";
			if (log == -1){
				updatedBanks[i] = true;
			}
		}
		numUpdated = 0;
		// for (int i=0;i<4;i++){
			// cout<<(updatedBanks[i]?"TRue ":"False ");
		// }
		for (int i=0;i<4;i++){
			if (updatedBanks[i]){
				numUpdated++;
			}
		}

		// cout<<"num:"<<numUpdated<<"\n";

	}




	for(int i=1;i<=4;i++){
		if(mrm_universal->clock_core[i]>max_clock){
			max_clock = mrm_universal->clock_core[i];
		}
	}
//Writeback
	for(int i=0;i<4;i++){
		if(mrm_universal->rowbuffer[i]!=-1){
			cout<<"Cycle "<<(max_clock+2)<<"-";
			max_clock+=10;
			mrm_universal->rowbuffer[i] = -1;
			cout<<(max_clock+1)<<" :DRAM Writeback rowbuffer number "<<i+1<<"\n";
		}
	}

	vector<string> reg_name = {"$0","$1","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9","$t10","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$s8","$k1","$gp","$sp","$fp","$ra"};
	

	for (int i=0;i<num_cores;i++){
		cout<<"Core "<<i+1<<":\n";
		cores[i]-> print_reg(mrm_universal->regesterFile[i]);
	}

return 0;


}