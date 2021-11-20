#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 256
#define MAXWEI  114514.0

typedef double EdgeType;
typedef int VertType;

typedef struct edgeNode{
	int adjVex;
	EdgeType weight;
	struct edgeNode *next;
} edgeNode;

typedef struct vertNode{
	VertType data;
	edgeNode *firstEdge;
} vertNode, AdjList[BUFSIZE];

typedef struct{
	AdjList adjList;
	int vertexCount, edgeCount;
} AGraph;

int visited[BUFSIZE];

//DFS_start
void DFS(AGraph A, int i){
	edgeNode *eN;
	visited[i] = 1;
	printf("[DFS]%d\n", A.adjList[i].data);
	eN = A.adjList[i].firstEdge;
	while (eN){
		if (!visited[eN->adjVex]){
			DFS(A, eN->adjVex);
		}
		eN = eN->next;
	}
}
void DFSTraverse(AGraph A){
	int i, j = 0;
	for (i = 0; i < A.vertexCount; i++){
		visited[i] = 0;
	}
	for (i = 0; i < A.vertexCount; i++){
		if (!visited[i]){
			DFS(A, i);
			j++;
		}
	}
	printf("[DFSNumber of connected components] %d\n", j);
}
//DFS_end

//BFS_start
//Queue_start
typedef struct LQueue{
	int data[BUFSIZE];
	int front, rear;
} Queue, *LQueue;
void EnQ(LQueue *Q, int i){
	if (!(((*Q)->rear + 1) % BUFSIZE == (*Q)->front)){
		(*Q)->data[(*Q)->rear] = i;
		(*Q)->rear = ((*Q)->rear + 1) % BUFSIZE;
	}
}
void DeQ(LQueue *Q, int *i){
	if (!((*Q)->front == (*Q)->rear)){
		*i = (*Q)->data[(*Q)->front];
		(*Q)->front = ((*Q)->front + 1) % BUFSIZE;
	}
}
//Queue_end
void BFS(AGraph A){
	int i, j = 0;
	LQueue Q = (LQueue)malloc(sizeof(Queue));
	for (i = 0; i < A.vertexCount; i++){
		visited[i] = 0;
	}
	Q->front = 0;
	Q->rear = 0;
	for (i = 0; i < A.vertexCount; i++){
		if (!visited[i]){
			visited[i] = 1;
			j++;
			printf("[BFS]%d\n", A.adjList[i].data);
			EnQ(&Q, i);
		}
		while (!(Q->front == Q->rear)){
			DeQ(&Q, &i);
			edgeNode *eN = A.adjList[i].firstEdge;
			while (eN){
				if (!visited[eN->adjVex]){
					visited[eN->adjVex] = 1;
					printf("[BFS]%d\n", A.adjList[eN->adjVex].data);
					EnQ(&Q, eN->adjVex);
				}
				eN = eN->next;
			}
		}
	}
	printf("[BFSNumber of connected components] %d\n", j);
}
//BFS_end

//PRIM_start
void init_tree(AGraph *G, AGraph *primtree){
	int i;
	for (i = 0; i < G->vertexCount; i++){
		primtree->adjList[i].data = G->adjList[i].data;
		primtree->adjList[i].firstEdge = NULL;
	}
	primtree->vertexCount = G->vertexCount;
}
edgeNode *mergeN(int aVex, EdgeType weight){
	edgeNode *newN = (edgeNode *)malloc(sizeof(edgeNode));
	newN->next = NULL;
	newN->adjVex = aVex;
	newN->weight = weight;
	return newN;
}
void MST_prim(AGraph *G, AGraph *primtree){
	int i, j, k, adjVex;
	EdgeType wei_tmp;
	edgeNode *eN_temp = (edgeNode *)malloc(sizeof(edgeNode));
	for (i = 0; i < G->vertexCount; i++){
		visited[i] = 0;
	}
	visited[0] = 1;
	for (i = 0; i < G->vertexCount; i++){
		wei_tmp = MAXWEI;
		for (j = 0; j < G->vertexCount; j++){
			if (visited[j]){
				eN_temp = G->adjList[j].firstEdge;
				while (eN_temp){
					if (wei_tmp > eN_temp->weight && !visited[eN_temp->adjVex]){
						wei_tmp = eN_temp->weight;
						adjVex = eN_temp->adjVex;
						k = j;
					}
					eN_temp = eN_temp->next;
				}
			}
		}
		if (!visited[adjVex]){
			if (!primtree->adjList[k].firstEdge){
				primtree->adjList[k].firstEdge = mergeN(adjVex, wei_tmp);
			}
			else{
				eN_temp = primtree->adjList[k].firstEdge;
				while (eN_temp->next){
					eN_temp = eN_temp->next;
				}
				eN_temp->next = mergeN(adjVex, wei_tmp);
			}
			visited[adjVex] = 1;
		}
	}
}
//PRIM_end

