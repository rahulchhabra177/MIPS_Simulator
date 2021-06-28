#include <bits/stdc++.h>
#include "core.h"
using namespace std;


vector<string> reg_name = {"$0","$1","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$s8","$k1","$gp","$sp","$fp","$ra"};    

void Core::print_reg(vector<int> regesterFile){
    for(int i =0;i<regesterFile.size();i++){
        if (regesterFile[i] != 0){
            cout<<reg_name[i]<<":"<<regesterFile[i]<<"\n";
        }
    }
}


string int_to_bin16(int n){ //funtion to convert integer to 32 bit binary number

   string s="0000000000000000";
   int i=15;
   while (n!=0 and i>=0){
   if (n%2==1){s[i]='1';}
   i--;
   n/=2; 
   }
   if (s.size()!=16){cout<<"Internal Error\n";}
   return s;
}

string lookup(string s) //converting first 6 digit binary opcode to relevant operation string
{

if (s=="000000"){
   return "add";
}
else if (s=="000001"){
   return "addi";
}
else if (s=="000010"){
   return "sub";
}

else if (s=="000011"){
   return "mul";
}

else if (s=="000100"){
   return "beq";
}

else if (s=="000101"){
   return "bne";
}

else if (s=="000111"){
   return "slt";
}

else if (s=="001000"){
   return "j";
}


else if (s=="001001"){
   return "lw";
}

else if (s=="001011"){
   return "sw";
}

else return s;

}



