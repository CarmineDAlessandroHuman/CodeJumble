/*A TOO SIMPLE EXAMPLE OF ARTIFICIAL INTELLIGENCE
 *
 *This program calculate the best path on a map
 *from a given start position to an end position.
 *
 *
 *The map is given in input from a .txt file.
 *
 *To calculate the path a brute force algorithm is used,
 *in particular a Breath-First-Search algorithm.
 *
 *First of running the program, you should create the map file.
 *The map file name is "mapInput.txt" but you can change it in the code
 *Here there are some examples of maps

5 5
+++++
+  E+
+   +
+S  +
+++++


8 10
++++++++++
+   E    +
+        +
+++++++  +
+        +
+     ++++
+   S    +
++++++++++


16 20
++++++++++++++++++++
+   E              +
+   ++++           +
+                  +
+      ++++++      +
+                  +
++++++++++       +++
+                  +
+++++++++++++++++  +
+                  +
+ ++ ++++++++++++  +
+     +    S  +    +
+   + +       + +  +
+   +         +++  +
+   +         +    +
++++++++++++++++++++

 *
 *
 *
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;
#define START_POSITION -1
#define END_POSITION -2

/*A map file is structured in this way
    <number of rows> <number of cols>
    <2D map>

    symbols:
    '+' wall
    ' ' empty space
    'S' start
    'E' end

    Example:


    8 10
    ++++++++++
    +   E    +
    +        +
    +++++++  +
    +        +
    +     ++++
    +   S    +
    ++++++++++
*/

/*this function reads the map from file*/
char ** readFromFile (char* name, int *nrRows, int *nrCols) {
    char ** map;
    int rows, cols;
    int i, j;
    FILE* fp;

    if((fp = fopen (name, "r")) == NULL) {
        printf("ERROR: No map found\n");
        exit(EXIT_FAILURE);
    }

    /*read the dimension of the map*/
    fscanf(fp,"%d %d\n", &rows, &cols);

    /*dynamic allocation of the memory of the map*/
    map = (char**) malloc (sizeof(char*) * rows);
    for (i = 0; i < rows; i ++) {
        map[i] = (char*) malloc (sizeof(char) * cols);

    }

    /*reading the elements of the map*/
    for (i = 0; i < rows; i ++){
        for (j = 0; j < cols; j ++)
            fscanf(fp,"%c", &map[i][j]);
        fscanf(fp,"\n");

    }


    *nrRows = rows;
    *nrCols = cols;
    fclose(fp);
    return map;
}

typedef struct {
    /*the id is the fastest way to identify a position
    id = x * 10000 + y;

    */
    int id;
    int x;
    int y;


    /*previous index in the array openSet*/
    /*this information will be used to rebuild the optimal path*/
    int prevIndex;
} Position;

Position* newPosition (int x, int y, int prevIndex) {
    Position* newPos = (Position*) malloc (sizeof(Position));

    newPos->x = x;
    newPos->y = y;
    newPos->id = x * 10000 + y;

    newPos->prevIndex = prevIndex;

    return newPos;
}

/*this one orders with increasing id value*/
int sortById (const void *first, const void *second) {
    return (*(Position**)first)->id - (*(Position**)second)->id;
}

/*this function search a given element into a set ordered by id
 *returns true if the element can be found
 *returns false if it can't
*/
bool binarySearch (Position **set, int start, int end, Position *el) {
    int middle;

    while (start < end) {
        middle = (start + end) / 2;

        if (set[middle]->id == el->id)
            return true;
        if (set[middle]->id > el->id)
            end = middle - 1;
        else
            start = middle + 1;

    }
    /*if the algorithm goes here, the element is not found*/
    return false;



}

/*returns true if the given element is already contained in the set
 *returns false if it is not
 */
bool isAlreadyContained (Position ** set, int size, Position* el) {
    /*first: order the set*/
    qsort(set, size, sizeof(Position*), sortById);
    /*second: find the value*/

    /*iterative binary search*/
    return binarySearch(set, 0, size, el);


}

/*given a position, this function adds to the openSet all the positions
that can be reached from the given one*/
void generateMovements (Position** openSet, int index, int *openSize, char** map) {
    Position *position = openSet[index];

    /*check in all the directions*/
    /*up*/
    if (map[position->x-1][position->y] != '+') {
        openSet[*openSize] = newPosition(position->x-1, position->y, index);
        (*openSize)++;
    }
    /*right*/
    if (map[position->x][position->y+1] != '+') {
        openSet[*openSize] = newPosition(position->x,position->y+1, index);
        (*openSize)++;
    }
    /*bottom*/
    if (map[position->x+1][position->y] != '+') {
        openSet[*openSize] = newPosition(position->x+1, position->y, index);
        (*openSize)++;
    }
    /*left*/
    if (map[position->x][position->y-1] != '+') {
        openSet[*openSize] = newPosition(position->x,position->y-1, index);
        (*openSize)++;
    }
}

/*
 *returns true if the given position is the final position
 *returns false if it is not
*/
bool isFinalPosition (Position *position, Position *end) {
    if (position->id == end->id)
        return true;
    else
        return false;
}

/*The "classic" Breath First Search algorithm*/
/*the final position is already known at the beginning of this
 *algorithm. So, at the end of the execution of this function
 *will be returned only the previous index assigned to the
 *final position in order to rebuild the optimal path
 */
int BFSsearch (Position **openSet, int openSize, Position **closedSet, int closedSize, char **map, Position* end) {
    int index = 0;

    while (index < openSize) {
        if (!isAlreadyContained(closedSet, closedSize, openSet[index])) {
            /*add the current position to the closed set*/
            closedSet[closedSize] = newPosition(openSet[index]->x, openSet[index]->y, openSet[index]->prevIndex);
            closedSize++;

            if (isFinalPosition(openSet[index], end))
                return openSet[index]->prevIndex;
            else
                generateMovements(openSet, index, &openSize, map);

        }
        index++;
    }

    /*the final position can't be reached*/
    return END_POSITION;

}

int main (int argc, char* argv []) {
    int i, j;
    char **map;
    int nrRows, nrCols;         /*the number of rows and cols of the map*/
    Position *start, *end;      /*the initial position and the end position*/

    map = readFromFile("mapInput.txt", &nrRows, &nrCols);


    for (i = 0; i < nrRows; i ++) {
        for (j = 0; j < nrCols; j ++) {
            if (map[i][j] == 'E') {
                end = newPosition (i, j, END_POSITION);
            }
            else if (map[i][j] == 'S') {
                start = newPosition (i, j, START_POSITION);
            }
        }
    }

    /*declaring the open and closed sets*/
    Position** openSet;
    Position** closedSet;

    int openSize = 0, closedSize = 0;

    openSet = (Position**) malloc (sizeof(Position*) * 1000);
    closedSet = (Position**) malloc (sizeof(Position*) * 1000);

    /*adding the starting position to the openSet*/
    openSet[openSize] = start;
    openSize++;


    /*the breath first search*/
    if( (end->prevIndex = BFSsearch(openSet, openSize,
                            closedSet, closedSize,
                            map, end)) == END_POSITION) {

                            printf("Sorry, the final position cannot be reached\n");
                            exit(EXIT_FAILURE);


                            }


    /*rebuild the solution*/
    Position *iter;
    iter = openSet[end->prevIndex];
    while (iter->prevIndex != START_POSITION) {
        map[iter->x][iter->y] = 'X';

        iter = openSet[iter->prevIndex];
    }

    /*show the map*/
    for (i = 0; i < nrRows; i ++) {
        for (j = 0; j < nrCols; j ++) {
            printf("%c",map[i][j]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
