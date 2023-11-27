#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PEDIDOS 100
#define FILENAME "pedidos2.txt"

struct Pedido {
    int ID;
    char nome[50];
    char telefone[15];
    char endereco[100];
    char saborPizza[800];
    char formaPagamento[20];
    char tamanhoPizza[20];
    double valorTroco;
    double valorPedido;
};

struct Pedido *pedidos = NULL;
int numPedidos = 0;
int proximoID = 1;

// Função para limpar o buffer do teclado (remover caracteres não lidos)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para salvar os pedidos em um arquivo
void salvarPedidosEmArquivo() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
                
    for (int i = 0; i < numPedidos; i++) {
        struct Pedido pedido = pedidos[i];
        fprintf(file, "ID: %d\n", pedido.ID);
        fprintf(file, "Nome: %s\n", pedido.nome);
        fprintf(file, "Telefone: %s\n", pedido.telefone);
        fprintf(file, "Endereço: %s\n", pedido.endereco);
        fprintf(file, "Pizza: %s\n", pedido.saborPizza);
        fprintf(file, "Forma de pagamento: %s\n", pedido.formaPagamento);
        fprintf(file, "Valor do Pedido: R$ %.2f\n", pedido.valorPedido);
       


        if (strcmp(pedido.formaPagamento, "Dinheiro") == 0) {
            fprintf(file, "Valor dado pelo cliente: R$ %.2f\n", pedido.valorTroco);
            double troco = pedido.valorTroco - pedido.valorPedido;
            fprintf(file, "Troco para o cliente: R$ %.2f\n", troco);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

void carregarPedidosDeArquivo() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), file) != NULL) {
        struct Pedido novoPedido;
        int id;
        char nome[50];
        char telefone[15];
        char endereco[100];
        char saborPizza[50];
        char tamanhoPizza[20];
        char formaPagamento[20];
        double valorPedido;
        double valorTroco;

        if (sscanf(line, "ID: %d", &id) == 1) {
            novoPedido.ID = id;
        } else {
            continue;  // Pular linhas que não começam com "ID:"
        }

        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }
        sscanf(line, "Nome: %49[^,]", nome);
        strcpy(novoPedido.nome, nome);

        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }
        sscanf(line, "Telefone: %14[^,]", telefone);
        strcpy(novoPedido.telefone, telefone);

        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }
        sscanf(line, "Endereço: %99[^,]", endereco);
        strcpy(novoPedido.endereco, endereco);

        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }
        sscanf(line, "Pizza: %49[^(](%19[^)])", saborPizza, tamanhoPizza);
        strcpy(novoPedido.saborPizza, saborPizza);
        strcpy(novoPedido.tamanhoPizza, tamanhoPizza);

        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }
        sscanf(line, "Forma de pagamento: %19s", formaPagamento);
        strcpy(novoPedido.formaPagamento, formaPagamento);

        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }
        sscanf(line, "Valor do Pedido: R$ %lf", &valorPedido);
        novoPedido.valorPedido = valorPedido;

        if (strcmp(novoPedido.formaPagamento, "Dinheiro") == 0) {
            if (fgets(line, sizeof(line), file) == NULL) {
                break;
            }
            sscanf(line, "Valor dado pelo cliente: R$ %lf", &valorTroco);
            novoPedido.valorTroco = valorTroco;
        } else {
            novoPedido.valorTroco = 0.0;
        }

        if (pedidos == NULL) {
            pedidos = (struct Pedido *)malloc(sizeof(struct Pedido));
        } else {
            pedidos = (struct Pedido *)realloc(pedidos, (numPedidos + 1) * sizeof(struct Pedido));
        }

        if (pedidos == NULL) {
            printf("Erro na alocação de memória.\n");
            exit(1);
        }

        pedidos[numPedidos] = novoPedido;
        numPedidos++;
    }

    fclose(file);
}

int compararPedidos(const void *a, const void *b) {
    struct Pedido *pedidoA = (struct Pedido *)a;
    struct Pedido *pedidoB = (struct Pedido *)b;
    return strcmp(pedidoA->nome, pedidoB->nome);
}

