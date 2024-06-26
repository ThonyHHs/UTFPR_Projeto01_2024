#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Estrutura da árvore binária para passageiros
typedef struct tree {
    char nome[30];
    struct tree *left;
    struct tree *right;
}Passageiros;

// Estrutura do nó da fila de voos
typedef struct node {
    struct node *prox;
    char Id[8];
    char Destino[4];
    char Empresa[20];
    char Modelo[20];
    int Np;
    Passageiros *lp;
}Node;

// Estrutura da fila de voos
typedef struct queue {
    Node *front;
    Node *rear;
}Queue;

// -=-=-=-=-=-=-=-=-=-=-= FUNÇÕES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void flushBuffer() {
    while (getchar() != '\n');
}

// -=-=-=-=-=-=-=-=-=-=-= FILA =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// inicializa a fila
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL){
        printf("Falha ao alocar memoria!\n");
        exit(1);
    }
    q->front = q->rear = NULL;
    return q;
}

// cria um novo nó na fila de voos
Node *newNode(char *Id, char *Destino, char *Empresa, char *Modelo, int Np) {
    Node *temp = (Node*)malloc(sizeof(Node));
    if (temp==NULL) {
        printf("Falha ao alocar memoria!\n");
        exit(1);
    }
    
    strcpy(temp->Id, Id);
    strcpy(temp->Destino, Destino);
    strcpy(temp->Empresa, Empresa);
    strcpy(temp->Modelo, Modelo);
    temp->Np = Np;
    temp->lp = NULL;
    temp->prox = NULL;
    return temp;
}

// adiciona um voo na fila
Queue *enQueue(Queue *q, Node *n) {
    if (q->front == NULL) {
        q->front = q->rear = n;
        q->front->prox = q->rear->prox = NULL;
    } else {
        q->rear->prox = n;
        q->rear = n;
        q->rear->prox = NULL;
    }
    return q;
}

// Remove um voo da lista
Queue *deQueue(Queue *q) {
    Node *aux = q->front;
    if (q->front == NULL) {
        printf("Fila de voos vazia!\n");
    } else {
        q->front = q->front->prox;
        printf("Voo %s liberado para decolagem\n", aux->Id);
        free(aux);
    }
    return q;
}

// Retorna o tamanho da fila
int queueSize(Queue *q) {
    int size = 0;
    Node *aux = q->front;
    if (q->front == NULL)
        return 0;
    
    while (aux!=NULL) {
        size++;
        aux = aux->prox;
    }
    return size;
}

