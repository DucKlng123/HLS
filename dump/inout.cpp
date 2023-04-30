#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "define.h"
using namespace std;

//定义资源类型数量，操作类型数量 和 面积限制
int nResourceType, nOpType , areaLimit;
//
float target;
vector<ResourceInfo> resourceLibrary;
int nBlock,nOperation;
vector<int> opCategories; //size = nOptye;
vector<BlockInfo> blocks; // size = nBlock;
vector<OpInfo> ops; //size = nOperation;

void read(string filename){
    fstream fs;
    fs.open(filename,ios::in|ios::binary);
    if(!fs.is_open()){
        cout << "can't open file: " << filename << endl;
    }
    string line;
    getline(fs,line);
    istringstream iss(line);
    //输入资源类型数量，操作类型数量，目标，面积限制
    iss >> nResouceType >> nOpType >> target >> areaLimit;

    //resourceLibrary init
    for(int i = 0 ; i < nResouceType ; i++){
        ResourceInfo tmp ;

        getline(fs,line);
        iss.clear();iss.str(line);
        
        int is_seq , area, is_pipelined = 0;
        iss >> is_seq >> area;

        if(is_seq == 1){
            tmp.isSequential = true ;
            getline(fs,line);
            iss.clear();iss.str(line);
            iss >> tmp.latency >> tmp.delay >> is_pipelined;
        }
        else{
            tmp.isSequential = false ;
            getline(fs,line);
            iss.clear();iss.str(line);
            iss >> tmp.delay;
        }
        tmp.area = area ;
        
        if(is_pipelined == 1){
            tmp.isPipelined = true;
        }
        else{
            tmp.isPipelined = false;
        }

        getline(fs,line);
        iss.clear() ; iss.str(line);
        iss >> tmp.nCompatibleOp;
        for(int k = 0 ; k < tmp.nCompatibleOp ; k++){
            int opType;
            iss >> opType;
            tmp.opTypes.push_back(opType);
        }
        resouceLibrary.push_back(tmp);
    }

    //CDFG
    getline(fs,line) ; 
    iss.clear() ; iss.str(line);
    iss >> nBlock >> nOperation;

    //Categories
    getline(fs, line);
    iss.clear() ; iss.str(line);
    for(int i = 0 ; i < nOpType ; i++){
        int cate;
        iss >> cate;
        opCategories.push_back(cate);
    }

    //blocks
    for(int i = 0 ; i < nBlock ; i++){
        BlockInfo tmp;
        getline(fs,line);
        iss.clear() ; iss.str(line);
        iss >> tmp.nOp >> tmp.nPred >> tmp.nSucc >> tmp.expectedTime;
        int control_n[3] = {tmp.nOp, tmp.nPred, tmp.nSucc};
        for(int k = 0 ; k < 3 ; k++){
            getline(fs,line);
            iss.clear() ;
            if(!line.empty()){
                iss.str(line);
                for(int x = 0 ; x < control_n[k] ; x++){
                    int t ;
                    iss >> t ;
                    switch(k){
                        case(0) : tmp.ops.push_back(t); break;
                        case(1) : tmp.preds.push_back(t);break;
                        case(2) : tmp.succs.push_back(t);break;
                    }
                }
            }
        }
        blocks.push_back(tmp);
    }

    //Data flow
    for(int i = 0 ; i < nOperation ; i++){
        OpInfo tmp;
        getline(fs,line);
        iss.clear(); iss.str(line);
        iss >> tmp.opType;
        getline(fs,line);
        iss.clear(); iss.str(line);
        iss >> tmp.nInput;
        for(int k = 0 ; k < tmp.nInput ; k++){
            int tmpInput;
            iss >> tmpInput;
            tmp.inputs.push_back(tmpInput);
        }
        ops.push_back(tmp);
    }
    
    fs.close();
}

