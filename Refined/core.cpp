#include <bits/stdc++.h>
#include "core.h"
using namespace std;



void Core::print_reg(vector<int> regesterFile){
    for(int i =0;i<regesterFile.size();i++){
        char hex[20];
        sprintf(hex,"%X",regesterFile[i]);
        cout<<hex<<" ";
    }
    cout<<"\n";
}
//convert registers
int Core::convertRegisters(string s){
    if(s=="$zero"||s=="$0"){
        return 0;
    }else if(s=="$at"||s=="$1"){
        return 1;
    }else if(s=="$gp"||s=="$28"){
        return 28;
    }else if(s=="$sp"||s=="$29"){
        return 29;
    }else if(s=="$fp"||s=="$30"){
        return 30;
    }else if(s=="$ra"||s=="$31"){
        return 31;
    }else if(s=="$v0"||s=="$2"){
        return 2;
    }else if(s=="$v1"||s=="$3"){
        return 3;
    }else{
        if(s.substr(0,1)!="$"){
            return -1;
        }
        int pos = s.find_first_of(alpha);
        if(pos==-1){
            string s1 = s.substr(1);
            return stoi(s1); 
        }else{
            if(s.at(1)=='a'){
                string s1 = s.substr(2);
                return 4+stoi(s1);
            }else if(s.at(1)=='t'){
                string s1 = s.substr(2);
                return 8+stoi(s1);
            }else if(s.at(1)=='s'){
                string s1 = s.substr(2);
                return 18+stoi(s1);
            }else if(s.at(1)=='k'){
                string s1 = s.substr(2);
                return 26+stoi(s1);
            }else{
                return -1;
            }
        }
    }      
}

int Core::getBankNum(int address){

    int BankSize=1024*256;
    if (address<BankSize){
        return 0;
    }
    else if (address>=BankSize && address<BankSize*2){
        return 1;
    }
    else if (address>=BankSize*2 && address<BankSize*3){
        return 2;
    }
    else if (address<BankSize<4){
        return 3;
    }
    else{
        return -1;
    }



}
int Core::coreAddress(int address,int index){
    int part = 1024/totalCores;
    return address+(part)*(index-1)*1024;
}

