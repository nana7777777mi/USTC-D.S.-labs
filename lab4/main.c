#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAXLINE 256

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
void initializeT();
void encodeT();

int main(){
	int c;

	system("cls");
	menu();
	while ((c = getchar()) != EOF){
		fflush(stdin);
		switch (c){
			case 'I':
			case 'i':
				initializeT();
				break;
			case 'E':
			case 'e':
				encodeT();
				break;
			case 'D':
			case 'd':

			case 'P':
			case 'p':

			case 'T':
			case 't':

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

//Encoding_start
void encodeT(){
	FILE *fp_hfmt = fopen("huffman.txt", "r"), *fp_tbt = fopen("ToBeTran.txt", "w+");
	if (!(fp_hfmt && fp_tbt)){
		printf("[-] Failed in reading charset while initializing.\n");
		exit(3);
	}
	HuffTree HT;
	HuffCode HC;

}
//Encoding_end

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
		fprintf(fp_hfmt, "%d\t%c\t%d\t%d\t%d\t%d\n", i, (*HT)[i].c, (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].lchild, (*HT)[i].rchild);
	}
	fclose(fp_hfmt);
}
void initializeT(){
	int i, nChar, tmp_weight;
	char tmp_c;
	HuffChar *hcharList;
	HuffTree HT;
	FILE *fp_charset;

	if (!(fp_charset = fopen("CharSet.txt", "r"))){
		printf("[-] Failed in reading charset while initializing.\n");
		exit(1);
	}
	fscanf(fp_charset, "%d\n", &nChar);
	hcharList = (HuffChar *)malloc(sizeof(HuffChar) * nChar);
	for (i = 0; i < nChar; i++){
		fscanf(fp_charset, "%c%d\n", &tmp_c, &tmp_weight);
		hcharList[i].c = tmp_c;
		hcharList[i].weight = tmp_weight;
	}
	constructT(&HT, hcharList, nChar);
	fclose(fp_charset);
}
//Initialization_end