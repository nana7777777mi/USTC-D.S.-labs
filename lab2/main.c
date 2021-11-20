#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define PRICE	2
#define M_PARK	3
#define M_WAIT	4

typedef struct{
//car data
	char state;
	int ID;
	int time;
}car;

typedef struct{
//parking area , stack
	car pArea[M_PARK];
	int top;
}parkStack;

typedef struct CNode{
//nodes of link queue of waiting cars
	car data;
	struct CNode *next;
}CNode;
typedef struct{
//link queue of waiting cars
	int qlen;
	CNode *front;
	CNode *rear;
}waitingcarQ;


void init();
void initStack(parkStack *S);
int isStackFull(parkStack S);
int isStackEmpty(parkStack S);
void pushStack(parkStack *S, car c);
car popStack(parkStack *S);
void printStack(parkStack S);
void initQueue(waitingcarQ *Q);
int isQueueFull(waitingcarQ Q);
void pushQueue(waitingcarQ *Q, car c);
car popQueue(waitingcarQ *Q);
void printQueue(waitingcarQ Q);
car leavePark(parkStack *rS, parkStack *tS, car c1);

int main(){
	char tmpOp;
	int tmpId, tmpTime;
	FILE *fin;
	car tmpCar, biandaoCar;
	parkStack realPark, tempPark;
	waitingcarQ waitQueue;
	fin = fopen("inputData.txt","r");
	init();
	initStack(&realPark), initStack(&tempPark);
	initQueue(&waitQueue);

	do{
		fflush(stdin);
//		scanf("%c %d %d", &tmpOp, &tmpId, &tmpTime);
		fscanf(fin, "%c %d %d\r\n", &tmpOp, &tmpId, &tmpTime);
//		printf("[input] %c %d %d\n", &tmpOp, &tmpId, &tmpTime);
		tmpCar.state = tmpOp, tmpCar.ID = tmpId, tmpCar.time = tmpTime;
		if (tmpOp == 'A' || tmpOp == 'a'){
			if (!isStackFull(realPark)){
				pushStack(&realPark, tmpCar);
				printf("[+] Car %d is parked in place %d of the parking area.\n", tmpCar.ID, realPark.top + 1);
			}
			else if (!isQueueFull(waitQueue)){
				pushQueue(&waitQueue, tmpCar);
				printf("[+] Car %d is waiting in place %d of the waiting area.\n", tmpCar.ID, waitQueue.qlen);
			}
			else{
				printf("[-] No more space for a coming car: %c, %d, %d !\n", tmpCar.state, tmpCar.ID, tmpCar.time);
			}
		}
		else if (tmpOp == 'D' || tmpOp == 'd'){
			if (!(waitQueue.front == waitQueue.rear)){
				leavePark(&realPark, &tempPark, tmpCar);
				biandaoCar = popQueue(&waitQueue);
				biandaoCar.time = tmpCar.time;
				pushStack(&realPark, biandaoCar);
			}
			else if(!isStackEmpty(realPark)){
				leavePark(&realPark, &tempPark, tmpCar);
			}
			else{
				printf("[-] No car in the parking area.\n");
			}
		}
		else if (tmpOp == 'P' || tmpOp == 'p'){
			printStack(realPark);
		}
		else if (tmpOp == 'W' || tmpOp == 'w'){
			printQueue(waitQueue);
		}
		else if (tmpOp == 'E' || tmpOp == 'e'){
			printf("[+] Exiting\n");
		}
		else{
			printf("[-] Invalid input: %c, %d, %d\n", tmpCar.state, tmpCar.ID, tmpCar.time);
		}
	} while(tmpOp != 'E' && tmpOp != 'e');

	system("pause");
	return 0;
}

