#include<bits/stdc++.h>
#include "mrm.h"
using namespace std;




MRM::MRM(int r,int c){
    rowdelay=r;
    coldelay=c;
    dram.resize(1048576,0);
    rowbufferUpdate.resize(4,0);
    rowbuffer.resize(4,-1);
    token_core.resize(4);
    doWriteBack.resize(4,true);
}

vector<int> MRM::indx_in_bank(int bankNum,int numCores){
	vector<int>indexes;
	int lowerlimit = 256*(bankNum);
	int upperlimit = 256*(bankNum+1)-1;
	for(int i=0;i<numCores;i++){
		int indexlowerlimit = (1024/numCores)*(i);
		int indexupperlimit = (1024/numCores)*(i+1)-1;
		if(upperlimit-indexlowerlimit>0&&lowerlimit-indexupperlimit<0){
            if(indexCompleted[i]==false){
                indexes.push_back(i);
            }
		}
	}
	return indexes;
}
string getIns(vector<string>t){
    string result = "";
    for (string s:t){
        result+=s;
        result+=" ";
    }
    return result;
}


vector<int> MRM::getBanks(int index, int totalCores){
    vector<int>banks;
    int lowerlimit = (1024/totalCores)*(index-1);
    int upperlimit = (1024/totalCores)*(index)-1;
    for(int i=0;i<4;i++){
        int bankLowerlimit = (256)*(i);
        int bankupperlimit = (256)*(i+1)-1;
        if(upperlimit-bankLowerlimit>0&&lowerlimit-bankupperlimit<0){
            banks.push_back(i);
        }
    }
    return banks;
}
void MRM::setClockCore(int index,int totalCores, int bankNum){
    for (auto u:getBanks(index,totalCores)){
        if(clock_core[u]< LastDRAM_cycle[index]){
            clock_core[u] = LastDRAM_cycle[index];
        }
        if(u==bankNum){
            clock_core[u] = LastDRAM_cycle[index];
        }
    }
}
bool MRM::sortVec(const vector<int>& a,const vector<int>& b/*,int priority_row1=priority_row1,int priority_row2=priority_row2*/){

    int x=a[4];
    int y=b[4];
    if (x==priority_row1[currBankSort]){

        if (y == priority_row1[currBankSort]){
            if (a[8] == priority_num[currBankSort]){
                return true;
            }
            else if (b[8] == priority_num[currBankSort]){
                return false;
            }
            else{
                return a[8]<b[8];
            }
        }
        return true;
    }
    else if (y==priority_row1[currBankSort]){
        return false;
    }
    else if (x == priority_row2[currBankSort]){

        if (a[8] == priority_num[currBankSort]){
                return true;
            }
            else if (b[8] == priority_num[currBankSort]){
                return false;
            }
            else{
                return a[8]<b[8];
            }

        return true;
    }
    else if (y==priority_row2[currBankSort]){
        return false;
    }
    else{
        if (a[4] == b[4]){
            if (a[8] == priority_num[currBankSort]){
                return true;
            }
            else if (b[8] == priority_num[currBankSort]){
                return false;
            }
            else{
                return a[8]<b[8];
            }
        }
        return a[4]<b[4];
    }

};
int MRM::checkSafe_op(int r1,int r2,int r3,vector<int>banks,int index){
    for(int j=0;j<banks.size();j++){
        for(int i=0;i<queue_op[banks[j]].size();i++){
            vector<int>row = queue_op[banks[j]][i];
            if(row[7]==index){
                if(row[0]==1){
                  
                    if(row[1]==r2||row[1]==r3){
                        currBank = banks[j];
                        // stored_value = row[4];
                        notSafeRow[currBank] = row[8];
                        return -1;
                    }
                    else if (row[1]==r1){
                        // while (rCycles.find(clock_core[j])!=rCycles.end()){
                            
                        // }
                        cout<</*"core:"<<index<<" "<<*/"cycle no:"<<(clock_core[j]+1)<<" (MRM)LW process :"<<getIns(token_core[row[7]-1][row[3]])<<"removed from queue\n";
                        MRM_Delay+=1;
                        rCycles.insert(clock_core[banks[j]]);
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
                if(row[0]==0){
                   
                }else{
                    if(row[1]==r2){
                        currBank = banks[j];
                        notSafeRow[currBank] = row[8];
                        return -1;
                    }
                    else if (row[1]==r1){
                        //mrm delay
                        cout<</*"core:"<<index<<" "<<*/"cycle no:"<<(clock_core[j]+1)<<"(MRM)LW process "<<getIns(token_core[row[7]-1][row[3]])<<"removed from queue\n";
                        MRM_Delay+=1;
                        rCycles.insert(clock_core[banks[j]]);
                        queue_op[banks[j]].erase(queue_op[banks[j]].begin()+i);
                        return -2;
                    }
                }
            }
        }
    }
    return 0;
}


void MRM::request_to_DRAM(int bankNum,int numCores){
    ins_executed++;
    priority_row1[bankNum] = rowbuffer[bankNum];
    if (clock_core[bankNum]+1>max_cycle){
        queue_op[bankNum].clear();
        current[bankNum].remaining_cycles = 0;
        return;


    }
    if (priority_num[bankNum]!=-1){

        for (auto process:queue_op[bankNum]){
            if (process[8] == priority_num[bankNum]){
                priority_row2[bankNum] = process[4];
                break;
            }
        }
        priority_num[bankNum]=-1;

    }
    currBankSort = bankNum;
    sort(queue_op[bankNum].begin(),queue_op[bankNum].end(),[&](vector<int>a,vector<int>b){return sortVec(a,b);});
    if(queue_op[bankNum][0][4]==rowbuffer[bankNum]||rowbuffer[bankNum]==-1){
        cout<</*"core:"<<queue_op[bankNum][0][7]<<*/"cycle "<<(clock_core[bankNum]+1)<<" MRM:Request Sent to Dram on bank number: "<<bankNum<<"\n";
        MRM_Delay+=1;
        rCycles.insert(clock_core[bankNum]);

        current[bankNum].reg0 = queue_op[bankNum][0][1];
        current[bankNum].isLW = queue_op[bankNum][0][0];
        int curr_row = rowbuffer[bankNum];
        int required_row = queue_op[bankNum][0][4] ;

        if (curr_row == required_row){
            if (clock_core[bankNum]+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }

            current[bankNum].remaining_cycles = coldelay;
            current[bankNum].waiting_cycle = coldelay;

        }
        else if (prev_row_changed[bankNum]){
            if (clock_core[bankNum]+2*rowdelay+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
            current[bankNum].remaining_cycles = 2*rowdelay + coldelay;
            current[bankNum].waiting_cycle = 2*rowdelay + coldelay;
            rowbufferUpdate[bankNum]+=1;
            prev_row_changed[bankNum] = false;
        }
        else {
            if (clock_core[bankNum]+rowdelay+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
            current[bankNum].remaining_cycles = rowdelay + coldelay;
            current[bankNum].waiting_cycle = rowdelay + coldelay;
            rowbufferUpdate[bankNum]+=1;
            prev_row_changed[bankNum] = false;
        }
        current[bankNum].startCycle = clock_core[bankNum];
        current[bankNum].indx =  queue_op[bankNum][0][7];
        current[bankNum].address = queue_op[bankNum][0][2];
        current[bankNum].SW_value = queue_op[bankNum][0][9];
        rowbuffer[bankNum] = required_row ;
        current[bankNum].regLW = token_core[current[bankNum].indx-1][queue_op[bankNum][0][3]][1];
        if (queue_op[bankNum][0][0] == 1){
            prev_row_changed[bankNum] = true;
        }


        queue_op[bankNum].erase(queue_op[bankNum].begin());

    }else{
        vector<int>indexes = indx_in_bank(bankNum,numCores);
        if(indexes.size()<=1){
            cout<</*"core:"<<queue_op[bankNum][0][7]<<*/"cycle "<<(clock_core[bankNum]+1)<<" MRM:Request Sent to Dram on bank number: "<<bankNum<<"\n";
            MRM_Delay+=1;
            rCycles.insert(clock_core[bankNum]);
            // clock_core[bankNum]+=1;
            current[bankNum].reg0 = queue_op[bankNum][0][1];
            current[bankNum].isLW = queue_op[bankNum][0][0];
            int curr_row = rowbuffer[bankNum];
            int required_row = queue_op[bankNum][0][4] ;

            if (curr_row == required_row){
                if (clock_core[bankNum]+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
                current[bankNum].remaining_cycles = coldelay;
                current[bankNum].waiting_cycle = coldelay;

            }
            else if (prev_row_changed[bankNum]){
                if (clock_core[bankNum]+2*rowdelay+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
                current[bankNum].remaining_cycles = 2*rowdelay + coldelay;
                current[bankNum].waiting_cycle = 2*rowdelay + coldelay;
                rowbufferUpdate[bankNum]+=1;
                prev_row_changed[bankNum] = false;
            }
            else {
                if (clock_core[bankNum]+rowdelay+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
                current[bankNum].remaining_cycles = rowdelay + coldelay;
                current[bankNum].waiting_cycle = rowdelay + coldelay;
                rowbufferUpdate[bankNum]+=1;
                prev_row_changed[bankNum] = false;
            }
            current[bankNum].startCycle = clock_core[bankNum];
            current[bankNum].indx =  queue_op[bankNum][0][7];
            current[bankNum].address = queue_op[bankNum][0][2];
            current[bankNum].SW_value = queue_op[bankNum][0][9];
            rowbuffer[bankNum] = required_row ;
            current[bankNum].regLW = token_core[current[bankNum].indx-1][queue_op[bankNum][0][3]][1];
            if (queue_op[bankNum][0][0] == 1){
                prev_row_changed[bankNum] = true;
            }


            queue_op[bankNum].erase(queue_op[bankNum].begin());
        }else{
            for(int i=0;i<queue_op[bankNum].size();i++){
                if(queue_op[bankNum][i][7]!=current[bankNum].indx){
                    priority_row1[bankNum] = queue_op[bankNum][i][4];
                    break;
                }
            }
            currBankSort = bankNum;
            sort(queue_op[bankNum].begin(),queue_op[bankNum].end(),[&](vector<int>a,vector<int>b){return sortVec(a,b);});
            cout<</*"core:"<<queue_op[bankNum][0][7]<<*/"cycle "<<(clock_core[bankNum]+1)<<" MRM:Request Sent to Dram on bank number: "<<bankNum<<"\n";
            MRM_Delay+=1;
            rCycles.insert(clock_core[bankNum]);
            current[bankNum].reg0 = queue_op[bankNum][0][1];
            current[bankNum].isLW = queue_op[bankNum][0][0];
            int curr_row = rowbuffer[bankNum];
            int required_row = queue_op[bankNum][0][4] ;

            if (curr_row == required_row){
                if (clock_core[bankNum]+rowdelay*0+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
                current[bankNum].remaining_cycles = coldelay;
                current[bankNum].waiting_cycle = coldelay;

            }
            else if (prev_row_changed[bankNum]){
                if (clock_core[bankNum]+2*rowdelay+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
                current[bankNum].remaining_cycles = 2*rowdelay + coldelay;
                current[bankNum].waiting_cycle = 2*rowdelay + coldelay;
                rowbufferUpdate[bankNum]+=1;
                prev_row_changed[bankNum] = false;
            }
            else {
                if (clock_core[bankNum]+rowdelay+coldelay>max_cycle){
                    queue_op[bankNum].clear();
                    current[bankNum].remaining_cycles = 0;
                    return;
            }
                current[bankNum].remaining_cycles = rowdelay + coldelay;
                current[bankNum].waiting_cycle = rowdelay + coldelay;
                rowbufferUpdate[bankNum]+=1;
                prev_row_changed[bankNum] = false;
            }
            current[bankNum].startCycle = clock_core[bankNum];
            current[bankNum].indx =  queue_op[bankNum][0][7];
            current[bankNum].address = queue_op[bankNum][0][2];
            current[bankNum].SW_value = queue_op[bankNum][0][9];
            rowbuffer[bankNum] = required_row ;
            current[bankNum].regLW = token_core[current[bankNum].indx-1][queue_op[bankNum][0][3]][1];
            if (queue_op[bankNum][0][0] == 1){
                prev_row_changed[bankNum] = true;
            }


            queue_op[bankNum].erase(queue_op[bankNum].begin());

        }
    }
    
}

int MRM::update(int bankNum,int numCores)
{
    if (current[bankNum].remaining_cycles==1){
            cout<<"core:"<<current[bankNum].indx<<"cycle "<<(current[bankNum].startCycle+1)<<"-"<<(current[bankNum].startCycle + current[bankNum].waiting_cycle )<<(current[bankNum].isLW?":LW":":SW")<<" process completed on bank number "<<bankNum;
            current[bankNum].remaining_cycles=0;
            LastDRAM_cycle[current[bankNum].indx] = current[bankNum].startCycle+current[bankNum].waiting_cycle;
            if (current[bankNum].isLW == 0){
                dram[current[bankNum].address] = current[bankNum].SW_value;
                string memup = to_string(current[bankNum].address)+"-"+to_string(current[bankNum].address + 3);
                memoryupdate[memup] = current[bankNum].SW_value;
                cout<<": "<<current[bankNum].address<<"-"<<(current[bankNum].address +3)<<" = "<<dram[current[bankNum].address]<<"\n";
            }
            else{
                regesterFile[current[bankNum].indx-1][current[bankNum].reg0] = dram[current[bankNum].address];
                cout<<": "<<current[bankNum].regLW<<" = "<<regesterFile[current[bankNum].indx-1][current[bankNum].reg0]<<"\n";
            }
            setClockCore(current[bankNum].indx,numCores,bankNum);
            current[bankNum].isLW=-1;
            if(!queue_op[bankNum].empty()){
                // if (rCycles.find(clock_core[bankNum])==rCycles.end()){
                    request_to_DRAM(bankNum,numCores);
                // }

            }else{
                //DRAM
                vector<int>complete_index = indx_in_bank(bankNum,numCores);
                if(complete_index.size()==0){
                    writeBackCycle[bankNum] = (current[bankNum].startCycle + current[bankNum].waiting_cycle);
                }
            }
    }
//10
    //9 


    else if (current[bankNum].remaining_cycles==0){

        if (!queue_op[bankNum].empty()){
            // if (rCycles.find(clock_core[bankNum])==rCycles.end()){
                    request_to_DRAM(bankNum,numCores);
                // }

        }
        else{
            vector<int>complete_index = indx_in_bank(bankNum,numCores);
            if(complete_index.size()==0){
                writeBackCycle[bankNum] = (current[bankNum].startCycle + current[bankNum].waiting_cycle);
            }
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
                    if(row[0]==1){
                        if(row[1]==r1){
                            currBank = banks[j];
                            notSafeRow[currBank] = row[8];
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
                    if(row[0]==1){
                        if(row[1]==r1||row[1]==r2){
                            currBank = banks[j];
                            notSafeRow[currBank] = row[8];
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
        for(int i=0;i<queue_op[banks[j]].size();i++){
            vector<int>row = queue_op[banks[j]][i];
            if(row[7]==index){
                if(row[0]==1){
                    if(row[1]==r1 && cur_ins==0){                
                        cout<<"core:"<<index<<" "<<"cycle no:"<<(clock_core[j]+1)<<" (MRM) LW process "<<getIns(token_core[row[7]-1][row[3]])<<"removed from queue\n";
                        MRM_Delay+=1;
                        rCycles.insert(clock_core[banks[j]]);
                        queue_op[banks[j]].erase(queue_op[banks[j]].begin()+i);
                        return 0;
                    }
                    if(row[1]==r2){
                        currBank = banks[j];
                        notSafeRow[currBank] = row[8];
                        return -1;
                    }
                    else if(row[5]==r1){
                        currBank = banks[j];
                        notSafeRow[currBank] = row[8];
                        return -1;
                    }
                }
                else{
                    if (row[2]==address && cur_ins==0){
                        stored_value={i,j};
                        
                        MRM_Delay+=1;
                        rCycles.insert(clock_core[banks[j]]);
                        return -2;
                    }
                    else if (row[2]==address && cur_ins==1){
                        // stored_value=row[3];
                        cout<<"core:"<<index<<" "<<"cycle no:"<<(clock_core[j]+1)<<"(MRM)SW process at "<<getIns(token_core[row[7]-1][row[3]])<<"removed from queue\n";
                        MRM_Delay+=1;
                        rCycles.insert(clock_core[banks[j]]);
                        queue_op[banks[j]].erase(queue_op[banks[j]].begin()+i);
                        return 0;
                    }


                }
            }
        }
    }
    return 0;
}

