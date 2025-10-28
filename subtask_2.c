#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define max_prime_check 3000000
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
        if (n == 0 || level_order[0] == 0) return NULL;

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
int solve_min_phn(Node *node) 
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
// prime & xor analysis
int primes[max_prime_check / 10];
int prime_count = 0;
void find_primes() 
    {
        bool is_prime[max_prime_check + 1];
        memset(is_prime, true, sizeof(is_prime));
        is_prime[0] = is_prime[1] = false;
        for (int p = 2; p * p <= max_prime_check; p++) 
            {
                if (is_prime[p]) 
                    {
                        for (int i = p * p; i <= max_prime_check; i += p)
                            is_prime[i] = false;
                    }
            }
        for (int p = 2; p <= max_prime_check; p++) 
            {
                if (is_prime[p]) 
                    {
                        primes[prime_count++] = p;
                    }
            }
    }
// counts pairs whose xor result is outside the [l, r] range
int count_xor_outside_range(int m, int l, int r) 
    {
        if (m <= 1) return 0;
        int total_pairs = m * (m - 1) / 2;
        int pairs_in_range = 0;
        for (int i = 0; i < m; i++) 
            {
                for (int j = i + 1; j < m; j++) 
                    {
                        int cacti_measure = primes[i] ^ primes[j];
                        if (cacti_measure >= l && cacti_measure <= r) 
                            {
                                pairs_in_range++;
                            }
                    }
            }
        return total_pairs - pairs_in_range;
    }
int main() 
    {
        find_primes(); 
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
        int l = -1, r = -1;
        printf("Enter l and r: ");
        if (scanf("%d %d", &l, &r) != 2) 
            {
                l = -1;
                r = -1;
            }
        Node *root = build_tree(level_order, n);
        min_phn = 0;
        int final_state = solve_min_phn(root);
        if (final_state == 0 && root != NULL) 
            {
                min_phn++;
            }
        int m = min_phn;
        printf("min no of phn required= %d\n", m);
        if (l != -1 && r != -1) 
            {
                int x = count_xor_outside_range(m, l, r);
                printf("no of pairs outside the range= %d\n", x);
            }
        free(level_order);
        return 0;
    }