// Função para listar os pedidos registrados em ordem alfabética
void listarPedidosOrdenados() {
    printf("\n************************************\n");
    printf("   Lista de Pedidos de Pizza (Ordenados por ID):  \n");
    printf("\n");

    if (numPedidos == 0) {
        printf("Nenhum pedido de pizza registrado.\n");
    } else {
        qsort(pedidos, numPedidos, sizeof(struct Pedido), compararPedidos);

        printf("Pedidos de Pizza Registrados:\n\n");
        for (int i = 0; i < numPedidos; i++) {
            struct Pedido pedido = pedidos[i];
            printf("ID: %d\n", pedido.ID);
            printf("Nome: %s\n", pedido.nome);
            printf("Telefone: %s\n", pedido.telefone);
            printf("Endereço: %s\n", pedido.endereco);
            printf("Pizza: %s \n", pedido.saborPizza);
            printf("Forma de pagamento: %s\n", pedido.formaPagamento);
            printf("Valor do Pedido: R$ %.2f\n", pedido.valorPedido);

            if (strcmp(pedido.formaPagamento, "Dinheiro") == 0) {
                double troco = pedido.valorTroco - pedido.valorPedido;
                printf("Valor dado pelo cliente: R$ %.2f\n", pedido.valorTroco);
                printf("Troco: R$ %.2f\n", troco);
            }

            printf("\n");
        }
    }
}

