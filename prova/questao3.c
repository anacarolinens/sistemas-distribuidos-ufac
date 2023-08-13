#include <math.h>       // Inclui a biblioteca math.h para funções matemáticas.
#include "mpi.h"        // Inclui a biblioteca MPI (Message Passing Interface) para programação paralela. 

//Desenvolva uma aplicação paralela em MPI para multiplicar duas matrizes 3x3.

int main (int argc, char** argv) {  // Função principal que inicia a execução do programa.
    int meu_rank, tag=0;   // Declaração de variáveis inteiras: 'meu_rank' representa o número do processo, 'tag' é uma marcação para as mensagens.
    int i, j, k;           // Variáveis de controle para loops.
    MPI_Status status;     // Estrutura que contém informações sobre o status de uma operação de comunicação.

    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI.

    MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);  // Obtém o número do processo atual.

    if (meu_rank == 0) {   // Trecho de código que será executado apenas pelo processo com rank 0.
        int mata[3][3]={{1,2,3},{1,0,1},{6,6,6}};  // Matriz 'mata' de dimensão 3x3 inicializada.
        int matb[3][3]={{3,4,5},{2,1,2},{3,3,3}};  // Matriz 'matb' de dimensão 3x3 inicializada.
        int matc[3][3] = {0};  // Matriz 'matc' de dimensão 3x3 inicializada com zeros.

        for (i = 1; i < 4; i++) {  // Loop para enviar as matrizes 'mata' e 'matb' para os processos 1, 2 e 3.
            MPI_Send(mata, 9, MPI_INT, i, tag, MPI_COMM_WORLD);  // Envia a matriz 'mata' ao processo i.
            MPI_Send(matb, 9, MPI_INT, i, tag, MPI_COMM_WORLD);  // Envia a matriz 'matb' ao processo i.
        }

        for (i = 1; i < 4; i++) {  // Loop para receber os resultados dos processos 1, 2 e 3.
            for (j = 0; j < 3; j++) {  // Loop para preencher a matriz 'matc' com os resultados recebidos.
                int result;
                MPI_Recv(&result, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);  // Recebe um resultado do processo i.
                matc[i-1][j] = result;  // Armazena o resultado na matriz 'matc'.
            }
        }

        for(i = 0; i < 3; i++) {  // Loop para imprimir a matriz resultante 'matc'.
            for(j = 0; j < 3; j++)
                printf("%d\t", matc[i][j]);
            printf("\n");
        }
    }
    else {  // Trecho de código que será executado pelos processos com rank 1, 2 e 3.
        int mata[3][3], matb[3][3];  // Declaração das matrizes locais 'mata' e 'matb'.
        MPI_Recv(mata, 9, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);  // Recebe a matriz 'mata' do processo 0.
        MPI_Recv(matb, 9, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);  // Recebe a matriz 'matb' do processo 0.

        for (j = 0; j < 3; j++) {  // Loop para realizar a multiplicação das matrizes locais 'mata' e 'matb'.
            int result = 0;
            for (k = 0; k < 3; k++)
                //Pega o indice com base no numero de processos, percorrendo a linha de mata e multiplica 
                //pela coluna de matb somando os resultados de cada coluna
                result += mata[meu_rank-1][k] * matb[k][j];  // Calcula o resultado da multiplicação.
            MPI_Send(&result, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);  // Envia o resultado ao processo 0.
        }
    }

    MPI_Finalize();  // Finaliza o ambiente MPI.

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso.
}