void initStack(parkStack *S){
	S->top = -1;
}
int isStackFull(parkStack S){
	return (S.top == (M_PARK - 1));
}
int isStackEmpty(parkStack S){
	return (S.top == -1);
}
void pushStack(parkStack *S, car c){
	S->pArea[++S->top] = c;
}
car popStack(parkStack *S){
	return S->pArea[S->top--];
}
void printStack(parkStack S){
	int i;
	for(i = 0; i < S.top + 1; i++){
		printf("[+] \tCar %d is in place %d of the parking area.\n", S.pArea[i].ID, i + 1);
	}
}
car leavePark(parkStack *rS, parkStack *tS, car c1){
	car c2;
	while(!isStackEmpty(*rS) && ( (c2 = popStack(rS)).ID != c1.ID)){
		pushStack(tS, c2);
	}
	if(c2.ID == c1.ID){
		c2.time = c1.time - c2.time;
		printf("[+] %d is leaving from the park, charge: %d\n", c2.ID, c2.time * PRICE);
	}
	else{
		printf("[-] Car %d dosen't exist.\n", c1.ID);
	}
	while(!isStackEmpty(*tS)){
		c2 = popStack(tS);
		pushStack(rS, c2);
	}
	return c2;
}

void initQueue(waitingcarQ *Q){
	Q->qlen = 0;
	Q->front = Q->rear = (CNode *)malloc(sizeof(CNode));
	Q->front->next = NULL;
}
int isQueueFull(waitingcarQ Q){
	return !(M_WAIT - Q.qlen);
}
void pushQueue(waitingcarQ *Q, car c){
	CNode *p = (CNode *)malloc(sizeof(CNode));
	p->data = c;
	p->next = NULL;
	Q->rear->next = p;
	Q->rear = p;
	++Q->qlen;
}
car popQueue(waitingcarQ *Q){
	car c;
	if(Q->rear == Q->front)
		exit(0);
	CNode *p = Q->front->next;
	Q->front->next = p->next;
	c = (p->data);
	if(Q->rear == p)
		Q->rear = Q->front;
	free(p);
	--Q->qlen;
	return c;
}
void printQueue(waitingcarQ Q){\
	int i = 1;
	CNode *p = Q.front;
	while(p != Q.rear){
		p = p->next;
		printf("[+] \tCar %d is waiting in place %d of the waiting place.\n", p->data.ID, i++);
	}
}

