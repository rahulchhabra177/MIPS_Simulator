#include <bits/stdc++.h>
#include "core.h"
using namespace std;





int main(){

	int num_cores=4;
	vector<Core*> cores(num_cores);
	vector<int> cycles(num_cores);
	for (int i=0;i<num_cores;i++){
		cores[i]=new Core("./testcases/testcase"+to_string(i+1)+".txt",i+1,10,2);
	}
	int num_completed=0;
	vector<bool> completed(num_cores,false);
	int debug=0;
	while (num_cores!=num_completed && debug<500){

		debug++;
		for (int i=0;i<num_cores;i++){

			if (!completed[i]){
				cores[i]->parse_next();
				if (cores[i]->isCompleted){
					completed[i]=true;
					num_completed++;
				}
			}


		}

	}


return 0;


}