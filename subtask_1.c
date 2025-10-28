#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct Node
    {
        int value; 
        struct Node *left;
        struct Node *right;
    } Node;
typedef struct queue_node 
    {
        Node *node;
        struct queue_node *next;
    } queue_node;
typedef struct 
    {
        queue_node *front;
        queue_node *rear;
    } queue;
queue* create_queue() 
    {
        queue *q = (queue*)malloc(sizeof(queue));
        q->front = q->rear = NULL;
        return q;
    }
bool is_empty(queue *q) 
    {
        return q->front == NULL;
    }
void enqueue(queue *q, Node *node) 
    {
        queue_node *temp = (queue_node*)malloc(sizeof(queue_node));
        temp->node = node;
        temp->next = NULL;
        if (q->rear == NULL) 
            {
                q->front = q->rear = temp;
                return;
            }
        q->rear->next = temp;
        q->rear = temp;
    }
Node* dequeue(queue *q) 
    {
        if (is_empty(q))
            return NULL;
        queue_node *temp = q->front;
        Node *node = temp->node;
        q->front = q->front->next;
        if (q->front == NULL)
            q->rear = NULL;
        free(temp);
        return node;
    }
Node* create_node(int val) 
    {
        if (val == 0) return NULL;
        Node *node = (Node*)malloc(sizeof(Node));
        node->value = val;
        node->left = node->right = NULL;
        return node;
    }
Node* build_tree(const int level_order[], int n) 
    {
        if (n == 0 || level_order[0] == 0) 
            return NULL;
        Node *root = create_node(level_order[0]);
        queue *q = create_queue();
        enqueue(q, root);
        int i = 1;
        while (i<n&&!is_empty(q)) 
            {
                Node *curr = dequeue(q);
                if (i<n) 
                    {
                        curr->left = create_node(level_order[i]);
                        if (curr->left) enqueue(q, curr->left);
                        i++;
                    }
                if (i<n) 
                    {
                        curr->right = create_node(level_order[i]);
                        if (curr->right) enqueue(q, curr->right);
                        i++;
                    }
            }
        free(q);
        return root;
    }
int min_phn = 0; //minimun no of phn
int solve_min_phn(Node *node) // finds the minimum phn needed using bottom-up recursion (DFS)
    {                                       
        if (node == NULL) 
            return 1;
        int left_state = solve_min_phn(node->left);
        int right_state = solve_min_phn(node->right);
        if (left_state == 0 || right_state == 0) 
            {
                min_phn++;
                return 2;
            }
        if (left_state == 2 || right_state == 2) 
            {
                return 1;
            }
        return 0;
    }
int main() 
    {
        int n;
        printf("Enter n: ");
        if (scanf("%d", &n) != 1) 
            return 0;
        int *level_order = (int*)malloc(n * sizeof(int));
        if (level_order == NULL) 
            return 0;
        printf("Enter level_order: ");
        for (int i = 0; i < n; i++) 
            {
                if (scanf("%d", &level_order[i]) != 1) 
                    {
                        free(level_order);
                        return 0;
                    }
            }
        Node *root = build_tree(level_order, n);
        min_phn = 0;
        int final_state = solve_min_phn(root);
        if (final_state == 0 && root != NULL) 
            {
                min_phn++;
            }
        int m = min_phn;
        printf("%d\n", m);
        free(level_order);
        return 0;
    }
