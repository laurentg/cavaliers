
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 6
#define N_MOVES 8

typedef struct {
	int curr_x;
	int curr_y;
	int visited[N][N];
	int num_visited;
} t_board;

typedef struct {
	int delta_x;
	int delta_y;
} t_move;

const t_move MOVES[N_MOVES] = {
	{ 1, 2 },
	{ 1, -2 },
	{ 2, 1 },
	{ 2, -1 },
	{ -1, 2 },
	{ -1, -2 },
	{ -2, 1 },
	{ -2, -1 }
};

void print_board(t_board *b) {
	static int printed = 0;
	if (printed)
		return;
	printed = 1;
	int x, y;
	fprintf(stdout, "\n");
	for (x = 0; x < N; x++) {
		fprintf(stdout, " | ");
		for (y = 0; y < N; y++) {
			fprintf(stdout, "%2d ", b->visited[x][y]);
		}
		fprintf(stdout, "|\n");
	}
	fprintf(stdout, " ---\n");
	//exit(0);
}

int num_solution(int level, t_board *b) {
	static int ntotal = 0;
	static int nfound = 0;
	t_board sb[N_MOVES];
	t_board *b2;
	int i, x, y, n = 0, retval = 0;
	ntotal++;
	if (ntotal % 0x10000 == 0) {
		fprintf(stdout, "%d/%d\r", ntotal, nfound);
	}
	if (b->num_visited == N * N) {
		nfound++;
		print_board(b);
		return 1;
	}
	for (i = 0; i < N_MOVES; i++) {
		x = b->curr_x + MOVES[i].delta_x;
		y = b->curr_y + MOVES[i].delta_y;
		if (x < 0 || y < 0 || x >= N || y >= N)
			continue;
		if (b->visited[x][y])
			continue;
		b2 = &(sb[n]);
		memcpy(b2, b, sizeof(t_board));
		b2->curr_x = x;
		b2->curr_y = y;
		b2->num_visited++;
		b2->visited[x][y] = b2->num_visited;
		retval += num_solution(level + 1, b2);
		n++;
	}
	return retval;
}

int main(int argc, char*argv[]) {

	t_board board;
	int ns;
	memset(&board, '\0', sizeof(t_board));
	board.visited[board.curr_x][board.curr_y] = 1;
	board.num_visited = 1;
	ns = num_solution(0, &board);
	fprintf(stderr, "%d\n", ns);
	return 0;
}