string for_j(int n){ //converting int to 26 bit binary

   string s="00000000000000000000000000";
   int i=25;
   while (n!=0 and i>=0){
      if (n%2==1){s[i]='1';}
      i--;
      n/=2;
   }

   return s;

}
string int_to_bin6(int n){ //converting int to 5 but binary

string s="00000";
int i=4;
while (n!=0 and i>=0){
if (n%2==1){s[i]='1';}
i--;
n/=2;
}

return s;

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
int integ(string s){ //converting binary to int
   int two=1;
   int sum=0;
   for (int i = s.length()-1; i >= 0; i--)   
   {
      if (s[i]=='1'){sum+=two;}
      two*=2;
      /* code */
   }
   return sum;

}
string int_to_bin32(int n){ //funtion to convert integer to 32 bit binary number

   string s="00000000000000000000000000000000";
   int i=31;
   while (n!=0 and i>=0){
   if (n%2==1){s[i]='1';}
   i--;
   n/=2; 
   }
   if (s.size()!=32){cout<<"Internal Error\n";}
   return s;

}
string Core::encode(string reg){


    return int_to_bin6(convertRegisters(reg));
}

string Core::encode_ins(vector<string>arguments){
    string r1,r2,r3,result;
    // cout<<arguments.size()<<"\n";
    if  (arguments[0]=="add"){         
               r1=encode(arguments[1]);
               r2=encode(arguments[2]);
               r3=encode(arguments[3]);
               result="000000"+r1+r2+r3+"00000000000";  //here opcode for add is 000000, 5 digits each are for regitser, last 11 digits are irrelevant

            }

 else if  (arguments[0]=="addi"){         
               r1=encode(arguments[1]);
               r2=encode(arguments[2]);
               r3=int_to_bin16(stoi(arguments[3]));

               result="000001"+r1+r2+r3;

            }
  else if  (arguments[0]=="sub"){         
               r1=encode(arguments[1]);
               r2=encode(arguments[2]);
               r3=encode(arguments[3]);
               result="000010"+r1+r2+r3+"00000000000";

            }           

else if  (arguments[0]=="mul"){         
               r1=encode(arguments[1]);
               r2=encode(arguments[2]);
               r3=encode(arguments[3]);
               result="000011"+r1+r2+r3+"00000000000";

            }           
else if  (arguments[0]=="beq"){         
               r1=encode(arguments[1]);
               r2=encode(arguments[2]);
               r3=int_to_bin16(labels[arguments[3]]-1);
               result="000100"+r1+r2+r3;

            }           

else if  (arguments[0]=="bne"){         
               r1=encode(arguments[1]);
               r2=encode(arguments[2]);
               r3=int_to_bin16(labels[arguments[3]]-1);
               result="000101"+r1+r2+r3;

            }           

else if  (arguments[0]=="slt"){         
               r1=encode(arguments[1]);
               r2=encode(arguments[2]);
               r3=encode(arguments[3]);
               result="000111"+r1+r2+r3+"00000000000";

            }  
else if  (arguments[0]=="j"){         
               r1=for_j(labels[arguments[1]]-1);
               result="001000"+r1;

            }              
else if  (arguments[0]=="lw"){         
               r1=encode(arguments[1]);

               result="001001"+r1+r3+r2;

            } 

else if  (arguments[0]=="sw"){         
               r1=encode(arguments[1]);
               result="001011"+r1+r3+r2;

            }    


return result;


}

void Core::encode_memory(vector<vector<string>>instructions){


    encoded_memory.resize(instructions.size());
    for (int i= 0; i<instructions.size();i++){
        if (instructions[i].size()!=0){
            encoded_memory[i] = encode_ins(instructions[i]);
        }
    }

}

vector<int> Core::queueBank(int index){
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
            address = mrm->regesterFile[index-1][address];
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
            rowno = mrm->regesterFile[index-1][rowno];
            return rowno;
        }else{
            int pr = s.find_first_of(parenr);
            string stradress = (s.substr(pos+1,pr-pos-1));
            rowno = convertRegisters(stradress);
            mrm->current[index].reg1 = rowno;
            if(rowno==-1){
                return -1;
            }
            rowno  = mrm->regesterFile[index-1][rowno];
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
    vector<int>banks = queueBank(index);
    for(int i=0;i<banks.size();i++){
        if(mrm->current[banks[i]].isLW==0){
            
        }else if(mrm->current[banks[i]].isLW==-1){

        }
        else{
            if(reg2 == mrm->current[banks[i]].reg0||reg3==mrm->current[banks[i]].reg0){
                return false;
            }else{
                if(mrm->current[banks[i]].reg0==reg1){
                    //invalid declaration
                    return false;
                }
            }
        }
    }
    return true;
}
bool Core::isSafe_beq_bne(int reg1,int reg2){
    vector<int>banks = queueBank(index);
    for(int i=0;i<banks.size();i++){
        if(mrm->current[index].isLW==1){
            if(mrm->current[index].isLW && (reg1==mrm->current[index].reg0||reg2 == mrm->current[index].reg0)){
                return false;
            }
        }
    }
    return true;
}
bool Core::isSafeAddi(int reg1,int reg2){
    vector<int>banks = queueBank(index);
    for(int i=0;i<banks.size();i++){
        if(mrm->current[banks[i]].isLW==0){
           
        }else if(mrm->current[banks[i]].isLW==-1){

        }
        else{
            if(reg2==mrm->current[banks[i]].reg0){
                return false;
            }else{

                if(mrm->current[banks[i]].reg0==reg1){
                    //invalid declaration
                    return false;
                }
            }
        }
    }
    return true;
}
int Core::beq(vector<string> tokens,int lineno,map<string,int>labels){
    string label = tokens[3];
    int r0 = convertRegisters( tokens[1]);
    int r1;
    if(tokens[2].substr(0,1)=="$"){
        r1 = convertRegisters( tokens[2]);
        if(mrm->regesterFile[index-1][(r0)]==mrm->regesterFile[index-1][(r1)]){
            lineno = labels[label];
        }else{
            lineno++;
        }
    }else{
        r1 = stoi(tokens[2]);
        if(mrm->regesterFile[index-1][(r0)]==(r1)){
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
        if(mrm->regesterFile[index-1][(r0)]!=mrm->regesterFile[index-1][(r1)]){
            lineno = labels[label];
        }else{
            lineno++;
        }
    }else{
        r1 = stoi(tokens[2]);
        if(mrm->regesterFile[index-1][(r0)]!=(r1)){
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
        isCompleted = true;mrm->indexCompleted[index-1] = true;
        return -1;
    }else{
        string label = tokens[1];
        lineno = labels[label]-1;
        return lineno;
    }
}

int Core::push_lw(vector<string> tokens,int lineno,int ins_size,int abs_lineno,int change){
    int r0 = convertRegisters( tokens[1]);
    int row = rownumber(tokens[2]);
    int col = colnumber(tokens[2]);
    int address = row+col;
    int addressnew = coreAddress(address,index);
    int rowb = (addressnew)/1024;
    int indexlowerlimit = (1024/totalCores)*(index-1)*1024;
    int indexupperlimit = (1024/totalCores)*(index)*1024 -1;

    if(addressnew<indexlowerlimit||addressnew>indexupperlimit){
        cerr<<"Error: Memory not accessible\n";
        isCompleted = true;
        mrm->indexCompleted[index -1] = true;
        return -4;
    }
    
    //Assignment 4 start
    int row_reg = row_regester(tokens[2]);
    vector<int>current_op;
    current_op.resize(10,0);
    current_op[0] = 1;
    current_op[1] = r0;
    current_op[2] = addressnew;
    current_op[3] = lineno;
    current_op[4] = rowb;
    current_op[5] = row_reg;
    current_op[6] = 1;
    current_op[7] = index; 
    current_op[8] = mrm->iNum;
    current_op[9] = -1;
    mrm->iNum++;
    if(mrm->queue_op[getBankNum(addressnew)].size()>16){
        return -3;
    }
    mrm->queue_op[getBankNum(addressnew)].push_back(current_op);
    

   
    return 0;
}




int Core::push_sw(vector<string> tokens,int lineno,int ins_size, int abs_lineno, int change){
    int r0 = convertRegisters( tokens[1]);
    int row = rownumber(tokens[2]);
    int col = colnumber(tokens[2]);
    int address = row+col;
    int addressnew = coreAddress(address,index);
    int rowb = addressnew/1024;
    int indexlowerlimit = (1024/totalCores)*(index-1)*1024;
    int indexupperlimit = (1024/totalCores)*(index)*1024 -1;

    if(addressnew<indexlowerlimit||addressnew>indexupperlimit){
        cerr<<"Error: Memory not accessible\n";
        isCompleted = true;
        mrm->indexCompleted[index -1] = true;
        return -4;
    }
    string memup = to_string(addressnew)+"-"+to_string(addressnew+3);
    mrm->memoryupdate[memup] = mrm->regesterFile[index-1][r0];

    int row_reg = row_regester(tokens[2]);
    vector<int> current_op;
    current_op.resize(10,0);
    current_op[0] = 0;
    current_op[1] = r0;
    current_op[2] = addressnew;
    current_op[3] = lineno;
    current_op[4] = rowb;
    current_op[5] = row_reg;
    current_op[6] = 1;
    current_op[7] = index;
    current_op[8] = mrm->iNum;
    current_op[9] = mrm->regesterFile[index-1][r0];
    mrm->iNum++;
    if(mrm->queue_op[getBankNum(addressnew)].size()>16){
        return -3;
    }
    mrm->queue_op[getBankNum(addressnew)].push_back(current_op);
    
    //Assignment 4 end

    
    return 0;
}


string Core::getIns(vector<string>t){
    string result = "";
    for (string s:t){
        result+=s;
        result+=" ";
    }
    return result;
}




void Core::increment(){

    for (auto u:queueBank(index)){
        mrm->clock_core[u]++;
        if(mrm->clock_core[u]<mrm->LastDRAM_cycle[index]){
            mrm->clock_core[u] = mrm->LastDRAM_cycle[index];
        }
    }

}

void Core::setSeqCycle(){
sequence_cycle=INT_MAX;
for (auto u:queueBank(index)){
        sequence_cycle=min(sequence_cycle,mrm->clock_core[u]);
    }

}
void Core::parse(vector<vector<string>> tokens,map<string,int>labels){
        
    int safe =0;
    int button =0;
    setSeqCycle();
    if (lineno>=tokens.size() || sequence_cycle+1 > mrm-> max_cycle){isCompleted=true;mrm->indexCompleted[index-1] = true;}
    
    
    if(lineno < tokens.size()){
        

        if(false){
            cerr<<"System frezzes"<<"\n";
            return;
        }

        
        
        else{

        if(tokens[lineno].size()==0){
            lineno++;
            button = -1;
            if (lineno>=tokens.size()){return;}
        }
            string first = lookup(tokens[lineno][0]);
            if(first=="add"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Error\n";
                        isCompleted = true;mrm->indexCompleted[index-1] = true;
                        return;
                    }
                    safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                    bool safe_op = isSafeOp(reg1,reg2,reg3);
                    if(safe_op == false){
                        increment();
                        return;
                    }
                        
                    if(safe!=-1){

                        if (safe==-2){

                            increment();
                            setSeqCycle();
                        }
                        mrm -> ins_executed++; 
                        mrm->regesterFile[index-1][reg1] = mrm->regesterFile[index-1][reg2]+mrm->regesterFile[index-1][reg3];
                        mrm->regesterFile[index-1][0] =0;
                        repetion[2] = repetion[2]+1;
                        cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<":"<<getIns(tokens[lineno])<<"; "<<tokens[lineno][1]<<" = "<<mrm->regesterFile[index-1][reg1]<<"\n";
                        lineno++;
                        increment();

                    }
                    else{
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();
                    }
                }
            }else if(first == "sub"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Error\n";
                        isCompleted = true;mrm->indexCompleted[index-1] = true;
                        return;
                    }
                    safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                    bool safe_op = isSafeOp(reg1,reg2,reg3);
                    if(safe_op == false){
                        increment();
                        return;

                    }
                    if(safe!=-1){
                        if(safe==-2){
                            increment();
                            setSeqCycle();
                        }
                        mrm -> ins_executed++;
                        mrm->regesterFile[index-1][reg1] = mrm->regesterFile[index-1][reg2]-mrm->regesterFile[index-1][reg3];
                        mrm->regesterFile[index-1][0] = 0;
                        repetion[2] = repetion[2]+1;
                        cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<":"<<getIns(tokens[lineno])<<"; "<<tokens[lineno][1]<<" = "<<mrm->regesterFile[index-1][reg1]<<"\n";
                        lineno++;
                        increment();
                    }else{
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();
                    }
                    
                }

            }
            else if(first=="mul"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        mrm->clock_core[index]++;
                        cerr<<"Error\n";
                        isCompleted = true;mrm->indexCompleted[index-1] = true;
                        return;
                    }
                    safe = mrm->checkSafe_op(reg1,reg2,reg3,banks,index);
                    bool safe_op = isSafeOp(reg1,reg2,reg3);
                    if(safe_op == false){
                        increment();
                        return;

                    }
                    if(safe!=-1){
                        if(safe==-2){
                            increment();
                            setSeqCycle();
                        }
                        mrm -> ins_executed++;
                        mrm->regesterFile[index-1][reg1] = mrm->regesterFile[index-1][reg2]*mrm->regesterFile[index-1][reg3];
                        mrm->regesterFile[index-1][0]=0;
                        repetion[2] = repetion[2]+1;
                        cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<":"<<getIns(tokens[lineno])<<"; "<<tokens[lineno][1]<<" = "<<mrm->regesterFile[index-1][reg1]<<"\n";
                        lineno++;
                        increment();
                    }else{
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();
                    }
                    
                }

            }
            else if(first == "beq"){
                if(tokens[lineno].size()!=4){
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    cerr<<"Syntax Error.\n";
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    safe = mrm->check_beq_bne(reg1,reg2,banks,index);
                    bool safe_op = isSafe_beq_bne(reg1,reg2);
                    if(safe_op==false){
                        increment();
                        return;
                    }
                    if(safe==0){
                        mrm -> ins_executed++;
                        lineno = beq(tokens[lineno],lineno,labels);
                        repetion[3] = repetion[3]+1;
                        cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<": beq:"<<tokens[lineno][3]<<"\n";
                        increment();
                    }else{
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();                        
                    }
                }

            }
            else if(first == "bne"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    safe = mrm->check_beq_bne(reg1,reg2,banks,index);
                    bool safe_op = isSafe_beq_bne(reg1,reg2);
                    if(safe_op==false){
                        increment();
                        return;
                    }
                    if(safe!=-1){
                        if(safe==-2){
                            increment();
                            setSeqCycle();
                        }
                        mrm -> ins_executed++;
                        string toLabel = tokens[lineno][3];
                        lineno = bne(tokens[lineno],lineno,labels);
                        repetion[4] = repetion[4]+1;
                        cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<": bne:"<<toLabel<<"\n";
                        increment();
                    }else{
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();                        
                    }                 
                }

            }
            else if(first == "slt"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    int r0 = convertRegisters(tokens[lineno][1]);
                    int r1 = convertRegisters(tokens[lineno][2]);
                    int r2;
                    if(tokens[lineno][3].at(0)=='$'){
                        r2 = convertRegisters(tokens[lineno][3]);
                        safe = mrm->checkSafe_op(r0,r1,r2,banks,index);
                        bool safe_op = isSafeOp(r0,r1,r2);
                        if(safe_op==false){
                            increment();
                            return;
                        }
                        if(safe!=-1){
                            if(safe==-2){
                                increment();
                                setSeqCycle();
                            }
                            if(mrm->regesterFile[index-1][(r1)]<mrm->regesterFile[index-1][(r2)]){
                                mrm->regesterFile[index-1][(r0)]= 1;
                            }else{
                                mrm->regesterFile[index-1][(r0)] =0;
                            }
                            mrm->regesterFile[index-1][0]=0;
                            cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<": "<<getIns(tokens[lineno])<<";"<<tokens[lineno][1]<<" = "<<mrm->regesterFile[index-1][r0]<<"\n";
                            lineno++;
                            increment();
                        }else{
                            mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                            increment();                            
                        }
                        
                        
                    }else{
                        string sr2 = tokens[lineno][3];
                        r2 = stoi(sr2);
                        safe=  mrm->checkSafe_addi(r0,r1,banks,index);
                        bool safe_op = isSafeAddi(r0,r1);
                        if(safe_op==false){
                            increment();
                            return;
                        }
                        if(safe!=-1){
                            if(safe==-2){
                                increment();
                                setSeqCycle();
                            }
                            if(mrm->regesterFile[index-1][r1]<(r2)){
                                mrm->regesterFile[index-1][r0]= 1;
                            }else{
                                mrm->regesterFile[index-1][(r0)] =0;
                            }
                            mrm->regesterFile[index-1][0]=0;
                            cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<": "<<getIns(tokens[lineno])<<";"<<tokens[lineno][1]<<" = "<<mrm->regesterFile[index-1][r0]<<"\n";
                            lineno++;
                            increment();
                        }else{
                            mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];

                            increment();
                        }
                        
                        
                    }
                    repetion[5] = repetion[5]+1;
                }

            }
            else if(first == "j"){
                if(tokens[lineno].size()!=2){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    mrm -> ins_executed++;
                    string toLabel = tokens[lineno][1];
                    lineno = j(tokens[lineno],lineno,labels);
                    repetion[6] = repetion[6]+1;
                    cout<<"core:"<<index<<",cycle "<<(mrm->clock_core[index]+1)<<": j:"<<toLabel<<"\n";
                    increment();
                }

            }else if(first == "lw"){

                if(tokens[lineno].size()!=3){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = row_regester(tokens[lineno][2]);
                    if(reg1==-1||reg2==-1){
                        cerr<<"Syntax Error.\n";
                        isCompleted = true;mrm->indexCompleted[index-1] = true;
                        return;
                    }
                    int address_sw_lw = coreAddress(rownumber(tokens[lineno][2])+colnumber(tokens[lineno][2]),index);
                    safe = mrm->check_sw_lw(reg1,reg2,address_sw_lw,0,banks,index);
                    if(safe==0){
                        cout<<"core "<<index<<" cycle "<<(sequence_cycle+1)<<" Request sent to MRM "<<getIns(tokens[lineno])<<"\n";
                        increment();
                        int ans = push_lw(tokens[lineno],lineno,ins_size,lineno,0);
                        if(ans==-1){
                            cerr<<"Syntax error\n";
                            isCompleted = true;mrm->indexCompleted[index-1] = true;
                            return;
                        }
                        else if(ans==-3){
                            increment();
                            return;
                        }
                        repetion[7] = repetion[7]+1;
                        lineno++;
                    }
                    else if (safe==-2){
                        vector<int> tmp = mrm->queue_op[banks[mrm->stored_value.second]][mrm->stored_value.first];
                        // cout<<getIns(tokens[lno])<<"\n";
                        cout<<"core "<<index<<" cycle "<<(sequence_cycle+1)<<" Request sent to MRM "<<getIns(tokens[lineno])<<"\n";
                        increment();
                        mrm->regesterFile[index-1][reg1]=mrm->regesterFile[index-1][tmp[1]];
                        cout<<"core:"<<index<<" "<<"cycle no:"<<(mrm->clock_core[mrm->stored_value.second]+1)<<":MRM Forwarding ";
                        cout<<"from sw process "<<getIns(tokens[tmp[3]])<<";"<<tokens[lineno][1]<<"="<<mrm->regesterFile[index-1][tmp[1]]<<"\n";
                        mrm->MRM_Delay+=1;
                        lineno++;
                    }
                    else{
                        int relative_address = (rownumber(tokens[lineno][2]) + colnumber(tokens[lineno][2]));
                        int new_address = coreAddress(relative_address,index);
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();
                    }


                }

            }else if(first == "sw"){
                if(tokens[lineno].size()!=3){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
                    return;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = row_regester(tokens[lineno][2]);
                    if(reg1==-1||reg2==-1){
                        cerr<<"Syntax Error.\n";
                        isCompleted = true;mrm->indexCompleted[index-1] = true;
                        return;
                    }
                    int address_sw_lw = coreAddress(rownumber(tokens[lineno][2])+colnumber(tokens[lineno][2]),index);
                    safe = mrm->check_sw_lw(reg1,reg2,address_sw_lw,1,banks,index);

                    if(safe==0){
                        cout<<"core:"<<index<<" cycle "<<(sequence_cycle+1)<<" Request sent to MRM "<<getIns(tokens[lineno])<<" \n";
                        increment();
                        int ans = push_sw(tokens[lineno],lineno,ins_size,lineno,0);
                        if(ans==-1){
                            cerr<<"Syntax error\n";
                            isCompleted = true;mrm->indexCompleted[index-1] = true;
                            return;
                        }else if(ans==-3){
                            increment();
                            return;
                        }
                        repetion[8] = repetion[8]+1;
                        lineno++;
                    }

                    else{
                        int relative_address = (rownumber(tokens[lineno][2]) + colnumber(tokens[lineno][2]));
                        int new_address = coreAddress(relative_address,index);
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();
                        
                    }


                }

            }
            else if(first == "addi"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    isCompleted = true;mrm->indexCompleted[index-1] = true;
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
                        isCompleted = true;mrm->indexCompleted[index-1] = true;
                        return;
                    }
                    safe = mrm->checkSafe_addi(reg1,reg2,banks,index);
                    bool safe_op = isSafeAddi(reg1,reg2);
                    if(safe_op == false){
                        return;

                    }
                    
                    if(safe!=-1){

                        if (safe==-2){
                            increment();
                            setSeqCycle();
                        }
                        mrm -> ins_executed++;
                        mrm->regesterFile[index-1][reg1] = mrm->regesterFile[index-1][reg2]+stoi(immi);
                        mrm->regesterFile[index-1][0]=0;
                        repetion[2] = repetion[2]+1;
                        cout<<"core:"<<index<<",cycle "<<(sequence_cycle+1)<<":"<<getIns(tokens[lineno])<<"; "<<tokens[lineno][1]<<" = "<<mrm->regesterFile[index-1][reg1]<<"\n";
                        lineno++;
                        increment();



                    }
                    else{
                        mrm->priority_num[mrm->currBank] = mrm->notSafeRow[mrm->currBank];
                        increment();
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
    mrm=mrm_universal;
    curr_process temp;
    temp.remaining_cycles=0;
    mrm->current.resize(4,temp);
    mrm->clock_core.resize(4,0);
    mrm->token_core.resize(total_cores);
    mrm->prev_row_changed.resize(4);
    mrm->regesterFile.resize(total_cores);
    mrm->LastDRAM_cycle.resize(total_cores,-1);
    mrm->notSafeRow.resize(4,-1);
    mrm->indexCompleted.resize(total_cores,false);
    mrm->priority_num.resize(4,-1);
    mrm->priority_row1.resize(4,-1);
    mrm->priority_row2.resize(4,-1);
    mrm->writeBackCycle.resize(4,0);
    for(int i=0;i<mrm->regesterFile.size();i++){
        mrm->regesterFile[i].resize(32,0);
    }
    totalCores = total_cores;
    repetion.resize(10,0);
    banks = queueBank(index);
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
    
    encode_memory(instruction_memory);
    ins_size = instruction_memory.size();
    mrm->token_core[index-1] = instruction_memory;

    mips.close();
    
}
