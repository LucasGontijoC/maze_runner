#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

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

bool exitFound = false;

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
}
// Função que imprime o labirinto com delay
void print_maze_delay()
{
	while (!exitFound)
	{
		system("clear");
		printf("%i %i\n", num_rows, num_cols);
		print_maze();
		this_thread::sleep_for(chrono::milliseconds(50)); // Sleep for 500ms
	}
}

// Função responsável pela navegação. Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada.
void walk(pos_t pos) {

    stack<pos_t> valid_positions;
	pos_t next_pos;
	valid_positions.push(pos);

    // Repita até que a saída seja encontrada ou não existam mais posições não exploradas
    while (!valid_positions.empty()) {
        valid_positions.pop();

        // Marcar a posição atual com o caractere 'o'
        maze[pos.i][pos.j] = 'o';
        this_thread::sleep_for(chrono::milliseconds(50));

        maze[pos.i][pos.j] = '.';

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
        
		// Verifica o tamanho da pilha e cria threads para explorar as próximas posições
		while (valid_positions.size() > 0)
		{
			next_pos = valid_positions.top();
			valid_positions.pop();
			thread f(walk, next_pos);
			f.detach();
		}
        
    
    }
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
    // Cria thread detached
    thread t(walk, initial_pos);
	t.detach();
    //Imprime o labirinto
	print_maze_delay();

    // Tratamento de erro
    if (exitFound)
        printf("Exit found!\n");
    else
        printf("Exit not found.\n");

    return 0;
}