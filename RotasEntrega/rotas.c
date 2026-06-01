#include "rotas.h"

// ======================= CONFIGURACAO DA REDE =======================

// Zera a rede: nenhum local e nenhuma estrada cadastrada.
void inicializar_rede(Rede* r) {
    int i, j;
    r->num_locais = 0;
    for (i = 0; i < MAX_LOCAIS; i++) {
        r->nomes[i][0] = '\0';
        for (j = 0; j < MAX_LOCAIS; j++) {
            r->matriz[i][j] = SEM_CONEXAO;
        }
    }
}

// Verifica se um indice de local realmente existe na rede.
bool indice_valido(Rede* r, int indice) {
    return (indice >= 0 && indice < r->num_locais);
}

// Define quantos locais a rede tera e da nomes padrao ("Local 0", "Local 1"...).
void definir_locais(Rede* r, int quantidade) {
    int i;
    if (quantidade < 1) quantidade = 1;
    if (quantidade > MAX_LOCAIS) quantidade = MAX_LOCAIS;

    inicializar_rede(r);
    r->num_locais = quantidade;
    for (i = 0; i < quantidade; i++) {
        sprintf(r->nomes[i], "Local %d", i);
    }
}

// Altera o nome de um local especifico.
bool definir_nome_local(Rede* r, int indice, const char* nome) {
    if (!indice_valido(r, indice)) return false;
    strncpy(r->nomes[indice], nome, 49);
    r->nomes[indice][49] = '\0';
    return true;
}

// Insere uma estrada (aresta) com peso entre dois locais.
// Se mao_dupla = true, a estrada vale nos dois sentidos.
bool adicionar_aresta(Rede* r, int origem, int destino, int peso, bool mao_dupla) {
    if (!indice_valido(r, origem) || !indice_valido(r, destino)) {
        printf("\n[!] Local de origem ou destino invalido.\n");
        return false;
    }
    if (origem == destino) {
        printf("\n[!] Origem e destino nao podem ser o mesmo local.\n");
        return false;
    }
    if (peso <= 0) {
        printf("\n[!] O peso (distancia/tempo) deve ser maior que zero.\n");
        return false;
    }

    r->matriz[origem][destino] = peso;
    if (mao_dupla) {
        r->matriz[destino][origem] = peso;
    }
    return true;
}

// Carrega uma rede de exemplo pronta para testes/demonstracao na defesa.
void carregar_rede_exemplo(Rede* r) {
    definir_locais(r, 5);
    definir_nome_local(r, 0, "Centro");
    definir_nome_local(r, 1, "Bairro A");
    definir_nome_local(r, 2, "Bairro B");
    definir_nome_local(r, 3, "Bairro C");
    definir_nome_local(r, 4, "Bairro D");

    // Conexoes em mao dupla (estradas com distancia em km)
    adicionar_aresta(r, 0, 1, 5, true);   // Centro  <-> Bairro A (5)
    adicionar_aresta(r, 0, 2, 10, true);  // Centro  <-> Bairro B (10)
    adicionar_aresta(r, 1, 3, 3, true);   // Bairro A <-> Bairro C (3)
    adicionar_aresta(r, 2, 3, 2, true);   // Bairro B <-> Bairro C (2)
    adicionar_aresta(r, 3, 4, 7, true);   // Bairro C <-> Bairro D (7)
    adicionar_aresta(r, 2, 4, 8, true);   // Bairro B <-> Bairro D (8)

    printf("\n[+] Rede de exemplo carregada (5 locais e 6 estradas).\n");
}

// ======================= VISUALIZACAO =======================

// Lista os locais cadastrados com seu indice (usado nos menus).
void listar_locais(Rede* r) {
    int i;
    if (r->num_locais == 0) {
        printf("\n[!] Nenhum local cadastrado ainda.\n");
        return;
    }
    printf("\n--- LOCAIS DA REDE ---\n");
    for (i = 0; i < r->num_locais; i++) {
        printf("  [%d] %s\n", i, r->nomes[i]);
    }
}

