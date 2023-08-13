#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char** argv)
{
        //Faça um programa que calcule o quadrado da soma (a + b)2 , em 4
        //processos: o processo ZERO envia “a” para o processo 1, “a” e “b”
        //para o processo 2 e “b” para o processo 3. No final cada processo
        //responde com a operação equivalente.



        int meu_rank, tag=0;
        int  a, b, quad_soma, quad_primeiro, dois_ps, quad_segundo;
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);


        if (meu_rank == 0) {
                a = 2;
                b = 5;
                //Enviando a para ó processo 1
                MPI_Send(&a, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);

                //Enviando a e b para o processo 2
                MPI_Send(&a, 1, MPI_INT, 2, tag, MPI_COMM_WORLD);
                MPI_Send(&b, 1, MPI_INT, 2, tag, MPI_COMM_WORLD);

                //Enviando b para o processo 3
                MPI_Send(&b, 1, MPI_INT, 3, tag, MPI_COMM_WORLD);

                //Recebendo o quadrado do primeiro  do processo 1
                MPI_Recv(&quad_primeiro, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);

                //Recebendo o duas vezes o primeiro vezes o segundo do processo 2
                MPI_Recv(&dois_ps, 1, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);

                //Recebendo o quadrado do segundo do pricesso 3
                MPI_Recv(&quad_segundo, 1, MPI_INT, 3, tag, MPI_COMM_WORLD, &status);


                //Realizando o quadrado da soma
                quad_soma = quad_primeiro + dois_ps  + quad_segundo;

                //Imprimindo o quadrado da soma
                printf("O quadrado da soma é: %d \n", quad_soma);


        }else if (meu_rank == 1) {
                //Recebenco a
                MPI_Recv(&a, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

                // Fazendo o quadrado do a
                quad_primeiro = a * a;

                //Enviando para o processo 0 o quadrado do primeiro
                MPI_Send(&quad_primeiro, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

        }else if(meu_rank == 2) {
                //Recebenco a
                MPI_Recv(&a, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

                //Recebendo b
                MPI_Recv(&b, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

                // Fazendo o quadrado do a
                dois_ps  = 2 * (a * b);

                ///Enviando para o processo 0 a multiplicação de duas vezes
                // o primeiro pelo segundo
                MPI_Send(&dois_ps, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }else if(meu_rank == 3) {
                //Recebendo b
                MPI_Recv(&b, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

                 // Fazendo o quadrado do a
                quad_segundo = b * b;

                //Enviando para o processo 0 o quadrado do segundo
                MPI_Send(&quad_segundo, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }

        MPI_Finalize( );
}


