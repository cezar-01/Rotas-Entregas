#include "rotas.h"   // Inclui o cabecalho com a struct Rede, constantes e prototipos das funcoes

// ======================= CONFIGURACAO DA REDE =======================

// Zera a rede: nenhum local e nenhuma estrada cadastrada.
void inicializar_rede(Rede* r) {                 // Recebe um ponteiro para a rede que sera zerada
    int i, j;                                    // Variaveis de controle dos dois lacos (linha e coluna)
    r->num_locais = 0;                           // Define que a rede comeca sem nenhum local cadastrado
    for (i = 0; i < MAX_LOCAIS; i++) {           // Percorre todas as posicoes possiveis de locais
        r->nomes[i][0] = '\0';                   // Esvazia o nome do local i (string vazia)
        for (j = 0; j < MAX_LOCAIS; j++) {       // Percorre todas as colunas (possiveis destinos)
            r->matriz[i][j] = SEM_CONEXAO;       // Marca que nao existe estrada entre i e j
        }
    }
}

// Verifica se um indice de local realmente existe na rede.
bool indice_valido(Rede* r, int indice) {        // Recebe a rede e o indice que se quer validar
    return (indice >= 0 && indice < r->num_locais); // Verdadeiro se o indice esta dentro do intervalo valido
}

// Define quantos locais a rede tera e da nomes padrao ("Local 0", "Local 1"...).
void definir_locais(Rede* r, int quantidade) {   // Recebe a rede e quantos locais devem existir
    int i;
    if (quantidade < 1) quantidade = 1;          // Garante o minimo de 1 local (evita valor invalido)
    if (quantidade > MAX_LOCAIS) quantidade = MAX_LOCAIS; // Limita ao maximo suportado pela rede

    inicializar_rede(r);                         // Zera a rede antes de configurar os novos locais
    r->num_locais = quantidade;                  // Registra a quantidade de locais escolhida
    for (i = 0; i < quantidade; i++) {           // Percorre cada local que sera criado
        sprintf(r->nomes[i], "Local %d", i);     // Gera um nome padrao do tipo "Local 0", "Local 1"...
    }
}

// Altera o nome de um local especifico.
bool definir_nome_local(Rede* r, int indice, const char* nome) { // Recebe a rede, o indice do local e o novo nome
    if (!indice_valido(r, indice)) return false; // Se o indice nao existe, nao altera e retorna falso
    strncpy(r->nomes[indice], nome, 49);         // Copia ate 49 caracteres do nome para o local
    r->nomes[indice][49] = '\0';                 // Garante o terminador nulo na ultima posicao (string segura)
    return true;
}

// Insere uma estrada (aresta) com peso entre dois locais.
// Se mao_dupla = true, a estrada vale nos dois sentidos.
bool adicionar_aresta(Rede* r, int origem, int destino, int peso, bool mao_dupla) { // Parametros da nova estrada
    if (!indice_valido(r, origem) || !indice_valido(r, destino)) { // Verifica se origem e destino existem
        printf("\n[!] Local de origem ou destino invalido.\n");
        return false;
    }
    if (origem == destino) {                     // Impede uma estrada que liga um local a ele mesmo
        printf("\n[!] Origem e destino nao podem ser o mesmo local.\n");
        return false;
    }
    if (peso <= 0) {                             // O peso (distancia/tempo) precisa ser positivo
        printf("\n[!] O peso (distancia/tempo) deve ser maior que zero.\n");
        return false;
    }

    r->matriz[origem][destino] = peso;           // Registra a estrada da origem para o destino com o peso
    if (mao_dupla) {                             // Se a estrada for de mao dupla...
        r->matriz[destino][origem] = peso;       // ...registra tambem o sentido contrario com o mesmo peso
    }
    return true;
}

// Carrega uma rede de exemplo pronta para testes/demonstracao na defesa.
void carregar_rede_exemplo(Rede* r) {
    definir_locais(r, 5);                        // Cria 5 locais com nomes padrao
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
    if (r->num_locais == 0) {                    // Se nao ha locais cadastrados, avisa e encerra
        printf("\n[!] Nenhum local cadastrado ainda.\n");
        return;
    }
    printf("\n--- LOCAIS DA REDE ---\n");
    for (i = 0; i < r->num_locais; i++) {        // Percorre cada local cadastrado
        printf("  [%d] %s\n", i, r->nomes[i]);   // Mostra o indice e o nome do local
    }
}

