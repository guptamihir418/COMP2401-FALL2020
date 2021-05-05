// Mihir Gupta
// 101172281

#include <stdio.h>

// defining the sizes of the 3 arrays used
#define SMALL_BOX_SIZE 8
#define MEDIUM_BOX_SIZE 14
#define LARGEL_BOX_SIZE 32


// forward declarations of all the functions
void printEncodedMAze(int Arr[], unsigned char size);
void printEncodedMazeWithPath(int mazeArr[], int PathArr[], unsigned char size);
void encode8by8Maze(char inputMaze[8][8], int encodemaze[8]);
void merge8by8Mazes(int topLeft[8], int topRight[8],
int bottomLeft[8], int bottomRight[8],
int finalMaze[14]);
char isPathInMaze(int encodedArr[], int encodedPath[], unsigned char size);




// Main function of the program
int main(){

    // defining an array of the 32 bit big maze
    // storing all the integers values of each row of big maze
    int maze[LARGEL_BOX_SIZE] = {-1, -2146430423,-1109928277,-1525661045, -1602496045, -1358950569,
    -1451509759, -1927115297, -1578180479, -1354977603, -1476294999, -2084818261, -1163749757,
    -1964319689, -1341614619, -1750141691, -1256060947, -1515522751, -1204136611, -1955867371,
    -1190652827, -1553272339, -1100839163, -1999963019, -631368865, -1882138047, -1172944163,
    -1412279803, -1567107339, -164346691, -2138762879, -1};

    // printing 32x32 maze 
    printf("The maze printed is 32x32 without path\n");
    printEncodedMAze(maze, LARGEL_BOX_SIZE);
    printf("\n");

    // defining the path Array for the 32 bit big maze
    int path[LARGEL_BOX_SIZE]= {0, 0, 0, 0, 12, 8, 56, 32, 8032, 4416, 134115648, 67354944, 67109184,
    67109312, 133169152, 1048576, 1835008, 262144, 262144, 262144, 458752,
    65536, 65536, 65536, 65536, 983040, 67633152, 67633152, 201850880,
    164102144, 259522560, 0};


    // printing 32x32 maze with path

    printf("The maze printed is 32x32 with path\n");
    printEncodedMazeWithPath(maze, path, LARGEL_BOX_SIZE);
    //storing returned value and printing the sentence
    int v = isPathInMaze(maze, path, LARGEL_BOX_SIZE);
    if(v == 1){
        printf("Yes! the path fits in the maze\n");
    }
    printf("\n\n");


    // hardcoding all the small mazes here
    char maze1[8][8]={{1,1,1,1,1,1,1,1},
                    {1,0,0,0,1,0,0,1},
                    {1,0,1,0,1,1,0,1},
                    {1,0,1,0,0,0,0,1},
                    {1,0,1,1,1,1,0,1},
                    {1,0,0,0,0,0,0,1},
                    {1,0,1,0,1,0,1,1},
                    {1,1,1,1,1,1,1,1}};

    char maze2[8][8]={{1,1,1,1,1,1,1,1},
                    {1,0,0,0,0,1,0,1},
                    {1,1,1,1,0,1,0,1},
                    {1,0,0,1,0,1,1,1},
                    {1,1,0,0,0,0,0,1},
                    {1,1,1,1,0,1,1,1},
                    {1,0,0,0,0,1,0,1},
                    {1,1,1,1,1,1,1,1}};

    char maze3[8][8]={{1,1,1,1,1,1,1,1},
                    {1,0,1,0,0,0,1,1},
                    {1,0,1,0,1,0,0,1},
                    {1,0,1,0,1,0,1,1},
                    {1,0,1,0,1,0,1,1},
                    {1,0,1,0,1,0,1,1},
                    {1,0,0,0,1,0,0,1},
                    {1,1,1,1,1,1,1,1}};

    char maze4[8][8]={{1,1,1,1,1,1,1,1},
                    {1,0,1,0,1,0,1,1},
                    {1,0,1,0,0,0,0,1},
                    {1,0,0,0,1,1,1,1},
                    {1,1,1,0,1,0,0,1},
                    {1,0,0,0,0,0,1,1},
                    {1,1,0,1,1,0,0,1},
                    {1,1,1,1,1,1,1,1}};

    // defining the empty arrays of size 8
    int encodedMaze1[SMALL_BOX_SIZE];
    int encodedMaze2[SMALL_BOX_SIZE];
    int encodedMaze3[SMALL_BOX_SIZE];
    int encodedMaze4[SMALL_BOX_SIZE];

    // calling the function that converts 
    // 2D maze into single int maze
    encode8by8Maze(maze1, encodedMaze1);
    encode8by8Maze(maze2, encodedMaze2);
    encode8by8Maze(maze3, encodedMaze3);
    encode8by8Maze(maze4, encodedMaze4);

    //printing all the encoded mazes
    // it doesn't say to print in assignment specification but if you want to print it 
    // uncomment the lines below


    // printEncodedMAze(encodedMaze1, SMALL_BOX_SIZE);
    // printf("\n");
    // printEncodedMAze(encodedMaze2, SMALL_BOX_SIZE);
    // printf("\n");
    // printEncodedMAze(encodedMaze3, SMALL_BOX_SIZE);
    // printf("\n");
    // printEncodedMAze(encodedMaze4, SMALL_BOX_SIZE);
    // printf("\n");



    // Final Maze of 14x14 by combining all the 4 8xb mazes
    // initializing this
    int finalMaze[MEDIUM_BOX_SIZE];


    //calling this function which gives out finalMaze of 14x14
    merge8by8Mazes(encodedMaze1, encodedMaze2, encodedMaze3, encodedMaze4, finalMaze);

    //printing the 14x14 maze
    // to print please uncomment this as it doesn't say to print in the specifications
    // printEncodedMAze(finalMaze, MEDIUM_BOX_SIZE);


    // defining the path for 14x14 maze
    int Path14x14[MEDIUM_BOX_SIZE] = {0, 4096, 4096, 4096, 4096, 7936, 256, 256, 448, 112, 16, 28, 6, 0};

    // Storing the pointers to all the 8x8 encoded mazes
    int *TestMazes[4] = {encodedMaze1, encodedMaze2, encodedMaze3, encodedMaze4};

    // defining the 14x14 merged maze array
    int mergedMaze[MEDIUM_BOX_SIZE];

    printf("Printing all the 256 combinations from here.\n");
    //looping over 4 mazes to produce 256 combinations of the 14x14 maze
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 4; k++){
                for(int z = 0; z < 4; z++){


                    // calling the function which produces the 14x14 maze by taking in 4 8x8 by mazes
                    // step 5 function
                    merge8by8Mazes(*&TestMazes[i],*&TestMazes[j], *&TestMazes[k], *&TestMazes[z], mergedMaze);
                    printf("\n");

                    // storing the returned value by ispathInMaze
                    int returnedValue = isPathInMaze(mergedMaze, Path14x14, MEDIUM_BOX_SIZE);

                    // printing all the 256 combinations
                    // step 2 function 
                    printEncodedMAze(mergedMaze, MEDIUM_BOX_SIZE);
                    printf("COMBINATION: (%d, %d, %d, %d)\n", i+1,j+1,k+1,z+1);


                    // if returned value == 1, which means path fits in the maze properly
                    // print some sentence
                    // prints the sequence of those mazes
                    if(returnedValue == 1){
                        printf("Path fits in for above maze\n");
                    }
                }
            }
        }

    }

}