void listaVoo(Node *n){
    printf("%-10s  %-10s  %-10s  %-20s  %-10d\n", n->Id, n->Destino, n->Empresa, n->Modelo, n->Np);
}
void percorre_fila(Node *n) {
    Node *aux = n;
    while (aux != NULL) {
        listaVoo(aux);
        aux = aux->prox;
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-= ARVORE =-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Cria uma novo nó na árvore (passageiro)
Passageiros *newTreeNode(char *nome) {
    Passageiros *temp = (Passageiros*)malloc(sizeof(Passageiros));
    if (temp == NULL) {
        printf("Falha ao alocar memoria!\n");
        exit(1);
    }
    
    strcpy(temp->nome, nome);
    temp->left = temp->right = NULL;
    return temp;
}

// Adiciona um passageiro na árvore binária
Passageiros *insertTree(Passageiros *root, Passageiros *r, char *nome) {
    if (r == NULL) {
        r = newTreeNode(nome);
        if (root == NULL) {
            return r;
        }
        if (strcmp(nome, root->nome) < 0) {
            root->left = r;
        } else root->right = r;
        return r;        
    }
    if (strcmp(nome, r->nome) < 0) 
        insertTree(r, r->left, nome);
    else insertTree(r,r->right, nome);
}

// Remove um passageiro da árvore
Passageiros *removeTree(Passageiros *root, char *nome) {
    Passageiros *p, *p2;
    if (strcmp(root->nome, nome) == 0) { // apagar a raiz
        // árvore vazia
        if (root->left == root->right) {
            free(root);
            return NULL;
        }
        // subárvore nula
        else if (root->left == NULL) {
            p = root->right;
            free(root);
            return p;
        }
        else if (root->right == NULL) {
            p = root->left;
            free(root);
            return p;
        }
        // subárvore não nula
        else {
            p2 = root->right;
            p = root->right;
            while (p->left) {
                p = p->left;
            }
            p->left = root->left;
            free(root);
            return p2;
        }
    }
    if (strcmp(nome, root->nome) > 0)
        root->right = removeTree(root->right, nome);
    else root->left = removeTree(root->left, nome);
    return root;
}

// Lista os passageiros em ordem alfabética (inorder traversal)
void inorder(Passageiros *root, FILE *file) {
    if (root != NULL) {
        inorder(root->left, file);
        printf("%s\n", root->nome);
        fprintf(file, "%s\n", root->nome);
        inorder(root->right, file);
    }
}

int numPass(Passageiros *root) {
    if (root == NULL)
        return 0;
    return numPass(root->left) + numPass(root->right) + 1;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void cadastrarPassageiro(Node *n) {
    char nome[30];
    int num;
    if (n == NULL)
        return;

    if (numPass(n->lp) == n->Np) {
        printf("Voo cheio\n");
        return;
    } else printf("Acentos livre: %d/%d\n", n->Np-numPass(n->lp), n->Np);
    
    printf("Insira o numero de passageiros a cadastrar: ");
    scanf("%d", &num);
    flushBuffer();
    if (num+numPass(n->lp) > n->Np) {
        printf("impossivel adicionar esse numero de passageiros\n");
        return;
    }

    for (int i = 0; i < num; i++) {
        printf("Informe o nome: ");
        scanf("%[^\n]s", nome);
        flushBuffer();
        if (n->lp == NULL)
            n->lp = insertTree(n->lp, n->lp, nome);
        else insertTree(n->lp, n->lp, nome);
    }
}

void removerPassageiro(Node *n) {
    char nome[30];
    if (n == NULL)
        return;
    if (n->lp == NULL) {
        printf("Voo vazio!\n");
        return;
    }
    
    printf("Informe o nome: ");
    scanf("%[^\n]s", nome);
    flushBuffer();
    removeTree(n->lp, nome);
}

Node *buscarVoo(Queue *q, char *Id) {
    Node *temp = q->front;
    if (q == NULL) {
        printf("Fila de voos vazia!\n");
        return NULL;
    }
    while (temp != NULL) {
        if (strcmp(temp->Id, Id) == 0) {
            return temp;
        }
        temp = temp->prox;
    }
    printf("Voo nao encontrado!\n");
    return NULL;
}

void listaPassageiros(Node *n) {
    if (n == NULL) 
        return;
    if (n->lp == NULL) {
        printf("Voo vazio!\n");
        return;
    }
    
    printf("%-10s  %-10s  %-10s  %-20s  %-10s\n", "ID do voo", "Destino", "Empresa", "Modelo", "Nº Passageiros");
    printf("%-10s  %-10s  %-10s  %-20s  %-10d\n", n->Id, n->Destino, n->Empresa, n->Modelo, n->Np);
    printf("Passageiros: %d/%d\n", numPass(n->lp), n->Np);

    // imprime passageiros no arquivo
    FILE *file = fopen("passageiros.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    fprintf(file, "%-10s  %-10s  %-10s  %-20s  %-10s\n", "ID do voo", "Destino", "Empresa", "Modelo", "Nº Passageiros");
    fprintf(file, "%-10s  %-10s  %-10s  %-20s  %-10d\n", n->Id, n->Destino, n->Empresa, n->Modelo, n->Np);
    fprintf(file,"Passageiros: %d/%d\n", numPass(n->lp), n->Np);
    inorder(n->lp, file);
    fclose(file);
}

Passageiros *liberaArvore(Passageiros *root) {
    if (root != NULL) {
        liberaArvore(root->left);
        liberaArvore(root->right);
        free(root);
    }
    return NULL;
}

Queue *liberaFila(Queue* q) {
    Node *temp = q->front;
    while (temp != NULL) {
        Node *aux = temp->prox;
        temp->lp = liberaArvore(temp->lp);
        free(temp);
        temp = aux;
    }
    free(q);
    return q;
}

int main() {
    Queue *voos = createQueue();
    int opcao;
    char Id[8], Destino[4], Empresa[20], Modelo[20];
    int Np;

    do {
        printf("1. Cadastrar voo\n");
        printf("2. Cadastrar nomes\n");
        printf("3. Remover nomes\n");
        printf("4. Caracteristicas do primeiro voo da fila\n");
        printf("5. Autorizar decolagem\n");
        printf("6. Tamanho da fila de decolagem\n");
        printf("7. Listar voos\n");
        printf("8. Listar passageiros\n");
        printf("9. Sair\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        flushBuffer();
        printf("\n");
        switch (opcao) {
            case 1:
                // cadastrar voo
                printf("-=-=-=-=-=-= CADASTRAR VOO =-=-=-=-=-=-\n");   
                printf("Digite o ID do voo: ");
                scanf("%s", Id);
                flushBuffer();
                printf("Digite o destino do voo [IATA]: ");
                scanf("%s", Destino);
                flushBuffer();
                printf("Digite a empresa do voo: ");
                scanf("%s", Empresa);
                flushBuffer();
                printf("Digite o modelo da aeronave: ");
                scanf("%[^\n]s", Modelo);
                flushBuffer();
                printf("Digite o numero maximo de passageiros: ");
                scanf("%d", &Np);
                flushBuffer();
                voos = enQueue(voos, newNode(strupr(Id), strupr(Destino), strupr(Empresa), strupr(Modelo), Np));
                printf("        -=-=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 2:
                // cadastrar nomes
                printf("-=-=-=-=-=-= CADASTRAR NOMES =-=-=-=-=-=-\n");
                printf("Informe o ID do voo: ");
                scanf("%s", Id);
                flushBuffer();
                cadastrarPassageiro(buscarVoo(voos, strupr(Id)));
                printf("         -=-=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 3:
                // remover nomes
                printf("-=-=-=-=-=-= REMOVER NOME =-=-=-=-=-=-\n");
                printf("Informe o ID do voo: ");
                scanf("%s", Id);
                flushBuffer();
                removerPassageiro(buscarVoo(voos, strupr(Id)));
                printf("        -=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 4:
                // Caracteristicas do primeiro voo da fila
                printf("-=-=-=-=-=-= PRIMEIRO VOO DA FILA =-=-=-=-=-=-\n");
                if (voos->front != NULL){
                    printf("%-10s  %-10s  %-10s  %-20s  %-10s\n", "ID do voo", "Destino", "Empresa", "Modelo", "Nº Passageiros");
                    listaVoo(voos->front);
                } else printf(" Sem voos no momento\n");
                printf("           -=-=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 5:
                // Autorizar decolagem
                printf("-=-=-=-=-=-= AUTORIZAR DECOLAGEM =-=-=-=-=-=-\n");
                voos = deQueue(voos);
                printf("           -=-=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 6:
                // Tamanho da fila de decolagem
                printf("-=-=-=-=-=-= Tamanho da fila =-=-=-=-=-=-\n");
                printf("%d voos na fila\n", queueSize(voos));
                printf("         -=-=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 7:
                // Listar voos
                printf("-=-=-=-=-=-= LISTA DE VOOS =-=-=-=-=-=-\n");
                if (voos->front != NULL) {
                    printf("%-10s  %-10s  %-10s  %-20s  %-10s\n", "ID do voo", "Destino", "Empresa", "Modelo", "Nº Passageiros");
                    percorre_fila(voos->front);
                } else printf(" Sem voos no momento\n");
                printf("        -=-=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 8:
                // Listar passageiros
                printf("-=-=-=-=-=-= LISTA DE PASSAGEIROS =-=-=-=-=-=-\n");
                printf("Digite o ID do voo para listar os passageiros: ");
                scanf("%s", Id);
                flushBuffer();
                listaPassageiros(buscarVoo(voos, strupr(Id)));
                printf("           -=-=-=-=-=-=-=-=-=-=-=-\n");
                break;
            case 9:
                // sair
                voos = liberaFila(voos);
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
        printf("\n");
    } while (opcao != 9);

    return 0;
}