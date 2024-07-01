#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max 100

struct node {
    char* key;
    struct node* left;
    struct node* right;
};
typedef struct node node;

node* newNode(char* value) {
    node* n = (node*)malloc(sizeof(node));
    n->key = (char*)malloc(strlen(value) + 1);
    strcpy(n->key, value);
    n->left = NULL;
    n->right = NULL;
    return n;
}

typedef struct {
    node* root;
} tree;

tree* constructTree() {
    tree* t = (tree*)malloc(sizeof(tree));
    t->root = NULL;
    return t;
}

node* insert(node* root, char* value) {
    if (!root) {
        return newNode(value);
    }
    if (strcasecmp(value, root->key) < 0) {
        root->left = insert(root->left, value);
    } else if (strcasecmp(value, root->key) > 0) {
        root->right = insert(root->right, value);
    }
    return root;
}

void inorderTraversal(node* root) {
    if (root) {
        inorderTraversal(root->left);
        printf("%s ", root->key);
        inorderTraversal(root->right);
    }
}


void loadWordsFromFile(tree* t, const char* filename) {
    FILE* file = fopen("Dictionary.txt", "r");
    if (!file) {
        printf("Failed to open file");
        return;
    }
    else{
        printf("the file has been opened successfully\n");
    }

    char word[Max];
    int counter = 0;

    while (fgets(word, Max, file)) {

        word[strcspn(word, "\n")] = 0;    // Remove newline character from the word
        t->root = insert(t->root, word);
        counter++;
    }

    fclose(file);

    //printf("Total number of words inserted: %d\n", counter);
}
int countNodes(node* root) {
    if (!root) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}
int height(node* root) {
    if (!root) {
        return 0;
    }
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    if(leftHeight > rightHeight) return 1+leftHeight;
    else {
        return 1+rightHeight;
    }
    //return 1 + (leftHeight > rightHeight ? l0eftHeight : rightHeight);
}

node* search(node* root, char* value, node** lastVisited) {
    *lastVisited = NULL;
    node* current = root;
    while (current) {
        *lastVisited = current;
        int cmp = strcasecmp(value, current->key);
        if (cmp == 0) {
            return current; // Word found
        } else if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL; // Word not found
}

node* findMin(node* root) {
    while (root && root->left) {
        root = root->left;
    }
    return root;
}

node* findMax(node* root) {
    while (root && root->right) {
        root = root->right;
    }
    return root;
}

node* findSuccessor(node* root, char* value) {
    node* current = root;
    node* successor = NULL;
    while (current) {
        int cmp = strcasecmp(value, current->key);
        if (cmp < 0) {
            successor = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return successor;
}

node* findPredecessor(node* root, char* value) {
    node* current = root;
    node* predecessor = NULL;
    while (current) {
        int cmp = strcasecmp(value, current->key);
        if (cmp > 0) {
            predecessor = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return predecessor;
}

void processInput(tree* t) {
    char input[256];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline character

    char* word = strtok(input, " ");
    while (word) {
        node* lastVisited = NULL;
        node* result = search(t->root, word, &lastVisited);

        if (result) {
            printf("'%s' is correct.\n", word);
        } else {
            printf("'%s' is wrong.\n", word);

            // Suggestions
            if (lastVisited){
                printf("Last visited node: %s\n", lastVisited->key);
            }
            else{
                printf("None");
            }

            node* successor = findSuccessor(t->root, lastVisited->key);
            printf("Successor: %s\n", successor ? successor->key : "None");

            node* predecessor = findPredecessor(t->root, lastVisited->key);
            printf("Predecessor: %s\n", predecessor ? predecessor->key : "None");
        }

        word = strtok(NULL, " ");
    }
}

int main() {
    tree* t = constructTree();
    loadWordsFromFile(t, "words.txt");


    /*printf("Inorder Traversal:\n");
    inorderTraversal(t->root);
    printf("\n");*/

    int totalNodes = countNodes(t->root);
    int Height=height(t->root);
    printf("Total number of nodes in the tree: %d\n", totalNodes);
    printf("height of the tree: %d\n", Height);
    processInput(t);

    return 0;
}
