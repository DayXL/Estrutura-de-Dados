#include <stdio.h>
#include <stdlib.h>

// Estrutura
struct tree_node {
   int value;
   struct tree_node *lchild;
   struct tree_node *rchild;
   unsigned int h;
   struct tree_node *parent;
};

struct tree_node** search(struct tree_node** root, int value) {
    if ((*root) != NULL) {
        if ((*root)->value == value) {
            return root;
        }

        else if ((*root)->value < value) {
            return search(&((*root)->lchild), value);
        }

        return search(&((*root)->rchild), value);
    }

    return NULL;
}

// Mostrar árvore em ordem
void show(struct tree_node* root) {
    if (root != NULL) {
        show(root->lchild);
        printf("%d ", root->value);
        show(root->rchild);
    }
}

// Calcular a altura do nó
unsigned int altura(struct tree_node* node) {
    if (node == NULL) {
        return 0;
    }
    
    unsigned int lheight = altura(node->lchild);
    unsigned int rheight = altura(node->rchild);
    
    if (lheight > rheight) {
        return lheight + 1;
    } 
    
    else {
        return rheight + 1;
    }
}


// Diferença entre as alturas dos filhos
int cdiff(struct tree_node* node) {
    if ((node->lchild != NULL )&& (node->rchild != NULL)) {
        return abs(node->lchild->h - node->rchild->h);
    } 

    else if ((node->lchild == NULL )&& (node->rchild != NULL)) {
        return node->rchild->h;
    } 

    else if ((node->lchild != NULL) && (node->rchild == NULL)) {
        return node->lchild->h;
    }

    return 0;
}

// Número do caso para balanceamento
int cas(struct tree_node* node) {
    if ((node != NULL) && (node->lchild != NULL) && (node->rchild != NULL)) {
        if ((node->lchild->h) > (node->rchild->h)) {
            if ((node->lchild->lchild != NULL) && (node->lchild->rchild != NULL)) {
                if ((node->lchild->lchild->h) > (node->lchild->rchild->h)) {
                    return 1;
                } 
                
                else {
                    return 3;
                }
            }
        } 
        
        else {
            if ((node->rchild->rchild != NULL) && (node->rchild->lchild != NULL)) {
                if ((node->rchild->rchild->h) > (node->rchild->lchild->h)) {
                    return 2;
                } 
                
                else {
                    return 4;
                }
            }
        }
    }

    return -1;
}

// Rotação à direita
void rd(struct tree_node* node) {
    if (node->rchild == NULL) {
        node->lchild->rchild = node;
    } else {
        struct tree_node* b = node->lchild->rchild;
        struct tree_node* y = node->lchild;
        node->lchild->rchild = node;
        y->rchild = b;
    }

    node->h = altura(node);
}

// Rotação à esquerda
void re(struct tree_node* node) {
    if (node->lchild == NULL) {
        node->rchild->lchild = node;
    } 
    
    else {
        struct tree_node* b = node->rchild->lchild;
        struct tree_node* y = node->rchild;
        node->rchild->lchild = node;
        y->lchild = b;
    }

    node->h = altura(node);
}

// Balanceamento
void balance(struct tree_node* node) {
    node = node->parent;
    
    while (node != NULL) {
        if (cdiff(node) > 1) {
            int c = cas(node);

            if (c == 1) {
                rd(node);
            } 
            else if (c == 2) {
                re(node);
            } 
            else if (c == 3) {
                re(node->lchild);
                rd(node);
            } 
            else {
                rd(node->rchild);
                re(node);
            }
        }

        node->h = altura(node);
        node = node->parent;
    }
}

// Inserir na árvore
void insert(struct tree_node** root, struct tree_node* node) {
    if (*root == NULL) {
        *root = node;
        (*root)->h = 1;
    } 
    
    else {
        node->parent = *root;

        if ((*root)->value < node->value) {
            insert(&((*root)->rchild), node);
        } 
        
        else {
            insert(&((*root)->lchild), node);
        }

        (*root)->h = altura(*root);
        balance(*root);
    }
}

// Criar novo nó
struct tree_node* create_node(int value) {
    struct tree_node* node = (struct tree_node*)malloc(sizeof(struct tree_node));
    node->value = value;
    node->lchild = NULL;
    node->rchild = NULL;
    node->h = 1;
    node->parent = NULL;
    
    return node;
}

int main(void) {
    struct tree_node* root = NULL;

    insert(&root, create_node(5));
    insert(&root, create_node(2));
    insert(&root, create_node(9));

    printf("Valores da árvore: ");
    show(root);
    printf("\n");

    struct tree_node** achar = search(&root, 5);

    if (achar != NULL) {
        printf("Achou.\n");
    }

    else {
        printf("Não achou.\n");
    }
    
    return 0;
}



// void tree_print_dot_body(struct tree_node *r)
// {
//     if (r != NULL) {
//         tree_print_dot_body(r->lchild);
//         printf("  \"%p\" [label=\"{%p|%d|{%p|%p}}\"];\n",
//         (void *) r,
// 	    r,
// 	    r->value,
// 	    r->lchild,
// 	    r->rchild);

//         if (r->lchild) {
//             printf("  \"%p\" -> \"%p\";\n", r, r->lchild);
//         }

//         if (r->rchild){
//             printf("  \"%p\" -> \"%p\";\n", r, r->rchild);
//         }

//         tree_print_dot_body(r->rchild);
//     }
// }