#include"ql_execNode.h"

void SelectNode::calculateCost() {
	if (conds_.size() == 0) alg_.push(Selection(A1, (Ts + reldata_.blocks * Tt)));//��������
	else if (conds_.size() == 1) {
		//������
		if (conds_[0].op == EQ_OP) {
			//��ֵ�Ƚ�
			alg_.push(Selection(A1, (Ts + (reldata_.blocks / 2) * Tt)));//�������� ��ֵ�Ƚ�
			if (conds_[0].Lattr.indexable) {
				if (attrs_[indicator_[conds_[0].Lattr.attrname]].offset == 0)
					 alg_.push(Selection(A2, (indexattrs_[0].height + 1) * (Tt + Ts)));//������ ��ֵ�Ƚ�
				else alg_.push(Selection(A40, (indexattrs_[0].height + 1) * (Tt + Ts)));//�������� ��ֵ�Ƚ�
			}
			else {
				for (int i = 0; i < indexattrs_.size(); i++) {
					if (attrs_[indicator_[indexattrs_[i].attrname]].offset == 0)
						alg_.push(Selection(A3, indexattrs_[0].height * (Tt + Ts) + reldata_.blocks * Tt));//��������������
					else
						alg_.push(Selection(A41, (indexattrs_[indexattrs_.size() - 1].height * (Tt + Ts) + reldata_.blocks * Tt)));//����������������
				}
			}
		}
		else {
			//�Ƚ�
			for (int i = 0; i < indexattrs_.size(); i++) {
				if (attrs_[indicator_[indexattrs_[i].attrname]].offset == 0)
					alg_.push(Selection(A5, (indexattrs_[0].height * (Tt + Ts) + reldata_.blocks * Tt)));//�������������� �Ƚ�
				else
					alg_.push(Selection(A6, (indexattrs_[indexattrs_.size() - 1].height * (Tt + Ts) + reldata_.blocks * Tt)));//���������������� �Ƚ�
			}
		}
	}
	else if (conds_.size() > 1) {
		//��������
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

	alg_.push(NestedLoopJoin, (lnode->reldata_.rcds * rnode->reldata_.blocks + lnode->reldata_.blocks), false);  //Ƕ��ѭ������
	alg_.push(NestedLoopJoin, (rnode->reldata_.rcds * lnode->reldata_.blocks + rnode->reldata_.blocks), true);   

	alg_.push(BlockJoin, (lnode->reldata_.blocks * rnode->reldata_.blocks + lnode->reldata_.blocks), false);       //��ѭ������
	alg_.push(BlockJoin, (rnode->reldata_.blocks * lnode->reldata_.blocks + rnode->reldata_.blocks), true);

	alg_.push(MergeJoin, (lnode->reldata_.blocks + rnode->reldata_.blocks + 2 * floor(lnode->reldata_.blocks / 2)   //�鲢����
		+ floor(lnode->reldata_.blocks) * (2 * ceil(log(lnode->reldata_.blocks / 2) / log(ceil(2 / 1) - 1)) - 1))
		+ 2 * floor(rnode->reldata_.blocks / 2) + floor(rnode->reldata_.blocks) * (2 * ceil(log(rnode->reldata_.blocks / 2)
			/ log(ceil(2 / 1) - 1)) - 1), false);
	
	alg_.push(HashJoin, (2 * (ceil(lnode->reldata_.blocks / 2) + ceil(rnode->reldata_.blocks / 2))                  //��ϣ����
		* (ceil(log(rnode->reldata_.blocks)) - 1)), false);

	alg_.push(HashJoin, (2 * (ceil(rnode->reldata_.blocks / 2) + ceil(lnode->reldata_.blocks / 2))
		* (ceil(log(lnode->reldata_.blocks)) - 1)), true);

}

int CombNode::getCost() {
	return alg_.top().cost;
}