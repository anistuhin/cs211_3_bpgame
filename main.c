

// file:  bpgame.c
#include "stdio.h"
#include "stdlib.h"
#include "bpgame.h"



/** TYPEDEFS / STRUCTs HERE ***/


struct bpgame {
   int score;
   int moves;
   int nrows;
   int ncols;
   char **currBoard;
   BPGame* next;
};

extern BPGame* bp_create(int nrows, int ncols);
extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols);
void defaultValues(BPGame* temp, int nrows, int ncols);
int validMatrix(char letter);
void popBoard(char** currBoard, int nrows, int ncols);
extern int bp_pop(BPGame* b, int r, int c);
int doPop(BPGame* temp, char Color, int r, int c);
void makeCopy(BPGame* old, BPGame* new);
extern int bp_is_compact(BPGame* b);
extern void bp_display(BPGame* b);
void bottomDisplay(BPGame* b);
extern void bp_float_one_step(BPGame* b);
extern int bp_score(BPGame* b);
extern void bp_destroy(BPGame* b);

/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/





// int main() {
//    BPGame *board = bp_create(5,5);
//    bp_display(board);
//    char line[10];
//    fgets(line, 5, stdin);
//    bp_pop(board, 0, 2);
//    bp_display(board);
//    printf("%i\n",bp_score(board));
//    bp_float_one_step(board);
//    // bp_display(next);
//    // char grid[MAX_ROWS][MAX_COLS] = {Red, Blue, Green, Green, Red, Blue, Green, Green, Red, Blue, Green, Green,
//    //                    Red, Blue, Green, Green, Red, Blue, Green, Green, Red, Blue, Green, Green,
//    //                    Red, Blue, Green, Green, Red, Blue, Green, Green, Red, Blue, Green, Green,
//    //                    Red, Blue, Green, Green, Red, Blue, Green, Green, Red, Blue, Green, Green};
//    // BPGame *board1 = bp_create_from_mtx(grid, 2, 10);
//    // bp_display(board);
// }

extern BPGame * bp_create(int nrows, int ncols) {
   if (nrows < 1 || ncols < 1 || nrows > MAX_ROWS || ncols > MAX_COLS) {
      printf("Invalid dimensions\n");
   }
   char **board = malloc(nrows * sizeof(char *));
   for (int i = 0; i < nrows; i++) {
      board[i] = malloc(ncols * sizeof(char));
   }
   popBoard(board, nrows, ncols);
   BPGame* temp = malloc(sizeof(BPGame));
   temp->currBoard = board;
   defaultValues(temp, nrows, ncols);
   return temp;
}

extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols) {
   if (nrows < 1 || ncols < 1 || nrows > MAX_ROWS || ncols > MAX_COLS) {
      printf("invalid dimensions\n");
      return NULL;
   }
   BPGame* temp = malloc(sizeof(BPGame));
   char** currboard = malloc(nrows * sizeof(char *));
   for (int i = 0; i < nrows; i++) {
      currboard[i] = malloc(ncols * sizeof(char));
   }int r = 0;
   int c = 0;
   int cut = 0;
   for (int i = 0; i < MAX_ROWS; i++) {
      for (int j = 0; j < MAX_COLS; j++) {
         if (r == nrows - 1 && c == ncols) {
            cut++;
            break;
         } else if (r < nrows && c == ncols) {
            c = 0;
            r++;
         }
         if (validMatrix(mtx[i][j]))
            return NULL;
         currboard[r][c++] = mtx[i][j];
      }if (cut)
         break;
   }
   temp->currBoard = currboard;
   defaultValues(temp, nrows, ncols);
   return temp;
}

void defaultValues(BPGame* temp, int nrows, int ncols) {
   temp->next = NULL;
   temp->moves = 0;
   temp->score = 0;
   temp->nrows = nrows;
   temp->ncols = ncols;
}

int validMatrix(char letter) {
   if (letter == None) {}
   else if (letter == Red) {}
   else if (letter == Blue) {}
   else if (letter == Green) {}
   else if (letter == Yellow) {}
   else {
      printf("Invalid elemtent in matrix\n");
      return 1;
   }return 0;
}

void popBoard(char** currBoard, int nrows, int ncols) {
   for (int i = 0; i < nrows; i++) {
      for (int j = 0; j < ncols; j++) {
         int colorPicker = rand() % 4;
         if (colorPicker == 0) {
            currBoard[i][j] = Green;
         } else if (colorPicker == 1) {
            currBoard[i][j] = Red;
         } else if (colorPicker == 2) {
            currBoard[i][j] = Yellow;
         } else if (colorPicker == 3) {
            currBoard[i][j] = Blue;
         }
      }
   }
}

