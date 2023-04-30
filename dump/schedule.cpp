#include<iostream>
#include"define.h"
#include<algorithm>

int maxOp = 15 ; //一次调度的最大操作数

//得到dominator树
/*
dom(n) = 所有经过 Blockn 的path的交集
dominator[n][i] : dominator(n) 包含 i   true/false
函数作用：得到path尾位置block的dominator,将非path路径熄灭
思路：遍历(dominator数组需要）+前序递归(path需要)
*/
bool **Dominator;
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
