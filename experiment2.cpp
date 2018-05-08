#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<random>
#include"array.h"
using namespace::std;
typedef int ElementType; 
#define ERROR -1 
#include"SqStack.h"

#define m 10000	//m��ʾ��ά���������
#define n 10000	//n��ʾ��ά���������  ps.���漰�ı���������У������ú궨���������ָ�붯̬����һ�����顣���������������Щ


void Simulation(int **Matrix, int L);		//��������
void Connection(int **Matrix, int *Area);
int MaxArea(int *Area,int &TotalArea);
void DFSTraverse(int **Matrix, int *Area);
void DFS(int **Matrix, int v, int &num,Stack S);

int visited[m*n];

int main() {
	int **Matrix = createArray2(m, n);		      //��̬��ά�����ʾ��ģ�����
	int *Area = new int[m*n];					  //Area�����洢�������������ĸ������ÿ�������
	int TotalArea=0;							  //�������������С
	if (Matrix == NULL||Area==NULL) {
		printf("Memory Allocation Error!");
		return 1;
	}
	int L;
	int i = 0, j = 0;
	int t1 = 0, t2 = 0;
	clock_t start, finish;
	double duration;
	start = clock();
	FILE *outfile = fopen("outfile.txt", "w");
	
	for (i = 0;i < m;i++)		 //��ʼ��
		for (j = 0;j < n;j++)
			Matrix[i][j] = 0;      //0��ʾû�����
	for (i = 0;i < m*n;i++) {
		Area[i] = 0;
	}
	printf("�������  L/(m*n)  �������/������  �������/(m*n)\n");
	for (L = m*n/2;L <= m*n;L+=0.01*m*n) {
		t1 = 0;
		t2 = 0;
		for (i = 0;i < 10;i++) {	//��L��Ӧ���������10��ƽ��ռ��
			Simulation(Matrix, L);
			Connection(Matrix, Area);
			t1 += MaxArea(Area,TotalArea);
			t2 += TotalArea;
		}
		fprintf(outfile, "%0.4f %0.3f\n", (double)t1 / t2,(double)L/(m*n));
		printf("%8d  %7.3f  %15.4f  %14.4f\n",t1/10,(double)L / (m*n), (double)t1 / t2, (double)t1 / (10 * m*n));
	}
	fclose(outfile);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("The run time is %f seconds.\n", duration);

	freeArray2(Matrix);
	delete [] Area;
	Matrix = NULL;
	Area = NULL;
	return 0;
}

void Simulation(int **Matrix,int L) {
	int i, j;
	int x;
	static default_random_engine e(time(0));  //���ں���Ҫ��ε��ã��ʱ��뽫��������������������ͷֲ�������Ϊstatic����ÿ�ε��ò�����ͬ����
	static uniform_int_distribution<unsigned> u(0, m*n-1);//time(0)��Ϊ���ӣ�����time����ָ�������ָ��Ϊ��ʱ������ Unix ��Ԫ��January 1 1970 00:00:00 GMT����ĵ�ǰʱ��������ĺ���

	for (i = 0;i<m;i++)			//��ʼ��
		for (j = 0;j < n;j++) {
			Matrix[i][j] = 0;
		}

	for (i = 0;i < L;i++) {		//�Ը�λ�ó��ֵ��Ƶ����ͳ�ƣ���¼��Matrix��
		x = u(e);
		Matrix[x / n][x % n]++;
	}
}

void DFSTraverse(int **Matrix, int *Area) {
	//��ģ��ͼMatrix��������ȱ���
	int i = 0, num=0;										//i��ʾ�����
	Stack S = CreateStack(m*n);							//��סSҪfree
	for (int v = 0;v < m*n;++v) visited[v] = 0;			//���������ʼ��
	for (int v = 0;v < m*n;++v) Area[v]= 0;			//���������ʼ��
	for (int v = 0;v < m*n;++v) {
		if (!visited[v] && Matrix[v / n][v%n] != 0) {		//��δ�����ʹ��ĸ��v����DFS
			num = 0;									//num����ͳ��v��������ĸ����
			DFS(Matrix, v, num,S);			//�Ӹ��v������������ȱ���ģ��ͼMatrix,i��ʾ�����			
			Area[i++] = num;
		}
	}
	FreeStack(S);
}