//KRUSKAL_start
int parents[BUFSIZE];
EdgeType sumWei = 0.0;
typedef struct Edge{
	EdgeType weight;
	int fd, bk;
} Edge;
Edge eList[BUFSIZE];
int cmp_E(const void *x, const void *y){
	return (((Edge *)x)->weight - ((Edge *)y)->weight);
}
int find(int x){
	if (parents[x] == x)
		return x;
	return parents[x] = find(parents[x]);
}
void build(int x){
	sumWei += eList[x].weight;
	printf("[KRUS](%d,%d)\n", eList[x].bk, eList[x].fd);
	parents[find(eList[x].bk)] = find(eList[x].fd);
}
void eList_init(AGraph A){
	qsort(eList, A.edgeCount, sizeof(Edge), cmp_E);
}
void MST_kruskal(AGraph A){
	int i, count_e = 0;
	eList_init(A);
	printf("[PRIM Display by edges]\n");
	for (i = 0; i < A.vertexCount; i++)
		parents[i] = i;
	for (i = 0; i < A.edgeCount; i++){
		if (count_e == A.vertexCount - 1)
			break;
		if (find(eList[i].bk) == find(eList[i].fd))
			continue;
		else{
			build(i);
			count_e++;
		}
	}
	printf("[sum of kruskal weight]%.1f\n", sumWei);
}
//KRUSKAL_end

//display_start
EdgeType sum_weight = 0.0;
void DFS_MST(AGraph A, int i){
	edgeNode *eN;
	visited[i] = 1;
	eN = A.adjList[i].firstEdge;
	while (eN){
		if (!visited[eN->adjVex]){
			printf("[PRIM](%d,%d)\n", i, eN->adjVex);
			sum_weight += eN->weight;
			DFS_MST(A, eN->adjVex);
		}
		eN = eN->next;
	}
}
void Display_MST(AGraph A){
	int i;
	for (i = 0; i < A.vertexCount; i++){
		visited[i] = 0;
	}
	printf("[PRIM Display by edges]\n");
	DFS_MST(A, 0);
	printf("[sum of prim weight]%.1f\n", sum_weight);
}
//display_end

int main(){
	AGraph A;
	edgeNode *eN, *eN_temp, *eN1;
	vertNode *vN;
	int vCount, eCount, i;
	VertType vbk, vfd;
	EdgeType wed;
	FILE *fp = fopen("data.txt", "r");

	if(!fp)
		exit(1);
	fscanf(fp, "%d%d\n", &vCount, &eCount);

	A.vertexCount = vCount;
	A.edgeCount = eCount;
	for (i = 0; i < vCount; i++){
		A.adjList[i].firstEdge = NULL;
		A.adjList[i].data = i;
	}
	for (i = 0; i < eCount; i++){
		eN = (edgeNode *)malloc(sizeof(edgeNode));
		fscanf(fp, "%d%d%lf\n", &vbk, &vfd, &wed);
		eN->adjVex = vfd;
		eN->weight = wed;
		eN->next = NULL;
		eList[i].weight = wed;
		eList[i].fd = vfd;
		eList[i].bk = vbk;
		if (!A.adjList[vbk].firstEdge)
			A.adjList[vbk].firstEdge = eN;
		else{
			eN_temp = A.adjList[vbk].firstEdge;
			while (eN_temp->next){
				eN_temp = eN_temp->next;
			}
			eN_temp->next = eN;
		}

		eN1 = (edgeNode *)malloc(sizeof(edgeNode));
		eN1->adjVex = vbk;
		eN1->weight = wed;
		eN1->next = NULL;
		if (!A.adjList[vfd].firstEdge){
			A.adjList[vfd].firstEdge = eN1;
		}
		else{
			eN_temp = A.adjList[vfd].firstEdge;
			while (eN_temp->next){
				eN_temp = eN_temp->next;
			}
			eN_temp->next = eN1;
		}
	}
//initialization of adjGraph end
//DFS_and_BFS_start
	DFSTraverse(A);
	BFS(A);
//DFS_and_BFS_end
//MST_start
	//prim
	AGraph *primtree = (AGraph *)malloc(sizeof(AGraph));
	init_tree(&A, primtree);
	MST_prim(&A, primtree);
	Display_MST(*primtree);
	//kruskal
	MST_kruskal(A);
//MST_end
//debug
	printf("[--------displayA--------debug--------]\n");
	for (i = 0; i < A.vertexCount; i++){
		if (!A.adjList[i].firstEdge)
			continue;
		eN_temp = A.adjList[i].firstEdge;
		printf("[%d]", i);
		do{
			printf("(%d) %.1f,", eN_temp->adjVex, eN_temp->weight);
		}while (eN_temp = eN_temp->next);
		putchar('\n');
	}
	printf("[--------displayT--------debug--------]\n");
	for (i = 0; i < primtree->vertexCount; i++){
		if (!primtree->adjList[i].firstEdge)
			continue;
		eN_temp = primtree->adjList[i].firstEdge;
		printf("[%d]", i);
		do{
			printf("(%d) %.1f,", eN_temp->adjVex, eN_temp->weight);
		}while (eN_temp = eN_temp->next);
		putchar('\n');
	}
}