#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<random>
#include"array.h"
using namespace::std;
typedef int ElementType; 
#define ERROR -1 
#include"SqStack.h"

#define m 10000	//m表示二维矩阵的行数
#define n 10000	//n表示二维矩阵的列数  ps.当涉及改变数组的行列，尽量用宏定义而不是用指针动态申请一个数组。这样用内置数组简单些


void Simulation(int **Matrix, int L);		//函数声明
void Connection(int **Matrix, int *Area);
int MaxArea(int *Area,int &TotalArea);
void DFSTraverse(int **Matrix, int *Area);
void DFS(int **Matrix, int v, int &num,Stack S);

int visited[m*n];

int main() {
	int **Matrix = createArray2(m, n);		      //动态多维数组表示的模拟矩阵
	int *Area = new int[m*n];					  //Area用来存储各个区域所含的格点数及每个格点数
	int TotalArea=0;							  //有落点的总区域大小
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
	
	for (i = 0;i < m;i++)		 //初始化
		for (j = 0;j < n;j++)
			Matrix[i][j] = 0;      //0表示没有落点
	for (i = 0;i < m*n;i++) {
		Area[i] = 0;
	}
	printf("最大区域  L/(m*n)  最大区域/总区域  最大区域/(m*n)\n");
	for (L = m*n/2;L <= m*n;L+=0.01*m*n) {
		t1 = 0;
		t2 = 0;
		for (i = 0;i < 10;i++) {	//求L对应的最大区域10次平均占比
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
	static default_random_engine e(time(0));  //由于函数要多次调用，故必须将随机数发生器（随机引擎和分布对象）设为static避免每次调用产生相同序列
	static uniform_int_distribution<unsigned> u(0, m*n-1);//time(0)作为种子，函数time单个指针参数，指针为空时返回自 Unix 纪元（January 1 1970 00:00:00 GMT）起的当前时间的秒数的函数

	for (i = 0;i<m;i++)			//初始化
		for (j = 0;j < n;j++) {
			Matrix[i][j] = 0;
		}

	for (i = 0;i < L;i++) {		//对各位置出现点的频度做统计，记录在Matrix中
		x = u(e);
		Matrix[x / n][x % n]++;
	}
}

void DFSTraverse(int **Matrix, int *Area) {
	//对模拟图Matrix作深度优先遍历
	int i = 0, num=0;										//i表示区域号
	Stack S = CreateStack(m*n);							//记住S要free
	for (int v = 0;v < m*n;++v) visited[v] = 0;			//访问数组初始化
	for (int v = 0;v < m*n;++v) Area[v]= 0;			//区域数组初始化
	for (int v = 0;v < m*n;++v) {
		if (!visited[v] && Matrix[v / n][v%n] != 0) {		//对未被访问过的格点v调用DFS
			num = 0;									//num用于统计v所在区域的格点数
			DFS(Matrix, v, num,S);			//从格点v出发，深度优先遍历模拟图Matrix,i表示区域号			
			Area[i++] = num;
		}
	}
	FreeStack(S);
}

void DFS(int **Matrix, int v, int &num,Stack S) {
	//从格点v出发递归地深度优先遍历模拟图Matrix,num用于统计顶点v所在的区域的方格数,i表示区域号
	S->Top = -1;
	visited[v] = 1;
	num=1;										//num加1
	Push(S, v);
	while (!IsEmpty(S)) {
		v = S->Data[S->Top];         //v为栈顶元素
		while (v%n != n - 1 && !visited[v + 1] && Matrix[(v + 1) / n][(v + 1) % n] != 0) {//若栈顶元素v不在第n-1列且右邻格点未被访问且有点覆盖则																	   						
			Push(S, v + 1);																  //一直向右并将沿途格点压入堆栈直到不能向右为止
			visited[v+1] = 1;
			num++;
			v = S->Data[S->Top];         //v为栈顶元素
		}
		if (v / n != m - 1 && !visited[v + n] && Matrix[(v + n) / n][(v + n) % n] != 0) {//若v不在第m-1行且下邻格点未被访问且有点覆盖则压入栈																	 
			Push(S, v + n);
			visited[v + n] = 1;
			num++;
		}
		else if (v%n != 0&& !visited[v - 1] && Matrix[(v - 1) / n][(v - 1) % n] != 0) {//若v不在第0列且左邻格点未被访问且有点覆盖则压入栈
				Push(S, v-1);
				visited[v-1] = 1;
				num++;
		}
		else if (v / n != 0 && !visited[v - n] && Matrix[(v - n) / n][(v - n) % n] != 0) {//若v不在第0行且上邻格点未被访问且有点覆盖则压入栈
			Push(S, v-n);
			visited[v-n] = 1;
			num++;
		}
		else {//若v的右、下、左、上格点均搜索完则弹出栈顶元素
			Pop(S);
		}

	}

}


void Connection(int **Matrix, int *Aera) {
	//联结相邻区域:连接每块区域，并统计每块区域中的方格数。
	DFSTraverse(Matrix, Aera);	//对模拟图Matrix作深度优先遍历,Aera用于记录区域和相应的方格数
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




/*  错误代码！！！
void DFS(int **Matrix, int v, int &num, Stack S) {
	//从格点v出发递归地深度优先遍历模拟图Matrix,num用于统计顶点v所在的区域的方格数,i表示区域号
	S->Top = -1;
	visited[v] = 1;
	num = 1;										//num加1
	Push(S, v);
	while (!IsEmpty(S)) {
		while (v%n != n - 1 && !visited[v + 1] && Matrix[(v + 1) / n][(v + 1) % n] != 0) {//若v不在第n-1列且右邻格点未被访问且有点覆盖则
			v = v + 1;																	   //一直向右并将沿途格点压入堆栈直到不能向右为止						
			Push(S, v);
			visited[v] = 1;

			num++;
		}
		if (v / n != m - 1 && !visited[v + n] && Matrix[(v + n) / n][(v + n) % n] != 0) {//若v不在第m-1行且下邻格点未被访问且有点覆盖则压入栈
			v = v + n;
			Push(S, v);
			visited[v] = 1;
			num++;
		}
		else if (v%n != 0 && !visited[v - 1] && Matrix[(v - 1) / n][(v - 1) % n] != 0) {//若v不在第0列且左邻格点未被访问且有点覆盖则压入栈
			v = v - 1;
			Push(S, v);
			visited[v] = 1;
			num++;
		}
		else if (v / n != 0 && !visited[v - n] && Matrix[(v - n) / n][(v - n) % n] != 0) {//若v不在第0行且上邻格点未被访问且有点覆盖则压入栈
			v = v - n;
			Push(S, v);
			visited[v] = 1;
			num++;
		}
		else {//若v的右、下、左、上格点均搜索完则弹出栈顶元素
			v = Pop(S);
		}

	}

}
*/