// Exibe a matriz de adjacencia (a "rede de distribuicao").
void exibir_matriz(Rede* r) {
    int i, j;
    if (r->num_locais == 0) {                    // Se a rede esta vazia, avisa e encerra
        printf("\n[!] Rede vazia. Defina os locais primeiro.\n");
        return;
    }

    listar_locais(r);                            // Mostra antes a legenda de indices/nomes dos locais

    printf("\n--- MATRIZ DE ADJACENCIA (pesos = distancia/tempo) ---\n");
    printf("      ");                            // Espaco para alinhar o cabecalho das colunas
    for (j = 0; j < r->num_locais; j++) {        // Percorre as colunas para imprimir os indices no topo
        printf("%5d", j);
    }
    printf("\n");

    for (i = 0; i < r->num_locais; i++) {        // Percorre cada linha (local de origem)
        printf("%3d | ", i);                     // Imprime o indice da linha (origem) e um separador
        for (j = 0; j < r->num_locais; j++) {    // Percorre as colunas (locais de destino)
            if (r->matriz[i][j] == SEM_CONEXAO) { // Se nao existe estrada direta entre i e j...
                printf("    .");   // ponto = sem estrada direta
            } else {
                printf("%5d", r->matriz[i][j]);  // ...senao imprime o peso (distancia/tempo) da estrada
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
void bfs(Rede* r, int inicio) {                  // Recebe a rede e o local de partida da busca
    int fila[MAX_LOCAIS];                        // Fila que guarda os locais a serem visitados
    int visitado[MAX_LOCAIS];                    // Marca quais locais ja foram visitados (1) ou nao (0)
    int frente = 0, tras = 0;                    // Indices da fila: frente (remove) e tras (insere)
    int i, atual;                                // i = controle do laco; atual = local sendo processado
    int total_alcancados = 0;                    // Conta quantos locais foram efetivamente alcancados

    if (!indice_valido(r, inicio)) {             // Verifica se o local de inicio existe
        printf("\n[!] Local de inicio invalido.\n");
        return;
    }

    for (i = 0; i < r->num_locais; i++) visitado[i] = 0; // Marca todos os locais como ainda nao visitados

    // Marca e enfileira o local de partida
    visitado[inicio] = 1;                        // Marca o local de inicio como visitado
    fila[tras++] = inicio;                       // Coloca o local de inicio na fila e avanca o fim da fila

    printf("\n--- SIMULACAO DE ENTREGA (BFS) a partir de '%s' ---\n", r->nomes[inicio]);
    printf("Ordem de visita (expansao das entregas):\n");

    while (frente < tras) {                       // Enquanto houver locais na fila para processar...
        atual = fila[frente++];                   // Retira o proximo local da frente da fila
        printf("  -> %s\n", r->nomes[atual]);     // Mostra o local que esta sendo visitado
        total_alcancados++;                       // Incrementa o contador de locais alcancados

        // Enfileira todos os vizinhos ainda nao visitados
        for (i = 0; i < r->num_locais; i++) {     // Percorre todos os locais procurando vizinhos de 'atual'
            if (r->matriz[atual][i] != SEM_CONEXAO && !visitado[i]) { // Se ha estrada e ainda nao foi visitado
                visitado[i] = 1;                  // Marca o vizinho como visitado
                fila[tras++] = i;                 // Enfileira o vizinho para ser processado depois
            }
        }
    }

    printf("\nTotal de locais alcancaveis a partir de '%s': %d de %d.\n",
           r->nomes[inicio], total_alcancados, r->num_locais);

    // Avisa quais locais NAO podem ser alcancados (ilhas na rede)
    if (total_alcancados < r->num_locais) {       // Se nem todos os locais foram alcancados...
        printf("Locais NAO alcancaveis:");
        for (i = 0; i < r->num_locais; i++) {
            if (!visitado[i]) printf(" %s;", r->nomes[i]); // Lista os que nao foram visitados
        }
        printf("\n");
    }
}

// ======================= 4. MENOR ROTA (DIJKSTRA) =======================
// Responde: "Qual o caminho mais curto para entregar de uma origem ate um destino?"
// Calcula a menor distancia da origem para TODOS os locais e reconstroi o caminho.
void dijkstra(Rede* r, int origem, int destino) { // Recebe a rede, o local de origem e o de destino
    int dist[MAX_LOCAIS];       // menor distancia conhecida da origem ate cada local
    int anterior[MAX_LOCAIS];   // local anterior no melhor caminho (para reconstruir a rota)
    int visitado[MAX_LOCAIS];   // Marca os locais ja "fechados" (distancia definitiva calculada)
    int i, j, u, menor;         // i,j = controle dos lacos; u = local escolhido; menor = menor distancia atual

    if (!indice_valido(r, origem)) {              // Verifica se o local de origem existe
        printf("\n[!] Local de origem invalido.\n");
        return;
    }

    // Inicializacao
    for (i = 0; i < r->num_locais; i++) {         // Percorre todos os locais para inicializar os vetores
        dist[i] = INFINITO;                       // Comeca com distancia "infinita" (ainda desconhecida)
        anterior[i] = -1;                         // Ainda nao ha local anterior no caminho
        visitado[i] = 0;                          // Nenhum local foi fechado ainda
    }
    dist[origem] = 0;                             // A distancia da origem ate ela mesma e zero

    // Loop principal: a cada passo "fecha" o local nao visitado mais proximo
    for (i = 0; i < r->num_locais; i++) {         // Repete uma vez para cada local da rede
        u = -1;                                   // Ainda nao escolhemos o proximo local
        menor = INFINITO;                         // Comeca buscando a menor distancia como "infinita"
        for (j = 0; j < r->num_locais; j++) {     // Procura, entre os nao visitados, o de menor distancia
            if (!visitado[j] && dist[j] < menor) { // Se j nao foi fechado e tem distancia menor que a atual
                menor = dist[j];                  // Atualiza a menor distancia encontrada
                u = j;                            // Guarda esse local como o escolhido
            }
        }

        if (u == -1) break;  // restantes sao inalcancaveis
        visitado[u] = 1;                          // Marca o local escolhido como fechado (definitivo)

        // Relaxa as arestas saindo de u
        for (j = 0; j < r->num_locais; j++) {     // Percorre os possiveis vizinhos de u
            if (r->matriz[u][j] != SEM_CONEXAO && !visitado[j]) { // Se ha estrada u->j e j nao foi fechado
                int nova = dist[u] + r->matriz[u][j]; // Calcula a distancia passando por u
                if (nova < dist[j]) {             // Se esse caminho e melhor que o conhecido...
                    dist[j] = nova;               // ...atualiza a menor distancia ate j
                    anterior[j] = u;              // ...e registra que se chega em j vindo de u
                }
            }
        }
    }

    // Relatorio das distancias minimas para todos os locais
    printf("\n--- MENOR ROTA (DIJKSTRA) a partir de '%s' ---\n", r->nomes[origem]);
    printf("Distancias minimas para todos os locais:\n");
    for (i = 0; i < r->num_locais; i++) {         // Percorre cada local para mostrar sua distancia minima
        if (dist[i] == INFINITO) {                // Se o local ficou inalcancavel...
            printf("  %-15s : inalcancavel\n", r->nomes[i]);
        } else {
            printf("  %-15s : %d\n", r->nomes[i], dist[i]); // ...senao mostra o nome e a distancia minima
        }
    }

    // Caminho detalhado ate um destino especifico (se informado)
    if (indice_valido(r, destino)) {              // So detalha o caminho se o destino for valido
        printf("\n--- CAMINHO ATE '%s' ---\n", r->nomes[destino]);
        if (dist[destino] == INFINITO) {          // Se o destino nao pode ser alcancado...
            printf("Nao existe rota de '%s' ate '%s'.\n",
                   r->nomes[origem], r->nomes[destino]);
        } else {
            // Reconstroi o caminho de tras para frente usando o vetor 'anterior'
            int caminho[MAX_LOCAIS];              // Vetor para armazenar a sequencia de locais do caminho
            int tam = 0;                          // Quantidade de locais guardados no caminho
            int atual = destino;                  // Comeca a reconstrucao a partir do destino
            while (atual != -1) {                 // Enquanto houver um local anterior valido...
                caminho[tam++] = atual;           // Guarda o local atual no vetor caminho
                atual = anterior[atual];          // Recua para o local anterior no melhor caminho
            }

            printf("Distancia minima: %d\n", dist[destino]); // Mostra a distancia total da rota
            printf("Caminho percorrido: ");
            for (i = tam - 1; i >= 0; i--) {      // Percorre o caminho de tras para frente (origem -> destino)
                printf("%s", r->nomes[caminho[i]]); // Imprime o nome do local atual do caminho
                if (i > 0) printf(" -> ");        // Coloca uma seta entre os locais (menos no ultimo)
            }
            printf("\n");
        }
    }
}
