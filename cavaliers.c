
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Size of the chessboard */
#define N 6
/* Maximal number of moves allowed for a knight.
   Do not even think of modifying this constant :) */
#define N_MOVES 8

/* A chessboard
   TODO: Check if an array of char would be faster (slower indexing but
   faster memcpy) */
typedef struct {
	/* The current position of the knight */
	int curr_x;
	int curr_y;
	/* The order of the visit if visited (starting at 1) or 0 if not.  */
	int visited[N][N];
	/* Total number of visited square, to quickly check if search ended. */
	int num_visited;
} t_board;

/* A single knight movement. */
typedef struct {
	int delta_x;
	int delta_y;
} t_move;

/* The table of allowed knight movements, in X-Y delta relative to the
   current position. */
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

/* Display a board to standard output. */
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
	/* Disable this for existing at first found solution. */
	/* exit(0); */
}

/* Iteratively scan a board. Loop for each possible movements and
   recursively call itself with the new board. Return the number
   of solutions found so far in this scan tree branch. */
int num_solution(int level, t_board *b) {
	static int ntotal = 0;
	static int nfound = 0;
	t_board sb[N_MOVES];
	/* With a clever compiler, we should not need to have to keep a
	   pointer on the current working board, but it's safer to not
	   make any assumption on how clever cc is. */
	t_board *b2;
	int i, x, y, n = 0, retval = 0;
	ntotal++;
	/* Do not print too often, we want to be fast. */
	if (ntotal % 0x10000 == 0) {
		fprintf(stdout, "%d/%d\r", ntotal, nfound);
	}
	if (b->num_visited == N * N) {
		nfound++;
		print_board(b);
		return 1;
	}
	/* Q: Why not keeping a single compound XY index? Array indexing
	   and movement delta computation would have been faster.
	   A: Because the chessboard out-of-bound checks would have been
	   made more complex, total CPU time would have been higher. */
	for (i = 0; i < N_MOVES; i++) {
		x = b->curr_x + MOVES[i].delta_x;
		y = b->curr_y + MOVES[i].delta_y;
		if (x < 0 || y < 0 || x >= N || y >= N)
			continue;
		if (b->visited[x][y])
			continue;
		b2 = &(sb[n]);
		/* This memcpy should be rather fast.
		   But see note on struct t_board. */
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
	/* Start at the corner (0,0). */
	board.visited[board.curr_x][board.curr_y] = 1;
	board.num_visited = 1;
	ns = num_solution(0, &board);
	fprintf(stderr, "%d\n", ns);
	return 0;
}
