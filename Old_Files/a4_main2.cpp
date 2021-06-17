#include<iostream>
#include<fstream>
#include<math.h>
#include<bits/stdc++.h>
#include<stdio.h>
using namespace std;

int Mem_Req_Order(int rowbuffer,vector<vector<int>> queue_op,vector<int>current_op,int lineno,int size,int safe){
    if(rowbuffer == -1){
        return -1;
    }else if(current_op[2]/1024 == rowbuffer){
        return 1;
    }else if(lineno == size|| safe==1){
        return 2;
    }else{
        return 0;
    }
}
bool sortVec(const vector<int>& a,const vector<int>& b){
    return a[4]<b[4];
};
int checkSafe_op(vector<vector<int>>queue_op,int r1,int r2,int r3){
    for(int i=0;i<queue_op.size();i++){
        vector<int>row = queue_op[i];
        if(row[0]==1){
            if(row[5]==r1){
                return -1;
            }else if(row[1]==r1){
                return -1;
            }
        }else{
            if(row[5]==r1){
                return -1;
            }else if(row[1]==r1||row[1]==r2||row[1]==r3){
                return -1;
            }
        }
    }
    return 0;
}
int checkSafe_addi(vector<vector<int>>queue_op,int r1,int r2){
    for(int i=0;i<queue_op.size();i++){
        vector<int>row = queue_op[i];
        if(row[0]==1){
            if(row[5]==r1){
                return -1;
            }else if(row[1]==r1){
                return -1;
            }
        }else{
            if(row[5]==r1){
                return -1;
            }else if(row[1]==r1||row[1]==r2){
                return -1;
            }
        }
    }
    return 0;
}
int check_beq_bne(vector<vector<int>>queue_op,int r1,int r2){
    if(r2==-1){
        for(int i=0;i<queue_op.size();i++){
            vector<int>row = queue_op[i];
            if(row[0]==0){
                if(row[1]==r1){
                    return -1;
                }
            }
        }
    }else{
        for(int i=0;i<queue_op.size();i++){
            vector<int>row = queue_op[i];
            if(row[0]==0){
                if(row[1]==r1||row[1]==r2){
                    return -1;
                }
            }
        }
    }
    return 0;
}
int check_sw_lw(vector<vector<int>> queue_op,int r1,int r2){
    for(int i=0;i<queue_op.size();i++){
        vector<int>row = queue_op[i];
        if(row[0]==0){
            if(row[1]==r1||row[1]==r2){
                return -1;
            }else if(row[5]==r1){
                return -1;
            }
        }
    }
    return 0;
}