void init(){
//initialize menu
	printf("\n\n");
	printf("------------------------------------PARKING AREA------------------------------------\n");
//	printf("                                                                                    \n");
//	printf("                                                                                    \n");
//	printf("                                    ~z31iia                                         \n");
//	printf("                                   oav1zzzauua!                                     \n");
//	printf("                                   u~~!u      ~vn                                   \n");
//	printf("            6uuuuuui31   1a       ~noaz        u3                                   \n");
//	printf("            6uuuuzzuuzvz3zuuu131   1aaz3       ~          i33vu63!1uuuzzz8          \n");
//	printf("           uuu1zaazvzzvvuuuzvvuu11u! izz!             1!1zvuiuuuuzzvvvzuiu          \n");
//	printf("           6uzvvvvvvzzv1uuzzzvu63!aoui6!a3n361uaaau!3uzzzvzu1zzzvvzvzuu116          \n");
//	printf("          1!avvvvvvvvvu!uua3~~ ~zzzzzzzzzzzzzzzzzzzzzzza6aau1vzvvvvzzvv113          \n");
//	printf("          6uzvvvvvvvvz!u3~~ ~zzzzzzazzazzzzzzzzuzzzzzzzzzza3!vvvvvvvzzzza!~         \n");
//	printf("          6u1zzvvvvvzu!o  nzzzzzzazzzuzzzzzzzzzzzzazzzzzzzzza3zvvvvvvvvvu!          \n");
//	printf("          !uuazvvvvvz8~~  ~ozzzavzzauzzzzzzzzvvvvvvv1zzzzzzzzzauvzvvvvvzu6          \n");
//	printf("          ~1uuzvvvzu ~  ~ ~zvzavvzzzzvvvvvvvvvvvvvvvvzzvvvvvvvvz6vvvvzzuu3          \n");
//	printf("           3uuuzvv1~ n~u~vvvzzvvvvvvvvvvvvvvvvvvvvvvvvv1vvvvnvvvzivvvzuu3           \n");
//	printf("            6uuuva~ vznnzvuivvvvzvuvvn~ vvvvvvvvvvvvv~nvzvvvvvvvvz1zzuuu1           \n");
//	printf("             6uuu!au1zoooonaazzuvzvvv~~~vvvvvvvvvvvvvvvvvzvvvvvvvzzu1uu!            \n");
//	printf("              !u3~zvunoooozavvzvvavvvvvvvvvvvvvvvvavvvvvvvzvvvvvvvzzui3             \n");
//	printf("               31zzvvanoznuvvvvvzavvvvvvvvvvvvvvvavvvvvvvvvvvvvvavvzziv             \n");
//	printf("               6~zzvvzuzauvvvzvz3azvvvvvvvvvvvvvv3uizzvvvv3zvvvvzzvvza!             \n");
//	printf("               6zzzvuzvvvvvvvvvziuvvvvvvvvvvvvvvvvavvvvvvvvvvvvvvuzvza3             \n");
//	printf("               izzzzvvvvvvvvvzvaa uvvvvvvvvvvvvvi  zavvvvvvvvvvvvzzvza1             \n");
//	printf("               vazzvvvvvvvvvuzz    1vvvvvvvvvvzvo  ~u~vzuzvvvvvvvvuvzaua            \n");
//	printf("              aoaavvvvvvvvvi 1      nvvvv1zvvv1u     o   !vvvvvvvvavvau3            \n");
//	printf("              6vaazvvvvvvz3           zvvvvvva            1vvvvvvvzvzua6            \n");
//	printf("              6aaavvvvvvv1      ~      o11zvvz~           zvvvvvvvvzzua3            \n");
//	printf("              iaaavvvvvvvi  !6z~   6   ovz1    ~n   ni6v   zvvvvvvvuauuu1           \n");
//	printf("             v~aaavvvvvvv8#3 ~ ~ i83           ~66o    ~88izvvvuvvv1aaua6           \n");
//	printf("             6~aauvvvvvzvva~ ~~~                          vvvzvuvvv1auaa6           \n");
//	printf("             !oaauvvvvzvuzvu~~~~~ ~      ~         ~~~~~~~avzuzvvvv1aaaauu          \n");
//	printf("            n aaa1vvvvvv1v~zn~~~      6nnnnni1         ~~vzauzzvvvv1aaaau6          \n");
//	printf("            6~aaa1vvvvvvv1n          3oooooooo            3z1vvvvvvuaaaaai          \n");
//	printf("            oaaaauvvvvvvvz~          ooooooooo!          6vvvvvvvvvaaaaaaa6         \n");
//	printf("           8~aaaa1vvvvvvvvv3         ~ooooooooz         ivvvzvvvvvvzaaaaaa8         \n");
//	printf("           1vuaaauvvvvvvvzvvvai       ooooooou       6zvzz1vvvvvvvvvuaaaaaa6        \n");
//	printf("          3~iaaaazv6na11  1zzzzv836n~           i333zzu!zvvvvvvvzvvv1uaaaaa6v       \n");
//	printf("          6uaaaa1o     o   1vz16333!!6oooooooo63i!!!!!!nvvn ~vz     #aaaaaa16       \n");
//	printf("         uiaaaa~    n!1!!v uvvva1   ~ oooooooz~    o ! vv6  a~~nnoo~  ~!aaaaa6      \n");
//	printf("         6aaau   ~i         zvvvu ~~  aooooon~   o  3~vvv!  6z      1n  aaaaaa3     \n");
//	printf("        6zzaao       on!i31~avvvv8a oo  ooo1~~oo vo6 vvvv1 o  ~ooo~      uaaaaa3    \n");
//	printf("       6~zaa6     !       1zvvvvv6 6!1  ~oi   u1~n8~vvvvv6 o1       a    aaaaaaa6   \n");
//	printf("      3~zaaaa         ou!iizvvvvva 8   ~zaua     nvvvvvvvva6  o~ooo      #v3aaaaa6~ \n");
	printf("\n\n----------------------------------------MENU----------------------------------------\n");
	printf("[*] Car capacity: %d, bian'dao capacity: %d, charge: %d\n", M_PARK, M_WAIT, PRICE);
	printf("[*] Input format: [Opperation Num] [Car ID] [Time]\n");
}