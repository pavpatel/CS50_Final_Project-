/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <string.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

//Blank tile positions
int brow;
int bcol; 

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();
    
    //Initial position of blank
    brow = d-1;
    bcol = d-1;

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }
        
        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int size = (d * d) - 1;
    
    //Loop for rows
    for (int i = 0; i < d; i++)
    {
        //Loop for columns
        for (int j = 0; j < d; j++)
        {
            board[i][j] = size--;
        }
    }
    
    //Flip positions of tiles numbered 1 and 2 for odd number of tiles
    if (d % 2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
    
    //Adjust blank tile
    //board[d-1][d-1] = *"_";
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //Loop for rows
    for (int i = 0; i < d; i++)
    {
        //Loop for columns
        for (int j = 0; j < d; j++)
        {
            //Align spacing of double digit and single digit tiles, add double space between tiles
            if (board[i][j] >= 10)
            {
                printf("%i", board[i][j]);
            }
            else
            {
                printf(" %i", board[i][j]);
            }
            if (j < d - 1)
            {
                printf("  ");
            }
        }
        //Print blank line after each row 
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    //Loop for rows
    for (int i = 0; i < d; i++)
    {
        //Loop for columns
        for (int j = 0; j < d; j++)
        {
            //Find tile position on board
            if (tile == board[i][j])
            {
                //Legal tile moves logic: above, right, below, left
                if (board[i][j] == board[brow-1][bcol] || board[i][j] == board[brow][bcol+1] 
                || board[i][j] == board[brow+1][bcol] || board[i][j] == board[brow][bcol-1])
                {
                    //Swap position of tile and blank
                    board[i][j] = 0; 
                    board[brow][bcol] = tile;
                    brow = i;
                    bcol = j;
                    return true;
                }
            }
        }
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    //Initialize check to expected value of first tile
    int check = 1;
    
    //Loop for rows
    for (int i = 0; i < d; i++)
    {
        //Loop for columns
        for (int j = 0; j < d; j++)
        {
            //Compare tile to check value and increment check on match    
            if (board[i][j] == check)
            {
                check++;
            }
            //Decrement check if tile in wrong position
            else 
            {
                check--;
            }
            //Increment check on final blank 
            if (board[i][j] == board[d-1][d-1])
            {
                check++;
            }
        }
    }
    //Return true if every tile is in correct position
    if (check == (d * d))
    {
        return true;
    }
    return false;
}
