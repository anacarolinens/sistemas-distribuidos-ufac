#include <stdio.h>  // Biblioteca padrão de entrada/saída em C para usar funções como printf()
#include <stdlib.h> // Biblioteca padrão em C para usar funções como malloc() (alocação dinâmica de memória)
#include "mpi.h"    // Biblioteca para computação paralela com Message Passing Interface (MPI)

//Utilizando MPI em C, faça um programa de soma de matrizes 5x5, onde cada um dos 5
//processos criados fará a soma de uma das linhas da matriz. No fim o processo ZERO deve
//imprimir em tela o resultado da soma.

int main(int argc, char** argv) // Inicio da função principal
{
    // Declarando variáveis
    int meu_rank, tag = 0; // 'meu_rank' armazenará o identificador do processo, 'tag' é um identificador de mensagem para MPI
    int a[5], b[5], matc[5][5], i, j, r[5]; // Arrays 'a' e 'b' serão usados para trocar informações entre processos. 'matc' será a matriz resultante. 'r' é um array temporário. 'i' e 'j' são contadores.
    MPI_Status status; // Objeto que armazenará o status das operações de recebimento

    MPI_Init(&argc, &argv); // Inicializando o MPI. Deve ser chamada antes de qualquer outra rotina do MPI.
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank); // Descobrindo o "rank" (ou seja, ID) do processo que está sendo executado.

    // O processo principal (rank 0) é responsável por enviar dados para os outros processos e recolher seus resultados.
    if (meu_rank == 0) {

        // As matrizes a serem somadas
        int mata[5][5] = {{1,2,3,4,5},{1,0,2,1,2},{2,1,0,1,2},{1,2,1,0,2},{2,1,2,1,0}};
        int matb[5][5] = {{3,4,5,1,2},{2,1,3,2,1},{1,2,1,3,2},{2,1,2,3,1},{1,2,3,4,5}};

        // Enviando os dados para os outros processos
        for (j = 1; j < 6; j++) {
            // Copiando linha 'j-1' das matrizes para os arrays 'a' e 'b'
            for (i = 0; i < 5; i++) {
                a[i] = mata[j-1][i];
                b[i] = matb[j-1][i];
            }
            // Enviando as linhas para o processo 'j'
            MPI_Send(a, 5, MPI_INT, j, tag, MPI_COMM_WORLD);
            MPI_Send(b, 5, MPI_INT, j, tag, MPI_COMM_WORLD);
        }

        // Recolhendo os resultados dos processos
        for (j = 1; j < 6; j++) {
            MPI_Recv(r, 5, MPI_INT, j, tag, MPI_COMM_WORLD, &status);
            // Colocando o resultado na matriz 'matc'
            for(i = 0; i < 5; i++)
                matc[j-1][i] = r[i];
        }

        // Imprimindo a matriz resultante
        for(i = 0; i < 5; i++){
            for(j = 0; j < 5; j++)
                printf("%d ", matc[i][j]);
            printf("\n");
        }
    }
    else { // Para os outros processos (ranks != 0)
        // Recebendo os dados do processo principal
        MPI_Recv(a, 5, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(b, 5, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        // Realizando a soma e armazenando o resultado em 'r'
        for (i = 0; i < 5; i++)
             r[i] = a[i] + b[i];

        // Enviando o resultado de volta para o processo principal
        MPI_Send(r, 5, MPI_INT, 0, tag, MPI_COMM_WORLD);// Envia a mensagem 'r' para o processo mestre (rank 0) no grupo global de processos (MPI_COMM_WORLD). 
        // A mensagem consiste em 5 elementos de tipo inteiro (MPI_INT), e usa a 'tag' como identificador.

    }

    MPI_Finalize(); // Finalizando o MPI. Depois dessa chamada, nenhuma outra rotina do MPI pode ser usada.
    return 0; // Indicando que o programa finalizou com sucesso
}