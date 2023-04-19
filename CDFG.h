#include<vector>
#include<iostream>
#include"parser.cpp"
/*
parser 的层次结构
    parser:
        _filename
        _function_name: get_function_name();
        vector<basic_block> _blocks: get_basic_blocks();
        vector<var> _function_params: get_function_params();
        int _ret_type: get_ret_type();
        string _line : get_current_line();

    basic_block:
        vector<statement> get_statements
        get_lable_name();

    statement:
        int get_type(); set_type(int)
        int get_num_oprands(); set_num_oprands(int);
        get_oprand(int) return _operands[]
        string get_var(),void set_var

    var:
        string _name
        bool _array_flag;

*/
/*扩展bb的前驱和后继*/
class BasicBlock: public basic_block{
    public:
        std::vector<BasicBlock*> predecessors;
        std::vector<BasicBlock*> successors;
        BasicBlock(basic_block bb){
        set_statements(bb.get_statements());
        }
};
//从已有的bb转化为新的包含前驱后继关系的bb;
std::vector<BasicBlock>& getBasicBlocks(std::vector<basic_block> bbs);

//控制流图的构建
class ControlFlowGraph
{
    public:
        std::vector<basic_block*> _blocks;
        basic_block* entry_block;
        basic_block* exit_block;
};

void buildCFG(std::vector<BasicBlock> bbs,ControlFlowGraph &CFG);


//数据流图的构建


//