int Core::addressnew(string s){
    string const parenl = {"("};
    string const parenr = {")"};
    int pos = s.find_first_of(parenl);
    int address;
    if(pos!=-1){
        if(pos==0){
            int pr = s.find_first_of(parenr);
            address = stoi(s.substr(1,pr-pos-1));
        }else{
            int front = stoi(s.substr(0,pos));
            int pr = s.find_first_of(parenr);
            string saddress = (s.substr(pos+1,pr-pos-1));
            address = convertRegisters(saddress);
            address = regesterFile[address];
            cout<<"string: "<<saddress<<" "<<"digit"<<address<<"\n";
            if(front%4!=0){
                return -1;
            }
            address = address+front;
        }
    }else{
        address = stoi(s);
    }
    cout<<"final address:"<<address<<"\n";
    if(address%4!=0){
        return -1;
    } 
    return address;
}
int Core::row_regester(string s){
    string const parenl = {"("};
    string const parenr = {")"};
    int pos = s.find_first_of(parenl);
    int rowno;
    if(pos!=-1){
        if(pos==0){
            int pr = s.find_first_of(parenr);
            string stradress = (s.substr(1,pr-pos-1));
            rowno = convertRegisters(stradress);
            return rowno;
        }
        else{
            int pr = s.find_first_of(parenr);
            string stradress = (s.substr(pos+1,pr-pos-1));
            rowno = convertRegisters(stradress);
            return rowno;
        }
    }else{
        return -1;
    }
}
int Core::rownumber(string s){
    string const parenl = {"("};
    string const parenr = {")"};
    int pos = s.find_first_of(parenl);
    int rowno;
    if(pos!=-1){
        if(pos==0){
            int pr = s.find_first_of(parenr);
            string stradress = (s.substr(1,pr-pos-1));
            rowno = convertRegisters(stradress);
            mrm->current[index].reg1 = rowno;
            rowno = regesterFile[rowno];
            return rowno;
        }else{
            int pr = s.find_first_of(parenr);
            string stradress = (s.substr(pos+1,pr-pos-1));
            rowno = convertRegisters(stradress);
            mrm->current[index].reg1 = rowno;
            if(rowno==-1){
                return -1;
            }
            rowno  = regesterFile[rowno];
            if(rowno%4!=0){
                return -1;
            }
            return rowno;
        }
    }else{
        return -1;
    }
}
int Core::colnumber(string s){
    string const parenl = {"("};
    string const parenr = {")"};
    int pos = s.find_first_of(parenl);
    int address;
    if(pos!=-1){
        if(pos==0){
            return 0;
        }else{
            address = stoi(s.substr(0,pos));
            if(address%4!=0){
                return -1;
            }
            return address;
        }
    }else{
        return -1;
    }
}
bool Core::isSafeOp(int reg1,int reg2,int reg3){
    if(mrm->current[index].isLW==0){
        if(reg1 == mrm->current[index].reg0){
            return false;
        }
        else{
            //why this?
            if(reg1==mrm->current[index].reg1){
                return false;
            }else{
                return true;
            }
        }
    }else{
        if(reg1==mrm->current[index].reg0||reg2 == mrm->current[index].reg0||reg3==mrm->current[index].reg0){
            return false;
        }else{
            if(mrm->current[index].reg1==reg1){
                return false;
            }
            return true;
        }
    }
}
bool Core::isSafe_beq_bne(int reg1,int reg2){
    if(mrm->current[index].isLW==1){
        if(reg1==mrm->current[index].reg0||reg2 == mrm->current[index].reg0){
            return false;
        }else{
            return true;
        }
    }else{
        return true;
    }
}
bool Core::isSafeAddi(int reg1,int reg2){
    if(mrm->current[index].isLW==0){
        if(reg1==mrm->current[index].reg0){
            return false;
        }else{
            if(reg1==mrm->current[index].reg1){
                return false;
            }else{
                return true;
            }
        }
    }else{
        if(reg1==mrm->current[index].reg0||reg2==mrm->current[index].reg0){
            return false;
        }else{
            if(mrm->current[index].reg1==reg1){
                return false;
            }
            return true;
        }
    }
}
int Core::beq(vector<string> tokens,int lineno,map<string,int>labels){
    string label = tokens[3];
    int r0 = convertRegisters( tokens[1]);
    int r1;
    if(tokens[2].substr(0,1)=="$"){
        r1 = convertRegisters( tokens[2]);
        if(regesterFile[(r0)]==regesterFile[(r1)]){
            lineno = labels[label];
        }else{
            lineno++;
        }
    }else{
        r1 = stoi(tokens[2]);
        if(regesterFile[(r0)]==(r1)){
            lineno = labels[label]-1;
        }else{
            lineno++;
        }
    }
    return lineno;
}
int Core::bne(vector<string> tokens,int lineno,map<string,int>labels){
    string label = tokens[3];
    int r0 = convertRegisters( tokens[1]);
    int r1;
    if(tokens[2].substr(0,1)=="$"){
        r1 =convertRegisters( tokens[2]);
        if(regesterFile[(r0)]!=regesterFile[(r1)]){
            lineno = labels[label];
        }else{
            lineno++;
        }
    }else{
        r1 = stoi(tokens[2]);
        if(regesterFile[(r0)]!=(r1)){
            lineno = labels[label]-1;
        }else{
            lineno++;
        }
    }
    return lineno;
}

