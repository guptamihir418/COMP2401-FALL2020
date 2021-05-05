// Mihir Gupta
// 101172281

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//main function at the bottom


// prints a 3d array everytime it gets called
void printArray( int b, int r, int c, char arr[b][r][c])
                // b = number of boxes
                // r = number of rows
                // c = number of columns
{
    for (int i = 0; i < b; i++){
        for(int j = 0; j < r; j++){
            for(int k = 0; k < c; k++){

            printf("%c",*&arr[i][j][k]);
            printf(" ");
            }
            printf("\n");
        }
        printf("\n");
    }
}

// gets the random number between upper bound and lower bound.
int getRandomNumber(int upper, int lower){
    return ((rand() % (upper - lower + 1)) + lower);
}

// returns the array of number of large/medium and small packages.
int* packageArray(){

    static int packages[3];

    int maxPackages = 24;

    srand(time(NULL));

    int large = getRandomNumber(maxPackages,0);

    int leftSum= maxPackages - large;
    int medium = getRandomNumber(leftSum,0);

    int finalSum = leftSum-medium;
    int small = finalSum;

    packages[0]=small;
    packages[1]=medium;
    packages[2]=large;

    return packages;
}

// returns the total amount charged and prints it bill nicely
int totalCost(int l, int m, int s,int boxesUsed ){

    float smallCost = (s*5.99);
    float mediumCost = (m*10.99);
    float largeCost = (l*19.99);
    float total = (smallCost+mediumCost+largeCost);
    float shipping = (boxesUsed*1.49);
    float HST = (0.13*total);
    float amountCharged = (total+HST+shipping);
    printf("\n");
    printf("Order:   Large=%d,   Medium=%d,   Small=%d\n",l,m,s);
    printf("\n");
    printf("Small (%2d Units)         $ %2.2f\n",s,smallCost);
    printf("Medium (%2d Units)        $ %2.2f\n",m,mediumCost);
    printf("Large (%2d Units)         $ %2.2f\n",l,largeCost);
    printf("----------------------------------------------\n");
    printf("Total                    $ %2.2f\n",total);
    printf("Shipping (%2d boxes)      $ %2.2f\n",boxesUsed,shipping);
    printf("HST (13%%)                $ %2.2f\n",HST);

    printf("----------------------------------------------\n");
    printf("Amount Charged           $ %2.2f\n",amountCharged);
    printf("\n");

    return amountCharged;

}


// the main functioning of the code
int main(){

    int* b = packageArray();

    int smallBoxes = b[0];
    int mediumBoxes = b[1];
    int largeBoxes = b[2];

    // assigning maximum values to each.
    // As according to assignment max boxes are 6
    // max rows in eac box are 5 and cols are 4
    int MAX_BOXES =6;
    int MAX_ROWS = 5;
    int MAX_COLS = 4;

    // creating an 3D array
    char packages[MAX_BOXES][MAX_ROWS][MAX_COLS];

    // this function removes the garbage values from the array
    // returns the new array with '_' char at very index.
    memset(packages, '_', MAX_BOXES*sizeof(packages[0]));

    // this whole thing assigns the 'L' after checking condition.
    int loop = largeBoxes;

    //controls the number of boxes
    for (int i = 0; i < MAX_BOXES; i++){
        //controls the number of rows
        //here j < MAX_ROWS -1 because the 5th row of any box
        // shouldn't be checked because if it would be checked 
        //it would shift to the next box and print Ls in last row of 1st box
        // and 1st row of next box
        for(int j = 0; j < MAX_ROWS-1; j+=2){
            //controls the columns
            for(int k = 0; k < MAX_COLS; k+=2){

                if(loop>0){

                    //checking the two adjacent indices and the diagonal index
                    if((packages[i][j][k+1]== '_') &&(packages[i][j+1][k]=='_') && (packages[i][j+1][k+1]== '_')){

                            // if condition is met then '_' => 'L' on all squares.
                            packages[i][j][k]= 'L';
                            packages[i][j][k+1]= 'L';
                            packages[i][j+1][k]='L';
                            packages[i][j+1][k+1]= 'L';

                            // this whole thing will only work "number of large boxes" times.
                            loop--;
                        }
                }
                    
            }
        }
    }

    int loop2 = mediumBoxes;

    // same logic just changes '_' => 'M'
    //checks the horizontal index
    for (int i = 0; i < 6; i++){
            for(int j = 0; j < 5; j++){
                for(int k = 0; k < 4; k++){
                    if(loop2>0){    
                        //checks ther horizontal                
                        if( (packages[i][j][k] =='_') && (packages[i][j][k+1] =='_')){
                            packages[i][j][k]= 'M';
                            packages[i][j][k+1]= 'M';
                            loop2--;
                        }
                    }
                }
            }
    }

    // same logic just changes '_' => 'M'
    //checks the vertical index
    for (int i = 0; i < 6; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 4; k++){
                if(loop2>0){           
                    // checks vertical index         
                    if( (packages[i][j][k] =='_') && (packages[i][j+1][k] =='_')){
                        packages[i][j][k]= 'M';
                        packages[i][j+1][k]= 'M';
                    }
                }
            }
        }
    }


    // same logic just changes '_' => 'S'
    int loop3= smallBoxes;

    for (int i = 0; i < 6; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 4; k++){

                if(loop3 > 0){
                    // checks for any empty space in every used box
                    if(packages[i][j][k]== '_'){
                        //if found changes its value
                        packages[i][j][k]= 'S';
                        loop3--;
                    }
                }
            }
        }
    }
    
    //calculating number of boxes used.

    //finding the total chars that must be used in array
    int totalChars = (largeBoxes*4)+(mediumBoxes*2)+smallBoxes;
    int boxes;


    // finding the number of big boxes to be used.
    if(totalChars % 20 == 0){
        boxes =(totalChars)/20;
    }
    else if(largeBoxes > 20){
        boxes = 6;
        }
    else if(largeBoxes > 16){
        boxes = 5;
    }
    else{
        boxes = (totalChars)/20 + 1;
    
    }


    //prints the bill
    totalCost(largeBoxes, mediumBoxes, smallBoxes, boxes);

    // remove the rest of '_' to an empty string
    for(int i=0; i<boxes; i++){
        for(int j=0; j<5; j++){
            for(int k = 0; k < 4; k++){
                if(packages[i][j][k]=='_'){
                    packages[i][j][k]=' ';
                }
            }
        }
    }


    printf("Individual boxes are packed as follows:\n");
    printf("\n");


    //printing the boxes and the way they are packed in.
    printArray(boxes,5,4, packages);

    return 0;
}
