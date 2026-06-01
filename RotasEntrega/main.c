#include "rotas.h"

void exibir_menu_principal() {
    printf("\n======================================================\n");
    printf("   SISTEMA DE ROTAS DE ENTREGA (GRAFO / MATRIZ ADJ.)\n");
    printf("======================================================\n");
    printf(" 1. Definir locais (vertices da rede)\n");
    printf(" 2. Cadastrar rota entre locais (aresta com peso)\n");
    printf(" 3. Exibir rede de distribuicao (matriz)\n");
    printf("------------------------------------------------------\n");
    printf(" 4. Simular entrega (BFS - locais alcancaveis)\n");
    printf(" 5. Calcular menor rota (Dijkstra)\n");
    printf("------------------------------------------------------\n");
    printf(" 6. Carregar rede de exemplo\n");
    printf(" 0. Sair\n");
    printf("======================================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Rede rede;
    char input[256];
    int opcao = -1;

    inicializar_rede(&rede);

    while (true) {
        exibir_menu_principal();
        if (!fgets(input, sizeof(input), stdin)) continue;
        if (sscanf(input, "%d", &opcao) != 1) {
            printf("\n[!] Entrada invalida. Digite um numero.\n");
            continue;
        }

        switch (opcao) {
            case 1: { // Definir locais (vertices)
                int n, i;
                printf("\n--- DEFINIR LOCAIS ---\n");
                printf("Quantos locais a rede tera? (1 a %d): ", MAX_LOCAIS);
                fgets(input, sizeof(input), stdin);
                if (sscanf(input, "%d", &n) != 1) {
                    printf("\n[!] Numero invalido.\n");
                    break;
                }

                definir_locais(&rede, n);
                printf("\nDeseja nomear os locais? (1-Sim / 0-Nao, usar nomes padrao): ");
                fgets(input, sizeof(input), stdin);
                int nomear = 0;
                sscanf(input, "%d", &nomear);

                if (nomear == 1) {
                    for (i = 0; i < rede.num_locais; i++) {
                        char nome[50];
                        printf("Nome do local [%d]: ", i);
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = 0;
                        if (nome[0] != '\0') definir_nome_local(&rede, i, nome);
                    }
                }
                printf("\n[+] %d locais definidos com sucesso.\n", rede.num_locais);
                break;
            }
            case 2: { // Cadastrar aresta
                int origem, destino, peso, dupla = 1;
                if (rede.num_locais == 0) {
                    printf("\n[!] Defina os locais primeiro (opcao 1).\n");
                    break;
                }
                listar_locais(&rede);
                printf("\n--- CADASTRAR ROTA ---\n");
                printf("Indice do local de ORIGEM: ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &origem);

                printf("Indice do local de DESTINO: ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &destino);

                printf("Peso da estrada (distancia/tempo, > 0): ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &peso);

                printf("Estrada de mao dupla? (1-Sim / 0-Nao): ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &dupla);

                if (adicionar_aresta(&rede, origem, destino, peso, dupla == 1)) {
                    printf("\n[+] Rota cadastrada: %s -> %s (peso %d)%s.\n",
                           rede.nomes[origem], rede.nomes[destino], peso,
                           dupla == 1 ? " [mao dupla]" : "");
                }
                break;
            }
            case 3: { // Exibir matriz
                exibir_matriz(&rede);
                break;
            }
            case 4: { // BFS
                int inicio;
                if (rede.num_locais == 0) {
                    printf("\n[!] Defina os locais primeiro (opcao 1).\n");
                    break;
                }
                listar_locais(&rede);
                printf("\nIndice do local de partida (centro de distribuicao): ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &inicio);
                bfs(&rede, inicio);
                break;
            }
            case 5: { // Dijkstra
                int origem, destino;
                if (rede.num_locais == 0) {
                    printf("\n[!] Defina os locais primeiro (opcao 1).\n");
                    break;
                }
                listar_locais(&rede);
                printf("\nIndice do local de ORIGEM: ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &origem);

                printf("Indice do local de DESTINO (-1 para ver so as distancias): ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &destino);

                dijkstra(&rede, origem, destino);
                break;
            }
            case 6: { // Rede de exemplo
                carregar_rede_exemplo(&rede);
                break;
            }
            case 0:
                printf("\nEncerrando o sistema...\n");
                return 0;
            default:
                printf("\n[!] Opcao Invalida!\n");
        }
    }
    return 0;
}
