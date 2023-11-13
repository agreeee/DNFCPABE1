#include <stdio.h>
#include <time.h>
#include <math.h>
#include "pbc-class.h"
#include "ABE_Declaration.h"
#include "param.h"
#include "ABE_SETUP.h"
#include "ABE_KEYGEN.h"
#include "ABE_ENCRPT.h"
#include "ABE_DECRPT.h"
#include "ABE_TOOL.h"

unsigned char buf[100] = { 0 }, * pb = buf;

int main(int argc, char* argv[]) {
	pbc_init(argc, argv);	// 初始化环境
	InitZrRandomBuff(900);
	clock_t begin[4] = { 0 }, end[4] = { 0 };
	G P; // 定义 G 群变量 P
	P.set1();		// 随机生成 G 群中的生成元 P

	printf("\n**********系统建立***********\n");
	begin[0] = clock();
	MAPK MPK;		//主公钥
	G gs;		//主私钥g^s
	Setup(P, MPK, gs);	//给系统生成主私钥和公钥
	unsigned char p_msk[32] = { 0 };
	PRF_Setup(p_msk);
	end[0] = clock() - begin[0];

	//构造数组T，存储[n]中选t个的所有组合
	unsigned char T[CNT][dt] = {0};
	unsigned char mid_0[n] = { 0 };//存储[n]
	for (int i = 0; i < n; i++) {
		mid_0[i] = i;
	}
	unsigned char mid_1[dt] = { 0 };
	int count = 0;
	Text(mid_0, mid_1, 0, n - 1, 0, T, &count);

	printf("\n**********私钥生成算法***********\n");
	begin[1] = clock();
	bool x[n] = { 0 };     //用户属性并初始化
	x[0] = 0; x[1] = 1;
	x[2] = 1; x[3] = 0;
	USK SK;     //用户私钥
	KeyGen(P, MPK, gs, x, SK);
	unsigned char p_t[32] = { 0 };//存储x的评估值
	PRF_Eval(p_msk, x, T, p_t);
	end[1] = clock() - begin[1];
	

	//加密算法
	printf("\n********加密算法*********\n");
	begin[2] = clock();
	GT M;		//随机明文
	M.rand(); 
	M.print("M = ");
	//访问结构
	int f_psi[N][dt] = { 0 };
	bool f_phi[N][dt] = { 0 };
	//取非？
	f_phi[0][0] = 1; f_phi[0][1] = 0; f_phi[1][0] = 1;
	f_phi[1][1] = 1; f_phi[2][0] = 1; f_phi[2][1] = 1;
	//属性序号？
	f_psi[0][0] = 0; f_psi[0][1] = 1; f_psi[1][0] = 1;
	f_psi[1][1] = 2; f_psi[2][0] = 2; f_psi[2][1] = 3;
	CIP C;         //定义密文
	Enc(P, MPK, M, f_psi, f_phi, C);
	unsigned char sk_f[L][32] = {0};//存储受限密钥
	PRF_Const(p_msk, f_psi, f_phi, T, sk_f);
	end[2] = clock() - begin[2];
	


	printf("\n**********解密算法***********\n");
	begin[3] = clock();
	GT  DM;
	Dec(SK, C, f_psi, f_phi, x, DM);
	DM.print("DM = ");
	end[3] = clock() - begin[3];

	printf("系统建立算法运行时间：%fms\n", (double)end[0] * 1000 / CLOCKS_PER_SEC);
	printf("私钥生成算法运行时间：%fms\n", (double)end[1] * 1000 / CLOCKS_PER_SEC);
	printf("加密算法运行时间：%fms\n", (double)end[2] * 1000 / CLOCKS_PER_SEC);
	printf("解密算法运行时间：%fms\n", (double)end[3] * 1000 / CLOCKS_PER_SEC);


	pbc_clear();			// 释放内存
	ClearZrRandomBuff();	 // 释放内存
	return 0;
}

