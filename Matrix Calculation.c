// 연결리스트를 이용한 행렬 더하기/빼기/곱하기
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define MAX_SIZE 50
typedef enum {head, entry} tagfield;
typedef struct matrix_node *matrix_pointer;
typedef struct entry_node {
        int row;
        int col;
        int value;
} entry_node;
typedef struct matrix_node {
        matrix_pointer down;
        matrix_pointer right;
        tagfield tag;
        union {
           matrix_pointer next;
           entry_node entry;
        } u;
}matrix_node;
matrix_pointer hdnode[MAX_SIZE];

matrix_pointer mread(FILE*fp)
{
int num_rows, num_cols, num_heads,i;
int row=0, col=0, value, current_row;
matrix_pointer temp, last, node;
fscanf(fp,"%d%d", &num_rows, &num_cols); // 최대 행렬 수를 입력
num_heads = (num_cols > num_rows) ? num_cols : num_rows; // 행과 열 중 긴 것을 num)heads에 저장
node = (matrix_pointer) malloc(sizeof(matrix_node)); // 헤드 포인터 생성
node->tag = entry; // 입력을 받을것
node->u.entry.row = num_rows; // 헤드 행 정보에 총 행수 저장
node->u.entry.col = num_cols; // 헤드노드 열 정보에 행렬의 총 열 수 저장
if (!num_heads) node->right = node; // 행과 열 중 가장 긴 것이 0이면 헤드노드 오른쪽에 헤드 연결
else { // 0이 아니면
	for (i=0; i<num_heads; i++) { // 0부터 num_heads-1까지 반복
		temp = (matrix_pointer) malloc(sizeof(matrix_node)); // 임시 포인터 생성
		hdnode[i] = temp; hdnode[i]->tag = head; // hdnode 포인터 i번째 배열에 임시 노드포인터 저장하고 헤드 표시
		hdnode[i]->right = temp; hdnode[i]->u.next=temp; // hdnode 오른쪽에 temp 연결, hdnode next에 temp 연결, 
		// hdnode, hdnode->right hdnode->u.next 세 개의 포인터가 모두 같은 공간을 가리킴.
	}
	current_row = 0; last = hdnode[0]; // 현재 행을 0으로 초기화, 마지막 포인터는 hdnode 0번째 포인터가 가리키는 쪽을 가리킴
	for (;row < num_rows && col < num_cols; col++) { // 행과 열 정보가 총 행렬 숫자를 넘어가지 않을 동안(현재 행과 열은 0, 0을 가리킴)
		fscanf(fp,"%d",&value); // 현재 행과 열의 좌표가 가리키는 값 하나를 읽음
		if(value != 0) { // 그 값이 0이 아니면
			if (row > current_row) { // 현재 프로그램이 가리키는 행보다 방금 읽어드린 값의 행 좌표가 큰 경우
				last->right = hdnode[current_row]; // last의 오른쪽에 프로그램이 가리키는 행의 headnode를 연결
				current_row = row; last = hdnode[row]; // 행렬의 실제 행 좌표와 프로그램이 가리키는 행 좌표를 일치시키고 last도 그 행의 headnode를 가리킴
			}
			temp = (matrix_pointer) malloc(sizeof(matrix_node)); temp->tag = entry; // 0이 아닐 때 값을 입력하기 위한 포인터 공간 생성
			temp->u.entry.row = row; temp->u.entry.col = col; // 좌표 입력
			temp->u.entry.value = value; last->right = temp; last = temp; // 값 저장하고, last가 가리키는 hdnode[i]의 오른쪽에 입력된 노드를 연결하고 last는 방금 만들어진 공간을 가리킴
			hdnode[col]->u.next->down = temp; // 현재 열의 hdnode 아래가 방금 입력된 공간을 가리킴
			hdnode[col]->u.next = temp; // next도 같은 곳 가리킴
		}
		if(col == num_cols-1) { // 열 정보가 총 열-1과 일치하면
			row += 1; // 행 1 증가
			col = -1; // 열 정보 -1 초기화(for문에서 +1 해줄것이기 때문에)
		}
	}
	last->right = hdnode[current_row];
	for (i=0; i<num_cols; i++)
		hdnode[i]->u.next->down = hdnode[i];
	for (i=0; i<num_heads-1; i++)
		hdnode[i]->u.next = hdnode[i+1];
	hdnode[num_heads-1]->u.next = node;
	node->right = hdnode[0];
}
return node;
}

