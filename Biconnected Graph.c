#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define MAX_VERTICES 30
#define MIN2(x,y) ((x) < (y) ? (x) : (y))
typedef struct node *node_pointer;
typedef struct node {
	int vertex;
	int end_vertex; 
	node_pointer link;
}node;
node_pointer top;
node_pointer graph[MAX_VERTICES];
int n;
short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];
int num;
void add_graph(node_pointer graph, int v);
void add(node_pointer top, int u, int v);
void delete(node_pointer top, int*x, int* y);
void init();
void bicon(int u, int v);

void add_graph(node_pointer graph, int v)
{
	node_pointer ptr;
	ptr = (node_pointer) malloc(sizeof(node)); 
	ptr->link = NULL; 
	ptr->vertex = v; 
	while (1) {
		if (graph == NULL || graph->link == NULL) break;
			graph = graph->link;
	}
	graph->link = ptr;
}

void add(node_pointer top, int u, int v) {
	node_pointer pt1, add1;
	pt1 = top;
	add1 = (node_pointer)malloc(sizeof(node)); 
	add1->link = NULL; add1->vertex = u; add1->end_vertex = v;
	if (pt1->link != NULL) {
		add1->link = pt1->link;
	}	
	pt1->link = add1;
}

void delete(node_pointer top, int*x, int* y) {
	node_pointer pt1;
	pt1 = top; 
	if (pt1->link != NULL) {
		*x = pt1->link->vertex; 
		*y = pt1->link->end_vertex; 
		pt1->link = pt1->link->link;
	}
}

void init(void){
int i;
for(i=0; i<n; i++){
	dfn[i] =low[i] = -1;
}
num =0;
}

void bicon(int u, int v) {
	node_pointer ptr; 
	int w, x, y;
	dfn[u] = low[u] = num++; 
	for (ptr = graph[u]; ptr; ptr = ptr->link) {
		w = ptr->vertex;
		if (dfn[w] < 0) {
			bicon(w, u); 
			if(low[w] > low[u]) add(&top, u, w);
			low[u] = MIN2(low[u], low[w]);
			if (low[w] > low[u]) {
				printf(" Bridge: "); 
					delete(&top, &x, &y); 
					printf(" < %d, %d >", x, y); 
				printf("\n");
			}

		}
		else if (w != v){
			low[u] = MIN2(low[u], dfn[w]);
}
	}
}

void main()
{
int i, item;
char space;
FILE * fp = fopen("input.txt", "rt");
fscanf(fp, "%d", &n);
for(i=0; i<n; i++){
		graph[i] = (node_pointer)malloc(sizeof(node)); 
		graph[i]->link = NULL;
		fscanf(fp, "%d%c", &graph[i]->vertex, &space);
	while(1) {
		fscanf(fp, "%d%c", &item, &space);
		add_graph(graph[i], item);
		if(space == '\n' || feof(fp)) break;
}
}
// for(i=0; i<n; i++) show_graph(graph[i]);
  init();
  bicon(3, -1);
  fclose(fp);
  system("pause");
}
