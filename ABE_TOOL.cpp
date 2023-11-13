#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ABE_TOOL.h"

//�����ȡx�ĺ���ext
void Xext(bool arr[], bool data[], int start, int end,
	int index, unsigned char vec[], int* count) {
	if (index == dt) {
		for (int i = 0; i < dt; i++) {
			vec[*count] = vec[*count] + (data[i] << i);
		}
		(*count)++;
		return;
	}
	for (int i = start; i <= end && end - i + 1 >= dt - index; i++) {
		data[index] = arr[i];
		Xext(arr, data, i + 1, end, index + 1, vec, count);
	}
}

//�����ȡT�ĺ���
void Text(unsigned char arr[], unsigned char data[], int start, int end,
	int index, unsigned char vec[][dt], int* count) {
	if (index == dt) {
		for (int i = 0; i < dt; i++) {
			vec[*count][i] = data[i];
			printf("%d, ", data[i]);
		}
		printf("\n");
		(*count)++;
		return;
	}
	for (int i = start; i <= end && end - i + 1 >= dt - index; i++) {
		data[index] = arr[i];
		Text(arr, data, i + 1, end, index + 1, vec, count);
	}
}

//�����Կ���ɺ���PRF_Setup,��Կ����Ϊ32���ֽ�
void PRF_Setup(unsigned char* a) {
	int i, j;
	long mid = 0;
	srand(time(nullptr));
	for (i = 0; i < 4; i++) {
		mid = rand() & 0x111111;
		for (j = 0; j < 8; j++) {
			a[i * 8 + j] = (mid >> (j * 8)) && 7;
		}
	}
}

//������������Eval(msk, x)
void PRF_Eval(unsigned char* a, bool* x, unsigned char T[][dt], unsigned char* b) {
	int i, j;
	int count = 0;
	unsigned char out[32] = { 0 };
	unsigned char c[33 + dt] = { 0 };
	memcpy(a, c, 32);

	unsigned char x_t[CNT] = { 0 }; //�洢CNT���Ӵ�
	bool data0[dt] = { 0 };
	Xext(x, data0, 0, n - 1, 0, x_t, &count);
	for (i = 0; i < CNT; i++) {
		printf("%d, ", x_t[i]);
		c[32] = x_t[i];
		for (j = 0; j < dt; j++) {
			c[33 + j] = T[i][j];
		}
		sm3(c, 33 + dt, out);
		for (j = 0; j < 32; j++) {
			b[j] = b[j] ^ out[j];
		}
	}
}

//����������Կ����Constant(msk, f)
void PRF_Const(unsigned char* a, int f_psi[][dt], bool f_phi[][dt],
	unsigned char T[][dt], unsigned char sk_f[][32]) {
	int i, j, k, num;
	bool mark[CNT] = { 0 };
	unsigned char c[33 + dt] = { 0 };
	memcpy(a, c, 32);
	//S^0 - US_i^f
	for (i = 0; i < N; i++) {
		for (j = 0; j < dt; j++) {
			c[32] = c[32] + (f_phi[i][j] << j);
			c[33 + j] = f_psi[i][j];
		}
		sm3(c, 33 + dt, sk_f[i]);
	}
	for (i = 0; i < CNT; i++) {
		mark[i] = 0;
		num = 0;
		for (j = 0; j < N; j++) {
			for (k = 0; k < dt; k++) {
				if (T[i][k] == f_psi[j][k]) {
					num++;
				}
			}
			if (num == dt) {
				mark[j] = 1;
				//printf("***************\n");
			}
		}
	}
	//S^1
	int pt = pow(2, dt);
	int pos = 0;//������
	for (i = 0; i < CNT; i++) {
		if (mark[i] == 0) {
			printf("pos = %d\n", pos);
			for (j = 0; j < pt; j++) {
				c[32] = j;
				for (k = 0; k < dt; k++) {
					c[33 + k] = T[i][k];
				}
				sm3(c, 33 + dt, sk_f[N + pos]);
				pos++;
			}
		}
	}
	printf("\npos = %d\n", pos);
}


//������Կģ�⺯��KeySim(,f)
void PRF_KeySim(int f_psi[][dt], bool f_phi[][dt],
	unsigned char T[][dt], unsigned char sk_f[][32]) {
	int i, j, k, num;
	bool mark[CNT] = { 0 };
	unsigned char c[33 + dt] = { 0 };
	unsigned char a[32] = { 0 };
	long mid = 0;
	srand(time(nullptr));
	for (i = 0; i < 4; i++) {
		mid = rand() & 0x111111;
		for (j = 0; j < 8; j++) {
			a[i * 8 + j] = (mid >> (j * 8)) && 7;
		}
	}
	memcpy(a, c, 32);
	//S^0 - US_i^f
	for (i = 0; i < N; i++) {
		for (j = 0; j < dt; j++) {
			c[32] = c[32] + (f_phi[i][j] << j);
			c[33 + j] = f_psi[i][j];
		}
		sm3(c, 33 + dt, sk_f[i]);
	}
	for (i = 0; i < CNT; i++) {
		mark[i] = 0;
		num = 0;
		for (j = 0; j < N; j++) {
			for (k = 0; k < dt; k++) {
				if (T[i][k] == f_psi[j][k]) {
					num++;
				}
			}
			if (num == dt) {
				mark[j] = 1;
				//printf("***************\n");
			}
		}
	}
	//S^1
	int pt = pow(2, dt);
	int pos = 0;//������
	for (i = 0; i < CNT; i++) {
		if (mark[i] == 0) {
			printf("pos = %d\n", pos);
			for (j = 0; j < pt; j++) {
				c[32] = j;
				for (k = 0; k < dt; k++) {
					c[33 + k] = T[i][k];
				}
				sm3(c, 33 + dt, sk_f[N + pos]);
				pos++;
			}
		}
	}
	printf("\npos = %d\n", pos);
}

//����ƥ�亯��CEval(f, x)
void PRF_Eval(int f_psi[][dt], bool f_phi[][dt], bool* x, unsigned char sk_f[][32], unsigned char* b) {
	int i, j;
	int count, pos = -1;
	//�����ж�f(x)=0�Ƿ����������������������ֵb
	for (i = 0; i < N; i++) {
		count = 0;
		for (j = 0; j < dt; j++) {
			if (f_phi[i][j] == x[f_psi[i][j]]) { count++; }
		}
		if (count == dt) {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		printf("*** f(x)=0 ***\n");
	}
	else {

	}



	
	unsigned char out[32] = { 0 };

	unsigned char x_t[CNT] = { 0 }; //�洢CNT���Ӵ�
	bool data0[dt] = { 0 };
	Xext(x, data0, 0, n - 1, 0, x_t, &count);
	for (i = 0; i < CNT; i++) {
		printf("%d, ", x_t[i]);
		c[32] = x_t[i];
		for (j = 0; j < dt; j++) {
			c[33 + j] = T[i][j];
		}
		sm3(c, 33 + dt, out);
		for (j = 0; j < 32; j++) {
			b[j] = b[j] ^ out[j];
		}
	}
}