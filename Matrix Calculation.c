// ���Ḯ��Ʈ�� �̿��� ��� ���ϱ�/����/���ϱ�
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
fscanf(fp,"%d%d", &num_rows, &num_cols); // �ִ� ��� ���� �Է�
num_heads = (num_cols > num_rows) ? num_cols : num_rows; // ��� �� �� �� ���� num)heads�� ����
node = (matrix_pointer) malloc(sizeof(matrix_node)); // ��� ������ ����
node->tag = entry; // �Է��� ������
node->u.entry.row = num_rows; // ��� �� ������ �� ��� ����
node->u.entry.col = num_cols; // ����� �� ������ ����� �� �� �� ����
if (!num_heads) node->right = node; // ��� �� �� ���� �� ���� 0�̸� ����� �����ʿ� ��� ����
else { // 0�� �ƴϸ�
	for (i=0; i<num_heads; i++) { // 0���� num_heads-1���� �ݺ�
		temp = (matrix_pointer) malloc(sizeof(matrix_node)); // �ӽ� ������ ����
		hdnode[i] = temp; hdnode[i]->tag = head; // hdnode ������ i��° �迭�� �ӽ� ��������� �����ϰ� ��� ǥ��
		hdnode[i]->right = temp; hdnode[i]->u.next=temp; // hdnode �����ʿ� temp ����, hdnode next�� temp ����, 
		// hdnode, hdnode->right hdnode->u.next �� ���� �����Ͱ� ��� ���� ������ ����Ŵ.
	}
	current_row = 0; last = hdnode[0]; // ���� ���� 0���� �ʱ�ȭ, ������ �����ʹ� hdnode 0��° �����Ͱ� ����Ű�� ���� ����Ŵ
	for (;row < num_rows && col < num_cols; col++) { // ��� �� ������ �� ��� ���ڸ� �Ѿ�� ���� ����(���� ��� ���� 0, 0�� ����Ŵ)
		fscanf(fp,"%d",&value); // ���� ��� ���� ��ǥ�� ����Ű�� �� �ϳ��� ����
		if(value != 0) { // �� ���� 0�� �ƴϸ�
			if (row > current_row) { // ���� ���α׷��� ����Ű�� �ຸ�� ��� �о�帰 ���� �� ��ǥ�� ū ���
				last->right = hdnode[current_row]; // last�� �����ʿ� ���α׷��� ����Ű�� ���� headnode�� ����
				current_row = row; last = hdnode[row]; // ����� ���� �� ��ǥ�� ���α׷��� ����Ű�� �� ��ǥ�� ��ġ��Ű�� last�� �� ���� headnode�� ����Ŵ
			}
			temp = (matrix_pointer) malloc(sizeof(matrix_node)); temp->tag = entry; // 0�� �ƴ� �� ���� �Է��ϱ� ���� ������ ���� ����
			temp->u.entry.row = row; temp->u.entry.col = col; // ��ǥ �Է�
			temp->u.entry.value = value; last->right = temp; last = temp; // �� �����ϰ�, last�� ����Ű�� hdnode[i]�� �����ʿ� �Էµ� ��带 �����ϰ� last�� ��� ������� ������ ����Ŵ
			hdnode[col]->u.next->down = temp; // ���� ���� hdnode �Ʒ��� ��� �Էµ� ������ ����Ŵ
			hdnode[col]->u.next = temp; // next�� ���� �� ����Ŵ
		}
		if(col == num_cols-1) { // �� ������ �� ��-1�� ��ġ�ϸ�
			row += 1; // �� 1 ����
			col = -1; // �� ���� -1 �ʱ�ȭ(for������ +1 ���ٰ��̱� ������)
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
	printf("(�� ��) ��\n");
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
		node->tag = entry; // �Է��� ������
		node->u.entry.row = node_a->u.entry.row; node->u.entry.col = node_a->u.entry.col;
		num_heads = (node->u.entry.col > node->u.entry.row) ? node->u.entry.col : node->u.entry.row; // ��� �� �� �� ���� num)heads�� ����
		if (!num_heads) node->right = node; // ��� �� �� ���� �� ���� 0�̸� ����� �����ʿ� ��� ����
		else { // 0�� �ƴϸ�
			for (i=0; i<num_heads; i++) { // 0���� num_heads-1���� �ݺ�
				temp = (matrix_pointer) malloc(sizeof(matrix_node)); // �ӽ� ������ ����
				hdnode[i] = temp; hdnode[i]->tag = head; // hdnode ������ i��° �迭�� �ӽ� ��������� �����ϰ� ��� ǥ��
				hdnode[i]->right = temp; hdnode[i]->u.next=temp; // hdnode �����ʿ� temp ����, hdnode next�� temp ����, 
			}
			current_row = 0; last = hdnode[0]; // ���� ���� 0���� �ʱ�ȭ, ������ �����ʹ� hdnode 0��° �����Ͱ� ����Ű�� ���� ����Ŵ
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
					if (row > current_row) { // ���� ���α׷��� ����Ű�� �ຸ�� ��� �о�帰 ���� �� ��ǥ�� ū ���
						last->right = hdnode[current_row]; // last�� �����ʿ� ���α׷��� ����Ű�� ���� headnode�� ����
						current_row = row; last = hdnode[row]; // ����� ���� �� ��ǥ�� ���α׷��� ����Ű�� �� ��ǥ�� ��ġ��Ű�� last�� �� ���� headnode�� ����Ŵ
					}
					temp = (matrix_pointer) malloc(sizeof(matrix_node)); temp->tag = entry; // 0�� �ƴ� �� ���� �Է��ϱ� ���� ������ ���� ����
					temp->u.entry.row = row; temp->u.entry.col = col; // ��ǥ �Է�
					temp->u.entry.value = value; last->right = temp; last = temp; // �� �����ϰ�, last�� ����Ű�� hdnode[i]�� �����ʿ� �Էµ� ��带 �����ϰ� last�� ��� ������� ������ ����Ŵ
					hdnode[col]->u.next->down = temp; // ���� ���� hdnode �Ʒ��� ��� �Էµ� ������ ����Ŵ
					hdnode[col]->u.next = temp; // next�� ���� �� ����Ŵ
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
	else printf("�־��� ����� ��� ���� ���� ���� �ʾ� ���ϱ� ������ �Ұ����մϴ�.\n");
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
		node->tag = entry; // �Է��� ������
		node->u.entry.row = node_a->u.entry.row; node->u.entry.col = node_b->u.entry.col;
		num_heads = (node->u.entry.col > node->u.entry.row) ? node->u.entry.col : node->u.entry.row; // ��� �� �� �� ���� num)heads�� ����
		if (!num_heads) node->right = node; // ��� �� �� ���� �� ���� 0�̸� ����� �����ʿ� ��� ����
		else { // 0�� �ƴϸ�
			for (i=0; i<num_heads; i++) { // 0���� num_heads-1���� �ݺ�
				temp = (matrix_pointer) malloc(sizeof(matrix_node)); // �ӽ� ������ ����
				hdnode[i] = temp; hdnode[i]->tag = head; // hdnode ������ i��° �迭�� �ӽ� ��������� �����ϰ� ��� ǥ��
				hdnode[i]->right = temp; hdnode[i]->u.next=temp; // hdnode �����ʿ� temp ����, hdnode next�� temp ����, 
			}
			current_row = 0; last = hdnode[0]; // ���� ���� 0���� �ʱ�ȭ, ������ �����ʹ� hdnode 0��° �����Ͱ� ����Ű�� ���� ����Ŵ
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
						if (row > current_row) { // ���� ���α׷��� ����Ű�� �ຸ�� ��� �о�帰 ���� �� ��ǥ�� ū ���
							last->right = hdnode[current_row]; // last�� �����ʿ� ���α׷��� ����Ű�� ���� headnode�� ����
							current_row = row; last = hdnode[row]; // ����� ���� �� ��ǥ�� ���α׷��� ����Ű�� �� ��ǥ�� ��ġ��Ű�� last�� �� ���� headnode�� ����Ŵ
						}
						temp = (matrix_pointer) malloc(sizeof(matrix_node)); temp->tag = entry; // 0�� �ƴ� �� ���� �Է��ϱ� ���� ������ ���� ����
						temp->u.entry.row = row; temp->u.entry.col = col; // ��ǥ �Է�
						temp->u.entry.value = value; last->right = temp; last = temp; // �� �����ϰ�, last�� ����Ű�� hdnode[i]�� �����ʿ� �Էµ� ��带 �����ϰ� last�� ��� ������� ������ ����Ŵ
						hdnode[col]->u.next->down = temp; // ���� ���� hdnode �Ʒ��� ��� �Էµ� ������ ����Ŵ
						hdnode[col]->u.next = temp; // next�� ���� �� ����Ŵ
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
	else printf("�־��� ����� ��� ���� ���� ���� �ʾ� ���ϱ� ������ �Ұ����մϴ�.\n");
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