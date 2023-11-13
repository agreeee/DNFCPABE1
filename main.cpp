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
	pbc_init(argc, argv);	// ��ʼ������
	InitZrRandomBuff(900);
	clock_t begin[4] = { 0 }, end[4] = { 0 };
	G P; // ���� G Ⱥ���� P
	P.set1();		// ������� G Ⱥ�е�����Ԫ P

	printf("\n**********ϵͳ����***********\n");
	begin[0] = clock();
	MAPK MPK;		//����Կ
	G gs;		//��˽Կg^s
	Setup(P, MPK, gs);	//��ϵͳ������˽Կ�͹�Կ
	unsigned char p_msk[32] = { 0 };
	PRF_Setup(p_msk);
	end[0] = clock() - begin[0];

	//��������T���洢[n]��ѡt�����������
	unsigned char T[CNT][dt] = {0};
	unsigned char mid_0[n] = { 0 };//�洢[n]
	for (int i = 0; i < n; i++) {
		mid_0[i] = i;
	}
	unsigned char mid_1[dt] = { 0 };
	int count = 0;
	Text(mid_0, mid_1, 0, n - 1, 0, T, &count);

	printf("\n**********˽Կ�����㷨***********\n");
	begin[1] = clock();
	bool x[n] = { 0 };     //�û����Բ���ʼ��
	x[0] = 0; x[1] = 1;
	x[2] = 1; x[3] = 0;
	USK SK;     //�û�˽Կ
	KeyGen(P, MPK, gs, x, SK);
	unsigned char p_t[32] = { 0 };//�洢x������ֵ
	PRF_Eval(p_msk, x, T, p_t);
	end[1] = clock() - begin[1];
	

	//�����㷨
	printf("\n********�����㷨*********\n");
	begin[2] = clock();
	GT M;		//�������
	M.rand(); 
	M.print("M = ");
	//���ʽṹ
	int f_psi[N][dt] = { 0 };
	bool f_phi[N][dt] = { 0 };
	//ȡ�ǣ�
	f_phi[0][0] = 1; f_phi[0][1] = 0; f_phi[1][0] = 1;
	f_phi[1][1] = 1; f_phi[2][0] = 1; f_phi[2][1] = 1;
	//������ţ�
	f_psi[0][0] = 0; f_psi[0][1] = 1; f_psi[1][0] = 1;
	f_psi[1][1] = 2; f_psi[2][0] = 2; f_psi[2][1] = 3;
	CIP C;         //��������
	Enc(P, MPK, M, f_psi, f_phi, C);
	unsigned char sk_f[L][32] = {0};//�洢������Կ
	PRF_Const(p_msk, f_psi, f_phi, T, sk_f);
	end[2] = clock() - begin[2];
	


	printf("\n**********�����㷨***********\n");
	begin[3] = clock();
	GT  DM;
	Dec(SK, C, f_psi, f_phi, x, DM);
	DM.print("DM = ");
	end[3] = clock() - begin[3];

	printf("ϵͳ�����㷨����ʱ�䣺%fms\n", (double)end[0] * 1000 / CLOCKS_PER_SEC);
	printf("˽Կ�����㷨����ʱ�䣺%fms\n", (double)end[1] * 1000 / CLOCKS_PER_SEC);
	printf("�����㷨����ʱ�䣺%fms\n", (double)end[2] * 1000 / CLOCKS_PER_SEC);
	printf("�����㷨����ʱ�䣺%fms\n", (double)end[3] * 1000 / CLOCKS_PER_SEC);


	pbc_clear();			// �ͷ��ڴ�
	ClearZrRandomBuff();	 // �ͷ��ڴ�
	return 0;
}

