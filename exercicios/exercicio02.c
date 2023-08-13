#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char** argv)
{
        //Fazer um programa em MPI que calcule a velocidade de um carro (V = D/T), a
        //distancia deve ser enviada por um processo e o tempo por outro processo. O
        //processo raíz deve efetuar o calculo e mostrar o resultado.


        int meu_rank, tag=0;
        float  d, t, v[2];
        double velocidade;
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);

        d  = 5;
        t  = 3;

        if (meu_rank == 0) {
                v[2];
                MPI_Recv(&v[0], 1, MPI_FLOAT, 1, tag, MPI_COMM_WORLD, &status);
                MPI_Recv(&v[1], 1, MPI_FLOAT, 2, tag, MPI_COMM_WORLD, &status);

                velocidade = (double)v[0]/v[1];

                printf("A velocidade do carro é: %.2lf\n", velocidade);


        }else if(meu_rank == 1) {
                v[0] = d;
                MPI_Send(&v[0], 1, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);

        }else if(meu_rank == 2) {
                v[1] = t;
                MPI_Send(&v[1], 1, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
        }

        MPI_Finalize( );
}
