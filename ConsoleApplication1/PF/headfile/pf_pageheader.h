#pragma once
#pragma once
#include"header.h"
/*+---------------- + -------------------------------- - +
*| PageHeaderData | linp1 linp2 linp3 ... |
*+---------- - +---- + -------------------------------- - +
*| ... linpN |									  |
*+---------- - +-------------------------------------- +
*|            ^ pd_lower |
*|												  |
*|            v pd_upper |
*+------------ - +------------------------------------ +
*|			 | tupleN ... |
*+------------ - +------------------ + ---------------- - +
*| ... tuple3 tuple2 tuple1 | "special space" |
*+-------------------------------- + ---------------- - +
*^pd_special
*/


typedef unsigned short uint16;
typedef uint16	LocationIndex;

/*linp�ṹ��*/
struct ItemIdData
{
	unsigned
		lp_off : 15,		/* offset to tuple (from start of page) */
		lp_flags : 2,		/* state of line pointer, see below */
		lp_len : 15;		/* byte length of tuple */
};

typedef ItemIdData* ItemId;
#define PF_LINP_LEN PF_LINP_NUM*sizeof(ItemIdData)

/* ҳ��ͷ��Ϣ�ṹ�� */
class PfPageHdr {

public:

	LocationIndex pd_lower;     /* offset to start of free space */
	LocationIndex pd_upper;     /* offset to end of free space */
	LocationIndex pd_special;   /* offset to start of special space */
	//ItemIdData pb_linp[];
public:
	PfPageHdr() :pd_lower(0), pd_upper(800), pd_special(0) {}
};
typedef PfPageHdr* PfPageHdrPtr;


/*
	ÿ���ײ�������lp_flagȡֵ
*/
#define LP_UNUSED		0		/* unused (should always have lp_len=0) */
#define LP_NORMAL		1		/* used (should always have lp_len>0) */
#define LP_REDIRECT		2		/* HOT redirect (should have lp_len=0) */
#define LP_DEAD			3		/* dead, may or may not have storage */

/*
	�궨�壺ҳ��ͷ��Ϣ��ƫ��
*/
#define PG_HEADER_OFFSET sizeof(PfPageHdr)