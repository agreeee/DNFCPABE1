#ifndef __ABE_TOOL_H__
#define __ABE_TOOL_H__
#include "pbc-class.h"
#include "param.h"
#include "SM3.h"

void PRF_Setup(unsigned char* a); 
void Text(unsigned char arr[], unsigned char data[], int start, int end,
	int index, unsigned char vec[][dt], int* count);
void PRF_Eval(unsigned char* a, bool* x, 
	unsigned char T[][dt], unsigned char* b);
void PRF_Const(unsigned char* a, int f_psi[][dt], bool f_phi[][dt],
	unsigned char T[][dt], unsigned char sk_f[][32]);
void PRF_KeySim(int f_psi[][dt], bool f_phi[][dt],
	unsigned char T[][dt], unsigned char sk_f[][32]);


#endif // !__TOOL_H__