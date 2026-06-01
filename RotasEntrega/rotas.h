#ifndef ROTAS_H
#define ROTAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ======================= CONSTANTES =======================
#define MAX_LOCAIS 50      // Numero maximo de locais (vertices) suportados
#define SEM_CONEXAO 0      // Valor na matriz quando nao existe estrada entre dois locais
#define INFINITO 1000000000 // "Infinito" usado pelo Dijkstra (distancia ainda nao calculada)

// ======================= ESTRUTURAS DE DADOS =======================
// A rede de entregas e representada por um GRAFO usando MATRIZ DE ADJACENCIA.
// - Cada LOCAL (centro, bairro, ponto logistico) e um VERTICE.
// - Cada ESTRADA entre dois locais e uma ARESTA com PESO (distancia/tempo).
typedef struct {
    int num_locais;                          // Quantidade de locais cadastrados
    char nomes[MAX_LOCAIS][50];              // Nome de cada local (ex: "Centro", "Bairro A")
    int matriz[MAX_LOCAIS][MAX_LOCAIS];      // matriz[i][j] = peso da estrada de i para j (0 = sem estrada)
} Rede;

// ======================= PROTOTIPOS DAS FUNCOES =======================

// --- Configuracao da rede ---
void inicializar_rede(Rede* r);
void definir_locais(Rede* r, int quantidade);
bool definir_nome_local(Rede* r, int indice, const char* nome);
bool adicionar_aresta(Rede* r, int origem, int destino, int peso, bool mao_dupla);
void carregar_rede_exemplo(Rede* r);

// --- Visualizacao ---
void exibir_matriz(Rede* r);
void listar_locais(Rede* r);
bool indice_valido(Rede* r, int indice);

// --- Algoritmos de rotas ---
void bfs(Rede* r, int inicio);
void dijkstra(Rede* r, int origem, int destino);

#endif