// Função para excluir um pedido por ID
void excluirPedidoPorID(int id) {
    int index = -1;
    for (int i = 0; i < numPedidos; i++) {
        if (pedidos[i].ID == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Pedido de pizza com ID %d não encontrado.\n", id);
        return;
    }

    printf("\nPedido de pizza encontrado:\n");
    struct Pedido pedido = pedidos[index];
    printf("ID: %d\n", pedido.ID);
    printf("Nome: %s\n", pedido.nome);
    printf("Telefone: %s\n", pedido.telefone);
    printf("Endereço: %s\n", pedido.endereco);
    printf("Sabor da Pizza: %s\n", pedido.saborPizza);
    printf("\n");

    char confirmacao;
    printf("Deseja confirmar a exclusão (S para sim, N para não): ");
    scanf(" %c", &confirmacao);

    if (confirmacao == 'S' || confirmacao == 's') {
        for (int i = index; i < numPedidos - 1; i++) {
            pedidos[i] = pedidos[i + 1];
        }
        numPedidos--;
        printf("\nPedido de pizza com ID %d excluído com sucesso.\n", pedido.ID);

        salvarPedidosEmArquivo();
    } else {
        printf("Exclusão cancelada.\n");
    }
}

// Função para excluir um pedido
void excluirPedido() {
    int id;
    
    printf("\n************************************\n");
    printf("   Excluir Pedido de Pizza    \n");
    printf("************************************\n");

    printf("Digite o ID do pedido que deseja excluir: ");
    scanf("%d", &id);

    excluirPedidoPorID(id);
}

// Função para registrar um novo pedido
void fazerPedido() {
    struct Pedido novoPedido;
    int quantidadePizzas;

    printf("\n************************************\n");
    printf("         Cadastre seu pedido  \n");
    printf("************************************\n");

    limparBuffer();

    novoPedido.ID = proximoID++;

    printf("Digite o seu nome: ");
    fgets(novoPedido.nome, sizeof(novoPedido.nome), stdin);
    novoPedido.nome[strlen(novoPedido.nome) - 1] = '\0';

    do {
        printf("Digite o número de telefone (DDD + Número): ");
        fgets(novoPedido.telefone, sizeof(novoPedido.telefone), stdin);
        novoPedido.telefone[strlen(novoPedido.telefone) - 1] = '\0';
    } while (strlen(novoPedido.telefone) != 11 || !strspn(novoPedido.telefone, "0123456789"));

    printf("Digite o endereço de entrega: ");
    fgets(novoPedido.endereco, sizeof(novoPedido.endereco), stdin);
    novoPedido.endereco[strlen(novoPedido.endereco) - 1] = '\0';

    printf("Digite a quantidade de pizzas: ");
    scanf("%d", &quantidadePizzas);
    limparBuffer();

    novoPedido.valorPedido = 0;
    strcpy(novoPedido.saborPizza, "");

    for (int i = 0; i < quantidadePizzas; i++) {
        printf("\nPizza %d:\n", i + 1);
        printf("Escolha o tamanho da pizza:\n");
        printf("1. Pequena (R$ 20.00)\n");
        printf("2. Média (R$ 25.00)\n");
        printf("3. Grande (R$ 30.00)\n");
        printf("Escolha uma opção: ");
        int opcaoTamanho;
        scanf("%d", &opcaoTamanho);
        double precoTamanho = 0.0;

        switch (opcaoTamanho) {
            case 1:
                strcpy(novoPedido.tamanhoPizza, "Pequena");
                precoTamanho = 20.00;
                break;
            case 2:
                strcpy(novoPedido.tamanhoPizza, "Média");
                precoTamanho = 25.00;
                break;
            case 3:
                strcpy(novoPedido.tamanhoPizza, "Grande");
                precoTamanho = 30.00;
                break;
            default:
                printf("Opção inválida. Tamanho padrão definido (MÉDIO).\n");
                strcpy(novoPedido.tamanhoPizza, "Média");
                precoTamanho = 25.00;
                break;
        }

        limparBuffer();
        printf("Escolha o sabor da pizza:\n");
        printf("1. Margherita\n");
        printf("2. Pepperoni\n");
        printf("3. Frango com Catupiry\n");
        printf("4. Calabresa\n");
        printf("5. Vegetariana\n");
        printf("Escolha uma opção: ");
        int escolhaSabor;
        scanf("%d", &escolhaSabor);

        if (escolhaSabor >= 1 && escolhaSabor <= 5) {
            char saborSelecionado[50];
            switch (escolhaSabor) {
                case 1:
                    strcpy(saborSelecionado, "Margherita");
                    break;
                case 2:
                    strcpy(saborSelecionado, "Pepperoni");
                    break;
                case 3:
                    strcpy(saborSelecionado, "Frango com Catupiry");
                    break;
                case 4:
                    strcpy(saborSelecionado, "Calabresa");
                    break;
                case 5:
                    strcpy(saborSelecionado, "Vegetariana");
                    break;
            }
            strcat(saborSelecionado, "(");
            strcat(saborSelecionado, novoPedido.tamanhoPizza);
            strcat(saborSelecionado, ")");
            if (i == 0) {
                strcpy(novoPedido.saborPizza, saborSelecionado);
            } else {
                strcat(novoPedido.saborPizza, ", ");
                strcat(novoPedido.saborPizza, saborSelecionado);
            }

            novoPedido.valorPedido += precoTamanho;
        } else {
            printf("Opção inválida. Tente novamente.\n");
            i--;
        }

        limparBuffer();
    }

    printf("Escolha a forma de pagamento (Dinheiro, Cartao, Pix): ");
    fgets(novoPedido.formaPagamento, sizeof(novoPedido.formaPagamento), stdin);
    novoPedido.formaPagamento[strlen(novoPedido.formaPagamento) - 1] = '\0';

    if (strcmp(novoPedido.formaPagamento, "Dinheiro") == 0) {
        do {
            printf("Digite o valor a ser entregue (em reais): ");
            scanf("%lf", &novoPedido.valorTroco);
            if (novoPedido.valorTroco < novoPedido.valorPedido) {
                printf("O valor do troco precisa ser maior que %.2f. Tente novamente.\n", novoPedido.valorPedido);
            }
        } while (novoPedido.valorTroco < novoPedido.valorPedido);
    } else {
        novoPedido.valorTroco = 0;
    }

    if (pedidos == NULL) {
        pedidos = (struct Pedido *)malloc(sizeof(struct Pedido));
    } else {
        pedidos = (struct Pedido *)realloc(pedidos, (numPedidos + 1) * sizeof(struct Pedido));
    }

    if (pedidos == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }

    pedidos[numPedidos] = novoPedido;
    numPedidos++;

    printf("\nPedido de pizza realizado com sucesso!\n");

    salvarPedidosEmArquivo();
}

// Função que define o menu principal do programa
void menuPrincipal() {
    carregarPedidosDeArquivo();

    int opcao;
    do {
        printf("====================================================================="
         "=================\n");
  printf("                                          ##\n");
  printf("                                        ######\n");
  printf("                                    ####      ####\n");
  printf("                                ####              ####\n");
  printf("                              ##########################\n");
  printf("                             ##    GUSTAVO'S PIZZARIA   ##                                                \n");
  printf("                              ##########################                                                  \n");
  printf("                                ##  ##  ##  ##  ##  ##                                                    \n");
  printf("                                ##  ##  ##  ##  ##  ##                                                    \n");
  printf("                                ##  ##  ##  ##  ##  ##                                                    \n");
  printf("                                ##  ##  ##  ##  ##  ##\n");
  printf("                                ##  ##  ##  ##  ##  ##\n");
  printf("                              ##########################\n");
  printf("======================================================================================\n");
  printf("------------------------------------------||------------------------------------------\n");
  printf("|          1. FAZER UM PEDIDO.            ||            2. LISTA DE PEDIDOS.          \n");
  printf("------------------------------------------||------------------------------------------\n");
  printf("|          3. EXCLUIR PEDIDO.             ||            4. SAIR DO PROGRAMA.          \n");
  printf("------------------------------------------||------------------------------------------\n");
  printf("======================================================================================\n");
scanf("%d", &opcao);
  printf("--------------------------------------------------------------------------------------\n");


        switch (opcao) {
            case 1:
                fazerPedido();
                break;
            case 2:
                listarPedidosOrdenados();
                break;
            case 3:
                excluirPedido();
                break;
            case 4:
                salvarPedidosEmArquivo();
                printf("\nPrograma da Pizzaria encerrado. Obrigado por usar nosso serviço!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 4);
}

// Função principal do programa
int main() {
    menuPrincipal();
    return 0;
}
