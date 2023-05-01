#include<iostream>
#include"define.h"
#include<algorithm>
#include<queue>

int maxOp = 15 ; //一次调度的最大操作数
int* opTool;
//得到dominator树
/*
dom(n) = 所有经过 Blockn 的path的交集
dominator[n][i] : dominator(n) 包含 i   true/false
函数作用：得到path尾位置block的dominator,将非path路径熄灭
思路：遍历(dominator数组需要）+前序递归(path需要)
*/
bool **Dominator;
vector<int> BlockOrder;
void dominator(vector<int> &path){
    int nowid = path[path.size() - 1];
    //熄灭未在path上的路径
    vector<bool> pathIndex(nBlock,false);
    for(int i = 0 ; i < path.size(); i++){
        pathIndex[path[i]] = true;
    }

    for(int i = 0 ; i < nBlock; i++){
        if(pathIndex[i] == false && Dominator[nowid][i] == true){
            Dominator[nowid][i] = false;
        }
    }
    //遍历完成，处理path迭代关系
    for(int i = 0 ; i < blocks[nowid].nSucc ; i++){
        int nextid = blocks[nowid].succs[i] ;
        //遍历子节点,除去环的情况
        if(find(path.begin(),path.end(),nextid) == path.end()){
            path.push_back(nextid);
            dominator(path);
            path.pop_back();
        }
    }
}
typedef struct inDegreeBlock
{
    int index ;
    int inDegree;
}inDeBlock;

class comp{
    bool operator()(inDeBlock a, inDeBlock b){
        return a.inDegree > b.inDegree;
    };
};
void defBlockOrder(){
    /*先建立dominator关系*/
    Dominator = new bool* [nBlock];
    for(int i = 0 ; i < nBlock ; i++){
        Dominator[i] = new bool [nBlock];
        for(int k = 0 ; k < nBlock ; k++){
            Dominator[i][k] = true;
        }
    }
    if(blocks[0].nPred != 0){
        cout << "entry is not block0" << endl;
    }
    else {
        vector<int> path(1,0);
        dominator(path);
    }

    /*然后通过dominator消回边*/
    //结点指向dom(n)就是回边
    vector<int> newPreds;
    for(int i = 0 ; i < nBlock ; i++){
        for(int k = 0 ; k < blocks[i].nPred ; k++){
            int preBlock = blocks[i].preds[k];
            if(Dominator[preBlock][i] == true){
                for(auto it = blocks[preBlock].succs.begin(); it != blocks[preBlock].succs.end() ; it++){
                    if(*it == i){
                        blocks[preBlock].succs.erase(it);
                        blocks[preBlock].nSucc -- ;
                        break;
                    }
                }
            }//回边
            else{//非回边
                newPreds.push_back(preBlock);
            }
        }//每个pred
        blocks[i].nPred = newPreds.size();
        blocks[i].preds = newPreds;
    }//每个block

    /*blocks拓扑排序*/
    int* degree = new int[nBlock];
    bool* if_order = new bool[nBlock];
    for(int i = 0; i < nBlock; ++i){
        if_order[i] = false;
        degree[i] = blocks[i].nPred;
    }

    while(BlockOrder.size() != nBlock){
        for(int i = 0; i < nBlock; ++i)
            if(degree[i] == 0 && !if_order[i]){
                if_order[i] = true;
                BlockOrder.push_back(i);
                for(int j = 0; j < blocks[i].nSucc; ++j)
                    degree[blocks[i].succs[j]]--;
            }
    }

    for(int i = 0 ; i < nBlock ; i++){
        delete [] Dominator[i];
    }
    delete[] Dominator;
    delete[] if_order;
    delete[] degree;
}


vector<int> beginTime;//每个操作的开始时间

int heuristic(vector<int> &scheduleOps, int blockBeginCylce){
    vector<int> readyList;

    /*ALAP*/
    //找到该部分scheduledOps的依赖关系,并把依赖关系存储到opNodes中
    int opNum = scheduleOps.size();
    vector<TmpInfo> opNodes(opNum);
    for(int i = 0 ; i < opNum ; i++){
        int opIndex = scheduleOps[i];
        bool ready = true;
        for(int k = 0; k < ops[opIndex].nInput ; k++){
            int sourceIndex = ops[opIndex].inputs[k];
            if(sourceIndex == -1){
                continue;
            }//输入为参数或常数 ,下一个输入
            else{
                auto p = find(scheduleOps.begin(),scheduleOps.end(),sourceIndex);
                if(p != scheduleOps.end()){
                    ready = false;
                    int sourceIDinSche = distance(scheduleOps.begin(),p);
                    opNodes[sourceIDinSche].nSucc ++ ;
                    opNodes[sourceIDinSche].succs.push_back(i);
                    opNodes[i].npred ++ ;
                    opNodes[i].preds.push_back(sourceIDinSche);
                }//找到了依赖
            }//输入依赖
        }//每个输入
        if(ready)
            readyList.push_back(i);//无前驱
    }//每个OP

    //ALAP
    int min_begin = 0 ;
    vector<int> endTime(opNum,1);//记录每个操作完成的时间
    bool flag = true ; //还有未schedule的结点
    while(flag){
        //没有后继结点的结点schedule
        for(int i = 0 ; i < opNum ; i++){
            if(opNodes[i].nSucc == 0){//后继结点已处理完毕
            //处理时序
                if(opNodes[i].succs.size() == 0){//无依赖
                    endTime[i] = 0;
                    beginTime[i] = (-1) * opTool[ops[scheduleOps[i]].opType];
                }
                else{
                    int sourceNum = opNodes[i].nSucc;
                    int minSouceEnd = 1 ;
                    for(int k = 0 ; k < sourceNum ; k++){
                        minSouceEnd = min(minSouceEnd , endTime[opNodes[i].succ[k]]);
                    }
                    endTime[i] = minSouceEnd;
                    beginTime[i] = minSouceEnd - lat(opTool[ops[scheduleOps[i]].opType]);

                }//有依赖
            //处理依赖关系,前序结点出度-1
                int predNum = opNodes[i].nPred;
                for(int k = 0 ; k < predNum ; k++){
                    int predId = opNodes[i].preds[k];
                    opNodes[predId].nSucc -- ;
                }

                min_begin = min(min_begin,beginTime[i]);

            }
        }
        //一轮schedule完成后，判断是否还有未schedule结点
        flag = false;
        for(int i = 0 ; i < opNum ; i++){
            if(endTime[i] == 1){
                flag = true;
                break;
            }
        }
    }//schedule完毕
    min_begin = - min_begin;
    for(int i = 0 ; i < opNum ; i++){
        beginTime[i] += min_begin;
    }


    //List schedule
    
}
