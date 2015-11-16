
# Cavalier

This small C program is a brute-force solver of the chess "knight-walk" problem (I do not remember the exact name of this problem however).

Basically this boils down to finding a path for a knight on a chessboard in order to visit *each* square *once* and *only once*, starting from a corner of the chessboard.

The program will follow a brute-force approach: it will scan all possible movements and find one that fits. Each time the knight land on a square already visited, the search tree backtrack. The tree of movements is scanned using the stack; so a stack of at least 8x8xN is needed (N the size of the local stack; usually a single board and a few integers). This should not be a problem on modern machines :)

A possible optimization would have been to trim symetrical solutions, but in order to do that you would need to keep a map of examined boards (note that for a 8x8 board and a bit per square you only need to handle a map of 64 bits long), and a fast way of examining if a symetrical board has already been visited. Given 1) the extra computing this needs and 2) the extra memory, destroying cache locality; I'm not sure this would lead to a faster search. The current search is rather fast as the different boards are kept on the stack, we thus make sure we optimize data locality for better cache hits. The various loops are smalls and the only instructions are 1) testing and setting a flag in a small 8x8 bytes array, and 2) incrementing the number of visited square (to quickly check if the search ended).

Everything else is in the source.

This code is released in the public-domain.
