#pragma warning( disable : 4996)
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAXLINE 256
#define MaxSize 256

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

void menu();
int initializeT();
void encodeT(HuffTree &HT, int tmp_n);
void printBT1(int tmp_n);
void codePrintHT();
void decodeHT(HuffTree &HT, int tmp_n);
void displayHT(HuffTree &HT, int n);

int main(){
	int c, tmp_n = 0;
	HuffTree HT;
	system("cls");
	menu();
	while ((c = getchar()) != EOF){
		fflush(stdin);
		switch (c){
			case '\n':
			case ' ':
			case '\t':
				break;
			case 'I':
			case 'i':
				tmp_n = initializeT();
				putchar('\n');
				break;
			case 'E':
			case 'e':
				encodeT(HT,tmp_n);
				putchar('\n');
				break;
			case 'D':
			case 'd':
				decodeHT(HT, tmp_n);
				putchar('\n');
				break;
			case 'P':
			case 'p':
				codePrintHT();
				putchar('\n');
				break;
			case 'T':
			case 't':
				displayHT(HT, tmp_n);
				putchar('\n');
				break;
			case 'Q':
			case 'q':
				exit(0);
				break;
			default:
				printf("[-] Invalid choice!\n");
				menu();
				break;
		}
	}

	return 0;
}

void menu(){
	printf("------huffman en/decoding system------\n");
	printf("[I/i] Initialization\n");
	printf("[E/e] Encoding\n");
	printf("[D/d] Decoding\n");
	printf("[P/p] Print\n");
	printf("[T/t] Tree printing\n");
	printf("[Q/q] Quit\n");
	printf("--------------------------------------\n");
}

//codePrint_start
void codePrintHT(){
	FILE *fp_readfrom = fopen("CodeFile.txt","r+");
	FILE *fp_writeto = fopen("CodePrint.txt", "w+");
	int i = 0, c;
	while ((c = fgetc(fp_readfrom)) != EOF){
		if (i++ == 50){
			fputc('\n', fp_writeto);
			putchar('\n');
			i = 1;
		}
		fputc(c, fp_writeto);
		putchar(c);
	}
	fclose(fp_readfrom);
	fclose(fp_writeto);
}
//codePrint_end

//Encoding_start
void encodeT(HuffTree &HT, int tmp_n){
	FILE *fp_hfmt = fopen("hfmtree.txt", "r+"), *fp_tbt = fopen("ToBeTran.txt", "r+"), *fp_cf = fopen("CodeFile.txt", "w+");
	if (!(fp_hfmt && fp_tbt && fp_cf)){
		printf("[-] Failed in reading files while encoding.\n");
		exit(3);
	}
	char line[MAXLINE], newword[MAXLINE];
	HT = (HuffTree)malloc(sizeof(HTNode) * (tmp_n*2));
	int i = 0, end = 0, tmp, c;
	while(i < (tmp_n*2-1)){
		fscanf(fp_hfmt, "%d,%c,%d,%d,%d,%d\n", &tmp, &HT[++i].c, &HT[i].weight, &HT[i].parent, &HT[i].lchild, &HT[i].rchild);
	}
	end = i;
	while ((c = fgetc(fp_tbt)) != EOF){
		if (c == '\n')
			continue;
		for (i = 1; i <= end; ++i){
			if (HT[i].c == c){
				int q = 0;
				while (HT[i].parent != 0){
					newword[q++] = (i == HT[HT[i].parent].lchild) ? '0' : '1';
					i = HT[i].parent;
				}
				newword[q] = 0;
				while (q){
					fputc(newword[--q], fp_cf);
				}
			}
		}
	}
	fclose(fp_hfmt);
	fclose(fp_tbt);
	fclose(fp_cf);
}
//Encoding_end

//decoding_start
void decodeHT(HuffTree &HT, int tmp_n){
	FILE *fp_readfrom = fopen("CodeFile.txt", "r+");
	FILE *fp_writeto = fopen("TextFile.txt", "w+");
	if (!(fp_readfrom && fp_writeto)){
		exit(5);
	}
	int root_index = tmp_n*2-1, c, ptr;
	ptr = root_index;
	while ((c = fgetc(fp_readfrom)) != EOF){

		if (c == '0'){
			if (HT[ptr].lchild == 0){
				putchar(HT[ptr].c);
				fputc(HT[ptr].c, fp_writeto);
				ptr = root_index;
			}
			ptr = HT[ptr].lchild;
		}
		else if (c == '1'){
			if (HT[ptr].rchild == 0){
				putchar(HT[ptr].c);
				fputc(HT[ptr].c, fp_writeto);
				ptr = root_index;
			}
			ptr = HT[ptr].rchild;
		}
	}
	fclose(fp_writeto);
	fclose(fp_readfrom);
}
//decoding_end

