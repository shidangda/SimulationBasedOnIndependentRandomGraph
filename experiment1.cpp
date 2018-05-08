#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define n 100					//n��ʾ��ά����ı߳�
#define L 1*n*n					//L��ʾ��ά�����������

bool visited[n*n];				//���ʱ�־����
								//��������
void Simulation(int Matrix[n][n]);
void DFSTraverse(int Matrix[n][n], int Area[n*n],int AreaDistri[n][n]);
void DFS(int Matrix[n][n], int v, int &num,int i,int AreaDistri[n][n]);
void Connection(int Matrix[n][n], int Aera[n*n],int AreaDistri[n][n]);
void Plot(int AreaDistri[n][n]);
int MaxArea(int *Area);

int main() {
	clock_t start, finish;
	double duration;
	int tem;
	start = clock();
	int Matrix[n][n];						//ģ�����
	int Area[n*n];							//��¼�������Ӧ�ķ�����
	int AreaDistri[n][n];					//��¼ÿ����������������
	Simulation(Matrix);						//ͳ��ÿ��λ������ĵ�������������ĸ��������õ�ģ�����Matrix
	Connection(Matrix, Area, AreaDistri);	//������������:����ÿ�����򣬲�ͳ��ÿ�������еķ�����
	Plot(AreaDistri);						//��ͼ:������ű��ÿ�����򣬻�������ֲ�ͼ
	tem = MaxArea(Area);
	int a = L;
	printf("\n\n��������С��%d  �������С��%d  ������%f  Lֵ��%d\n", tem, n*n, (double)tem / (n*n),a );//ע��궨��Lֱ�ӷ��ڴ˴������ǿ������ת������Ϊ0
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("\nThe runtime is %f seconds\n", duration);
	return 0;
}


void Simulation(int Matrix[n][n]) {
	int i, j;
	int x;
	srand((unsigned)time(NULL));//������,time(0)�ķ���ֵ��Ϊseed.
	printf("\n");

	for(i=0;i<n;i++)			//��ʼ��
		for (j = 0;j < n;j++) {
			Matrix[i][j] = 0;
		}

	for (i = 0;i < L;i++) {		//�Ը�λ�ó��ֵ��Ƶ����ͳ�ƣ���¼��Matrix��
		x = rand() % (n*n);
		Matrix[x / n][x % n]++;
	}

	for (i = 0;i < n;i++) {
		for (j = 0;j < n;j++) {
			printf("%2d ", Matrix[i][j]);
		}
		printf("\n");
	}
	
	for (i = 0;i < n;i++) {			//����ģ��ͼ
		for (j = 0;j < n;j++) {
			if (Matrix[i][j])
				printf("��");
			else
				printf("  ");
		}
		printf("\n");
	}
	
}


void DFSTraverse(int Matrix[n][n],int Area[n*n],int AreaDistri[n][n]) {
														//��ģ��ͼMatrix��������ȱ���
	int i=0,num;										//i��ʾ��������1
	for (int v = 0;v < n*n;++v) visited[v] = 0;			//���������ʼ��
	for (int v = 0;v < n*n;++v) Area[v] = 0;			//���������ʼ��
	for (int v = 0;v < n*n;++v) {
		if (!visited[v]&& Matrix[v/n][v%n] > 0) {		//��δ�����ʹ��ĸ��v����DFS
			num = 0;									//num����ͳ��v��������ĸ����
			DFS(Matrix, v,num,i,AreaDistri);			//�Ӹ��v������������ȱ���ģ��ͼMatrix,i��ʾ�����			
			Area[i++] = num;
		}
	}
}

void DFS(int Matrix[n][n],int v,int &num ,int i,int AreaDistri[n][n]) {		
												//�Ӹ��v�����ݹ��������ȱ���ģ��ͼMatrix,num����ͳ�ƶ���v���ڵ�����ķ�����,i��ʾ�����
												//
	visited[v] = 1;
	AreaDistri[v / n][v%n] = i;
	num++;										//num��1
	int w;										//w���ڱ�ʾv���ڽӸ�㣬����v�������е㸲�ǵĸ��

	if (v%n != n - 1) {									
		w = v + 1;										//��v���ڵ�n-1�У������v�����ڸ��
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {    //��v����δ���ʵ��ڽӸ��ݹ����DFS����
			DFS(Matrix, w, num, i, AreaDistri);
		}
	}
	if (v < (n*(n - 1) - 1)) {
		w = v + n;								//��v���ڵ�n-1�У������v�����ڸ��
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {
			DFS(Matrix, w, num,i,AreaDistri);
		}
	}
	if (v%n != 0) {
		w = v - 1;								//��v���ڵ�һ�У������v�����ڸ��
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {
			DFS(Matrix, w, num,i,AreaDistri);
		}
	}
	if (v > n - 1) {
		w = v - n;								//��v���ڵ�һ�У������v�����ڸ��
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {
			DFS(Matrix, w, num,i,AreaDistri);
		}
	}
}


void Connection(int Matrix[n][n],int Aera[n*n],int AreaDistri[n][n]) {
								//������������:����ÿ�����򣬲�ͳ��ÿ�������еķ�������
	for (int i = 0;i < n;i++)	//�����������ʼ����-1��ʾ�õ㲻�����κ�����
		for (int j = 0;j < n;j++)
			AreaDistri[i][j] = -1;
	DFSTraverse(Matrix,Aera,AreaDistri);	//��ģ��ͼMatrix��������ȱ���,Aera���ڼ�¼�������Ӧ�ķ�����
	printf("\n�����    �������\n");
	for (int i = 0;Aera[i];i++) {
		printf("%6d    %8d\n", i, Aera[i]);
	}
}

void Plot(int AreaDistri[n][n]) {
	int i, j;
	printf("\n");
	for (i = 0;i < n;i++) {
		for (j = 0;j < n;j++) {
			if (AreaDistri[i][j] != -1) {
				printf("%2d", AreaDistri[i][j]);
			}
			else printf("��");
		}
		printf("\n");
	}
}

int MaxArea(int *Area) {
	int t = Area[0];
	int i = 0;
	for (i = 0;i < n*n;i++) {
		if (t < Area[i])
			t = Area[i];
	}
	return t;
}












