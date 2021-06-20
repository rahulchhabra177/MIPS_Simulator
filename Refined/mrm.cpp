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
bool MRM::sortVec(const vector<int>& a,const vector<int>& b){
    return a[4]<b[4];
};
int MRM::checkSafe_op(int r1,int r2,int r3,vector<int>banks,int index){
    for(int j=0;j<banks.size();j++){
        for(int i=0;i<queue_op[j].size();i++){
            vector<int>row = queue_op[j][i];
            if(row[7]==index){
                if(row[0]==1){
                    if(row[5]==r1){
                        currBank = j;
                        return -1;
                    }else if(row[1]==r1){
                        currBank = j;
                        return -1;
                    }
                }
                else{
                    if(row[5]==r1){
                        currBank = j;
                        return -1;
                    }else if(row[1]==r1||row[1]==r2||row[1]==r3){
                        currBank = j;
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}
int MRM::checkSafe_addi(int r1,int r2,vector<int>banks,int index){

    for(int j=0;j<banks.size();j++){
        for(int i=0;i<queue_op[j].size();i++){
            vector<int>row = queue_op[j][i];
            if(row[7]==index){
                if(row[0]==1){
                    if(row[5]==r1){
                        currBank = j;
                        return -1;
                    }else if(row[1]==r1){
                        currBank = j;
                        return -1;
                    }
                }else{
                    if(row[5]==r1){
                        currBank = j;
                        return -1;
                    }else if(row[1]==r1||row[1]==r2){
                        currBank = j;
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}
int MRM::check_beq_bne(int r1,int r2,vector<int>banks,int index){
    if(r2==-1){

        for(int j=0;j<banks.size();j++){
            for(int i=0;i<queue_op[j].size();i++){
                vector<int>row = queue_op[j][i];
                if(row[7]==index){
                    if(row[0]==0){
                        if(row[1]==r1){
                            currBank = j;
                            return -1;
                        }
                    }
                }
            }
        }
    }else{
        for(int j=0;j<banks.size();j++){
            for(int i=0;i<queue_op[j].size();i++){
                vector<int>row = queue_op[j][i];
                if(row[7]==index){
                    if(row[0]==0){
                        if(row[1]==r1||row[1]==r2){
                            currBank = j;
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



int MRM::check_sw_lw(int r1,int r2,int address,int cur_ins,vector<int>banks,int index){
    for(int j=0;j<banks.size();j++){
        for(int i=0;i<queue_op[j].size();i++){
            vector<int>row = queue_op[j][i];
            if(row[7]==index){
                if(row[0]==0){
                    /*if(row[1]==r1 && cur_ins==0){                
                        cout<<"MRM cycle no:"<<clock_cycle<<" LW process at line number :"<<row[3]+1<<" removed from queue\n";
                        clock_cycle++;
                        cout<<"ini "<<queue_op[j].size()<<"\n";
                        queue_op[j].erase(queue_op[j].begin()+i);
                        cout<<"fin "<<queue_op[j].size()<<"\n";
                        return 0;
                    }*/
                    if(row[1]==r2){
                        return -1;
                    }
                    else if(row[5]==r1){
                        return -1;
                    }
                }
                /*else{
                    if (row[2]==address && cur_ins==0){
                        stored_value=row[1];
                        cout<<"MRM Forwarding ,cycle no:"<<clock_cycle;
                        clock_cycle++;
                        return -2;
                    }
                    else if (row[2]==address && cur_ins==1){
                        stored_value=row[3];
                        cout<<"MRM cycle no:"<<clock_cycle<<" SW process at line number :"<<row[3]+1<<" removed from queue\n";
                        clock_cycle++;
                        queue_op[j].erase(queue_op[j].begin()+i);
                        return 0;
                    }


                }*/
            }
        }
    }
    return 0;
}

