#include <stdio.h>
#include <mpi.h>

//Construir um programa em C usando MPI que dado o grafo acima, envie uma
//informação partindo do nó 0(ZERO) e alcance todos os nos da rede de modo a formar uma
//árvore. No fim, cada nó deve imprimir a distância para cada um dos outros nós. (EX: O Nó
//2 tem distância 1 para o nó 5)

/*Definir o grafo da aplicação antes de executar*/
int numeroDeTarefas = 6;
int matrizVizinhanca[6][6] = {{0, 1, 1, 0, 0, 0},
                              {1, 0, 1, 1, 1, 0},
                              {1, 1, 0, 0, 0, 1},
                              {0, 1, 0, 0, 0, 0},
                              {0, 1, 0, 0, 0, 0},
                              {0, 0, 1, 0, 0, 0}};
/*retorna o número de vizinhos da tarefa myRank*/
int contaNumeroDeVizinhos(int myRank)
{
    int i;
    int contador = 0;

    for (i = 0; i < numeroDeTarefas; i++)
        if (matrizVizinhanca[myRank][i] == 1)
            contador++;

    return contador;
}
/*programa principal*/
int main(int argc, char **argv)
{
    int i;
    int numeroDeVizinhos;
    int myRank;
    int source;
    int tag = 50;
    int pai;
    int message[2]; // Nossa mensagem agora contém 2 valores, a origem e a distância
    MPI_Status status;
    int distancias[numeroDeTarefas];
    for (i = 0; i < numeroDeTarefas; i++)
    {
        distancias[i] = -1; // Inicializamos todas as distâncias como -1
    }
    // inicialização do MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    numeroDeVizinhos = contaNumeroDeVizinhos(myRank);
    if (myRank == 0)
    {
        distancias[myRank] = 0;
        /*enviando para todos os vizinhos de myRank*/
        for (i = 0; i < numeroDeTarefas; i++)
            if (matrizVizinhanca[myRank][i] == 1)
            {
                printf("Enviando a informação para %d\n", i);
                message[0] = myRank;
                message[1] = 1;
                MPI_Send(message, 2, MPI_INT, i, tag, MPI_COMM_WORLD);
            }

        /*recebendo de todos os vizinhos de myRank*/
        for (i = 0; i < numeroDeTarefas; i++)
            if (matrizVizinhanca[myRank][i] == 1)
            {
                printf("Recebendo a informação de %d\n", i);
                MPI_Recv(message, 2, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
            }
    }
    else
    {
        /*recebendo msg de uma tarefa vizinha qualquer*/
        MPI_Recv(message, 2, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        pai = status.MPI_SOURCE;
        distancias[message[0]] = message[1];
        /*enviando para todos os vizinhos de myRank menos seu pai*/
        for (i = 0; i < numeroDeTarefas; i++)
            if ((matrizVizinhanca[myRank][i] == 1) && (i != pai))
            {
                message[0] = 0;
                message[1] += 1;
                MPI_Send(message, 2, MPI_INT, i, tag, MPI_COMM_WORLD);
            }
        /*recebendo de todos os vizinhos de myRank menos 1*/
        for (i = 0; i < (numeroDeVizinhos - 1); i++)
        {
            MPI_Recv(message, 2, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            if (message[1] < distancias[message[0]] || distancias[message[0]] == -1)
            {
                distancias[message[0]] = message[1];
            }
        }
        message[0] = 0;
        message[1] = distancias[0];
        MPI_Send(message, 2, MPI_INT, pai, tag, MPI_COMM_WORLD);
    }
    // Após toda a comunicação, imprimimos as distâncias
    if (myRank != 0)
    {
        printf("Distâncias do nó %d:\n", myRank);
        for (i = 0; i < numeroDeTarefas; i++)
        {
            if (i != myRank)
            {
                printf("Distância para o nó %d: %d\n", i, distancias[i]);
            }
        }
    }
    // Finalização do MPI
    MPI_Finalize();
}

