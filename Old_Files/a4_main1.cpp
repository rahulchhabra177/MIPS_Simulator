#include<iostream>
#include<fstream>
#include<math.h>
#include <bits/stdc++.h>
using namespace std;

struct curr_process
{
    int reg0,reg1,isLW;
};

vector<int> regesterFile;
int stackpointer;
string const alpha = {"abcdefghijklmnopqrstuvwxyz"};
string const digit = {"0123456789"};
vector<int>repetion;
array<int,1048576>ram;
map<string,int>memoryupdate;
vector<int>dram;
curr_process current;
int rowbuffer = -1;
int clock_cycle = 0;
int rowBufferUpdate = 0;
int rowdelay,coldelay;
int sequence_cycle = 0;
vector<vector<string>> instruction_memory;


//Assignment 4 start
vector<vector<int>>queue_op;
int Mem_Req_Order(int rowbuffer,vector<vector<int>>current_queue_op,vector<int> current_op,int lineno, int size, int safe);
bool sortVec(const vector<int>& a,const vector<int>& b);
int checkSafe_op(vector<vector<int>>queue_op,int r1,int r2,int r3);
int checkSafe_addi(vector<vector<int>>queue_op,int r1,int r2);
int check_beq_bne(vector<vector<int>>queue_op,int r1,int r2);
int check_sw_lw(vector<vector<int>>queue_op,int r1,int r2);
//Assignment 4 end