extern int bp_pop(BPGame* b, int r, int c) {
   if (b->next != NULL) {
      bp_pop(b->next, r, c);
   }else {
      BPGame* temp = bp_create(b->nrows, b->ncols);
      makeCopy(b, temp);
      char Color = temp->currBoard[r][c];
      int popped = doPop(temp, Color, r, c);
      if (popped == 1) {
         popped--;
         temp->currBoard[r][c] = Color;
      }
      temp->score += (popped * (popped - 1));
      b->next = temp;
      return popped;
   }
}

int doPop(BPGame* temp, char Color, int r, int c) {
   if (r < 0 || c < 0 || r == temp->nrows || c == temp->ncols)
      return 0;
   else if (temp->currBoard[r][c] != Color)
      return 0;
   int popped = 1;
   temp->currBoard[r][c] = ' ';
   popped += doPop(temp, Color, r + 1, c);
   popped += doPop(temp, Color, r - 1, c);
   popped += doPop(temp, Color, r, c + 1);
   popped += doPop(temp, Color, r, c - 1);

   return popped;
}

void makeCopy(BPGame* old, BPGame* new) {
   for (int i = 0; i < old->nrows; i++) {
      for (int j = 0; j < old->ncols; j++) {
         new->currBoard[i][j] = old->currBoard[i][j];
      }
   }
   new->moves = old->moves + 1;
   new->score = old->score;
}

extern int bp_is_compact(BPGame* b) {
   if(b->next == NULL) {
      for (int i = 0; i < b->ncols; i++) {
         for (int j = 1; j < b->nrows; j++) {
            if (b->currBoard[j-1][i] == ' ' && b->currBoard[j][i] != ' ') {
               return 1;
            }
         }
      } return 0;
   }else 
      return bp_is_compact(b->next);
}

extern void bp_display(BPGame * b) {
   if (b->next != NULL) {
      bp_display(b->next);
   }else {
      printf("  +");
      for (int i = 0; i < b->ncols; i++) {
         printf("--");
      }printf("-+\n");
      for (int i = 0; i < (b->nrows); i++) {
         printf("%i /", i);
         for (int j = 0; j < b->ncols; j++) {
            printf(" %c", b->currBoard[i][j]);
         }printf(" /\n");
      }
      printf("  +");
      for (int i = 0; i < b->ncols; i++) {
         printf("--");
      }printf("-+\n");
      bottomDisplay(b);
   }
}

void bottomDisplay(BPGame* b) {
   int temp = b->ncols;
   int limit = 10;
   int loops = 0;
   while (limit < temp) {
      limit *= 10;
      loops++;
   }
   for (int i = loops; i > 0; i--) {
      printf("   ");
      int zeros = loops * 10;
      while (zeros > 0) {
         printf(" 0");
         zeros--;
      }
      int index = 1;
      int counter = 1;
      temp -= (loops * 10);
      while (temp > 0) {
         if (counter == 11) {
            counter = 1;
            index++;
         }printf(" %i", index); 
         temp--;
         counter++;
      }printf("\n");
   }printf("   ");
   for (int j = 0; j < b->ncols; j++) {
      printf(" %i", j % 10);
   }printf("\n");
}

extern void bp_float_one_step(BPGame* b) {
   if (b->next != NULL) {
      bp_float_one_step(b->next);
   }else {
      while (bp_is_compact(b) == 1) {
         for (int r = b->nrows - 1; r > 0; r--) {
            int count = 0;
            for (int c =  0; c < b->ncols; c++) {
               if (b->currBoard[r-1][c] == ' ' && b->currBoard[r][c] != ' ') {
                  b->currBoard[r-1][c] = b->currBoard[r][c];
                  b->currBoard[r][c] = ' ';
                  count++;
               }
            }if (count > 0)
               bp_display(b);
         }
      }
   }
}

extern int bp_score(BPGame * b) {
   if (b->next != NULL) {
      bp_score(b->next);
   }else 
      return b->score;
}

extern void bp_destroy(BPGame* b) {
   while (b != NULL) {
      
      for (int i = 0; i < b->nrows; i++) {
         free(b->currBoard[i]);
      }free(b->currBoard);
      free(b);
      b = b->next;
   }
}

extern int bp_undo(BPGame* b) {
   if (b->moves == 0) {
      return 0;
   } else if (b->next->next != NULL) {
      return bp_undo(b->next);
   }else if (b->next->next == NULL) {
      bp_destroy(b->next);
      b->next = NULL;
   }
}