int Core::j(vector<string> tokens,int lineno,map<string,int>labels){
    if(tokens.size()!=2){
        cerr<<"Syntax Error.\n";
        return -1;
    }else{
        string label = tokens[1];
        lineno = labels[label]-1;
        return lineno;
    }
}
int Core::lw(vector<string> tokens,int lineno,int ins_size,int abs_lineno,int change){
    int r0 = convertRegisters( tokens[1]);
    int row = rownumber(tokens[2]);
    int col = colnumber(tokens[2]);
    int address = row+col;
    int addressnew = coreAddress(address,index);
    int rowb = (addressnew)/1024;
    //cout<<"address lw "<<addressnew<<" "<<index<<"\n";
    //cout<<"mrm->current row buffer: "<<mrm->rowbuffer<<"\n";
    
    
    //Assignment 4 start
    int row_reg = row_regester(tokens[2]);
    vector<int>current_op;
    current_op.resize(8,0);
    current_op[0] = 0;
    current_op[1] = r0;
    current_op[2] = addressnew;
    current_op[3] = lineno;
    current_op[4] = rowb;
    current_op[5] = row_reg;
    current_op[6] = 1;
    current_op[7] = index;
    int aq = mrm->Mem_Req_Order(mrm->rowbuffer[getBankNum(addressnew)],mrm->queue_op[getBankNum(addressnew)],current_op,abs_lineno,ins_size, change);
    //cout<<"lw:"<<aq<<"\n";
    if(aq==-1){
        mrm->clock_core[index]++;
        sequence_cycle = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": DRAM request issued\n";

        mrm->clock_arr[getBankNum(addressnew)] = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]+1<<"-";
        mrm->clock_core[index]+=rowdelay;
        mrm->rowbuffer[getBankNum(addressnew)] = rowb;
        mrm->clock_core[index]+=coldelay;
        regesterFile[r0] = mrm->dram[addressnew];
        mrm->rowbufferUpdate[0]+=1;
        cout<<mrm->clock_core[index]<<": "<<tokens[1]<<" = "<<regesterFile[r0]<<"\n";
        mrm->current[index].reg0 = r0;mrm->current[index].isLW = 1;
    }else if(aq==1){
        mrm->clock_core[index]++;
        sequence_cycle = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": DRAM request issued\n";

        mrm->clock_arr[getBankNum(addressnew)] = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]+1<<"-";
        mrm->clock_core[index]+=coldelay;
        regesterFile[r0] = mrm->dram[addressnew];
        cout<<mrm->clock_core[index]<<": "<<tokens[1]<<" = "<<regesterFile[r0]<<"\n";
        mrm->current[index].reg0 = r0;mrm->current[index].isLW = 1;
        if(change==1){
            mrm->queue_op[getBankNum(addressnew)].erase(mrm->queue_op[getBankNum(addressnew)].begin());
        }
    }else if(aq==2){
        mrm->clock_core[index]++;
        sequence_cycle = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": DRAM request issued\n";

        mrm->clock_arr[getBankNum(addressnew)] = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]+1<<"-";
        mrm->clock_core[index]+=2*rowdelay;
        mrm->rowbuffer[getBankNum(addressnew)]=rowb;
        mrm->clock_core[index]+=coldelay;
        regesterFile[r0] = mrm->dram[addressnew];
        mrm->rowbufferUpdate[0]+=2;
        cout<<mrm->clock_core[index]<<": "<<tokens[1]<<" = "<<regesterFile[r0]<<"\n";
        mrm->current[index].reg0 = r0;mrm->current[index].isLW = 1;
        mrm->queue_op[getBankNum(addressnew)].erase(mrm->queue_op[getBankNum(addressnew)].begin());
    }else if(aq==0){
        mrm->queue_op[getBankNum(addressnew)].push_back(current_op);
    }
    //Assignment 4 end

   
    return 0;
}
int Core::sw(vector<string> tokens,int lineno,int ins_size, int abs_lineno, int change){
    int r0 = convertRegisters( tokens[1]);
    int row = rownumber(tokens[2]);
    int col = colnumber(tokens[2]);
    int address = row+col;
    int addressnew = coreAddress(address,index);
    int rowb = addressnew/1024;
    //cout<<"address sw "<<addressnew<<" "<<index<<"\n";
    string memup = to_string(addressnew)+"-"+to_string(addressnew+3);
    mrm->memoryupdate[memup] = regesterFile[r0];
    //cout<<"mrm->current row buffer: "<<mrm->rowbuffer<<"\n";

    //Assignment 4 start
    int row_reg = row_regester(tokens[2]);
    vector<int> current_op;
    current_op.resize(8,0);
    current_op[0] = 1;
    current_op[1] = r0;
    current_op[2] = addressnew;
    current_op[3] = lineno;
    current_op[4] = rowb;
    current_op[5] = row_reg;
    current_op[6] = 1;
    current_op[7] = index;
    int aq = mrm->Mem_Req_Order(mrm->rowbuffer[getBankNum(addressnew)],mrm->queue_op[getBankNum(addressnew)],current_op,abs_lineno,ins_size, change);
    //cout<<"sw:"<<aq<<"\n";
    if(aq==-1){
        mrm->clock_core[index]++;
        sequence_cycle = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": DRAM request issued\n";
        mrm->clock_arr[getBankNum(addressnew)] = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]+1<<"-";
        mrm->clock_core[index]+=rowdelay;
        mrm->rowbuffer[getBankNum(addressnew)] = rowb;
        mrm->clock_core[index]+=coldelay;
        mrm->dram[addressnew] = regesterFile[r0];
        cout<<mrm->clock_core[index]<<": memory address "<<memup<<" = "<<mrm->memoryupdate[memup]<<"\n";
        mrm->rowbufferUpdate[0]+=1;
        mrm->current[index].reg0 = r0;mrm->current[index].isLW = 0;
    }else if(aq==1){
        mrm->clock_core[index]++;

        sequence_cycle = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": DRAM request issued\n";
        
        mrm->clock_arr[getBankNum(addressnew)] = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]+1<<"-";
        mrm->clock_core[index]+=coldelay;
        mrm->dram[addressnew] = regesterFile[r0];
        cout<<mrm->clock_core[index]<<": memory address "<<memup<<" = "<<mrm->memoryupdate[memup]<<"\n";
        mrm->current[index].reg0 = r0;mrm->current[index].isLW = 0;
        if(change==1){
            mrm->queue_op[getBankNum(addressnew)].erase(mrm->queue_op[getBankNum(addressnew)].begin());
        }
    }else if(aq==2){
        mrm->clock_core[index]++;
        sequence_cycle = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": DRAM request issued\n";

        mrm->clock_arr[getBankNum(addressnew)] = mrm->clock_core[index];
        cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]+1<<"-";
        mrm->clock_core[index]+=2*rowdelay;
        mrm->rowbuffer[getBankNum(addressnew)]=rowb;
        mrm->clock_core[index]+=coldelay;
        mrm->dram[addressnew] = regesterFile[r0];
        mrm->rowbufferUpdate[0]+=2;
        cout<<mrm->clock_core[index]<<": memory address "<<memup<<" = "<<mrm->memoryupdate[memup]<<"\n";
        mrm->current[index].reg0 = r0;mrm->current[index].isLW = 0;
        mrm->queue_op[getBankNum(addressnew)].erase(mrm->queue_op[getBankNum(addressnew)].begin());
    }else if(aq==0){
        mrm->queue_op[getBankNum(addressnew)].push_back(current_op);
    }
    //Assignment 4 end

    
    return 0;
}
vector<int> Core::queueBank(int index){
    vector<int>banks;
    int lowerlimit = (1024/totalCores)*(index-1);
    int upperlimit = (1024/totalCores)*(index)-1;
    //cout<<"index "<<index<<" "<<lowerlimit<<" "<<upperlimit<<"\n";
    for(int i=0;i<4;i++){
        int bankLowerlimit = (256)*(i);
        int bankupperlimit = (256)*(i+1)-1;
        if(upperlimit-bankLowerlimit>0&&lowerlimit-bankupperlimit<0){
            //cout<<"index "<<index<<" "<<i<<" "<<bankupperlimit<<" "<<bankLowerlimit<<"\n";
            banks.push_back(i);
        }
    }
    return banks;

}
void Core::queue_func(vector<vector<string>>tokens,map<string,int>labels,int safe,int button)
{
    //cout<<"size "<<mrm->queue_op.size()<<"\n";
    int size = mrm->queue_op[mrm->currBank].size();
    //cout<<"mrm size "<<size<<"\n";
    for(int i = 0;i<size;i++){
        //cout<<"loop "<<safe<<"\n";
        if(lineno==ins_size||safe==-1){
            if(button == -1){
                lineno--;
            }
            button =0;
            if(mrm->queue_op[mrm->currBank].size()!=0){
                sort(mrm->queue_op[mrm->currBank].begin(),mrm->queue_op[mrm->currBank].end(),mrm->sortVec);

                vector<int>curr_fake = mrm->queue_op[mrm->currBank][0];
                while (!mrm->queue_op[mrm->currBank].empty() && curr_fake[6]==0){
                    cout<<"Element popped\n";
                    curr_fake = mrm->queue_op[mrm->currBank][0];
                    mrm->queue_op[mrm->currBank].erase(mrm->queue_op[mrm->currBank].begin());
                }

                if(curr_fake[0]==1){
                    sw(tokens[curr_fake[3]],curr_fake[3],ins_size,lineno,1);
                }else{
                    lw(tokens[curr_fake[3]],curr_fake[3],ins_size,lineno,1);
                }
            }else{
                safe = 0;
                lineno++;
            }
        }
    }
}
void Core::parse(vector<vector<string>> tokens,map<string,int>labels){
    
    // cout<<"core:"<<index<<":"<<mrm->clock_core[index]<<":"<<(int)isCompleted<<"\n";
    vector<int>banks = queueBank(index);
    int safe =0;
    int button =0;
    if (lineno>tokens.size()){isCompleted=true;}
    if(lineno<=tokens.size()){
        if(mrm->clock_core[index]>10000){
            cerr<<"System frezzes"<<"\n";
            return;
        }
        if(tokens[lineno].size()==0){
            lineno++;
            button = -1;
        }
        /*cout<<"line ins "<<lineno<<" "<<ins_size<<"\n";
        //Assignment 4 start
        if(lineno<tokens.size()){
            cout<<"size "<<tokens.size()<<" "<<lineno<<"\n";
            cout<<"line start\n";
            for(int i=0;i<tokens[lineno].size();i++){
                cout<<tokens[lineno][i]<<" ";
            }
        }
        cout<<"\n line ends\n";
        cout<<"queue start\n";
        for(int j=0;j<banks.size();j++){
            for(int i=0;i<mrm->queue_op[banks[j]].size();i++){
                cout<<"bank "<<banks[j]<<"\n";
                for(int k=0;k<mrm->queue_op[banks[j]][i].size();k++){
                    cout<<mrm->queue_op[banks[j]][i][k]<<" ";
                }
                cout<<"\n";
            }
        }
        cout<<"queue ends\n";
*/

        if(lineno==ins_size||safe==-1){
           
            if(button ==-1){
                lineno--;
            }
            button=0;  
            int loopend = 0;
            for(int i=0;i<banks.size();i++){ 
                if(mrm->queue_op[banks[i]].size()!=0){
                    sort(mrm->queue_op[banks[i]].begin(),mrm->queue_op[banks[i]].end(),mrm->sortVec);
                    vector<int> curr_fake = mrm->queue_op[banks[i]][0];
                    loopend++;
                    if(curr_fake[0]==1){
                        sw(tokens[curr_fake[3]],curr_fake[3],ins_size,lineno,1);
                    }else{

                        lw(tokens[curr_fake[3]],curr_fake[3],ins_size,lineno,1);
                    }                


                }else{
                    safe=0;
                }
            }
            if(loopend==0){
                lineno++;
            }

        }
        //Assignment 4 end

        
        
        else{
            string first = tokens[lineno][0];
            //cout<<mrm->current[index].reg0<<" "<<mrm->current[index].reg1<<" "<<mrm->mrm->mrm->mrm->mrm->mrm->mrm->mrm->mrm->current[index].isLW<<"\n";
            if(first=="add"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Error\n";
                        return;
                    }
                    if(sequence_cycle<mrm->clock_core[index]){
                        bool safe_op = isSafeOp(reg1,reg2,reg3);
                        if(safe_op == true){
                            safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                            if(safe==0){
                                sequence_cycle++;
                                regesterFile[reg1] = regesterFile[reg2]+regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }
                            else{
                                queue_func(tokens,labels,safe,button);
                            }

                        }
                        else{
                            safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                            if(safe==0){
                                sequence_cycle=mrm->clock_core[index]+1;
                                mrm->clock_core[index]++;
                                regesterFile[reg1] = regesterFile[reg2]+regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                queue_func(tokens,labels,safe,button);
                            }
                        }
                    }
                    else{
                        safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                        if(safe==0){
                            mrm->clock_core[index]++;sequence_cycle++;
                            regesterFile[reg1] = regesterFile[reg2]+regesterFile[reg3];
                            repetion[2] = repetion[2]+1;
                            cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                            lineno++;
                        }else{
                            queue_func(tokens,labels,safe,button);
                        }
                    }
                }
            }else if(first == "sub"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Error\n";
                        return;
                    }
                    if(sequence_cycle<mrm->clock_core[index]){
                        bool safe_op = isSafeOp(reg1,reg2,reg3);
                        if(safe_op == true){
                            safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                            if(safe==0){
                                sequence_cycle++;
                                regesterFile[reg1] = regesterFile[reg2]-regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                queue_func(tokens,labels,safe,button);
                            }

                        }else{
                            safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                            if(safe==0){
                                sequence_cycle=mrm->clock_core[index]+1;
                                mrm->clock_core[index]++;
                                regesterFile[reg1] = regesterFile[reg2]-regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                queue_func(tokens,labels,safe,button);
                            }
                        }
                    }else{
                        safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                        if(safe==0){
                            mrm->clock_core[index]++;sequence_cycle++;
                            regesterFile[reg1] = regesterFile[reg2]-regesterFile[reg3];
                            repetion[2] = repetion[2]+1;
                            cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                            lineno++;
                        }else{
                            queue_func(tokens,labels,safe,button);
                            
                        }
                    }
                }

            }
            else if(first=="mul"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Error\n";
                        return;
                    }
                    //cout<<"ini line no. "<<lineno<<"\n";
                    if(sequence_cycle<mrm->clock_core[index]){
                        bool safe_op = isSafeOp(reg1,reg2,reg3);
                        if(safe_op == true){
                            safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                            if(safe==0){
                                sequence_cycle++;
                                regesterFile[reg1] = regesterFile[reg2]*regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                queue_func(tokens,labels,safe,button);

                            }

                        }else{
                            safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                            if(safe==0){
                                sequence_cycle=mrm->clock_core[index]+1;
                                mrm->clock_core[index]++;
                                regesterFile[reg1] = regesterFile[reg2]*regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                queue_func(tokens,labels,safe,button);

                            }
                        }
                    }else{
                        safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                        if(safe==0){
                            mrm->clock_core[index]++;sequence_cycle++;
                            regesterFile[reg1] = regesterFile[reg2]*regesterFile[reg3];
                            repetion[2] = repetion[2]+1;
                            cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                            lineno++;
                        }else{
                            queue_func(tokens,labels,safe,button);
                        }
                    }
                    //cout<<"final line no "<<lineno<<"\n";
                }

            }
            else if(first == "beq"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    if(sequence_cycle<mrm->clock_core[index]){
                        bool safe_op = isSafe_beq_bne(reg1,reg2);
                        if(safe_op==true){
                            safe = mrm->check_beq_bne(reg1,reg2,banks,index);
                            if(safe==0){
                                sequence_cycle++;
                                lineno = beq(tokens[lineno],lineno,labels);
                                repetion[3] = repetion[3]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": beq:"<<tokens[lineno][3]<<"\n";
                            }else{
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }else{
                            safe = mrm->check_beq_bne(reg1,reg2,banks,index);
                            if(safe==0){
                                sequence_cycle=mrm->clock_core[index]+1;
                                mrm->clock_core[index]++;
                                lineno = beq(tokens[lineno],lineno,labels);
                                repetion[3] = repetion[3]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": beq:"<<tokens[lineno][3]<<"\n";
                            }else{
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }
                    }else{
                        safe = mrm->check_beq_bne(reg1,reg2,banks,index);
                        if(safe==0){
                            mrm->clock_core[index]++;sequence_cycle++;
                            lineno = beq(tokens[lineno],lineno,labels);
                            repetion[3] = repetion[3]+1;
                            cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": beq:"<<tokens[lineno][3]<<"\n";
                        }else{
                            queue_func(tokens,labels,safe,button);
                        
                        }
                    }
                }

            }
            else if(first == "bne"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    safe = mrm->check_beq_bne(reg1,reg2,banks,index);
                    if(sequence_cycle<mrm->clock_core[index]){
                        bool safe_op = isSafe_beq_bne(reg1,reg2);
                        if(safe_op==true){
                            if(safe==0){
                                sequence_cycle++;
                                string toLabel = tokens[lineno][3];
                                lineno = bne(tokens[lineno],lineno,labels);
                                repetion[4] = repetion[4]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": bne:"<<toLabel<<"\n";
                            }else{
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }else{
                            if(safe==0){
                                sequence_cycle =mrm->clock_core[index]+1;
                                mrm->clock_core[index]++;
                                string toLabel = tokens[lineno][3];
                                lineno = bne(tokens[lineno],lineno,labels);
                                repetion[4] = repetion[4]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": bne:"<<toLabel<<"\n";
                            }else{
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }
                    }else{
                        if(safe==0){
                            mrm->clock_core[index]++;sequence_cycle++;
                            string toLabel = tokens[lineno][3];
                            lineno = bne(tokens[lineno],lineno,labels);
                            repetion[4] = repetion[4]+1;
                            cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": bne:"<<toLabel<<"\n";
                        }else{
                            queue_func(tokens,labels,safe,button);
                            
                        }

                    }
                    
                }

            }
            else if(first == "slt"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int r0 = convertRegisters(tokens[lineno][1]);
                    int r1 = convertRegisters(tokens[lineno][2]);
                    int r2;
                    if(tokens[lineno][3].at(0)=='$'){
                        r2 = convertRegisters(tokens[lineno][3]);
                        if(sequence_cycle<mrm->clock_core[index]){
                            bool safe_op = isSafeOp(r0,r1,r2);
                            if(safe_op==true){
                                safe = mrm->checkSafe_op(r0,r1,r2,banks,index);
                                if(safe==0){
                                    sequence_cycle++;
                                    if(regesterFile[(r1)]<regesterFile[(r2)]){
                                        regesterFile[(r0)]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    queue_func(tokens,labels,safe,button);
                                    
                                }
                            }else{
                                safe = mrm->checkSafe_op(r0,r1,r2,banks,index);
                                if(safe==0){
                                    sequence_cycle=mrm->clock_core[index]+1;
                                    mrm->clock_core[index]++;
                                    if(regesterFile[(r1)]<regesterFile[(r2)]){
                                        regesterFile[(r0)]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    queue_func(tokens,labels,safe,button);
                                    
                                }
                            }
                        }else{
                            safe = mrm->checkSafe_op(r0,r1,r2,banks,index);
                            if(safe==0){
                                mrm->clock_core[index]++;sequence_cycle++;
                                if(regesterFile[(r1)]<regesterFile[(r2)]){
                                    regesterFile[(r0)]= 1;
                                }else{
                                    regesterFile[(r0)] =0;
                                }
                                cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                lineno++;
                            }else{
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }
                    }else{
                        string sr2 = tokens[lineno][3];
                        r2 = stoi(sr2);
                        if(sequence_cycle<mrm->clock_core[index]){
                            bool safe_op = isSafeAddi(r0,r1);
                            if(safe_op==true){
                                safe=  mrm->checkSafe_addi(r0,r1,banks,index);
                                if(safe==0){
                                    sequence_cycle++;
                                    if(regesterFile[r1]<(r2)){
                                        regesterFile[r0]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    queue_func(tokens,labels,safe,button);
                                    
                                }
                            }else{
                                safe=  mrm->checkSafe_addi(r0,r1,banks,index);
                                if(safe==0){
                                    sequence_cycle = mrm->clock_core[index]+1;
                                    mrm->clock_core[index]++;
                                    if(regesterFile[r1]<(r2)){
                                        regesterFile[r0]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    queue_func(tokens,labels,safe,button);
                                    
                                }
                            }
                        }else{
                            safe=  mrm->checkSafe_addi(r0,r1,banks,index);
                            if(safe==0){
                                mrm->clock_core[index]++;sequence_cycle++;
                                if(regesterFile[r1]<(r2)){
                                    regesterFile[r0]= 1;
                                }else{
                                    regesterFile[(r0)] =0;
                                }
                                cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                lineno++;
                            }else{
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }
                        
                    }
                    repetion[5] = repetion[5]+1;
                }

            }
            else if(first == "j"){
                sequence_cycle++;
                if(tokens[lineno].size()!=2){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    mrm->clock_core[index]++;
                    string toLabel = tokens[lineno][1];
                    lineno = j(tokens[lineno],lineno,labels);
                    repetion[6] = repetion[6]+1;
                    cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": j:"<<toLabel<<"\n";

                }

            }else if(first == "lw"){

                if(tokens[lineno].size()!=3){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = row_regester(tokens[lineno][2]);
                    if(reg1==-1||reg2==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Syntax Error.\n";
                        return;
                    }
                    int address_sw_lw = coreAddress(rownumber(tokens[lineno][2])+colnumber(tokens[lineno][2]),index);
                    safe = mrm->check_sw_lw(reg1,reg2,address_sw_lw,0,banks,index,mrm->clock_core[index]);
                    if(safe==0){
                        int ans = lw(tokens[lineno],lineno,ins_size,lineno,0);
                        if(ans==-1){
                            mrm->clock_core[index]++;
                            cerr<<"Syntax error\n";
                            return;
                        }
                        repetion[7] = repetion[7]+1;
                        lineno++;
                    }
                    else if (safe==-2){
                        regesterFile[reg1]=regesterFile[mrm->stored_value];
                        cout<<" : Value directly taken from sw process "<<""<<"="<<regesterFile[mrm->stored_value]<<"\n";
                        lineno++;
                    }
                    else{
                        queue_func(tokens,labels,safe,button);
                        
                    }


                }

            }else if(first == "sw"){
                if(tokens[lineno].size()!=3){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = row_regester(tokens[lineno][2]);
                    if(reg1==-1||reg2==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Syntax Error.\n";
                        return;
                    }
                    int address_sw_lw = coreAddress(rownumber(tokens[lineno][2])+colnumber(tokens[lineno][2]),index);
                    safe = mrm->check_sw_lw(reg1,reg2,address_sw_lw,0,banks,index,mrm->clock_core[index]);

                    if(safe==0){
                        int ans = sw(tokens[lineno],lineno,ins_size,lineno,0);
                        if(ans==-1){
                            mrm->clock_core[index]++;
                            cerr<<"Syntax error\n";
                            return;
                        }
                        repetion[8] = repetion[8]+1;
                        lineno++;
                    }

                    else{
                        queue_func(tokens,labels,safe,button);
                        
                    }


                }

            }
            else if(first == "addi"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    string immi = tokens[lineno][3];
                    int pos = immi.find_first_not_of(digit);
                    if(reg1==-1||reg2==-1||pos!=-1){
                        mrm->clock_core[index]++;
                        cerr<<"Error\n";
                        return;
                    }
                    if(sequence_cycle<mrm->clock_core[index]){
                        bool safe_op = isSafeAddi(reg1,reg2);
                        safe = mrm->checkSafe_addi(reg1,reg2,banks,index);
                        if(safe_op==true){
                            if(safe==0){
                                sequence_cycle++;
                                int r1 = regesterFile[reg2]+stoi(immi);
                                regesterFile[reg1] = r1;
                                repetion[9] = repetion[9]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<"="<<r1<<"\n";
                                lineno++;
                            }else if(safe==-1){
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }else{
                            if(safe==0){
                                sequence_cycle=mrm->clock_core[index]+1;
                                mrm->clock_core[index]++;
                                int r1 = regesterFile[reg2]+stoi(immi);
                                regesterFile[reg1] = r1;
                                repetion[9] = repetion[9]+1;
                                cout<<"core:"<<index<<",cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<"="<<r1<<"\n";
                                lineno++;
                            }else if(safe==-1){
                                queue_func(tokens,labels,safe,button);
                                
                            }
                        }
                    }else{
                        safe = mrm->checkSafe_addi(reg1,reg2,banks,index);
                        if(safe==0){
                            mrm->clock_core[index]++;sequence_cycle++;
                            int r1 = regesterFile[reg2]+stoi(immi);
                            regesterFile[reg1] = r1;
                            repetion[9] = repetion[9]+1;
                            cout<<"core:"<<index<<",cycle "<<mrm->clock_core[index]<<": "<<tokens[lineno][1]<<"="<<r1<<"\n";
                            lineno++;
                        }else if(safe==-1){
                            queue_func(tokens,labels,safe,button);
                            
                        }
                    }

                }

            }
            else{
                cerr<<"Token not Defined."<<first<<"\n";
                return;
            }
        }

    }
    if(mrm->rowbuffer[0]!=-1 &&lineno==tokens.size() && mrm->queue_op[mrm->currBank].size()==0){
        
    }
}

void Core::parse_next(){
    parse(instruction_memory,labels);
}

Core::Core(string fileName, int idx, int rDelay , int cDelay, MRM* mrm_universal,int total_cores){
    vector<string>identi;
    identi.push_back("add");
    identi.push_back("sub");
    identi.push_back("mul");
    identi.push_back("beq");
    identi.push_back("bne");
    identi.push_back("slt");
    identi.push_back("j");
    identi.push_back("lw");
    identi.push_back("sw");
    identi.push_back("addi");
    index=idx;
    vector<string>labelstr;
    regesterFile.resize(32,0);
    mrm=mrm_universal;
    mrm->current.resize(total_cores);
    mrm->clock_core.resize(total_cores);
    totalCores = total_cores;
    repetion.resize(10,0);
    rowdelay = rDelay ;
    coldelay = cDelay;
    fstream mips;
    mips.open(fileName,ios::in);
    string line;
    string const delims{" ,\t"};
    int linenumber = 0;
    while(getline(mips,line)){
        size_t beg,pos = 0;
        vector<string> tokens;
        while((beg = line.find_first_not_of(delims,pos))!= string::npos){
            pos = line.find_first_of(delims,beg+1);
            tokens.push_back(line.substr(beg,pos-beg));
        }
        if(tokens.size()!=0){
            linenumber++;
            char s = tokens[0].back();
            if(s==':'){
                string label = tokens[0].substr(0,tokens[0].length()-1);
                labels[label] = linenumber;
                labelstr.push_back(label);
                tokens.erase(tokens.begin());
            }
            instruction_memory.push_back(tokens);
        }
    }
    stackpointer = linenumber*4;
    
    // for (auto U:instruction_memory){

    //     for (auto a:U){
    //         cout<<a<<" ";
    //     }
    //     cout<<"\n";
    // }
    ins_size = instruction_memory.size();
    // cout<<"--------------------------------\n";
    // for (auto u:labels){
    //     cout<<u.first<<":"<<u.second<<"\n";
    // }
    // cout<<"\n";
    // // parse(instruction_memory,labels);
    // cout<<"--------------------------------\n";
     
    // cout<<"\n";
    // cout<<"Total number of cycles: "<<mrm->clock_core[index]<<"\n";
    // cout<<"Number of row buffer updates are: "<<mrm->rowbufferUpdate[0]<<"\n";
    
    // cout<<"\n";
    // cout<<"Memory content at the end of the execution:\n";
    // for(auto i:mrm->memoryupdate){
    //     cout<<i.first<<" : "<<i.second<<endl;
    // }
    mips.close();
    
}