//Initialization_start
void Select(HuffTree HT,int n,int *s1,int *s2){
	int i,flag = 0;
	for(i = 1; i <= n; i++){
		if(HT[i].parent == 0 && flag == 0){
			*s1 = i;
			flag = 1;
		}
		else if(HT[i].parent == 0 && flag == 1){
			*s2 = i;
			break;
		} 
	}
	if(HT[*s1].weight > HT[*s2].weight){
		flag = *s1;
		*s1 = *s2;
		*s2 = flag;
	}
	for(i++ ; i <= n; i++){
		if(HT[i].parent == 0 && HT[i].weight < HT[*s1].weight){
			*s2 = *s1;
			*s1 = i;
		}
		else if(HT[i].parent == 0 && HT[i].weight < HT[*s2].weight){
			*s2 = i;
		}
	}
}
void constructT(HuffTree *HT, HuffChar *w, int n){
	if (n <= 1)
		return;
	int i, m = n * 2 - 1, s1, s2;
	*HT = (HuffTree)malloc(sizeof(HTNode) * (m + 1));
	HuffTree p = (*HT) + 1;
	for (i = 1; i <= n; i++){
		p->c = w->c;
		p->weight = w->weight;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
		p++;
		w++;
	}
	for (i = n + 1; i <= m; i++){
		p->c = '#';
		p->weight = 0;
		p->parent=0;
		p->lchild=0;
		p->rchild=0;
		p++;
	}

	for (i = n + 1; i <= m; i++){
		Select((*HT), i - 1, &s1, &s2);
		(*HT)[s1].parent=i;
		(*HT)[s2].parent=i;
		(*HT)[i].lchild=s1;
		(*HT)[i].rchild=s2;
		(*HT)[i].weight=(*HT)[s1].weight+(*HT)[s2].weight;
	}

	FILE *fp_hfmt;
	if (!(fp_hfmt = fopen("hfmtree.txt", "w+"))){
		printf("[-] Failed in reading charset while initializing.\n");
		exit(2);
	}
	for (i = 1; i < 2*n; i++){
		//format: index char weight parent lchild rchild
		fprintf(fp_hfmt, "%d,%c,%d,%d,%d,%d\n", i, (*HT)[i].c, (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].lchild, (*HT)[i].rchild);
	}
	fclose(fp_hfmt);
}
int initializeT(){
	int i, nChar, tmp_weight;
	char tmp_c, line[MAXLINE];
	HuffChar *hcharList;
	HuffTree HT;
	FILE *fp_charset;

	if (!(fp_charset = fopen("CharSet.txt", "r"))){
		printf("[-] Failed in reading charset while initializing.\n");
		exit(1);
	}
	fgets(line, MAXLINE, fp_charset);
	nChar = atoi(line);
	hcharList = (HuffChar *)malloc(sizeof(HuffChar) * nChar);
	for (i = 0; i < nChar; i++){
		tmp_c = fgetc(fp_charset);
		fgetc(fp_charset);
		fgets(line, MAXLINE, fp_charset);
		tmp_weight = atoi(line);
		hcharList[i].c = tmp_c;
		hcharList[i].weight = tmp_weight;
	}
	constructT(&HT, hcharList, nChar);
	fclose(fp_charset);
	return nChar;
}
//Initialization_end

//treePrint_start
void displayHT(HuffTree &HT, int tmp_n){
	system("cls");
	FILE *fp = fopen("TreePrint.txt","w+");
	if (!fp)
		exit(6);
	HTNode stack[MaxSize], p;
	int level[MaxSize][2], top, i, n, width = 4, root_index;
	char type;
	root_index = tmp_n*2-1;
	if (HT)
	{
		top = 1;
		stack[top] = HT[root_index];
		level[top][0] = width;
		level[top][1] = 2;

		while (top > 0)
		{
			p = stack[top];
			n = level[top][0];

			switch (level[top][1])
			{
			case 0:
				type = '0';
				break;
			case 1:
				type = '1';
				break;
			case 2:
				type = 'r';
				break;
			}

			for (i = 1; i <= n; i++){
				putchar(' ');
				fputc(' ', fp);
			}
			putchar(p.c);
			putchar('(');
			putchar(type);
			putchar(')');
			fputc(p.c, fp);
			fputc('(', fp);
			fputc(type, fp);
			fputc(')', fp);
			for (i = n + 1; i <= MaxSize - n; i += 2){
				putchar('-');
				fputc('-', fp);
			}
			putchar('\n');
			fputc('\n', fp);
			top--;

			if (p.rchild != 0)
			{
				top++;
				stack[top] = HT[p.rchild];
				level[top][0] = n + width;
				level[top][1] = 1;
			}
			if (p.lchild != 0)
			{
				top++;
				stack[top] = HT[p.lchild];
				level[top][0] = n + width;
				level[top][1] = 0;
			}
		}
	}
	fclose(fp);
}
//treePrint_end

