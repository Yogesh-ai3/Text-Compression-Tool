#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

// ---------- Node Structure ----------
typedef struct Node {
    char ch;
    int freq;
    struct Node *left, *right;
} Node;

// ---------- Priority Queue ----------
typedef struct {
    Node* arr[MAX];
    int size;
} PriorityQueue;

// ---------- Create Node ----------
Node* createNode(char ch, int freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// ---------- Initialize PQ ----------
void initPQ(PriorityQueue* pq) {
    pq->size = 0;
}

// ---------- Insert into PQ ----------
void insertPQ(PriorityQueue* pq, Node* node) {
    int i = pq->size++;
    
    // Insert and keep sorted (smallest freq first)
    while (i > 0 && pq->arr[i - 1]->freq > node->freq) {
        pq->arr[i] = pq->arr[i - 1];
        i--;
    }
    pq->arr[i] = node;
}

// ---------- Extract Min ----------
Node* extractMin(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;
    
    Node* min = pq->arr[0];
    
    for (int i = 1; i < pq->size; i++) {
        pq->arr[i - 1] = pq->arr[i];
    }
    pq->size--;
    
    return min;
}

// ---------- Build Huffman Tree ----------
Node* buildHuffman(PriorityQueue* pq) {
    while (pq->size > 1) {
        Node* left = extractMin(pq);
        Node* right = extractMin(pq);

        Node* newNode = createNode('$', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        insertPQ(pq, newNode);
    }
    return extractMin(pq);
}

// ---------- Generate Codes ----------
void generateCodes(Node* root, char* code, int depth) {
    if (!root) return;

    // Leaf node
    if (!root->left && !root->right) {
        code[depth] = '\0';
        printf("%c -> %s\n", root->ch, code);
        return;
    }

    // Left = 0
    code[depth] = '0';
    generateCodes(root->left, code, depth + 1);

    // Right = 1
    code[depth] = '1';
    generateCodes(root->right, code, depth + 1);
}

// ---------- Free Tree ----------
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// ---------- MAIN ----------
int main() {
    char text[1000];

    printf("Enter text: ");
    scanf("%s", text);

    int freq[MAX] = {0};

    // Step 1: Frequency Count
    for (int i = 0; text[i] != '\0'; i++) {
        freq[(unsigned char)text[i]]++;
    }

    printf("\nCharacter Frequencies:\n");
    for (int i = 0; i < MAX; i++) {
        if (freq[i] > 0) {
            printf("%c -> %d\n", i, freq[i]);
        }
    }

    // Step 2: Build Priority Queue
    PriorityQueue pq;
    initPQ(&pq);

    for (int i = 0; i < MAX; i++) {
        if (freq[i] > 0) {
            insertPQ(&pq, createNode(i, freq[i]));
        }
    }

    // Step 3: Build Huffman Tree
    Node* root = buildHuffman(&pq);

    // Step 4: Generate Codes
    printf("\nHuffman Codes:\n");
    char code[100];
    generateCodes(root, code, 0);

    // Free memory
    freeTree(root);

    return 0;
}