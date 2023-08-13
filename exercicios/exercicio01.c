#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char** argv)
{
        //Fazer um programa em MPI que utilize 3 processos, os processos 1 e 2
        //devem enviar um valor inteiro para o processo 0. O processo 0 deve
        //efetuar as 4 operações básicas com os inteiros recebidos (+,-.*,/),
        //após isso deve imprimir os resultados.


        int meu_rank, tag=0, p1, p2;
        int p3[2], soma, subtracao, mutiplicacao;
        double divisao;
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);

        p1 = 5;
        p2 = 3;

        if (meu_rank == 0) {
                p3[2];
                MPI_Recv(&p3[0], 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
                MPI_Recv(&p3[1], 1, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);
                soma = p3[0] + p3[1];
                subtracao = p3[0] - p3[1];
                mutiplicacao = p3[0] * p3[1];
                divisao = (double)p3[0]/p3[1];

                printf("Soma: %d\n", soma);
                printf("Subtração: %d\n", subtracao);
                printf("Multiplicação: %d\n", mutiplicacao);
                printf("Divisao: %.2lf\n", divisao);

        }else if(meu_rank == 1) {
                p3[0] = p1;
                MPI_Send(&p3[0], 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

        }else if(meu_rank == 2) {
                p3[1] = p2;
                MPI_Send(&p3[1], 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }

        MPI_Finalize( );
}
