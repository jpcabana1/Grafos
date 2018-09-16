//Aluno: João Pedro de Melo Cabana
//Turno: Noite
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <queue>

//numero maximo de vertices
#define  MAX_NUM_VERT  50


typedef enum cores {
	BRANCO = 0, CINZA = 1, PRETO = 2
}cores;

//entrada da lista de adjacencias
struct no {
	int vertice;
	no *prox;
};

// variaveis globais que representam o grafo
int numVertices = 0;
no *lista_adj[MAX_NUM_VERT];


// prototipos das funcoes auxiliares
void inicializa_grafo(no* adj[]);
void insere_adjacente(no *adj[], int v, int sucessor);
void desaloca_grafo(no* adj[], int n);
void ler_grafo(const char caminho[]);
void imprime_grafo(no* adj[], int n);
void converter(no *adj[], int n);
void defineGrau(no *adj[], int n);
void BFS(no *adj[], int n);

// Funcao principal (ponto de entrada do programa).
int main() {
	
	
	//inicializa o grafo
	inicializa_grafo(lista_adj);

	//le grafo a partir do arquivo
	ler_grafo("grafo1.txt");

	//imprime as listas de adjacencia do grafo
	imprime_grafo(lista_adj, numVertices);

	//converte a lista adjacente em uma matriz adjacente
	converter(lista_adj, numVertices);

	defineGrau(lista_adj, numVertices);
	//libera memoria

	BFS(lista_adj, numVertices);
	desaloca_grafo(lista_adj, numVertices);

	//espera digitar um caracter
	system("pause");
	return 0;
}

// Inicializa as listas de adjacencias.
void inicializa_grafo(no* adj[]) {
	for (int i = 0; i < MAX_NUM_VERT; i++) {
		adj[i] = NULL;
	}
}

// Insere o vertice "sucessor" no final da lista de adjacencias 
// de "v", ou seja, insere o arco (v -> sucessor)
void insere_adjacente(no *adj[], int v, int sucessor) {
	no* aux;
	no* novo;

	novo = new no;
	novo->vertice = sucessor;
	novo->prox = NULL;

	if (adj[v] == NULL) {
		// se for o primeiro elemento inserido na lista, 
		// a cabeça da lista passa a apontar para ele
		adj[v] = novo;

	}
	else {
		// se nao for o primeiro, percorre a lista inteira
		// e insere o elemento após a ultima posição
		aux = adj[v];

		while (aux->prox != NULL)
			aux = aux->prox;

		aux->prox = novo;
	}
}

// Le um grafo a partir do arquivo passado por parametro
void ler_grafo(const char caminho[100]) {
	FILE* arquivo;
	int v, sucessor;

	arquivo = fopen(caminho, "r");

	if (arquivo == NULL) {
		printf("Erro: nao foi possivel abrir o arquivo.\n");

	}
	else {
		//le a primeira linha		
		fscanf(arquivo, "%d", &numVertices);

		//le cada uma das demais linhas
		for (v = 0; v < numVertices; v++) {
			fscanf(arquivo, "%d", &sucessor);

			while (sucessor != -1) {
				insere_adjacente(lista_adj, v, sucessor);
				fscanf(arquivo, "%d", &sucessor);
			}
		}

		fclose(arquivo);
	}

}

// Imprime as listas de adjacencias dadas
void imprime_grafo(no* adj[], int n) {
	int i;
	no* aux;
	printf("Lista adjacente\n");
	for (i = 0; i < n; i++) {
		//imprime o vertice
		printf("Adj[%d] - ", i);

		//imprime os sucessores do vertice
		aux = adj[i];
		while (aux != NULL) {
			printf("%d->", aux->vertice);

			aux = aux->prox;
		}

		printf(".\n");
	}
}

// Percorre as listas desalocando memoria
void desaloca_grafo(no* adj[], int n) {
	int i;
	no* anterior;
	no* atual;

	for (i = 0; i < n; i++) {
		atual = adj[i];
		while (atual != NULL) {
			anterior = atual;
			atual = atual->prox;
			delete anterior;
		}
	}

}


void converter(no *adj[], int n) {
	no *aux;
	int i, j;
	printf("\nMatriz convertida\n");
	//Alocar matrix dinamicamente
	int **mat = (int **)malloc(n * sizeof(int*));
	for (i = 0; i < n; i++) mat[i] = (int *)malloc(n * sizeof(int));

	//Preencher com zeros
	for (i = 0; i < n;i++) {
		for (j = 0; j < n; j++)
		{
			mat[i][j] = 0;
		}
	}

	for (i = 0; i < n; i++) {
		aux = adj[i];
		while (aux != NULL) {
			mat[i][aux->vertice] = 1;
			aux = aux->prox;

		}
	}

	for (i = 0; i < n;i++) {
		for (j = 0; j < n; j++)
		{
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}

void defineGrau(no *adj[], int n) {
	int *vetor, count = 0, menor = 1, maior = menor;
	no* aux;
	vetor = (int*)malloc((n) * sizeof(int*));
	printf("\nVetor de graus:\n");

	for (int i = 0; i < n; i++) {
		aux = adj[i];
		while (aux != NULL)
		{
			count++;
			aux = aux->prox;
		}
		vetor[i] = count;
		printf("Vertice %d = %d\n", i, vetor[i]);
		count = 0;
	}


	for (int i = 0; i < n; i++) {

		if (menor > vetor[i]) menor = vetor[i];
		if (maior < vetor[i]) maior = vetor[i];
	}

	printf("\nMenor grau = %d\n", menor);
	printf("Maior grau = %d\n", maior);

}

void BFS(no *adj[], int n) {
	no *v;
	cores *c;
	int *d, *ante, u;
	std::queue<int> q;

	c = (cores*)malloc((n) * sizeof(cores*));
	d = (int*)malloc((n) * sizeof(int*));
	ante = (int*)malloc((n) * sizeof(int*));
	
	//Inicia com todos brancos
	for (int i = 0; i < n; i++) {
		c[i] = BRANCO;
		d[i] = 0;
		ante[i] = 0;
	}
	c[0] = CINZA;
	d[0] = 0;
	ante[0] = -1;

	q.push(0);
	while (!q.empty()) {
		u = q.front();
		q.pop();
		v = adj[u];
		
		while (v != NULL)
		{
			if (c[v->vertice] == BRANCO) {
				c[v->vertice] = CINZA;
				d[v->vertice] = d[u] + 1;
				ante[v->vertice] = u;
				q.push(v->vertice);
			}
			c[u] = PRETO;
			v = v->prox;
		}
	}
	printf("\nVetor D:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", d[i]);
	}
	printf("\nVetor C:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", c[i]);
	}

	printf("\nVetor ante:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", ante[i]);
	}
}
