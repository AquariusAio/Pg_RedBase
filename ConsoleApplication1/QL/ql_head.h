#pragma once

enum Operator {
	NO_OP,										// num comparison
	EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP	// binary atomic operators
};

//
// ¾Û¼¯º¯Êý
//
enum AggFun {
	NO_F,
	MIN_F, MAX_F, COUNT_F,
	SUM_F, AVG_F
};

