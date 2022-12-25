#include"ql_execNode.h"

void SelectNode::calculateCost() {
	if (conds_.size() == 0) alg_.push(Selection(A1, (Ts + reldata_.blocks * Tt)));//线性搜索
	else if (conds_.size() == 1) {
		//单属性
		if (conds_[0].op == EQ_OP) {
			//等值比较
			alg_.push(Selection(A1, (Ts + (reldata_.blocks / 2) * Tt)));//线性搜索 等值比较
			if (conds_[0].Lattr.indexable) {
				if (attrs_[indicator_[conds_[0].Lattr.attrname]].offset == 0)
					 alg_.push(Selection(A2, (indexattrs_[0].height + 1) * (Tt + Ts)));//主索引 等值比较
				else alg_.push(Selection(A40, (indexattrs_[0].height + 1) * (Tt + Ts)));//辅助索引 等值比较
			}
			else {
				for (int i = 0; i < indexattrs_.size(); i++) {
					if (attrs_[indicator_[indexattrs_[i].attrname]].offset == 0)
						alg_.push(Selection(A3, indexattrs_[0].height * (Tt + Ts) + reldata_.blocks * Tt));//主索引非码属性
					else
						alg_.push(Selection(A41, (indexattrs_[indexattrs_.size() - 1].height * (Tt + Ts) + reldata_.blocks * Tt)));//辅助索引费码属性
				}
			}
		}
		else {
			//比较
			for (int i = 0; i < indexattrs_.size(); i++) {
				if (attrs_[indicator_[indexattrs_[i].attrname]].offset == 0)
					alg_.push(Selection(A5, (indexattrs_[0].height * (Tt + Ts) + reldata_.blocks * Tt)));//主索引非码属性 比较
				else
					alg_.push(Selection(A6, (indexattrs_[indexattrs_.size() - 1].height * (Tt + Ts) + reldata_.blocks * Tt)));//辅助索引非码属性 比较
			}
		}
	}
	else if (conds_.size() > 1) {
		//复杂属性
		for (int i = 0; i < attrs_.size(); i++) {
			if (attrs_[i].indexable) {
				alg_.push(Selection(A1, (Ts + (reldata_.blocks / 2) * Tt)));
				alg_.push(Selection(A40, (indexattrs_[i].height + 1) * (Tt + Ts)));
				break;
			}
		}
	}
}

int SelectNode::getCost(){
	return alg_.top().cost;
}

void CombNode::calculateCost() {

	alg_.push(NestedLoopJoin, (lnode->reldata_.rcds * rnode->reldata_.blocks + lnode->reldata_.blocks), false);  //嵌套循环连接
	alg_.push(NestedLoopJoin, (rnode->reldata_.rcds * lnode->reldata_.blocks + rnode->reldata_.blocks), true);   

	alg_.push(BlockJoin, (lnode->reldata_.blocks * rnode->reldata_.blocks + lnode->reldata_.blocks), false);       //块循环连接
	alg_.push(BlockJoin, (rnode->reldata_.blocks * lnode->reldata_.blocks + rnode->reldata_.blocks), true);

	alg_.push(MergeJoin, (lnode->reldata_.blocks + rnode->reldata_.blocks + 2 * floor(lnode->reldata_.blocks / 2)   //归并连接
		+ floor(lnode->reldata_.blocks) * (2 * ceil(log(lnode->reldata_.blocks / 2) / log(ceil(2 / 1) - 1)) - 1))
		+ 2 * floor(rnode->reldata_.blocks / 2) + floor(rnode->reldata_.blocks) * (2 * ceil(log(rnode->reldata_.blocks / 2)
			/ log(ceil(2 / 1) - 1)) - 1), false);
	
	alg_.push(HashJoin, (2 * (ceil(lnode->reldata_.blocks / 2) + ceil(rnode->reldata_.blocks / 2))                  //哈希连接
		* (ceil(log(rnode->reldata_.blocks)) - 1)), false);

	alg_.push(HashJoin, (2 * (ceil(rnode->reldata_.blocks / 2) + ceil(lnode->reldata_.blocks / 2))
		* (ceil(log(lnode->reldata_.blocks)) - 1)), true);

}

int CombNode::getCost() {
	return alg_.top().cost;
}