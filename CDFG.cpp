#include "CDFG.h"
#include <map>
std::vector<BasicBlock> &getBasicBlocks(std::vector<basic_block> bbs){
    //遍历每个basic_block的语句，遇到br,设置前驱后继

    std::map<std::string,BasicBlock> BBLable;
    for(auto bb:bbs){
        BBLable[bb.get_label_name()] = BasicBlock(bb);
    }
    for(auto bb : BBLable){
        BasicBlock &BB = bb.second;
        for(auto s : BB.get_statements()){
            int type = s.get_type();
            if(type == OP_BR){
                int numOperands = s.get_num_oprands();
                if(numOperands == 1){//无条件跳转   
                    //设置successor和predcessor
                    BasicBlock &succ = BBLable[s.get_oprand(0)];
                    BB.successors.push_back(&succ);
                    succ.predecessors.push_back(&BB);
                }
                else if(numOperands == 3){//条件分支
                    BasicBlock &succ1 = BBLable[s.get_oprand(1)];
                    BasicBlock &succ2 = BBLable[s.get_oprand(2)];
                    BB.successors.push_back(&succ1);
                    BB.successors.push_back(&succ2);
                    succ1.predecessors.push_back(&BB);
                    succ2.predecessors.push_back(&BB);
                }
                else{
                    std::cout<<"err br op!"<<std::endl;
                }
            }
        }
    }
