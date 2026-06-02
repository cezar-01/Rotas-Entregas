#include "rotas.h"   // Inclui as estruturas, constantes e funcoes da rede de rotas

// Mostra o menu de opcoes do sistema na tela.
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
    printf("Escolha uma opcao: ");                  // Solicita a escolha do usuario
}

int main() {
    Rede rede;                                       // Estrutura que guarda toda a rede de entregas
    char input[256];                                 // Buffer para ler as entradas digitadas pelo usuario
    int opcao = -1;                                  // Opcao escolhida no menu

    inicializar_rede(&rede);                         // Zera a rede antes de comecar a usar

    while (true) {                                   // Laco principal: mantem o programa rodando ate o usuario sair
        exibir_menu_principal();                     // Mostra o menu a cada iteracao
        if (!fgets(input, sizeof(input), stdin)) continue; // Le a linha digitada; se falhar, repete o menu
        if (sscanf(input, "%d", &opcao) != 1) {      // Tenta converter a entrada em numero
            printf("\n[!] Entrada invalida. Digite um numero.\n"); // Avisa se nao for um numero
            continue;                                // Volta ao inicio do laco
        }

        switch (opcao) {                             // Decide o que fazer conforme a opcao escolhida
            case 1: { // Definir locais (vertices)
                int n, i;                            // n = quantidade de locais; i = controle do laco
                printf("\n--- DEFINIR LOCAIS ---\n");
                printf("Quantos locais a rede tera? (1 a %d): ", MAX_LOCAIS);
                fgets(input, sizeof(input), stdin);  // Le a quantidade digitada
                if (sscanf(input, "%d", &n) != 1) {  // Valida se foi digitado um numero
                    printf("\n[!] Numero invalido.\n");
                    break;
                }

                definir_locais(&rede, n);            // Cria os n locais com nomes padrao
                printf("\nDeseja nomear os locais? (1-Sim / 0-Nao, usar nomes padrao): ");
                fgets(input, sizeof(input), stdin);  // Le a resposta sobre nomear os locais
                int nomear = 0;                      // Indica se o usuario quer nomear (1) ou nao (0)
                sscanf(input, "%d", &nomear);

                if (nomear == 1) {                   // Se o usuario quiser dar nomes personalizados...
                    for (i = 0; i < rede.num_locais; i++) { // ...percorre cada local cadastrado
                        char nome[50];               // Buffer para o nome digitado
                        printf("Nome do local [%d]: ", i);
                        fgets(nome, sizeof(nome), stdin); // Le o nome digitado
                        nome[strcspn(nome, "\n")] = 0;    // Remove o '\n' deixado pelo fgets
                        if (nome[0] != '\0') definir_nome_local(&rede, i, nome); // Aplica o nome se nao for vazio
                    }
                }
                printf("\n[+] %d locais definidos com sucesso.\n", rede.num_locais);
                break;
            }
            case 2: { // Cadastrar aresta
                int origem, destino, peso, dupla = 1; // Dados da estrada a ser cadastrada
                if (rede.num_locais == 0) {          // Nao da para criar estrada sem locais definidos
                    printf("\n[!] Defina os locais primeiro (opcao 1).\n");
                    break;
                }
                listar_locais(&rede);                // Mostra os locais para o usuario escolher os indices
                printf("\n--- CADASTRAR ROTA ---\n");
                printf("Indice do local de ORIGEM: ");
                fgets(input, sizeof(input), stdin);  // Le o indice de origem
                sscanf(input, "%d", &origem);

                printf("Indice do local de DESTINO: ");
                fgets(input, sizeof(input), stdin);  // Le o indice de destino
                sscanf(input, "%d", &destino);

                printf("Peso da estrada (distancia/tempo, > 0): ");
                fgets(input, sizeof(input), stdin);  // Le o peso da estrada
                sscanf(input, "%d", &peso);

                printf("Estrada de mao dupla? (1-Sim / 0-Nao): ");
                fgets(input, sizeof(input), stdin);  // Le se a estrada e de mao dupla
                sscanf(input, "%d", &dupla);

                if (adicionar_aresta(&rede, origem, destino, peso, dupla == 1)) { // Tenta criar a estrada
                    printf("\n[+] Rota cadastrada: %s -> %s (peso %d)%s.\n",       // Confirma o cadastro
                           rede.nomes[origem], rede.nomes[destino], peso,
                           dupla == 1 ? " [mao dupla]" : "");  // Indica se ficou em mao dupla
                }
                break;
            }
            case 3: { // Exibir matriz
                exibir_matriz(&rede);                // Mostra a matriz de adjacencia da rede
                break;
            }
            case 4: { // BFS
                int inicio;                          // Local de partida da simulacao
                if (rede.num_locais == 0) {          // Precisa ter locais cadastrados
                    printf("\n[!] Defina os locais primeiro (opcao 1).\n");
                    break;
                }
                listar_locais(&rede);                // Mostra os locais para escolher o ponto de partida
                printf("\nIndice do local de partida (centro de distribuicao): ");
                fgets(input, sizeof(input), stdin);  // Le o indice de partida
                sscanf(input, "%d", &inicio);
                bfs(&rede, inicio);                  // Executa a busca em largura a partir do local escolhido
                break;
            }
            case 5: { // Dijkstra
                int origem, destino;                 // Origem e destino do calculo da menor rota
                if (rede.num_locais == 0) {          // Precisa ter locais cadastrados
                    printf("\n[!] Defina os locais primeiro (opcao 1).\n");
                    break;
                }
                listar_locais(&rede);                // Mostra os locais para escolher origem e destino
                printf("\nIndice do local de ORIGEM: ");
                fgets(input, sizeof(input), stdin);  // Le o indice de origem
                sscanf(input, "%d", &origem);

                printf("Indice do local de DESTINO (-1 para ver so as distancias): ");
                fgets(input, sizeof(input), stdin);  // Le o indice de destino (-1 mostra so as distancias)
                sscanf(input, "%d", &destino);

                dijkstra(&rede, origem, destino);    // Calcula a menor rota da origem ate o destino
                break;
            }
            case 6: { // Rede de exemplo
                carregar_rede_exemplo(&rede);        // Carrega uma rede pronta para demonstracao
                break;
            }
            case 0:                                  // Opcao de sair
                printf("\nEncerrando o sistema...\n");
                return 0;                            // Encerra o programa
            default:                                 // Qualquer numero fora do menu
                printf("\n[!] Opcao Invalida!\n");
        }
    }
    return 0;                                        // Retorno padrao do main (nunca alcancado por causa do while)
}
