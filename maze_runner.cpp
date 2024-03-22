#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <unistd.h> // Para utilização do usleep

using namespace std;

// Matriz de char representnado o labirinto
char** maze; 

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
    int i;
    int j;
};

// Estrutura de dados contendo as próximas posicões a serem exploradas no labirinto
stack<pos_t> valid_positions;

// Função que le o labirinto de um arquivo texto, carrega em memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
    pos_t initial_pos;
    // Abre o arquivo para leitura (fopen)
    FILE* file = fopen(file_name, "r"); 
    // Le o numero de linhas e colunas (fscanf) e salva em num_rows e num_cols
    fscanf(file, "%d %d", &num_rows, &num_cols);

    // Aloca a matriz maze (malloc)
    maze = (char**)malloc(num_rows * sizeof(char*)); 
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = (char*)malloc(num_cols * sizeof(char)); 
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            // Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]. Se o valor for 'e' salvar o valor em initial_pos
            fscanf(file, " %c", &maze[i][j]); 

            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
    }

    // Fecha o arquivo
    fclose(file); 
    return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    usleep(20000);
}

// Função responsável pela navegação. Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada.
bool walk(pos_t pos) {
    // Repita até que a saída seja encontrada ou não existam mais posições não exploradas
    while (!valid_positions.empty()) {
        bool exitFound = false;

        // Marcar a posição atual com o caractere 'o'
        maze[pos.i][pos.j] = 'o';

        system("clear");
        print_maze();

        // Verificação do caminho à direita
        if (pos.j < num_cols - 1) {
            if (maze[pos.i][pos.j + 1] == 's') {
                exitFound = true;
            } else if (maze[pos.i][pos.j + 1] == 'x') {
                pos_t next_pos = {pos.i, pos.j + 1};
                valid_positions.push(next_pos);
            }
        }
        
        // Verificação do caminho à esquerda
        if (pos.j > 0) {
            if (maze[pos.i][pos.j - 1] == 's') {
                exitFound = true;
            } else if (maze[pos.i][pos.j - 1] == 'x') {
                maze[pos.i][pos.j] = 'o';
                pos_t next_pos = {pos.i, pos.j - 1};
                valid_positions.push(next_pos);
            }
        }

        // Verificação do caminho acima
        if (pos.i > 0) {
            if (maze[pos.i - 1][pos.j] == 's') {
                exitFound = true;
            } else if (maze[pos.i - 1][pos.j] == 'x') {
                pos_t next_pos = {pos.i - 1, pos.j};
                valid_positions.push(next_pos);
            }
        }

        // Verificação do caminho abaixo
        if (pos.i < num_rows - 1) {
            if (maze[pos.i + 1][pos.j] == 's') {
                exitFound = true;
            } else if (maze[pos.i + 1][pos.j] == 'x') {
                pos_t next_pos = {pos.i + 1, pos.j};
                valid_positions.push(next_pos);
            }
        }
        
        if (!valid_positions.empty()) {
            pos = valid_positions.top();
            valid_positions.pop();
        }
        
        if (exitFound) 
            return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    // Verifica se o número de argumentos está correto
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <arquivo do labirinto>" << endl;
        return EXIT_FAILURE;
    }

    // Carregar o labirinto com o nome do arquivo recebido como argumento
    pos_t initial_pos = load_maze(argv[1]);
    valid_positions.push(initial_pos);
    
    // Chamar a função de navegação
    bool exit_found = walk(initial_pos);
    
    // Tratamento de erro
    if (exit_found)
        printf("Exit found!\n");
    else
        printf("Exit not found.\n");

    return 0;
}

