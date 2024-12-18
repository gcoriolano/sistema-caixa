#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 2 // Definindo constantes que não serão alteradas, como máximo de usuários, produtos e vendas.
#define MAX_PRODUCTS 100
#define MAX_SALES_ITEMS 100
#define MAX_KG 1000
#define clrscr() printf("\e[1;1H\e[2J") // Função constante que serve para limpar o console.

typedef struct { // Typedef struct é usado para abrangir diversas variáveis em uma estrutura só.
    char username[20];
    char password[20];
    int role; // Define o cargo e permissões de quem está uilizando, sendo 0 para operador de caixa e 1 para administrador
} User;

typedef struct {
    char nome[30];
    float precoKG;
    float preco;
    int quantidade; // Define variáveis de produto, como nome, preço, quantidade e código de idenitficação.
    int codigo;
    int op;
} Produto;

User users[MAX_USERS] = {
    {"caixa", "12345", 0}, // usuario e senha para operador de caixa
    {"admin", "administrador", 1}  // usuario e senha para administrador
};

Produto produtos[MAX_PRODUCTS];
int contagem_produtos = 0;

// Definindo cada função que será utilizada no programa.

void login();
void caixa_menu();
void admin_menu();
void adicionar_produto();
void remover_produto();
void lista_produtos();
void logout();
void estoque();

int main() {
    estoque();
    login(); // Função principal na abertura do programa, vai imediatamente chamar a função "login" para que seja determinado o cargo do usuário.
    return 0;
}

void login() {
    char username[20], password[20]; // Definição de variáveis login e senha
    int i;

    printf("Por favor, faca seu login.\n"); // Prompt para o usuário colocar seu login e senha.
    printf("Usuario: ");
    scanf("%s", username);
    printf("Senha: ");
    scanf("%s", password);
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) { // strcmp é usado aqui para comparar se o usuário e senha inseridos equivalem a algum usuario e senha valido.
            if (users[i].role == 0) { // Também é verificado o nível de permissão do usuário. 0 sendo o mais baixo para operador de caixa, 1 o mais alto para administrador.
                caixa_menu();
            } else { // Dependendo do nível de permissão do usuário, um menu diferente é apresentado.
                admin_menu();
            }
            return;
        }
    }

    printf("Usuario ou senha invalidos!\n"); // Se o usuário e senha não for válido, finaliza o programa indicando o motivo.
    exit(1);
}

