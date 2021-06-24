#include<bits/stdc++.h>
#include "mrm.h"
using namespace std;




MRM::MRM(int r,int c){
    rowdelay=r;
    coldelay=c;
    dram.resize(1048576,0);
    rowbufferUpdate.resize(4,0);
    rowbuffer.resize(4,-1);
}


int MRM::Mem_Req_Order(int rowbuffer,vector<vector<int>> queue_op,vector<int>current_op,int lineno,int size,int safe){
    if(rowbuffer == -1){
        return -1;
    }
    else if(current_op[2]/1024 == rowbuffer){
        return 1;
    }
    else if(lineno == size|| safe==1){
        return 2;
    }
    else{
        return 0;
    }
}
bool MRM::sortVec(const vector<int>& a,const vector<int>& b/*,int priority_row1=priority_row1,int priority_row2=priority_row2*/){

    int x=a[4];
    int y=b[4];
    if (x==priority_row1){
        return true;
    }
    else if (y==priority_row1){
        return false;
    }
    else if (x==priority_row2){
        return true;
    }
    else if (y==priority_row2){
        return false;
    }
    else{

        return a[4]<b[4];
    }

};
int MRM::checkSafe_op(int r1,int r2,int r3,vector<int>banks,int index){
    for(int j=0;j<banks.size();j++){
        for(int i=0;i<queue_op[banks[j]].size();i++){
            vector<int>row = queue_op[banks[j]][i];
            if(row[7]==index){
                if(row[0]==0){
                  
                    if(row[1]==r2||row[1]==r3){
                        currBank = banks[j];
                        return -1;
                    }
                    else if (row[1]==r1){
                        cout<<"Core: "<<index<<" "<<"MRM cycle no:"<<clock_core[j]<<" LW process at line number :"<<row[3]+1<<" removed from queue\n";
                        queue_op[banks[j]].erase(queue_op[banks[j]].begin()+i);
                        return -2;
                    }
                }
            }
        }
    }
    return 0;
}
int MRM::checkSafe_addi(int r1,int r2,vector<int>banks,int index){

    for(int j=0;j<banks.size();j++){
        for(int i=0;i<queue_op[banks[j]].size();i++){
            vector<int>row = queue_op[banks[j]][i];
            if(row[7]==index){
                if(row[0]==1){
                   
                }else{
                    if(row[1]==r2){
                        currBank = banks[j];
                        return -1;
                    }
                    else if (row[1]==r1){
                        cout<<"Core: "<<index<<" "<<"MRM cycle no:"<<clock_core[j]<<" LW process at line number :"<<row[3]+1<<" removed from queue\n";
                        queue_op[banks[j]].erase(queue_op[banks[j]].begin()+i);
                        return -2;
                    }
                }
            }
        }
    }
    return 0;
}

int MRM::update(int bankNum)
{
// cout<<"update:"<<current[bankNum].remaining_cycles<<"\n";
    if (current[bankNum].remaining_cycles==1){
            cout<<"core:"<<current[bankNum].indx<<" cycle "<<current[bankNum].startCycle<<"-"<<clock_core[bankNum]<<(current[bankNum].isLW?":LW":":SW")<<" process completed ";
            current[bankNum].remaining_cycles=0;
            if (current[bankNum].isLW == 0){
                cout<<queue_op[bankNum][0][2]<<"-"<<queue_op[bankNum][0][2]<<"="<<queue_op[bankNum][0][8]<<"\n";
            }
            else{
                cout<<"\n";
            }
    }



    else if (current[bankNum].remaining_cycles==0){

        if (!queue_op[bankNum].empty()){
            
            priority_row1 = rowbuffer[bankNum];
            sort(queue_op[bankNum].begin(),queue_op[bankNum].end(),[&](vector<int>a,vector<int>b){return sortVec(a,b);});
            cout<<"core:"<<queue_op[bankNum][0][7]<<" cycle "<<clock_core[bankNum]<<" MRM:Request Sent to Dram\n";

            clock_core[bankNum]++;
            current[bankNum].reg0 = queue_op[bankNum][0][1];
            current[bankNum].isLW = queue_op[bankNum][0][0];
            int curr_row = rowbuffer[bankNum];
            int required_row = queue_op[bankNum][0][4] ;

            if (curr_row == required_row){
                current[bankNum].remaining_cycles = coldelay;

            }
            else if (prev_row_changed){
                current[bankNum].remaining_cycles = 2*rowdelay + coldelay;
                prev_row_changed = false;
            }
            else {
                current[bankNum].remaining_cycles = rowdelay + coldelay;
                prev_row_changed = false;
            }
            current[bankNum].startCycle = clock_core[bankNum];
            current[bankNum].indx =  queue_op[bankNum][0][7];
            rowbuffer[bankNum] = required_row ;
            if (queue_op[bankNum][0][0] == 1){
                prev_row_changed = true;
            }


            queue_op[bankNum].erase(queue_op[bankNum].begin());
            
            // only would work for 4 cores

        }
        else{
            return -1;
        }

    }
    else{
        current[bankNum].remaining_cycles--;
    }    
    return 0;

}

int MRM::check_beq_bne(int r1,int r2,vector<int>banks,int index){
    if(r2==-1){

        for(int j=0;j<banks.size();j++){
            for(int i=0;i<queue_op[banks[j]].size();i++){
                vector<int>row = queue_op[banks[j]][i];
                if(row[7]==index){
                    if(row[0]==0){
                        if(row[1]==r1){
                            currBank = banks[j];
                            return -1;
                        }
                    }
                }
            }
        }
    }else{
        for(int j=0;j<banks.size();j++){
            for(int i=0;i<queue_op[banks[j]].size();i++){
                vector<int>row = queue_op[banks[j]][i];
                if(row[7]==index){
                    if(row[0]==0){
                        if(row[1]==r1||row[1]==r2){
                            currBank = banks[j];
                            return -1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// forwarding,sw-sw ,


//clock_cycle
int MRM::check_sw_lw(int r1,int r2,int address,int cur_ins,vector<int>banks,int index){
    for(int j=0;j<banks.size();j++){
        for(int i=0;i<queue_op[banks[j]].size();i++){
            vector<int>row = queue_op[banks[j]][i];
            if(row[7]==index){
                if(row[0]==0){
                    if(row[1]==r1 && cur_ins==0){                
                        cout<<"Core: "<<index<<" "<<"MRM cycle no:"<<clock_core[j]<<" LW process at line number :"<<row[3]+1<<" removed from queue\n";
                        clock_core[index]++;
                        queue_op[banks[j]].erase(queue_op[banks[j]].begin()+i);
                        return 0;
                    }
                    if(row[1]==r2){
                        return -1;
                    }
                    else if(row[5]==r1){
                        return -1;
                    }
                }
                else{
                    if (row[2]==address && cur_ins==0){
                        stored_value=row[1];
                        cout<<"Core: "<<index<<" "<<"MRM Forwarding ,cycle no:"<<clock_core[j]<<" ";
                        clock_core[index]++;
                        return -2;
                    }
                    else if (row[2]==address && cur_ins==1){
                        stored_value=row[3];
                        cout<<"Core: "<<index<<" "<<"MRM cycle no:"<<clock_core[j]<<" SW process at line number :"<<row[3]+1<<" removed from queue\n";
                        clock_core[index]++;
                        queue_op[banks[j]].erase(queue_op[banks[j]].begin()+i);
                        return 0;
                    }


                }
            }
        }
    }
    return 0;
}