void print_reg(vector<int> regesterFile){
    for(int i =0;i<regesterFile.size();i++){
        char hex[20];
        sprintf(hex,"%X",regesterFile[i]);
        cout<<hex<<" ";
    }
    cout<<"\n";
}
//convert registers
int convertRegisters(string s){
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
int addressnew(string s){
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
int row_regester(string s){
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
int rownumber(string s){
    string const parenl = {"("};
    string const parenr = {")"};
    int pos = s.find_first_of(parenl);
    int rowno;
    if(pos!=-1){
        if(pos==0){
            int pr = s.find_first_of(parenr);
            string stradress = (s.substr(1,pr-pos-1));
            rowno = convertRegisters(stradress);
            current.reg1 = rowno;
            rowno = regesterFile[rowno];
            return rowno;
        }else{
            int pr = s.find_first_of(parenr);
            string stradress = (s.substr(pos+1,pr-pos-1));
            rowno = convertRegisters(stradress);
            current.reg1 = rowno;
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
int colnumber(string s){
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
bool isSafeOp(int reg1,int reg2,int reg3){
    if(current.isLW==0){
        if(reg1 == current.reg0){
            return false;
        }
        else{
            //why this?
            if(reg1==current.reg1){
                return false;
            }else{
                return true;
            }
        }
    }else{
        if(reg1==current.reg0||reg2 == current.reg0||reg3==current.reg0){
            return false;
        }else{
            if(current.reg1==reg1){
                return false;
            }
            return true;
        }
    }
}
bool isSafe_beq_bne(int reg1,int reg2){
    if(current.isLW==1){
        if(reg1==current.reg0||reg2 == current.reg0){
            return false;
        }else{
            return true;
        }
    }else{
        return true;
    }
}
bool isSafeAddi(int reg1,int reg2){
    if(current.isLW==0){
        if(reg1==current.reg0){
            return false;
        }else{
            if(reg1==current.reg1){
                return false;
            }else{
                return true;
            }
        }
    }else{
        if(reg1==current.reg0||reg2==current.reg0){
            return false;
        }else{
            if(current.reg1==reg1){
                return false;
            }
            return true;
        }
    }
}
int beq(vector<string> tokens,int lineno,map<string,int>labels){
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
int bne(vector<string> tokens,int lineno,map<string,int>labels){
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

int j(vector<string> tokens,int lineno,map<string,int>labels){
    if(tokens.size()!=2){
        cerr<<"Syntax Error.\n";
        return -1;
    }else{
        string label = tokens[1];
        lineno = labels[label]-1;
        return lineno;
    }
}
int lw(vector<string> tokens,int lineno,int ins_size,int abs_lineno,int change){
    int r0 = convertRegisters( tokens[1]);
    int row = rownumber(tokens[2]);
    int col = colnumber(tokens[2]);
    int rowb = (row+col)/1024;
    int address = row+col;
    //cout<<"Current row buffer: "<<rowbuffer<<"\n";
    
    
    //Assignment 4 start
    int row_reg = row_regester(tokens[2]);
    vector<int>current_op;
    current_op.resize(6,0);
    current_op[0] = 0;
    current_op[1] = r0;
    current_op[2] = address;
    current_op[3] = lineno;
    current_op[4] = address/1024;
    current_op[5] = row_reg;
    int aq = Mem_Req_Order(rowbuffer,queue_op,current_op,abs_lineno,ins_size, change);
    //cout<<"lw:"<<aq<<"\n";
    if(aq==-1){
        clock_cycle++;
        cout<<"cycle "<<clock_cycle<<": DRAM request issued\n";

        cout<<"cycle "<<clock_cycle+1<<"-";
        clock_cycle+=rowdelay;
        rowbuffer = rowb;
        clock_cycle+=coldelay;
        regesterFile[r0] = dram[row+col];
        rowBufferUpdate+=1;
        cout<<clock_cycle<<": "<<tokens[1]<<" = "<<regesterFile[r0]<<"\n";
        current.reg0 = r0;current.isLW = 1;
    }else if(aq==1){
        clock_cycle++;
        cout<<"cycle "<<clock_cycle<<": DRAM request issued\n";

        cout<<"cycle "<<clock_cycle+1<<"-";
        clock_cycle+=coldelay;
        regesterFile[r0] = dram[row+col];
        cout<<clock_cycle<<": "<<tokens[1]<<" = "<<regesterFile[r0]<<"\n";
        current.reg0 = r0;current.isLW = 1;
        if(change==1){
            queue_op.erase(queue_op.begin());
        }
    }else if(aq==2){
        clock_cycle++;
        cout<<"cycle "<<clock_cycle<<": DRAM request issued\n";

        cout<<"cycle "<<clock_cycle+1<<"-";
        clock_cycle+=2*rowdelay;
        rowbuffer=rowb;
        clock_cycle+=coldelay;
        regesterFile[r0] = dram[row+col];
        rowBufferUpdate+=2;
        cout<<clock_cycle<<": "<<tokens[1]<<" = "<<regesterFile[r0]<<"\n";
        current.reg0 = r0;current.isLW = 1;
        queue_op.erase(queue_op.begin());
    }else if(aq==0){
        queue_op.push_back(current_op);
    }
    //Assignment 4 end

   
    return 0;
}
int sw(vector<string> tokens,int lineno,int ins_size, int abs_lineno, int change){
    int r0 = convertRegisters( tokens[1]);
    int row = rownumber(tokens[2]);
    int col = colnumber(tokens[2]);
    int rowb = (row+col)/1024;
    int address = row+col;
    string memup = to_string((row)+col)+"-"+to_string((row)+col+3);
    memoryupdate[memup] = regesterFile[r0];
    //cout<<"Current row buffer: "<<rowbuffer<<"\n";

    //Assignment 4 start
    int row_reg = row_regester(tokens[2]);
    vector<int> current_op;
    current_op.resize(6,0);
    current_op[0] = 1;
    current_op[1] = r0;
    current_op[2] = address;
    current_op[3] = lineno;
    current_op[4] = address/1024;
    current_op[5] = row_reg;
    int aq = Mem_Req_Order(rowbuffer,queue_op,current_op,abs_lineno,ins_size, change);
    //cout<<"sw:"<<aq<<"\n";
    if(aq==-1){
        clock_cycle++;
        cout<<"cycle "<<clock_cycle<<": DRAM request issued\n";

        cout<<"cycle "<<clock_cycle+1<<"-";
        clock_cycle+=rowdelay;
        rowbuffer = rowb;
        clock_cycle+=coldelay;
        dram[row+col] = regesterFile[r0];
        cout<<clock_cycle<<": memory address "<<memup<<" = "<<memoryupdate[memup]<<"\n";
        rowBufferUpdate+=1;
        current.reg0 = r0;current.isLW = 0;
    }else if(aq==1){
        clock_cycle++;

        cout<<"cycle "<<clock_cycle<<": DRAM request issued\n";
        
        cout<<"cycle "<<clock_cycle+1<<"-";
        clock_cycle+=coldelay;
        dram[row+col] = regesterFile[r0];
        cout<<clock_cycle<<": memory address "<<memup<<" = "<<memoryupdate[memup]<<"\n";
        current.reg0 = r0;current.isLW = 0;
        if(change==1){
            queue_op.erase(queue_op.begin());
        }
    }else if(aq==2){
        clock_cycle++;
        cout<<"cycle "<<clock_cycle<<": DRAM request issued\n";

        cout<<"cycle "<<clock_cycle+1<<"-";
        clock_cycle+=2*rowdelay;
        rowbuffer=rowb;
        clock_cycle+=coldelay;
        dram[row+col] = regesterFile[r0];
        rowBufferUpdate+=2;
        cout<<clock_cycle<<": memory address "<<memup<<" = "<<memoryupdate[memup]<<"\n";
        current.reg0 = r0;current.isLW = 0;
        queue_op.erase(queue_op.begin());
    }else if(aq==0){
        queue_op.push_back(current_op);
    }
    //Assignment 4 end

    
    return 0;
}

void parse(vector<vector<string>> tokens,map<string,int>labels){
    cout<<"core:"<<index<<":"<<mrm->clock_cycle<<"\n";
    int lineno = 0;
    int ins_size = tokens.size();
    int safe =0;
    int button =0;
    while(lineno<=tokens.size()){
        if(clock_cycle>10000){
            cerr<<"System frezzes"<<"\n";
            return;
        }
        if(tokens[lineno].size()==0){
            lineno++;
            button = -1;
        }
        
        
        //Assignment 4 start
        if(lineno==ins_size||safe==-1){
           
            if(button ==-1){
                lineno--;
            }
            button=0;   
            if(queue_op.size()!=0){
                
                sort(queue_op.begin(),queue_op.end(),sortVec);
                vector<int> curr_fake = queue_op[0];
                if(curr_fake[0]==1){
                    sw(tokens[curr_fake[3]],curr_fake[3],ins_size,lineno,1);
                }else{

                    lw(tokens[curr_fake[3]],curr_fake[3],ins_size,lineno,1);
                }                


            }else{
                safe=0;
                lineno++;
            }

        }
        //Assignment 4 end

        
        
        else{
            string first = tokens[lineno][0];
            //cout<<current.reg0<<" "<<current.reg1<<" "<<current.isLW<<"\n";
            if(first=="add"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        clock_cycle++;
                        cerr<<"Error\n";
                        break;
                    }
                    if(sequence_cycle<clock_cycle){
                        bool safe_op = isSafeOp(reg1,reg2,reg3);
                        if(safe_op == true){
                            safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                            if(safe==0){
                                sequence_cycle++;
                                regesterFile[reg1] = regesterFile[reg2]+regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }
                            else{
                                lineno--;
                            }

                        }
                        else{
                            safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                            if(safe==0){
                                sequence_cycle=clock_cycle+1;
                                regesterFile[reg1] = regesterFile[reg2]+regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                lineno--;
                            }
                        }
                    }
                    else{
                        safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                        if(safe==0){
                            clock_cycle++;sequence_cycle++;
                            regesterFile[reg1] = regesterFile[reg2]+regesterFile[reg3];
                            repetion[2] = repetion[2]+1;
                            cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                            lineno++;
                        }else{
                            lineno--;
                        }
                    }
                }
            }else if(first == "sub"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        clock_cycle++;
                        cerr<<"Error\n";
                        break;
                    }
                    if(sequence_cycle<clock_cycle){
                        bool safe_op = isSafeOp(reg1,reg2,reg3);
                        if(safe_op == true){
                            safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                            if(safe==0){
                                sequence_cycle++;
                                regesterFile[reg1] = regesterFile[reg2]-regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                lineno--;
                            }

                        }else{
                            safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                            if(safe==0){
                                sequence_cycle=clock_cycle+1;
                                regesterFile[reg1] = regesterFile[reg2]-regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                lineno--;
                            }
                        }
                    }else{
                        safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                        if(safe==0){
                            clock_cycle++;sequence_cycle++;
                            regesterFile[reg1] = regesterFile[reg2]-regesterFile[reg3];
                            repetion[2] = repetion[2]+1;
                            cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                            lineno++;
                        }else{
                            lineno--;
                        }
                    }
                }

            }
            else if(first=="mul"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    int reg3 = convertRegisters(tokens[lineno][3]);
                    if(reg1==-1||reg2==-1||reg3==-1){
                        clock_cycle++;
                        cerr<<"Error\n";
                        break;
                    }
                    if(sequence_cycle<clock_cycle){
                        bool safe_op = isSafeOp(reg1,reg2,reg3);
                        if(safe_op == true){
                            safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                            if(safe==0){
                                sequence_cycle++;
                                regesterFile[reg1] = regesterFile[reg2]*regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                lineno--;
                            }

                        }else{
                            safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                            if(safe==0){
                                sequence_cycle=clock_cycle+1;
                                regesterFile[reg1] = regesterFile[reg2]*regesterFile[reg3];
                                repetion[2] = repetion[2]+1;
                                cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                                lineno++;
                            }else{
                                lineno--;
                            }
                        }
                    }else{
                        safe = checkSafe_op(queue_op,reg1,reg2,reg3);
                        if(safe==0){
                            clock_cycle++;sequence_cycle++;
                            regesterFile[reg1] = regesterFile[reg2]*regesterFile[reg3];
                            repetion[2] = repetion[2]+1;
                            cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[reg1]<<"\n";
                            lineno++;
                        }else{
                            lineno--;
                        }
                    }
                }

            }
            else if(first == "beq"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    if(sequence_cycle<clock_cycle){
                        bool safe_op = isSafe_beq_bne(reg1,reg2);
                        if(safe_op==true){
                            safe = check_beq_bne(queue_op,reg1,reg2);
                            if(safe==0){
                                sequence_cycle++;
                                lineno = beq(tokens[lineno],lineno,labels);
                                repetion[3] = repetion[3]+1;
                                cout<<"cycle "<<sequence_cycle<<": beq:"<<tokens[lineno][3]<<"\n";
                            }else{
                                lineno--;
                            }
                        }else{
                            safe = check_beq_bne(queue_op,reg1,reg2);
                            if(safe==0){
                                sequence_cycle=clock_cycle+1;
                                lineno = beq(tokens[lineno],lineno,labels);
                                repetion[3] = repetion[3]+1;
                                cout<<"cycle "<<clock_cycle<<": beq:"<<tokens[lineno][3]<<"\n";
                            }else{
                                lineno--;
                            }
                        }
                    }else{
                        safe = check_beq_bne(queue_op,reg1,reg2);
                        if(safe==0){
                            clock_cycle++;sequence_cycle++;
                            lineno = beq(tokens[lineno],lineno,labels);
                            repetion[3] = repetion[3]+1;
                            cout<<"cycle "<<clock_cycle<<": beq:"<<tokens[lineno][3]<<"\n";
                        }else{
                            lineno--;
                        }
                    }
                }

            }
            else if(first == "bne"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    safe = check_beq_bne(queue_op,reg1,reg2);
                    if(sequence_cycle<clock_cycle){
                        bool safe_op = isSafe_beq_bne(reg1,reg2);
                        if(safe_op==true){
                            if(safe==0){
                                sequence_cycle++;
                                string toLabel = tokens[lineno][3];
                                lineno = bne(tokens[lineno],lineno,labels);
                                repetion[4] = repetion[4]+1;
                                cout<<"cycle "<<sequence_cycle<<": bne:"<<toLabel<<"\n";
                            }else{
                                lineno--;
                            }
                        }else{
                            if(safe==0){
                                sequence_cycle =clock_cycle+1;
                                string toLabel = tokens[lineno][3];
                                lineno = bne(tokens[lineno],lineno,labels);
                                repetion[4] = repetion[4]+1;
                                cout<<"cycle "<<clock_cycle<<": bne:"<<toLabel<<"\n";
                            }else{
                                lineno--;
                            }
                        }
                    }else{
                        if(safe==0){
                            clock_cycle++;sequence_cycle++;
                            string toLabel = tokens[lineno][3];
                            lineno = bne(tokens[lineno],lineno,labels);
                            repetion[4] = repetion[4]+1;
                            cout<<"cycle "<<clock_cycle<<": bne:"<<toLabel<<"\n";
                        }else{
                            lineno--;
                        }

                    }
                    
                }

            }
            else if(first == "slt"){
                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int r0 = convertRegisters(tokens[lineno][1]);
                    int r1 = convertRegisters(tokens[lineno][2]);
                    int r2;
                    if(tokens[lineno][3].at(0)=='$'){
                        r2 = convertRegisters(tokens[lineno][3]);
                        if(sequence_cycle<clock_cycle){
                            bool safe_op = isSafeOp(r0,r1,r2);
                            if(safe_op==true){
                                safe = checkSafe_op(queue_op,r0,r1,r2);
                                if(safe==0){
                                    sequence_cycle++;
                                    if(regesterFile[(r1)]<regesterFile[(r2)]){
                                        regesterFile[(r0)]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    lineno--;
                                }
                            }else{
                                safe = checkSafe_op(queue_op,r0,r1,r2);
                                if(safe==0){
                                    sequence_cycle=clock_cycle+1;
                                    if(regesterFile[(r1)]<regesterFile[(r2)]){
                                        regesterFile[(r0)]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    lineno--;
                                }
                            }
                        }else{
                            safe = checkSafe_op(queue_op,r0,r1,r2);
                            if(safe==0){
                                clock_cycle++;sequence_cycle++;
                                if(regesterFile[(r1)]<regesterFile[(r2)]){
                                    regesterFile[(r0)]= 1;
                                }else{
                                    regesterFile[(r0)] =0;
                                }
                                cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                lineno++;
                            }else{
                                lineno--;
                            }
                        }
                    }else{
                        string sr2 = tokens[lineno][3];
                        r2 = stoi(sr2);
                        if(sequence_cycle<clock_cycle){
                            bool safe_op = isSafeAddi(r0,r1);
                            if(safe_op==true){
                                safe=  checkSafe_addi(queue_op,r0,r1);
                                if(safe==0){
                                    sequence_cycle++;
                                    if(regesterFile[r1]<(r2)){
                                        regesterFile[r0]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    lineno--;
                                }
                            }else{
                                safe=  checkSafe_addi(queue_op,r0,r1);
                                if(safe==0){
                                    sequence_cycle = clock_cycle+1;
                                    if(regesterFile[r1]<(r2)){
                                        regesterFile[r0]= 1;
                                    }else{
                                        regesterFile[(r0)] =0;
                                    }
                                    cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                    lineno++;
                                }else{
                                    lineno--;
                                }
                            }
                        }else{
                            safe=  checkSafe_addi(queue_op,r0,r1);
                            if(safe==0){
                                clock_cycle++;sequence_cycle++;
                                if(regesterFile[r1]<(r2)){
                                    regesterFile[r0]= 1;
                                }else{
                                    regesterFile[(r0)] =0;
                                }
                                cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<" = "<<regesterFile[r0]<<"\n";
                                lineno++;
                            }else{
                                lineno--;
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
                    break;
                }else{
                    button =0;
                    clock_cycle++;
                    string toLabel = tokens[lineno][1];
                    lineno = j(tokens[lineno],lineno,labels);
                    repetion[6] = repetion[6]+1;
                    cout<<"cycle "<<clock_cycle<<": j:"<<toLabel<<"\n";

                }

            }else if(first == "lw"){
                sequence_cycle++;

                if(tokens[lineno].size()!=3){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = row_regester(tokens[lineno][2]);
                    if(reg1==-1||reg2==-1){
                        clock_cycle++;
                        cerr<<"Syntax Error.\n";
                        break;
                    }
                    safe = check_sw_lw(queue_op,reg1,reg2);
                    if(safe==0){
                        int ans = lw(tokens[lineno],lineno,ins_size,lineno,0);
                        if(ans==-1){
                            clock_cycle++;
                            cerr<<"Syntax error\n";
                            break;
                        }
                        repetion[7] = repetion[7]+1;
                        lineno++;
                    }else{
                        lineno--;
                    }


                }

            }else if(first == "sw"){
                sequence_cycle++;
                if(tokens[lineno].size()!=3){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = row_regester(tokens[lineno][2]);
                    if(reg1==-1||reg2==-1){
                        clock_cycle++;
                        cerr<<"Syntax Error.\n";
                        break;
                    }
                    safe = check_sw_lw(queue_op,reg1,reg2);
                    if(safe==0){
                        int ans = sw(tokens[lineno],lineno,ins_size,lineno,0);
                        if(ans==-1){
                            clock_cycle++;
                            cerr<<"Syntax error\n";
                            break;
                        }
                        repetion[8] = repetion[8]+1;
                        lineno++;
                    }else{
                        lineno--;
                    }


                }

            }
            else if(first == "addi"){

                if(tokens[lineno].size()!=4){
                    cerr<<"Syntax Error.\n";
                    break;
                }else{
                    button =0;
                    int reg1 = convertRegisters(tokens[lineno][1]);
                    int reg2 = convertRegisters(tokens[lineno][2]);
                    string immi = tokens[lineno][3];
                    int pos = immi.find_first_not_of(digit);
                    if(reg1==-1||reg2==-1||pos!=-1){
                        clock_cycle++;
                        cerr<<"Error\n";
                        break;
                    }
                    if(sequence_cycle<clock_cycle){
                        bool safe_op = isSafeAddi(reg1,reg2);
                        if(safe_op==true){
                            if(safe==0){
                                sequence_cycle++;
                                int r1 = regesterFile[reg2]+stoi(immi);
                                regesterFile[reg1] = r1;
                                repetion[9] = repetion[9]+1;
                                cout<<"cycle "<<sequence_cycle<<": "<<tokens[lineno][1]<<"="<<r1<<"\n";
                                lineno++;
                            }else if(safe==-1){
                                lineno--;
                            }
                        }else{
                            if(safe==0){
                                sequence_cycle=clock_cycle+1;
                                int r1 = regesterFile[reg2]+stoi(immi);
                                regesterFile[reg1] = r1;
                                repetion[9] = repetion[9]+1;
                                cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<"="<<r1<<"\n";
                                lineno++;
                            }else if(safe==-1){
                                lineno--;
                            }
                        }
                    }else{
                        safe = checkSafe_addi(queue_op,reg1,reg2);
                        if(safe==0){
                            clock_cycle++;sequence_cycle++;
                            int r1 = regesterFile[reg2]+stoi(immi);
                            regesterFile[reg1] = r1;
                            repetion[9] = repetion[9]+1;
                            cout<<"cycle "<<clock_cycle<<": "<<tokens[lineno][1]<<"="<<r1<<"\n";
                            lineno++;
                        }else if(safe==-1){
                            lineno--;
                        }
                    }
                }

            }
            else{
                cerr<<"Token not Defined."<<first<<"\n";
                break;
            }
        }
    }
    if(rowbuffer!=-1){
        cout<<"cycle "<<clock_cycle+1<<"-";
        clock_cycle+=rowdelay;
        rowbuffer=-1;
        cout<<clock_cycle<<" :DRAM Writeback row\n";
    }
}

int main(int argc,char* argv[]){
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
    map<string,int>labels;
    vector<string>labelstr;
    regesterFile.resize(32,0);
    dram.resize(1048576,0);
    repetion.resize(10,0);
    if(argc<4){
        cerr<<"Error:Either Name of text file or row access delay or column access delay is not given.\n";
        return -1;
    }
    string fileName = argv[1];
    rowdelay = stoi(argv[2]);
    coldelay = stoi(argv[3]);
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
    // parse(instruction_memory,labels);
    for (auto U:instruction_memory){

        for (auto a:U){
            cout<<a<<" ";
        }
        cout<<"\n";
    }

    cout<<"--------------------------------\n";
    for (auto u:labels){
        cout<<u.first<<":"<<u.second<<"\n";
    }
    cout<<"\n";
    cout<<"--------------------------------\n";
     
    cout<<"\n";
    cout<<"Total number of cycles: "<<clock_cycle<<"\n";
    cout<<"Number of row buffer updates are: "<<rowBufferUpdate<<"\n";
    
    cout<<"\n";
    cout<<"Memory content at the end of the execution:\n";
    for(auto i:memoryupdate){
        cout<<i.first<<" : "<<i.second<<endl;
    }
    mips.close();
    return 0;
}