//print_test1__start
#define OK 1
#define ERROR 0
#define SUCCESS 1
#define UNSUCCESS -1
typedef int Status;
typedef char TElemType;
typedef struct BiTNode {
	TElemType data;
	struct BiTNode* lchild, * rchild;
} BiTNode, * BiTree;
Status BreakBiTree(BiTree& T, BiTree& L, BiTree& R);
int BiTreeDepth(BiTree T);
void gotoxy(int x, int y)
{
	COORD pos;
	HANDLE hOutput = GetStdHandle(-11);
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hOutput, pos);
}
int BiTreeDepth(BiTree T) {
	if (T == NULL) return 0;
	int depthLeft, depthRight;
	depthLeft = BiTreeDepth(T->lchild);
	depthRight = BiTreeDepth(T->rchild);

	return 1 + (depthLeft > depthRight ? depthLeft : depthRight);
}
Status BreakBiTree(BiTree& T, BiTree& L, BiTree& R) {
	if (T == NULL) return ERROR;
	L = T->lchild;
	R = T->rchild;
	return OK;
}
Status Traverse_R(BiTree T, int depth, int right, int tap) {
	if (T == NULL) return OK;
	FILE *fp_tp = fopen("TreePrint.txt", "a+");
	if (!fp_tp)
		exit(3);
	static int treeDepth = BiTreeDepth(T);
	int x, y;
	BiTree L, R;
	BreakBiTree(T, L, R);
	int tap1 = tap * pow(2, treeDepth - depth);
	int tap2 = right * (pow(2, treeDepth - depth));
	int tap3 = pow(2, treeDepth - depth - 1);
	x = tap1 + tap2 + tap3 - 1;
	y = depth * 2;
	gotoxy(x * 2, y);
	printf("%c", T->data);
	fprintf(fp_tp,"%c", T->data);
	depth++;
	tap = tap * 2 + (right == 1 ? 2 : 0);
	if (L == NULL && R == NULL) return OK;
	else if (R == NULL) {
		gotoxy(x * 2 - tap3, y + 1);
		printf("[");
		fprintf(fp_tp,"[");
		for (int i = 0; i < tap3 - 1; i++){
			printf("-");
			fprintf(fp_tp, "-");
		}
		printf("}");
		fprintf(fp_tp, "}");
		Traverse_R(L, depth, 0, tap);
	}
	else if (L == NULL) {
		gotoxy(x * 2, y + 1);
		printf("{");
		fprintf(fp_tp,"{");
		for (int i = 0; i < tap3 - 1; i++){
			printf("-");
			fprintf(fp_tp,"-");
		}
		printf("]");
		fprintf(fp_tp,"]");
		Traverse_R(R, depth, 1, tap);
	}
	else {
		gotoxy(x * 2 - tap3, y + 1);
		printf("[");
		fprintf(fp_tp,"[");
		for (int i = 0; i < tap3 - 1; i++){
			printf("-");
			fprintf(fp_tp, "-");
		}
		printf("+");
		fprintf(fp_tp,"+");
		for (int i = 0; i < tap3 - 1; i++){
			printf("-");
			fprintf(fp_tp, "-");
		}
		printf("]");
		fprintf(fp_tp, "]");
		Traverse_R(L, depth, 0, tap);
		Traverse_R(R, depth, 1, tap);
	}
	fclose(fp_tp);
}
Status Traverse(BiTree T) {
	system("cls");
	Traverse_R(T, 0, 0, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	return OK;
}
void convertHT(BiTree &T, int HC[][6], int hcPtr){
	T = (BiTree)malloc(sizeof(BiTNode));
	T->data = HC[hcPtr][1];
	if (HC[hcPtr][4] == 0 && HC[hcPtr][5] == 0){
		T->lchild = NULL;
		T->rchild = NULL;
	}
	else{
		convertHT(T->lchild, HC, HC[hcPtr][4]);
		convertHT(T->rchild, HC, HC[hcPtr][5]);
	}
}
void printBT1(int tmp_n){
	BiTree T;
	FILE *fp_tp = fopen("TreePrint.txt", "w");
	fclose(fp_tp);
	FILE *fp = fopen("hfmtree.txt", "r");
	HuffTree *HT;
	int HC[MAXLINE][6];
	if(!fp){
		exit(1);
	}
	int i = 0, j = 0;
	while(i < (tmp_n*2-1)){
		fscanf(fp, "%d,%c,%d,%d,%d,%d\n", &HC[++i][0], &HC[i][1], &HC[i][2], &HC[i][3], &HC[i][4], &HC[i][5]);
	}
	fclose(fp);
	convertHT(T, HC, i);
	Traverse(T);
}
//print_test1__end