// All functions are coded from here

void printEncodedMAze(int Arr[], unsigned char size){

    for(int i = 0; i < size; i++){

        // reads bit from right to left
        for(int j = size - 1; j >= 0; j--){

            // bitwise manipulation for reading each bit
            int x = Arr[i];
            int mask = 1 << j;
            int bit = (x & mask) >> j;

            // if bit is 1 or -1 that means it is a wall
            if(bit == 1 || bit == -1){

                // then will print "@"
                printf("@");
            }
            if(bit == 0){

                // else prints empty space
                printf(" ");
            }
        }
        printf("\n");
    }
}


void printEncodedMazeWithPath(int mazeArr[], int PathArr[], unsigned char size){

    for(int i = 0; i < size; i++){
        // reads bit from right to left
        for(int j = size - 1; j >= 0; j--){

            // bitwise maniupulation 
            int x = mazeArr[i];
            int y = PathArr[i];
            int mask = 1 << j;
            int MazeBit = (x & mask) >> j;
            int pathBit = (y & mask) >> j;

            // if the maze bit == 1 or -1
            if(MazeBit == 1 || MazeBit == -1){
                // it is a wall print @
                printf("@");
            }
            // if maze bit is not wall and pathBit is on
            if((MazeBit == 0) && (pathBit==1) ){
                // then print "." to show path
                printf(".");
            }

            // mazeBit == 0 then print empty spaces
            else if(MazeBit == 0){
                printf(" ");
            }
        }
        printf("\n");
    }
}


void encode8by8Maze(char inputMaze[8][8], int encodemaze[8]){


    int sum = 0;
    for(int i = 0; i < SMALL_BOX_SIZE; i++){
        for(int j =0; j < SMALL_BOX_SIZE; j++){

            // reversing the bit string to store it in the maze as I want
            // same as palindrome thing
            sum += inputMaze[i][SMALL_BOX_SIZE-j-1] * (1 << j);
        }
        encodemaze[i] = sum - (1<<8) /* similar to 2^8 */;
        sum = 0;

    }

}


void merge8by8Mazes(int topLeft[8], int topRight[8],
int bottomLeft[8], int bottomRight[8],
int finalMaze[14]) {

    for (int i = 0; i < 7; i++) {

       // the two's compliment 
        // Bit wise maniupulation to get two's compliment and store it
        finalMaze[i] = ((((topLeft[i] + 256) >> 1) << 7) + ((topRight[i] + 256) & (~(1 << 7)))) - (1<<14) /* similar to 2^14 */ ; 
    }

        for (int i = 1; i < 8; i++) {
        finalMaze[i + 6] = ((((bottomLeft[i] + 256) >> 1) << 7) + ((bottomRight[i] + 256) & (~(1 << 7)))) - (1<<14) /* similar to 2^14 */ ;
    }

}


char isPathInMaze(int encodedArr[], int encodedPath[], unsigned char size){

    // same code as printEncodedmazeWithPath

    for(int i = 0; i < size; i++){
        // reading bit from right to left
        for(int j = size - 1; j >= 0; j--){
            // bitwise maniupulation
            int x = encodedArr[i];
            int y = encodedPath[i];
            int mask = 1 << j;
            int MazeBit = (x & mask) >> j;
            int pathBit = (y & mask) >> j;

            // if wall is equal to path off => returns 0
            // else returns 1
            if (MazeBit == pathBit){
                if(MazeBit != 0){
                    return 0;
                }
            }
        }
    }
    return 1;
}
