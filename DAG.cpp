
//遍历所有基本块，将
void buildCFG(const IR &ir, CFG &cfg) {
    // 遍历函数定义的所有基本块，将每个基本块转换为一个节点，并建立节点之间的控制边和数据边
    for (const auto &bb : ir.basic_blocks) {
        // 将基本块转换为一个节点
        NodePtr node = std::make_shared<Node>(bb.id);

        // 如果基本块的前驱基本块不为空，则为基本块建立控制边
        if (!bb.preds.empty()) {
            for (const auto &pred_bb : bb.preds) {
                node->addPred(pred_bb->id);
            }
        }

        // 遍历基本块的每个指令
        for (const auto &inst : bb.instructions) {
            // 如果指令的操作符为 phi，则需要为 phi 操作建立数据边
            if (inst.op == "phi") {
                // 为 phi 操作的每个参数建立数据边
                for (size_t i = 0; i < inst.args.size(); i += 2) {
                    auto pred_bb_id = inst.args[i + 1].get<int>();
                    auto pred_bb = ir.getBasicBlockById(pred_bb_id);
                    auto pred_bb_end_inst = pred_bb->instructions.back();
                    auto arg_val = inst.args[i];
                    auto arg_node = cfg.getNodeById(arg_val.get<int>(), pred_bb_id);
                    node->addDataPred(arg_node, i / 2);
                }
            } else {
                // 如果指令的操作符不是 phi，则需要为指令建立数据边
                for (const auto &arg : inst.args) {
                    auto arg_node = cfg.getNodeById(arg.get<int>(), bb.id);
                    node->addDataPred(arg_node);
                }
            }
        }

        // 将节点添加到控制数据流图中
        cfg.addNode(node);
    }
}
