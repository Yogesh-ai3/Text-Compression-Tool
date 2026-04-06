#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

typedef struct Node {
    char ch;
    int freq;
    struct Node *left, *right;
} Node;


typedef struct {
    Node* arr[MAX];
    int size;
} PriorityQueue;


Node* createNode(char ch, int freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}


void initPQ(PriorityQueue* pq) {
    pq->size = 0;
}


void insertPQ(PriorityQueue* pq, Node* node) {
    int i = pq->size++;
    
    
    while (i > 0 && pq->arr[i - 1]->freq > node->freq) {
        pq->arr[i] = pq->arr[i - 1];
        i--;
    }
    pq->arr[i] = node;
}


Node* extractMin(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;
    
    Node* min = pq->arr[0];
    
    for (int i = 1; i < pq->size; i++) {
        pq->arr[i - 1] = pq->arr[i];
    }
    pq->size--;
    
    return min;
}


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


void generateCodes(Node* root, char* code, int depth) {
    if (!root) return;

    
    if (!root->left && !root->right) {
        code[depth] = '\0';
        printf("%c -> %s\n", root->ch, code);
        return;
    }

    
    code[depth] = '0';
    generateCodes(root->left, code, depth + 1);

    
    code[depth] = '1';
    generateCodes(root->right, code, depth + 1);
}


void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}


int main() {
    char text[1000];

    printf("Enter text: ");
    scanf("%s", text);

    int freq[MAX] = {0};

    
    for (int i = 0; text[i] != '\0'; i++) {
        freq[(unsigned char)text[i]]++;
    }

    printf("\nCharacter Frequencies:\n");
    for (int i = 0; i < MAX; i++) {
        if (freq[i] > 0) {
            printf("%c -> %d\n", i, freq[i]);
        }
    }

    
    PriorityQueue pq;
    initPQ(&pq);

    for (int i = 0; i < MAX; i++) {
        if (freq[i] > 0) {
            insertPQ(&pq, createNode(i, freq[i]));
        }
    }

    
    Node* root = buildHuffman(&pq);

    
    printf("\nHuffman Codes:\n");
    char code[100];
    generateCodes(root, code, 0);

    
    freeTree(root);

    return 0;
}