// Exibe a matriz de adjacencia (a "rede de distribuicao").
void exibir_matriz(Rede* r) {
    int i, j;
    if (r->num_locais == 0) {
        printf("\n[!] Rede vazia. Defina os locais primeiro.\n");
        return;
    }

    listar_locais(r);

    printf("\n--- MATRIZ DE ADJACENCIA (pesos = distancia/tempo) ---\n");
    printf("      ");
    for (j = 0; j < r->num_locais; j++) {
        printf("%5d", j);
    }
    printf("\n");

    for (i = 0; i < r->num_locais; i++) {
        printf("%3d | ", i);
        for (j = 0; j < r->num_locais; j++) {
            if (r->matriz[i][j] == SEM_CONEXAO) {
                printf("    .");   // ponto = sem estrada direta
            } else {
                printf("%5d", r->matriz[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n* Linha = origem | Coluna = destino | '.' = sem estrada direta\n");
}

// ======================= 3. BUSCA EM LARGURA (BFS) =======================
// Responde: "Quais locais podem ser alcancados a partir de um ponto?"
// Visita os locais por "camadas" (vizinhos diretos primeiro), simulando
// a expansao das entregas a partir do centro de distribuicao.
void bfs(Rede* r, int inicio) {
    int fila[MAX_LOCAIS];
    int visitado[MAX_LOCAIS];
    int frente = 0, tras = 0;
    int i, atual;
    int total_alcancados = 0;

    if (!indice_valido(r, inicio)) {
        printf("\n[!] Local de inicio invalido.\n");
        return;
    }

    for (i = 0; i < r->num_locais; i++) visitado[i] = 0;

    // Marca e enfileira o local de partida
    visitado[inicio] = 1;
    fila[tras++] = inicio;

    printf("\n--- SIMULACAO DE ENTREGA (BFS) a partir de '%s' ---\n", r->nomes[inicio]);
    printf("Ordem de visita (expansao das entregas):\n");

    while (frente < tras) {
        atual = fila[frente++];
        printf("  -> %s\n", r->nomes[atual]);
        total_alcancados++;

        // Enfileira todos os vizinhos ainda nao visitados
        for (i = 0; i < r->num_locais; i++) {
            if (r->matriz[atual][i] != SEM_CONEXAO && !visitado[i]) {
                visitado[i] = 1;
                fila[tras++] = i;
            }
        }
    }

    printf("\nTotal de locais alcancaveis a partir de '%s': %d de %d.\n",
           r->nomes[inicio], total_alcancados, r->num_locais);

    // Avisa quais locais NAO podem ser alcancados (ilhas na rede)
    if (total_alcancados < r->num_locais) {
        printf("Locais NAO alcancaveis:");
        for (i = 0; i < r->num_locais; i++) {
            if (!visitado[i]) printf(" %s;", r->nomes[i]);
        }
        printf("\n");
    }
}

// ======================= 4. MENOR ROTA (DIJKSTRA) =======================
// Responde: "Qual o caminho mais curto para entregar de uma origem ate um destino?"
// Calcula a menor distancia da origem para TODOS os locais e reconstroi o caminho.
void dijkstra(Rede* r, int origem, int destino) {
    int dist[MAX_LOCAIS];       // menor distancia conhecida da origem ate cada local
    int anterior[MAX_LOCAIS];   // local anterior no melhor caminho (para reconstruir a rota)
    int visitado[MAX_LOCAIS];
    int i, j, u, menor;

    if (!indice_valido(r, origem)) {
        printf("\n[!] Local de origem invalido.\n");
        return;
    }

    // Inicializacao
    for (i = 0; i < r->num_locais; i++) {
        dist[i] = INFINITO;
        anterior[i] = -1;
        visitado[i] = 0;
    }
    dist[origem] = 0;

    // Loop principal: a cada passo "fecha" o local nao visitado mais proximo
    for (i = 0; i < r->num_locais; i++) {
        u = -1;
        menor = INFINITO;
        for (j = 0; j < r->num_locais; j++) {
            if (!visitado[j] && dist[j] < menor) {
                menor = dist[j];
                u = j;
            }
        }

        if (u == -1) break;  // restantes sao inalcancaveis
        visitado[u] = 1;

        // Relaxa as arestas saindo de u
        for (j = 0; j < r->num_locais; j++) {
            if (r->matriz[u][j] != SEM_CONEXAO && !visitado[j]) {
                int nova = dist[u] + r->matriz[u][j];
                if (nova < dist[j]) {
                    dist[j] = nova;
                    anterior[j] = u;
                }
            }
        }
    }

    // Relatorio das distancias minimas para todos os locais
    printf("\n--- MENOR ROTA (DIJKSTRA) a partir de '%s' ---\n", r->nomes[origem]);
    printf("Distancias minimas para todos os locais:\n");
    for (i = 0; i < r->num_locais; i++) {
        if (dist[i] == INFINITO) {
            printf("  %-15s : inalcancavel\n", r->nomes[i]);
        } else {
            printf("  %-15s : %d\n", r->nomes[i], dist[i]);
        }
    }

    // Caminho detalhado ate um destino especifico (se informado)
    if (indice_valido(r, destino)) {
        printf("\n--- CAMINHO ATE '%s' ---\n", r->nomes[destino]);
        if (dist[destino] == INFINITO) {
            printf("Nao existe rota de '%s' ate '%s'.\n",
                   r->nomes[origem], r->nomes[destino]);
        } else {
            // Reconstroi o caminho de tras para frente usando o vetor 'anterior'
            int caminho[MAX_LOCAIS];
            int tam = 0;
            int atual = destino;
            while (atual != -1) {
                caminho[tam++] = atual;
                atual = anterior[atual];
            }

            printf("Distancia minima: %d\n", dist[destino]);
            printf("Caminho percorrido: ");
            for (i = tam - 1; i >= 0; i--) {
                printf("%s", r->nomes[caminho[i]]);
                if (i > 0) printf(" -> ");
            }
            printf("\n");
        }
    }
}
