//常用头文件
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//常用常量
#define OK 1
#define ERROR 0
#define SUCCESS 1
#define UNSUCCESS -1

//常用类型定义
typedef int Status;

// 树数据类型
typedef int TElemType;

// 二叉树结构体
typedef struct BiTNode {
	TElemType data;
	struct BiTNode* lchild, * rchild;
} BiTBode, *BiTree;

// 部分需要的接口
// 将二叉树分为根，左子树，右子树三个部分
Status BreakBiTree(BiTree *T, BiTree *L, BiTree *R);
// 获取树的深度
int BiTreeDepth(BiTree T);


#include <Windows.h>

//改变光标位置
void gotoxy(int x, int y)
{
	// 更新光标位置
	COORD pos;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOutput, pos);
}

// 获取树的深度
int BiTreeDepth(BiTree T) {
	if (T == NULL) return 0;
	
	int depthLeft, depthRight;
	depthLeft = BiTreeDepth(T->lchild);
	depthRight = BiTreeDepth(T->rchild);

	return 1 + (depthLeft > depthRight ? depthLeft : depthRight);
}


// 将二叉树分为根，左子树，右子树三个部分
Status BreakBiTree(BiTree *T, BiTree *L, BiTree *R) {
	if (T == NULL) return ERROR;
	*L = T->lchild;
	*R = T->rchild;

	return OK;
}


/*
* 递归打印打印出树形
* T		正在打印的树
* depth	目前在打印树的第几层
* right	该子树是否为右子树
* tap	目前子树需要的相对偏移数量
*/
Status Traverse_R(BiTree T, int depth, int right, int tap) {
	if (T == NULL) return OK;

	// 获取一次树的初始高度，用于计算相对偏移数量
	static int treeDepth;
	treeDepth = BiTreeDepth(T);
	// 记录当前光标位置，用于在递归中记录当前递归时树的位置
	int x, y;
	// 拆分树，将树的左右子树拆分开来
	BiTree L, R;
	BreakBiTree(&T, &L, &R);

	// 计算父树的偏移量
	int tap1 = tap * pow(2, treeDepth - depth);
	// 计算子树的偏移量
	int tap2 = right * (pow(2, treeDepth - depth));
	// 计算半偏移量
	int tap3 = pow(2, treeDepth - depth - 1);

	// 获取根的坐标
	// x 计算方法为：父偏移量 + 子偏移量 + 半偏移量 - 1
	// y 计算方法为：目前打印的层数 * 2
	x = tap1 + tap2 + tap3 - 1;
	y = depth * 2;

	// 打印根的位置
	gotoxy(x * 2, y);
	printf("%d", T->data);

	// 在打印子树时，当前层数+1
	depth++;
	// 计算子树的父偏移量
	tap = tap * 2 + (right == 1 ? 2 : 0);
	if (L == NULL && R == NULL) return OK;
	else if (R == NULL) {
		// 打印左子树的位置
		gotoxy(x * 2 - tap3, y + 1);
		printf("┏");
		for (int i = 0; i < tap3-1; i++) printf("━");
		printf("┛");
		Traverse_R(L, depth, 0, tap);
	}
	else if (L == NULL) {
		// 打印右子树的位置
		gotoxy(x * 2, y + 1);
		printf("┗");
		for (int i = 0; i < tap3-1; i++) printf("━");
		printf("┓");
		Traverse_R(R, depth, 1, tap);
	}
	else {
		// 打印左右子树的位置
		gotoxy(x * 2 - tap3, y + 1);
		printf("┏");
		for (int i = 0; i < tap3 - 1; i++) printf("━");
		printf("┻");
		for (int i = 0; i < tap3 - 1; i++) printf("━");
		printf("┓");
		Traverse_R(L, depth, 0, tap);
		Traverse_R(R, depth, 1, tap);
	}
}

// 打印树形接口
Status Traverse(BiTree T) {
	Traverse_R(T, 0, 0, 0);
	return OK;
}


typedef struct HTNode{
	char c;
	int weight;
	int parent, lchild, rchild;
}HTNode, *HuffTree;

typedef struct{
	char c;
	int weight;
}HuffChar;

typedef char** HuffCode;

int main(){
	FILE *fp = fopen("hfmtree.txt", "r");
	HuffCode HC;
	HuffTree HT;
	while(!feof(fp)){
		HC
	}
	return 0;
}