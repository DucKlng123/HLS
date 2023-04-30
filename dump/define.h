#include<vector>
#include<string>
using namespace std;
//vac
#define needSchedule(a) ()

typedef struct{
    bool isSequential;
    bool isPipelined;
    int area;
    int latency;
    float delay;
    int nCompatibleResources;
    vector<int> opTypes; 

    int num = 0 ;
}ResourceInfo;

typedef struct{
    int nOp,nPred,nSucc;
    float times;
    vector<int> ops;
    vector<int> preds,succs;
    int end = 0 ;//结束的周期
}BlockInfo;

typedef struct{
    int opType;
    int nInput;
    vector<int> inputs;
    //schedule结果
    int cycle;
    int instance = -1 ; //绑定结果
    int cycle_h;
    int instance_h = -1;
}OpInfo;

extern int nResouceType, nOpType, areaLimit;
extern float target;
extern vector<ResourceInfo> resouceLibrary;
extern int nBlock,nOperation;
extern vector<int> opCategories;
extern vector<BlockInfo> blocks;
extern vector<OpInfo> ops;
extern int* opTool;
extern bool test;

typedef struct{
    int nSucc = 0;
    vector<int> succs;
    int npred = 0 ;
    vector<int> preds;
}TmpInfo;

typedef struct{
    int num;
    vector<int> borrow;
}Record;

