#include"rm_pagehandle.h"
#include<iostream>

unsigned RmPageHandle::nextSlot(PfPageHandle& page) {

	LocationIndex offset = page.head_.pd_lower;
	PageBuffer ptr = page.getLinpBuffer()+ offset;
	ItemIdData linp;
	memcpy( &linp, ptr, sizeof(ItemIdData));
	while (linp.lp_flags == LP_NORMAL) {
		memcpy( &linp, ptr, sizeof(ItemIdData));
		page.head_.pd_lower += sizeof(ItemIdData);
	}
	return linp.lp_off;
}

void RmPageHandle::setUsed(PfPageHandle& page,unsigned rcdlen) {

	LocationIndex offset = page.head_.pd_lower;
	PageBuffer ptr = page.getLinpBuffer() + offset;
	ItemIdData linp, nextlinp;
	memcpy(&linp, ptr, sizeof(ItemIdData));
	memcpy(&nextlinp, ptr + sizeof(ItemIdData), sizeof(ItemIdData));

	linp.lp_flags = LP_NORMAL;//标志使用
	linp.lp_len = rcdlen;
	nextlinp.lp_off = linp.lp_off + rcdlen;

	page.head_.pd_lower += sizeof(ItemIdData);//tuple偏移量修改

	/*tuple写回*/
	memcpy(ptr, &linp, sizeof(ItemIdData));
	memcpy(ptr + sizeof(ItemIdData), &nextlinp, sizeof(ItemIdData));
	return;
}

void RmPageHandle::printPage(PfPageHandle& page) {

	PageBuffer linpPtr = page.getLinpBuffer(),rcdPtr=page.getInsertBuffer();
	ItemIdData linp;

	memcpy(&linp, linpPtr, sizeof(ItemIdData));
	while (linp.lp_flags != LP_UNUSED) {
		if (linp.lp_flags == LP_NORMAL) {
			long offset = linp.lp_off;
			char rcd[linp.lp_len+1];
			rcd[linp.lp_len] = '/0';//尾部标志

			memcpy(&rcd, rcdPtr, linp.lp_len);
			rcdPtr += linp.lp_len;//记录指针后移

			std::cout << rcd << endl;
		}

		linpPtr += sizeof(ItemIdData);
		memcpy(&linp, linpPtr, sizeof(ItemIdData));//linp指针后移
	}
}

int RmPageHandle::isValid(PfPageHandle& page, PageSlot slot) {

	PageBuffer linpPtr = page.getLinpBuffer();
	ItemIdData linp;
	long offset = slot * sizeof(ItemIdData);

	memcpy(&linp, linpPtr+offset, sizeof(ItemIdData));
	if (linp.lp_flags == LP_NORMAL) return 1;
	return 0;
}

unsigned RmPageHandle::getOffset(PfPageHandle& page, PageSlot slot) {

	PageBuffer linpPtr = page.getLinpBuffer();
	ItemIdData linp;
	long offset = slot * sizeof(ItemIdData);

	memcpy(&linp, linpPtr + offset, sizeof(ItemIdData));
	return linp.lp_off;
}

/*
while (linp.lp_flags != LP_NORMAL) {
		offset += sizeof(ItemIdData);
		if (page.head_.pd_lower == offset) return -1;
		linpPtr = linpPtr + sizeof(ItemIdData);
		memcpy(&linp, linpPtr, sizeof(ItemIdData));


	}
*/