void caixa_menu() { // Define o menu de permissão "caixa", logo embaixo as variáveis são definidas.
    int codigo[MAX_SALES_ITEMS];
    int quantidade[MAX_SALES_ITEMS];
    float total = 0.0;
    int contagem_itens;
    float kg[MAX_KG];
    char continuar = 's';
    clrscr();
    printf("Seja bem-vindo operador de caixa!\n");

    lista_produtos();

    if (contagem_produtos == 0) { // Se a quantidade de produtos no estoque for "0", demonstra ao usuário que não há estoque.
        printf("Sem estoque!\n");
        printf("Contatar um administrador para resolver o problema.\n");
        return;
    }   
    
    do {
        printf("Em ordem, insira o codigo, a quantidade e o peso do item:\n");
        printf("Exemplo: Codigo1 | Quantidade1 | Peso1\n");
        printf("OBS: Se o item for vendido em valor unitário, coloque peso como 0.\n");

        scanf("%d %d %f", &codigo[contagem_itens], &quantidade[contagem_itens], &kg[contagem_itens]);
        contagem_itens++;
        clrscr();
        printf("Deseja adicionar mais um item? (s/n): "); // Prompt de usuário, o código só para de adicionar itens quando o usuário não quiser mais.
        scanf(" %c", &continuar);
        clrscr();
        } while (continuar == 's' || continuar == 'S');


    printf("Resumo da venda:\n");
    for (int i = 0; i < contagem_itens; i++) { // O código só para após todos os itens terem sido processados.
        int encontrado = 0;
        for (int j = 0; j < contagem_produtos; j++) {
            
            if (produtos[j].codigo == codigo[i]) {
                if (quantidade[i] <= produtos[j].quantidade) {
                    float subtotalu = quantidade[i] * produtos[j].preco;
                    total += subtotalu;
                    if (kg[i] == 0 ) {
                        produtos[j].quantidade -= quantidade[i];
                        printf("Produto: %s, Quantidade: %d, Preco Unidade: %.2f, Subtotal: %.2f\n",
                        produtos[j].nome, quantidade[i], produtos[j].preco, subtotalu);
                    } else {
                    float subtotal = kg[i] * produtos[j].precoKG; // É feito o cálculo do valor total do produto em KG.
                    total += subtotal;
                    
                    produtos[j].quantidade -= quantidade[i]; // Atualiza a quantidade em estoque
                    printf("Produto: %s, Quantidade: %d, Peso: %.2fKG, Preco KG: %.2f, Subtotal: %.2f\n",
                           produtos[j].nome, quantidade[i], kg[i], produtos[j].precoKG, subtotal);
                    }
                } else {
                            clrscr();
                    printf("Quantidade solicitada de %s é maior que a disponivel.\n", produtos[j].nome); // Se a quantidade de itens solicitada for maior que a que estiver no estoque, avisar ao usuário.
                }
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
                    clrscr();
            printf("Codigo %d nao encontrado.\n", codigo[i]); // Se o código inserido não existir no estoque, avisa ao usuário que o código não foi encontrado.
        }
    }

    printf("Total a pagar: %.2f\n", total);
}

void admin_menu() {
    int choice; // O menu de administrador, do qual possui mais funções, incluindo adicionar e remover produtos do estoque.
            clrscr();
    do {
        printf("\nMenu do Administrador:\n");
        printf("1. Adicionar Produto\n");
        printf("2. Remover Produto\n");
        printf("3. Produtos em estoque\n");
        printf("4. Ir para caixa\n");
        printf("5. Deslogar\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) { // Dependendo da escolha do administrador, diferentes funções são chamadas para execução.
            case 1:
                    clrscr();
                adicionar_produto();
                break;
            case 2:
                    clrscr();
                remover_produto();
                break;
            case 3:
                    clrscr();
                lista_produtos();
                break;
            case 4:
                    clrscr();
                caixa_menu();
                break;
            case 5:
                    clrscr();
                logout();
                return; // Retorna para o menu de login
            case 0:
                    clrscr();
                printf("Saindo...\n");
                break;
            default:
                    clrscr();
                printf("Opcao invalida!\n");
        }
    } while (choice != 0);
}

void logout() {
            clrscr();
    printf("Deslogado com sucesso!\n");
    login(); // Volta para a tela de login
}

void adicionar_produto() {
    int op;
    op = 0;
        clrscr();
    if (contagem_produtos >= MAX_PRODUCTS) { // Nosso estoque possui um espaço limitado de 100 itens, definido na constante no início do código.
        printf("Limite de produtos atingido!\n");
        return;
    }

    Produto p; // A typedef "produto" nos permite que seja inserido diversas variáveis numa só estrutura. Assim, podemos definir as características de um produto.
    printf("Voce deseja adicionar um produto com valor unitario, ou com valor em KG?\n");
    printf("Escolha 1 para em KG | Escolha 2 para valor unitario\n");
    scanf("%d", &op);
    if (op == 1) {
                clrscr();
    printf("Nome do produto: ");
    scanf("%s", p.nome);
            clrscr();
    printf("Preco do produto em KG: ");
    scanf("%f", &p.precoKG);
            clrscr();
    printf("Quantidade do produto: ");
    scanf("%d", &p.quantidade); } else {
                clrscr();
    printf("Nome do produto: ");
    scanf("%s", p.nome);
            clrscr();
    printf("Preco do produto em UNIDADE: ");
    scanf("%f", &p.preco);
            clrscr();
    printf("Quantidade do produto: ");
    scanf("%d", &p.quantidade);       
    }

    if (p.quantidade <= 0) {
                clrscr();
        printf("A quantidade deve ser maior que zero!\n"); // Não tem como colocar 0 produtos no estoque.
        return;
    }
        clrscr();
    printf("Codigo do produto: ");
    scanf("%d", &p.codigo);

    // Verifica se o código já existe
    for (int i = 0; i < contagem_produtos; i++) {
        if (produtos[i].codigo == p.codigo) {
                    clrscr();
            printf("Erro: Codigo %d ja existe. Nao e possivel adicionar o produto.\n", p.codigo);
            return;
        }
    }

    produtos[contagem_produtos++] = p;
            clrscr();
    printf("Produto adicionado com sucesso!\n"); // Se nada tiver dado problema, o produto foi adicionado com sucesso.
}

void remover_produto() {
    int codigo;
            clrscr();
    printf("Codigo do produto a ser removido: "); // Caso um produto precise ser removido do estoque, temos essa função.
    scanf("%d", &codigo);

    for (int i = 0; i < contagem_produtos; i++) { // Um loop usado para verificar tudo que há dentro dos vetores "produtos", até que se encontre o código do produto à se remover.
        if (produtos[i].codigo == codigo) { // Encontrado o código,
            for (int j = i; j < contagem_produtos - 1; j++) { // a contagem total dos produtos em estoque é verificada,
                produtos[j] = produtos[j + 1];
            }
            contagem_produtos--; // e enfim, o produto é removido do estoque.
                    clrscr();
            printf("Produto removido com sucesso!\n");
            return;
        }
    }
        clrscr();
    printf("Produto nao encontrado!\n"); // Se o código do produto não for encontrado, é avisado ao usuário.
}

void lista_produtos() {
    if (contagem_produtos == 0) {
                clrscr();
        printf("Nenhum produto cadastrado!\n"); // Se não houver produtos no estoque, é avisado ao usuário.
        return;
    }
        clrscr();
    printf("\nLista de Produtos:\n");
    for (int i = 0; i < contagem_produtos; i++) {
        if (produtos[i].precoKG < 1) {
            printf("Nome: %s, Codigo: %d, Preco Unidade: %.2f, Quantidade: %d\n", produtos[i].nome, produtos[i].codigo, produtos[i].preco, produtos[i].quantidade);
        } else {
            printf("Nome: %s, Codigo: %d, Preco KG: %.2f, Quantidade: %d\n", produtos[i].nome, produtos[i].codigo, produtos[i].precoKG, produtos[i].quantidade);
        }
    } // Aqui todas as características dos produtos em estoque são detalhadas ao usuário.

}

void estoque() {
// Função feita para adicionar itens no estoque assim que o programa inicializar.
// Substituição para a ausência de um banco de dados, itens armazenados no cache.
Produto b;
b.precoKG = 10;
b.quantidade = 20;
b.codigo = 1;
strcpy(b.nome, "Banana");
produtos[contagem_produtos++] = b;

Produto c;
c.preco = 5;
c.quantidade = 15;
c.codigo = 2;
strcpy(c.nome, "Alface");
produtos[contagem_produtos++] = c;
}