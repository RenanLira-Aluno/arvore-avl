#include <iostream>
#include <algorithm>
using namespace std;

struct Node
{
    int valor;
    Node *esquerda;
    Node *direita;
    int altura;

    Node(int v) : valor(v), esquerda(nullptr), direita(nullptr), altura(1) {}
};

// Funções auxiliares
int altura(Node *no)
{
    return no ? no->altura : 0;
}

int fatorBalanceamento(Node *no)
{
    return no ? altura(no->esquerda) - altura(no->direita) : 0;
}

void atualizarAltura(Node *no)
{
    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));
}

// Rotações
Node *rotacaoDireita(Node *y)
{
    Node *x = y->esquerda;
    Node *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

Node *rotacaoEsquerda(Node *x)
{
    Node *y = x->direita;
    Node *T2 = y->esquerda;
    
    y->esquerda = x;
    x->direita = T2;

    atualizarAltura(x);
    atualizarAltura(y);
    
    return y;
}

Node *rotacaoDuplaDireita(Node *y)
{
    y->esquerda = rotacaoEsquerda(y->esquerda);
    return rotacaoDireita(y);
}

Node *rotacaoDuplaEsquerda(Node *x)
{
    x->direita = rotacaoDireita(x->direita);
    return rotacaoEsquerda(x);
}

// Inserção com balanceamento
Node *inserir(Node *no, int valor)
{
    if (!no)
        return new Node(valor);

    if (valor < no->valor)
        no->esquerda = inserir(no->esquerda, valor);
    else if (valor > no->valor)
        no->direita = inserir(no->direita, valor);
    else
        return no;

    atualizarAltura(no);

    int fb = fatorBalanceamento(no);

    // Casos de rotação
    // Esquerda-Esquerda
    if (fb > 1 && valor < no->esquerda->valor)
        return rotacaoDireita(no);
    // Direita-Direita
    if (fb < -1 && valor > no->direita->valor)
        return rotacaoEsquerda(no);
    // Esquerda-Direita
    if (fb > 1 && valor > no->esquerda->valor)
    {
        return rotacaoDuplaDireita(no);
    }
    // Direita-Esquerda
    if (fb < -1 && valor < no->direita->valor)
    {
        return rotacaoDuplaEsquerda(no);
    }

    return no;
}

// Nó com menor valor (usado na remoção)
Node *menorValor(Node *no)
{
    Node *atual = no;
    while (atual->esquerda)
        atual = atual->esquerda;
    return atual;
}

// Remoção com balanceamento
Node *remover(Node *raiz, int valor)
{
    if (!raiz)
        return raiz;

    if (valor < raiz->valor)
        raiz->esquerda = remover(raiz->esquerda, valor);
    else if (valor > raiz->valor)
        raiz->direita = remover(raiz->direita, valor);
    else
    {
        if (!raiz->esquerda || !raiz->direita)
        {
            Node *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            delete raiz;
            return temp;
        }
        Node *temp = menorValor(raiz->direita);
        raiz->valor = temp->valor;
        raiz->direita = remover(raiz->direita, temp->valor);
    }

    atualizarAltura(raiz);

    int fb = fatorBalanceamento(raiz);

    // Casos de rotação
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0)
    {
        return rotacaoDuplaDireita(raiz);
    }
    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);
    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0)
    {
        return rotacaoDuplaEsquerda(raiz);
    }

    return raiz;
}

// Impressão in-ordem
void imprimirInOrdem(Node *raiz)
{
    if (raiz)
    {
        imprimirInOrdem(raiz->esquerda);
        cout << raiz->valor << " ";
        imprimirInOrdem(raiz->direita);
    }
}

// Imprimir fatores de balanceamento
void imprimirFB(Node *raiz)
{
    if (raiz)
    {
        imprimirFB(raiz->esquerda);
        cout << "Nó " << raiz->valor << ": FB = " << fatorBalanceamento(raiz) << endl;
        imprimirFB(raiz->direita);
    }
}

// Altura total da árvore
int alturaArvore(Node *raiz)
{
    return altura(raiz);
}

// Função principal com exemplos
int main()
{
    Node *raiz = nullptr;

    cout << "==== Caso 1: Rotação simples (inserção)\n";
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 10); // Gera rotação simples para direita

    cout << "In-ordem: ";
    imprimirInOrdem(raiz);
    cout << "\nFatores de balanceamento:\n";
    imprimirFB(raiz);
    cout << "Altura: " << alturaArvore(raiz) << "\n";

    cout << "\n==== Caso 2: Rotação dupla (inserção)\n";
    raiz = nullptr;
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 20); // Gera rotação dupla esquerda-direita

    cout << "In-ordem: ";
    imprimirInOrdem(raiz);
    cout << "\nFatores de balanceamento:\n";
    imprimirFB(raiz);
    cout << "Altura: " << alturaArvore(raiz) << "\n";

    cout << "\n==== Caso 1: Rotação simples (remoção)\n";
    raiz = nullptr;
    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 30);
    raiz = remover(raiz, 10); // Gera rotação simples para esquerda

    cout << "In-ordem: ";
    imprimirInOrdem(raiz);
    cout << "\nFatores de balanceamento:\n";
    imprimirFB(raiz);
    cout << "Altura: " << alturaArvore(raiz) << "\n";

    cout << "\n==== Caso 2: Rotação dupla (remoção)\n";
    raiz = nullptr;
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 25);
    raiz = inserir(raiz, 40);
    raiz = inserir(raiz, 22);
    raiz = remover(raiz, 40); // Gera rotação dupla direita-esquerda

    cout << "In-ordem: ";
    imprimirInOrdem(raiz);
    cout << "\nFatores de balanceamento:\n";
    imprimirFB(raiz);
    cout << "Altura: " << alturaArvore(raiz) << "\n";

    return 0;
}
