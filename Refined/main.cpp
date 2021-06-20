#include <bits/stdc++.h>
#include "core.h"
using namespace std;




int main(){

	int num_cores=1;
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
	while (num_cores!=num_completed && debug<700){

		debug++;
		for (int i=0;i<num_cores;i++){

			if (!completed[i]){
				cores[i]->parse_next();
				//cout<<"comp "<<cores[i]->isCompleted<<"\n";
				if (cores[i]->isCompleted){
					cout<<"core "<<i<<"\n";
					completed[i]=true;
					num_completed++;
				}
			}


		}

	}

	for(int i=0;i<4;i++){
		if(mrm_universal->rowbuffer[i]!=-1){
			cout<<"Cycle "<<mrm_universal->clock_core[1]+1<<"-";
			mrm_universal->clock_core[1]+=10;
			mrm_universal->rowbuffer[i] = -1;
			cout<<mrm_universal->clock_core[1]<<" :DRAM Writeback rowbuffer number "<<i+1<<"\n";
		}
	}


return 0;


}