void mwrite(matrix_pointer node)
{
	if(!node) return;
	int i;
	matrix_pointer temp, head = node->right;
	printf("(행 열) 값\n");
	for (i=0; i<node->u.entry.row; i++) {
		for (temp = head->right; temp != head; temp = temp->right)
			printf("(%d %d) %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		head = head->u.next;
	}
printf("\n");
}

void merase(matrix_pointer *node)
{
if(!*node) return;
int i, num_heads;
matrix_pointer x,y, head = (*node)->right;
for (i=0; i<(*node)->u.entry.row; i++) {
	y = head->right;
	while (y != head) {
		x = y; y = y->right; free(x);
	}
	x = head; head = head->u.next; free(x);
}
// free remaining head nodes
y = head;
while (y != *node) {
	x = y; y = y->u.next; free(x);
}
free(*node); *node = NULL;
}

matrix_pointer madd(matrix_pointer node_a, matrix_pointer node_b)
{
	matrix_pointer temp_a, head_a = node_a->right;
	matrix_pointer temp_b, head_b = node_b->right;
	if(node_a->u.entry.row == node_b->u.entry.row && node_a->u.entry.col == node_b->u.entry.col) {
		int num_heads, i=0, j=0;
		int row=0, col=0, value, current_row;
		matrix_pointer temp, last, node;
		node = (matrix_pointer)malloc(sizeof(matrix_node));
		node->tag = entry; // 입력을 받을것
		node->u.entry.row = node_a->u.entry.row; node->u.entry.col = node_a->u.entry.col;
		num_heads = (node->u.entry.col > node->u.entry.row) ? node->u.entry.col : node->u.entry.row; // 행과 열 중 긴 것을 num)heads에 저장
		if (!num_heads) node->right = node; // 행과 열 중 가장 긴 것이 0이면 헤드노드 오른쪽에 헤드 연결
		else { // 0이 아니면
			for (i=0; i<num_heads; i++) { // 0부터 num_heads-1까지 반복
				temp = (matrix_pointer) malloc(sizeof(matrix_node)); // 임시 포인터 생성
				hdnode[i] = temp; hdnode[i]->tag = head; // hdnode 포인터 i번째 배열에 임시 노드포인터 저장하고 헤드 표시
				hdnode[i]->right = temp; hdnode[i]->u.next=temp; // hdnode 오른쪽에 temp 연결, hdnode next에 temp 연결, 
			}
			current_row = 0; last = hdnode[0]; // 현재 행을 0으로 초기화, 마지막 포인터는 hdnode 0번째 포인터가 가리키는 쪽을 가리킴
			for (i=0,j=0; i<node_a->u.entry.row && j<node_b->u.entry.row; i++, j++) {
				for(temp_a = head_a->right, temp_b = head_b->right; temp_a != head_a || temp_b != head_b;) {
					if (temp_a->u.entry.row == temp_b->u.entry.row && temp_a->u.entry.col == temp_b->u.entry.col) {
						row = temp_a->u.entry.row; col = temp_a->u.entry.col;
						value = temp_a->u.entry.value + temp_b->u.entry.value;
						temp_a = temp_a->right; temp_b = temp_b->right;
					}
					else if(temp_a->u.entry.row == temp_b->u.entry.row && temp_a->u.entry.col < temp_b->u.entry.col) {
						row = temp_a->u.entry.row; col = temp_a->u.entry.col;
						value = temp_a->u.entry.value;
						temp_a = temp_a->right;
					}
					else if(temp_a->u.entry.row == temp_b->u.entry.row && temp_a->u.entry.col > temp_b->u.entry.col) {
						row = temp_b->u.entry.row; col = temp_b->u.entry.col;
						value = temp_b->u.entry.value;
						temp_b = temp_b->right;
					}
					else if(temp_a != head_a && temp_b == head_b) {
						row = temp_a->u.entry.row; col = temp_a->u.entry.col;
						value = temp_a->u.entry.value;
						temp_a = temp_a->right;
					}
					else if(temp_a == head_a && temp_b != head_b) {
						row = temp_b->u.entry.row; col = temp_b->u.entry.col;
						value = temp_b->u.entry.value;
						temp_b = temp_b->right;
					}
					if (row > current_row) { // 현재 프로그램이 가리키는 행보다 방금 읽어드린 값의 행 좌표가 큰 경우
						last->right = hdnode[current_row]; // last의 오른쪽에 프로그램이 가리키는 행의 headnode를 연결
						current_row = row; last = hdnode[row]; // 행렬의 실제 행 좌표와 프로그램이 가리키는 행 좌표를 일치시키고 last도 그 행의 headnode를 가리킴
					}
					temp = (matrix_pointer) malloc(sizeof(matrix_node)); temp->tag = entry; // 0이 아닐 때 값을 입력하기 위한 포인터 공간 생성
					temp->u.entry.row = row; temp->u.entry.col = col; // 좌표 입력
					temp->u.entry.value = value; last->right = temp; last = temp; // 값 저장하고, last가 가리키는 hdnode[i]의 오른쪽에 입력된 노드를 연결하고 last는 방금 만들어진 공간을 가리킴
					hdnode[col]->u.next->down = temp; // 현재 열의 hdnode 아래가 방금 입력된 공간을 가리킴
					hdnode[col]->u.next = temp; // next도 같은 곳 가리킴
				}
				head_a = head_a->u.next;
				head_b = head_b->u.next;
			}
			last->right = hdnode[current_row];
			for (i=0; i<node->u.entry.col; i++)
				hdnode[i]->u.next->down = hdnode[i];
			for (i=0; i<num_heads-1; i++)
				hdnode[i]->u.next = hdnode[i+1];
			hdnode[num_heads-1]->u.next = node;
			node->right = hdnode[0];
		}
		return node;
	}
	else printf("주어진 행렬의 행과 열이 서로 같지 않아 더하기 연산이 불가능합니다.\n");
return NULL;
}


matrix_pointer mmult(matrix_pointer node_a, matrix_pointer node_b)
{
	matrix_pointer temp_a, head_a = node_a->right;
	matrix_pointer temp_b, head_b = node_b->right;
	if(node_a->u.entry.col == node_b->u.entry.row) {
		int num_heads, i=0, j=0, sum;
		int row=0, col=0, value, current_row;
		matrix_pointer temp, last, node;
		node = (matrix_pointer)malloc(sizeof(matrix_node));
		node->tag = entry; // 입력을 받을것
		node->u.entry.row = node_a->u.entry.row; node->u.entry.col = node_b->u.entry.col;
		num_heads = (node->u.entry.col > node->u.entry.row) ? node->u.entry.col : node->u.entry.row; // 행과 열 중 긴 것을 num)heads에 저장
		if (!num_heads) node->right = node; // 행과 열 중 가장 긴 것이 0이면 헤드노드 오른쪽에 헤드 연결
		else { // 0이 아니면
			for (i=0; i<num_heads; i++) { // 0부터 num_heads-1까지 반복
				temp = (matrix_pointer) malloc(sizeof(matrix_node)); // 임시 포인터 생성
				hdnode[i] = temp; hdnode[i]->tag = head; // hdnode 포인터 i번째 배열에 임시 노드포인터 저장하고 헤드 표시
				hdnode[i]->right = temp; hdnode[i]->u.next=temp; // hdnode 오른쪽에 temp 연결, hdnode next에 temp 연결, 
			}
			current_row = 0; last = hdnode[0]; // 현재 행을 0으로 초기화, 마지막 포인터는 hdnode 0번째 포인터가 가리키는 쪽을 가리킴
			i = 0;
			for(head_a = node_a->right; i < node_a->u.entry.row; head_a = head_a->u.next) {
				j = 0;
				for(head_b = node_b->right; j < node_b->u.entry.col; head_b = head_b->u.next) {
					temp_a = head_a->right;
					temp_b = head_b->down;
					sum = 0;
				row = temp_a->u.entry.row; col = temp_b->u.entry.col;
					while(temp_a != head_a && temp_b != head_b) {
						if(temp_a->u.entry.col == temp_b->u.entry.row) {
							sum += (temp_a->u.entry.value) * (temp_b->u.entry.value);
							temp_a = temp_a->right; temp_b = temp_b->down;
						}
						else if(temp_a->u.entry.col < temp_b->u.entry.row) temp_a = temp_a->right;
						else if(temp_a->u.entry.col > temp_b->u.entry.row) temp_b = temp_b->down;
					}
					if(sum) {
						value = sum;
						if (row > current_row) { // 현재 프로그램이 가리키는 행보다 방금 읽어드린 값의 행 좌표가 큰 경우
							last->right = hdnode[current_row]; // last의 오른쪽에 프로그램이 가리키는 행의 headnode를 연결
							current_row = row; last = hdnode[row]; // 행렬의 실제 행 좌표와 프로그램이 가리키는 행 좌표를 일치시키고 last도 그 행의 headnode를 가리킴
						}
						temp = (matrix_pointer) malloc(sizeof(matrix_node)); temp->tag = entry; // 0이 아닐 때 값을 입력하기 위한 포인터 공간 생성
						temp->u.entry.row = row; temp->u.entry.col = col; // 좌표 입력
						temp->u.entry.value = value; last->right = temp; last = temp; // 값 저장하고, last가 가리키는 hdnode[i]의 오른쪽에 입력된 노드를 연결하고 last는 방금 만들어진 공간을 가리킴
						hdnode[col]->u.next->down = temp; // 현재 열의 hdnode 아래가 방금 입력된 공간을 가리킴
						hdnode[col]->u.next = temp; // next도 같은 곳 가리킴
					}
					j++;
				}
				i++;
			}
		last->right = hdnode[current_row];
		for (i=0; i<node->u.entry.col; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (i=0; i<num_heads-1; i++)
			hdnode[i]->u.next = hdnode[i+1];
		hdnode[num_heads-1]->u.next = node;
		node->right = hdnode[0];
	}
		return node;
	}
	else printf("주어진 행렬의 행과 열이 서로 같지 않아 곱하기 연산이 불가능합니다.\n");
return NULL;
}


void main()
{
    matrix_pointer a, b, d;
    FILE* fp = fopen("a.txt","r");
    FILE* fw = fopen("b.txt","r");
    a = mread(fp);
    b = mread(fw);
    mwrite(a);
    mwrite(b);
    d = madd(a, b);
    mwrite(d); 
    merase(&d); 
    d = mmult(a, b); 
    mwrite(d); 
    merase(&d); 
    merase(&a);
    merase(&b);
    mwrite(a);
    mwrite(b);
fclose(fp);
fclose(fw);
system("pause");
}        