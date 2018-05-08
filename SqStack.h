#ifndef SQSTACK_H
#define SQSTACK_H
//Ҫʹ�ñ�˳��ջ������typedef����ElementType���ͺ���#define�궨��ElemtType���͵�ERRORֵ

#include<stdlib.h>
#include<stdio.h>
typedef int Position;
typedef struct SNode *PtrToSNode;
struct SNode {
	ElementType *Data;//�洢Ԫ�ص�����  ջԪ������ElementType����������������typedef����
	Position Top;
	int MaxSize;//��ջ�������
};
typedef PtrToSNode Stack;

Stack CreateStack(int MaxSize) {//����һ�����������Ŀն�ջ
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
		return ERROR;//ERROR����������������#define�궨��ΪElementype���͵���������־������������ջԪ��ȡ������ֵ
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

