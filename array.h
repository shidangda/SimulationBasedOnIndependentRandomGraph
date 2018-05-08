#ifndef _ARRAY_H_
#define _ARRAY_H_
#include <stdio.h>
#include <malloc.h>

int **createArray2(int m, int n){
	int i = 0;
	int k = 0;
	int** result = NULL;
	int *p = NULL;
	if ((m > 0) && (n > 0)) {
		result = (int**)malloc(m * sizeof(int*));  // key
		p = (int*)malloc(m * n * sizeof(int));     // key
		if ((result != NULL) && (p != NULL)) {
			for (i = 0;i < m;i++)
				result[i] = p + i * n;		//int **result[m]存放int p[m*n]对应的二维数组的行向量组成的数组的首地址。
		}
		else {
			free(result);
			free(p);
			result = NULL;
			p = NULL;
		}
	}
	return result;
}

void freeArray2(int** p){
	if (*p != NULL) {
		if (*p != NULL)
			free(*p);
		*p = NULL;
	}
	free(p);
	p = NULL;
}

#endif _ARRAY_H_