void DFS(int **Matrix, int v, int &num,Stack S) {
	//�Ӹ��v�����ݹ��������ȱ���ģ��ͼMatrix,num����ͳ�ƶ���v���ڵ�����ķ�����,i��ʾ�����
	S->Top = -1;
	visited[v] = 1;
	num=1;										//num��1
	Push(S, v);
	while (!IsEmpty(S)) {
		v = S->Data[S->Top];         //vΪջ��Ԫ��
		while (v%n != n - 1 && !visited[v + 1] && Matrix[(v + 1) / n][(v + 1) % n] != 0) {//��ջ��Ԫ��v���ڵ�n-1�������ڸ��δ���������е㸲����																	   						
			Push(S, v + 1);																  //һֱ���Ҳ�����;���ѹ���ջֱ����������Ϊֹ
			visited[v+1] = 1;
			num++;
			v = S->Data[S->Top];         //vΪջ��Ԫ��
		}
		if (v / n != m - 1 && !visited[v + n] && Matrix[(v + n) / n][(v + n) % n] != 0) {//��v���ڵ�m-1�������ڸ��δ���������е㸲����ѹ��ջ																	 
			Push(S, v + n);
			visited[v + n] = 1;
			num++;
		}
		else if (v%n != 0&& !visited[v - 1] && Matrix[(v - 1) / n][(v - 1) % n] != 0) {//��v���ڵ�0�������ڸ��δ���������е㸲����ѹ��ջ
				Push(S, v-1);
				visited[v-1] = 1;
				num++;
		}
		else if (v / n != 0 && !visited[v - n] && Matrix[(v - n) / n][(v - n) % n] != 0) {//��v���ڵ�0�������ڸ��δ���������е㸲����ѹ��ջ
			Push(S, v-n);
			visited[v-n] = 1;
			num++;
		}
		else {//��v���ҡ��¡����ϸ����������򵯳�ջ��Ԫ��
			Pop(S);
		}

	}

}


void Connection(int **Matrix, int *Aera) {
	//������������:����ÿ�����򣬲�ͳ��ÿ�������еķ�������
	DFSTraverse(Matrix, Aera);	//��ģ��ͼMatrix��������ȱ���,Aera���ڼ�¼�������Ӧ�ķ�����
}
int MaxArea(int *Area,int &TotalArea) {
	int t=Area[0];
	TotalArea = 0;
	for (int i = 0;i < m*n;i++) {
		if (t < Area[i])
			t = Area[i];
		TotalArea += Area[i];
	}
	return t;
}




/*  ������룡����
void DFS(int **Matrix, int v, int &num, Stack S) {
	//�Ӹ��v�����ݹ��������ȱ���ģ��ͼMatrix,num����ͳ�ƶ���v���ڵ�����ķ�����,i��ʾ�����
	S->Top = -1;
	visited[v] = 1;
	num = 1;										//num��1
	Push(S, v);
	while (!IsEmpty(S)) {
		while (v%n != n - 1 && !visited[v + 1] && Matrix[(v + 1) / n][(v + 1) % n] != 0) {//��v���ڵ�n-1�������ڸ��δ���������е㸲����
			v = v + 1;																	   //һֱ���Ҳ�����;���ѹ���ջֱ����������Ϊֹ						
			Push(S, v);
			visited[v] = 1;

			num++;
		}
		if (v / n != m - 1 && !visited[v + n] && Matrix[(v + n) / n][(v + n) % n] != 0) {//��v���ڵ�m-1�������ڸ��δ���������е㸲����ѹ��ջ
			v = v + n;
			Push(S, v);
			visited[v] = 1;
			num++;
		}
		else if (v%n != 0 && !visited[v - 1] && Matrix[(v - 1) / n][(v - 1) % n] != 0) {//��v���ڵ�0�������ڸ��δ���������е㸲����ѹ��ջ
			v = v - 1;
			Push(S, v);
			visited[v] = 1;
			num++;
		}
		else if (v / n != 0 && !visited[v - n] && Matrix[(v - n) / n][(v - n) % n] != 0) {//��v���ڵ�0�������ڸ��δ���������е㸲����ѹ��ջ
			v = v - n;
			Push(S, v);
			visited[v] = 1;
			num++;
		}
		else {//��v���ҡ��¡����ϸ����������򵯳�ջ��Ԫ��
			v = Pop(S);
		}

	}

}
*/