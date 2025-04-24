#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int comparar(const void *a, const void *b) {
    Cidade *c1 = (Cidade *)a;
    Cidade *c2 = (Cidade *)b;
    return c1->Posicao - c2->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL;

    Estrada *estrada = malloc(sizeof(Estrada));
    if (!estrada) {
        fclose(arquivo);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &estrada->T) != 1 ||
        fscanf(arquivo, "%d", &estrada->N) != 1) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    if (estrada->T < 3 || estrada->T > 1000000 || estrada->N < 2 || estrada->N > 10000) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    estrada->C = malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        Cidade *cidade = estrada->C + i;
        if (fscanf(arquivo, "%d %[^\n]", &cidade->Posicao, cidade->Nome) != 2 ||
            cidade->Posicao <= 0 || cidade->Posicao >= estrada->T) {
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }

        for (int j = 0; j < i; j++) {
            if ((estrada->C + j)->Posicao == cidade->Posicao) {
                free(estrada->C);
                free(estrada);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    qsort(estrada->C, estrada->N, sizeof(Cidade), comparar);
    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1;

    Cidade *cidades = estrada->C;
    int n = estrada->N;

    double menor = estrada->T; 

    for (int i = 0; i < n; i++) {
        double esquerda = (i == 0) ? 0 : (cidades[i].Posicao + cidades[i - 1].Posicao) / 2.0;
        double direita  = (i == n - 1) ? estrada->T : (cidades[i].Posicao + cidades[i + 1].Posicao) / 2.0;
        double vizinhanca = direita - esquerda;
        if (vizinhanca < menor)
            menor = vizinhanca;
    }

    free(estrada->C);
    free(estrada);
    return menor;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    Cidade *cidades = estrada->C;
    int n = estrada->N;
    double *vizinhas = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) {
        double esquerda = (i == 0) ? 0 : (cidades[i].Posicao + cidades[i - 1].Posicao) / 2.0;
        double direita  = (i == n - 1) ? estrada->T : (cidades[i].Posicao + cidades[i + 1].Posicao) / 2.0;
        vizinhas[i] = direita - esquerda;
    }

    int menorIndex = 0;
    for (int i = 1; i < n; i++) {
        if (vizinhas[i] < vizinhas[menorIndex]) {
            menorIndex = i;
        }
    }

    char *nome = strdup(cidades[menorIndex].Nome);

    free(estrada->C);
    free(estrada);
    free(vizinhas);
    return nome;
}
