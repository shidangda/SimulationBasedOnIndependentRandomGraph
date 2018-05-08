#ifndef SQSTACK_H
#define SQSTACK_H
//要使用本顺序栈必须用typedef定义ElementType类型和用#define宏定义ElemtType类型的ERROR值

#include<stdlib.h>
#include<stdio.h>
typedef int Position;
typedef struct SNode *PtrToSNode;
struct SNode {
	ElementType *Data;//存储元素的数组  栈元素类型ElementType必须在主函数中用typedef定义
	Position Top;
	int MaxSize;//堆栈最大容量
};
typedef PtrToSNode Stack;

Stack CreateStack(int MaxSize) {//创建一个给定容量的空堆栈
	Stack S = new struct SNode;
	S->Data = new ElementType[MaxSize];
	S->Top = -1;
	S->MaxSize = MaxSize;
	return S;
}

bool IsFull(Stack S) {
	return(S->Top == S->MaxSize - 1);
}

bool Push(Stack S, ElementType X) {
	if (IsFull(S)) {
		printf("The Stack is full!\n");
		return false;
	}
	else {
		S->Data[++(S->Top)] = X;
		return true;
	}
}

bool IsEmpty(Stack S) {
	return(S->Top == -1);
}

ElementType Pop(Stack S) {
	if (IsEmpty(S)) {
		printf("The Stack is empty!\n");
		return ERROR;//ERROR必须在主函数中用#define宏定义为Elementype类型的特殊错误标志，必须是正常栈元素取不到的值
	}
	else
		return(S->Data[(S->Top)--]);
}

int FreeStack(Stack S) {
	delete [] S->Data;
	delete S;
	return 0;
}


#endif // !SQSTACK_H

