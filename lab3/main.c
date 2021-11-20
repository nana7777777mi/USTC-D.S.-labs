#include <stdio.h>
#include <string.h>
#define MAXLINE 256

int next[MAXLINE];
int match_BF(char v[], char key[], int l);
void get_next(char v[]);
int match_KMP(char v[], char key[], int lineNum);

int main(){
	FILE *fp;
	int linecount = 0, keycount = 0;
	char filename[MAXLINE], line[MAXLINE], keyword[MAXLINE];

	printf("[*] Filename: ");
	gets(filename);
	printf("[*] Key word: ");
	gets(keyword);
	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("[-] Fail to load %s\n", filename);
		return 1;
	}
	printf("[+] Result: \n[+] line \tcontent\n");
	while(!feof(fp)){
		fgets(line, MAXLINE, fp);
//		keycount += match_BF(line, keyword, ++linecount);
		get_next(keyword);
		keycount += match_KMP(line, keyword, ++linecount);

	}
	printf("\n[+] Key count: %d\n", keycount);
	
	fclose(fp);
	return 0;
}

int match_BF(char v[], char key[], int lineNum){
	int i, j, count = 0;
	for (i = 0; i < strlen(v); i++){
		for (j = 0; j < strlen(key); j++){
			if (v[i + j] != key[j])
				break;
		}
		if(j == strlen(key)){
			count++;
		}
	}
	if (count){
		printf("[+] %d \t%s", lineNum, v);
	}
	return count;
}

void get_next(char v[]){
	int j = 0, k = -1;
	next[0] = -1;
	while(j < strlen(v)){
		if(k == -1 || v[j] == v[k]){
			j++;
			k++;
			if(v[j] != v[k]){
				next[j] = k;
			}
			else{
				next[j] = next[k];
			}
		}
		else{
			k = next[k];
		}
	}
}
int match_KMP(char v[], char key[], int lineNum){
	int i = 0, j = 0, count = 0;
	while (i < strlen(v) && j < (int)strlen(key)){
		if(j == -1 || v[i] == key[j]){
			i++;
			j++;
		}
		else{
			j = next[j];
		}
		if(j == strlen(key)){
			count++;
			j = 0;
		}
	}
	if (count){
		printf("[+] %d \t%s", lineNum, v);
	}
	return count;
}