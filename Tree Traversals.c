// 트리 자료구조 순회
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define MAX_TERMS 100
typedef struct node *tree_pointer;
typedef struct node {
	tree_pointer left_child;
	int data;
	tree_pointer right_child;
} tree_node;

void inorder(tree_pointer ptr, int k1, int k2);
void insert_node(tree_pointer *node, int num);
tree_pointer modified_search(tree_pointer root, int key);

void inorder(tree_pointer ptr, int k1, int k2)
{
	if (ptr) {
		inorder(ptr->left_child, k1, k2);
		if(ptr->data >= k1 && ptr->data <= k2)
			printf("%d ", ptr->data);
		inorder(ptr->right_child, k1, k2);
	}
}



tree_pointer modified_search(tree_pointer tree, int key) {
	if (tree == NULL) return NULL;
	while (tree) {
		if (key == tree->data)return NULL;
		else if (key<tree->data) {
			if (tree->left_child == NULL)return tree;
			else tree = tree->left_child;
		}
		else {
			if (tree->right_child == NULL)return tree;
			else tree = tree->right_child;
		}
	}
	return NULL;
}

void insert_node(tree_pointer *node, int num) {
	/* If num is in the tree pointed at by node do nothing;
	otherwise add a new node with data = num */
	tree_pointer ptr;
	tree_pointer temp = modified_search(*node, num);
	if (temp || !(*node)) {
		/*num is not in the tree */
		ptr = (tree_pointer)malloc(sizeof(tree_node));
		// 	if(IS_FULL(ptr)){		fprintf(stderr, “The memory is full“);
		// 		exit(1);

		ptr->data = num;
		ptr->left_child = ptr->right_child = NULL;
		if (*node) /*Insert as child of temp */
			if (num < temp->data) temp->left_child = ptr;
			else temp->right_child = ptr;
		else *node = ptr;
	}
}

void main()
{
	int i, n, A[MAX_TERMS], k1, k2;
	tree_pointer tree = NULL;
	scanf("%d", &n);
	for(i=0; i<n; i++)
		scanf("%d", &A[i]);
	scanf("%d%d", &k1, &k2);
	for(i=0; i<n; i++)
		insert_node(&tree, A[i]);
	inorder(tree, k1, k2);
	printf("\n");
	system("pause");
}