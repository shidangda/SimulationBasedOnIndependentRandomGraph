#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define n 100					//n表示二维矩阵的边长
#define L 1*n*n					//L表示二维矩阵上落点数

bool visited[n*n];				//访问标志数组
								//函数声明
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
	int Matrix[n][n];						//模拟矩阵
	int Area[n*n];							//记录区域和相应的方格数
	int AreaDistri[n][n];					//记录每个格点所属的区域号
	Simulation(Matrix);						//统计每个位置上落的点数（即随机数的个数），得到模拟矩阵Matrix
	Connection(Matrix, Area, AreaDistri);	//联结相邻区域:连接每块区域，并统计每块区域中的方格数
	Plot(AreaDistri);						//画图:用区域号标记每块区域，画出区域分布图
	tem = MaxArea(Area);
	int a = L;
	printf("\n\n最大区域大小：%d  总区域大小：%d  比例：%f  L值：%d\n", tem, n*n, (double)tem / (n*n),a );//注意宏定义L直接放在此处会产生强制类型转换，变为0
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("\nThe runtime is %f seconds\n", duration);
	return 0;
}


void Simulation(int Matrix[n][n]) {
	int i, j;
	int x;
	srand((unsigned)time(NULL));//播种子,time(0)的返回值作为seed.
	printf("\n");

	for(i=0;i<n;i++)			//初始化
		for (j = 0;j < n;j++) {
			Matrix[i][j] = 0;
		}

	for (i = 0;i < L;i++) {		//对各位置出现点的频度做统计，记录在Matrix中
		x = rand() % (n*n);
		Matrix[x / n][x % n]++;
	}

	for (i = 0;i < n;i++) {
		for (j = 0;j < n;j++) {
			printf("%2d ", Matrix[i][j]);
		}
		printf("\n");
	}
	
	for (i = 0;i < n;i++) {			//画出模拟图
		for (j = 0;j < n;j++) {
			if (Matrix[i][j])
				printf("■");
			else
				printf("  ");
		}
		printf("\n");
	}
	
}


void DFSTraverse(int Matrix[n][n],int Area[n*n],int AreaDistri[n][n]) {
														//对模拟图Matrix作深度优先遍历
	int i=0,num;										//i表示区域数减1
	for (int v = 0;v < n*n;++v) visited[v] = 0;			//访问数组初始化
	for (int v = 0;v < n*n;++v) Area[v] = 0;			//区域数组初始化
	for (int v = 0;v < n*n;++v) {
		if (!visited[v]&& Matrix[v/n][v%n] > 0) {		//对未被访问过的格点v调用DFS
			num = 0;									//num用于统计v所在区域的格点数
			DFS(Matrix, v,num,i,AreaDistri);			//从格点v出发，深度优先遍历模拟图Matrix,i表示区域号			
			Area[i++] = num;
		}
	}
}

void DFS(int Matrix[n][n],int v,int &num ,int i,int AreaDistri[n][n]) {		
												//从格点v出发递归地深度优先遍历模拟图Matrix,num用于统计顶点v所在的区域的方格数,i表示区域号
												//
	visited[v] = 1;
	AreaDistri[v / n][v%n] = i;
	num++;										//num加1
	int w;										//w用于表示v的邻接格点，即与v相邻且有点覆盖的格点

	if (v%n != n - 1) {									
		w = v + 1;										//若v不在第n-1列，则访问v的右邻格点
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {    //对v的尚未访问的邻接格点递归调用DFS遍历
			DFS(Matrix, w, num, i, AreaDistri);
		}
	}
	if (v < (n*(n - 1) - 1)) {
		w = v + n;								//若v不在第n-1行，则访问v的下邻格点
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {
			DFS(Matrix, w, num,i,AreaDistri);
		}
	}
	if (v%n != 0) {
		w = v - 1;								//若v不在第一列，则访问v的左邻格点
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {
			DFS(Matrix, w, num,i,AreaDistri);
		}
	}
	if (v > n - 1) {
		w = v - n;								//若v不在第一行，则访问v的上邻格点
		if (Matrix[w / n][w%n] > 0 && !visited[w]) {
			DFS(Matrix, w, num,i,AreaDistri);
		}
	}
}


void Connection(int Matrix[n][n],int Aera[n*n],int AreaDistri[n][n]) {
								//联结相邻区域:连接每块区域，并统计每块区域中的方格数。
	for (int i = 0;i < n;i++)	//区域分配矩阵初始化，-1表示该点不属于任何区域
		for (int j = 0;j < n;j++)
			AreaDistri[i][j] = -1;
	DFSTraverse(Matrix,Aera,AreaDistri);	//对模拟图Matrix作深度优先遍历,Aera用于记录区域和相应的方格数
	printf("\n区域号    方格块数\n");
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
			else printf("□");
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












