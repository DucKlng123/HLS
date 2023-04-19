#include "CDFG.h"
#include <map>
std::vector<BasicBlock> &getBasicBlocks(std::vector<basic_block> bbs){
    //遍历每个basic_block的语句，遇到br,设置前驱后继

    std::vector<BasicBlock> BB;
    std::map<std::string,BasicBlock> BBLable;
    for(auto bb:bbs){
        BBLable[bb.get_label_name()] = BasicBlock(bb);
    }
    for(auto bb : BBLable){
        for(auto s : bb.second.get_statements()){
            int type = s.get_type();
            if(type == OP_BR){
                int numOperands = s.get_num_oprands();
                if(numOperands == 1){//无条件跳转   

                }
                else if(numOperands == 3){//条件分支

                }
                else{
                    std::cout<<"err br op!"<<std::endl;
                }
            }
        }
    }
}
