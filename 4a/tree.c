#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char * key;
    unsigned int value;
    struct node * left;
    struct node * right;
    struct node * parent;
} node;

node * newNode(char * key, unsigned int value) {
    node * new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->key = malloc(strlen(key) + 1);
    strcpy(new_node->key, key);
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    return new_node;
}

void freeTree(node * root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->key);
        free(root);
    }
}

node * search(node * root, char * key) {
    if (root == NULL || strcmp(root->key, key) == 0) {
        return root;
    }
    if (strcmp(root->key, key) < 0) {
        return search(root->right, key);
    }
    else {
        return search(root->left, key);
    }
}

node * insert(node * root, char * key, unsigned int value) {
    node * new_node = newNode(key, value);
    if (new_node == NULL) {
        return root;
    }
    if (root == NULL) {
        root = new_node;
        return root;
    }
    node * parent = NULL;
    node * current = root;
    while (current != NULL) {
        parent = current;
        if (strcmp(current->key, key) == 0) {
            free(new_node->key);
            free(new_node);
            return NULL;
        }
        else if (strcmp(current->key, key) < 0) {
            current = current->right;
        }
        else {
            current = current->left;
        }
    }
    new_node->parent = parent;
    if (strcmp(parent->key, key) < 0) {
        parent->right = new_node;
    }
    else {
        parent->left = new_node;
    }
    return root;
}

node * deleteNode(node * root, char * key) {
    node * node_to_delete = search(root, key);
    if (node_to_delete == NULL) {
        return root;
    }
    if (node_to_delete->left == NULL) {
        if (node_to_delete->right == NULL) {
            if (node_to_delete == root) {
                free(node_to_delete->key);
                free(node_to_delete);
                return NULL;
            }
            else if (node_to_delete->parent->left == node_to_delete) {
                node_to_delete->parent->left = NULL;
            }
            else {
                node_to_delete->parent->right = NULL;
            }
            free(node_to_delete->key);
            free(node_to_delete);
            return root;
        }
        else if (node_to_delete == root) {
            node_to_delete->right->parent = NULL;
            free(node_to_delete->key);
            free(node_to_delete);
            return node_to_delete->right;
        }
        else if (node_to_delete->parent->left == node_to_delete) {
            node_to_delete->right->parent = node_to_delete->parent;
            node_to_delete->parent->left = node_to_delete->right;
            free(node_to_delete->key);
            free(node_to_delete);
            return root;
        }
        else {
            node_to_delete->right->parent = node_to_delete->parent;
            node_to_delete->parent->right = node_to_delete->right;
            free(node_to_delete->key);
            free(node_to_delete);
            return root;
        }
    }
    else if (node_to_delete->right == NULL) {
        if (node_to_delete == root) {
            node_to_delete->left->parent = NULL;
            free(node_to_delete->key);
            free(node_to_delete);
            return node_to_delete->left;
        }
        else if (node_to_delete->parent->left == node_to_delete) {
            node_to_delete->left->parent = node_to_delete->parent;
            node_to_delete->parent->left = node_to_delete->left;
            free(node_to_delete->key);
            free(node_to_delete);
            return root;
        }
        else {
            node_to_delete->left->parent = node_to_delete->parent;
            node_to_delete->parent->right = node_to_delete->left;
            free(node_to_delete->key);
            free(node_to_delete);
            return root;
        }
    }
    else {
        node * successor = node_to_delete->right;
        while (successor->left != NULL) {
            successor = successor->left;
        }
        node_to_delete->value = successor->value;
        free(node_to_delete->key);
        node_to_delete->key = malloc(strlen(successor->key) + 1);
        strcpy(node_to_delete->key, successor->key);
        root = deleteNode(root, successor->key);
        return root;
    }
}

void printTree(node * root) {
    static int level = 0;
    if (root == NULL) {
        return;
    }

    level++;
    printTree(root->right);

    for (int i = 0; i < level-1; i++) {
        printf("     ");
    }

    printf(" └───%s\n", root->key);

    printTree(root->left);
    level--;
}


node * findMax(node * root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}


int main() {
    node * root = NULL;
    char command[10], key[50];
    unsigned int value;
    while (1) {
        printf("Enter a command (search, insert, delete, print, max, quit): ");
        scanf("%s", command);
        if (strcmp(command, "search") == 0) {
            printf("Enter a key: ");
            scanf("%s", key);
            node * result = search(root, key);
            if (result == NULL) {
                printf("Key not found\n");
            } else {
                printf("Key found: %s, value: %d\n", result->key, result->value);
            }
        } else if (strcmp(command, "insert") == 0) {
            printf("Enter a key: ");
            scanf("%s", key);
            if(search(root, key))
            	printf("Dublicate key\n");
            else {
            printf("Enter a value: ");
            scanf("%u", &value);
            root = insert(root, key, value);
            printf("Key inserted\n");
            }
        } else if (strcmp(command, "delete") == 0) {
            printf("Enter a key: ");
            scanf("%s", key);
            root = deleteNode(root, key);
            printf("Key deleted\n");
        } else if (strcmp(command, "print") == 0) {
            printTree(root);
            printf("\n");
        } else if (strcmp(command, "max") == 0) {
            node * maxNode = findMax(root);
            if (maxNode == NULL) {
                printf("The tree is empty\n");
            } else {
                printf("Max node: %s, value: %d\n", maxNode->key, maxNode->value);
            }
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Invalid command\n");
        }
    }
    return 0;
}
