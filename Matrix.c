#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> /* for ENOSYS */
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

int matrix_allocate(matrix_t *m, int rows, int columns) {
      if (rows <= 0){
            return -1;
      }
      if (columns <= 0){
            return -1;
      }

      int i;
      m->rows = rows;
      m->columns = columns;
      m->content = (int **)malloc(rows * sizeof(int*));
      if (m->content == NULL){
            //printf("failed to allocate the matrix\n");
            return -1;
      }

      for (i=0; i<rows; i++){
            (m->content)[i] = (int *)malloc(columns * sizeof(int));
            if ((m->content)[i] == NULL){
                  //printf("failed to allocate the matrix.\n");
                  return -1;
            }
      }

      return 0;
}

void matrix_free(matrix_t *m) {
      if (m != NULL){
            int i;
            for (i=0; i<m->rows; i++){
                  free((m->content)[i]);
            }
            free(m->content);
      }
      return;
}

void matrix_init_zeros(matrix_t *m) {
      if (m != NULL){
            int i, j;
            for (i=0; i<m->rows; i++){
                  for (j=0; j<m->columns; j++){
                        (m->content)[i][j] = 0;
                  }
            }
      }
      return;
}

int matrix_init_identity(matrix_t *m){
      if (m==NULL){
            return -1;
      }

      if (m->rows <= 0){
            return -1;
      }
      if (m->columns <= 0){
            return -1;
      }

      if (m->columns != m->rows){
            return -1;
      }

      if (m->content == NULL){
            return -1;
      }
      
      int i,j;
      for (i=0; i<m->rows; i++){
            for (j=0; j<m->columns; j++){
                  if (i==j){
                        (m->content)[i][j] = 1;
                  }
                  else{
                        (m->content)[i][j] = 0;
                  }
            }
      }

      return 0;

}
void matrix_init_rand(matrix_t *m) {
      if (m != NULL){
            int i, j, ranNum;
            srand(time(NULL));
            for (i=0; i < m->rows; i++){
                  for (j=0; j < m->columns; j++){
                        //srand(time(NULL));
                        ranNum = rand()%201 - 100;
                        (m->content)[i][j] = ranNum;
                  }
            }
      }
      return;
}

int matrix_equal(matrix_t *m1, matrix_t *m2) {
      if ((m1==NULL)||(m2==NULL)){
            return 0;
      }

      if ((m1->rows <= 0)||(m1->columns <= 0)||(m2->rows <= 0)||(m2->columns <= 0)){
            return 0;
      }

      if (m1->rows != m2->rows){
            return 0;
      }

      if (m1->columns != m2->columns){
            return 0;
      }

      if (m1->content == NULL){
            return 0;
      }

      if (m2->content == NULL){
            return 0;
      }

      int i,j;
      for (i=0; i<m1->rows; i++){
            for (j=0; j<m1->columns; j++){
                  if ((m1->content)[i][j] != (m2->content)[i][j]){
                        return 0;
                  }
            }
      }
      return 1;
}

int matrix_sum(matrix_t *m1, matrix_t *m2, matrix_t *result) {
      if ((m1==NULL)||(m2==NULL)){
            return -1;
      }

      if ((m1->rows <= 0)||(m1->columns <= 0)||(m2->rows <= 0)||(m2->columns <= 0)){
            return -1;
      }

      if ((m1->rows != m2->rows)||(m1->columns != m2->columns)){
            return -1;
      }

      if (m1->content == NULL){
            return -1;
      }

      if (m2->content == NULL){
            return -1;
      }

      int i, j;
      if (matrix_allocate(result, m1->rows, m1->columns) != 0){
            return -1;
      }

      for (i=0; i<m1->rows; i++){
            for (j=0; j<m1->columns; j++){
                  (result->content)[i][j] = (m1->content)[i][j] + (m2->content)[i][j];
            }
      }
      return 0;
}

int matrix_scalar_product(matrix_t *m, int scalar, matrix_t *result) {
      if (m==NULL){
            return -1;
      }

      if ((m->rows <= 0)||(m->columns <= 0)){
            return -1;
      }

      if (m->content == NULL){
            return -1;
      }

      int i,j;
      if (matrix_allocate(result, m->rows, m->columns) != 0){
            return -1;
      }

      for (i=0; i<m->rows; i++){
            for (j=0; j<m->columns; j++){
                  (result->content)[i][j] = ((m->content)[i][j]) * scalar;
            }
      }
      return 0;
}

int matrix_transposition(matrix_t *m, matrix_t *result) {
      if (m==NULL){
            return -1;
      }

      if ((m->rows <= 0)||(m->columns <= 0)){
            return -1;
      }

      if (m->content == NULL){
            return -1;
      }

      int i,j;
      if (matrix_allocate(result, m->columns, m->rows) != 0){
            return -1;
      }

      for (i=0; i<result->rows; i++){
            for (j=0; j<result->columns; j++){
                  (result->content)[i][j] = (m->content)[j][i];
            }
      }
      return 0;
}

int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *result) {
      if ((m1==NULL)||(m2==NULL)){
            return -1;
      }

      if ((m1->rows <= 0)||(m1->columns <= 0)||(m2->rows <= 0)||(m2->columns <= 0)){
            return -1;
      }

      if ((m1->columns)!=(m2->rows)){
            return -1;
      }

      if (m1->content == NULL){
            return -1;
      }

      if (m2->content == NULL){
            return -1;
      }

      int i,j,k;
      if (matrix_allocate(result, m1->rows, m2->columns) != 0){
            return -1;
      }

      for (i=0; i<result->rows; i++){
            for (j=0; j<result->columns; j++){
                  (result->content)[i][j] = 0;
            }
      }


      for (i=0; i<result->rows; i++){
            for (j=0; j<result->columns; j++){
                  for (k=0; k<m1->columns; k++){
                        (result->content)[i][j] = (result->content)[i][j] + (m1->content)[i][k] * (m2->content)[k][j];
                  }
            }
      }
      return 0;
}

int matrix_dump_file(matrix_t *m, char *output_file) {
      if (m == NULL){
            return -1;
      }
      if (m->content == NULL){
            return -1;
      }

    int i,j,k;
    int numOfRows = m->rows;
    int numOfCols = m->columns;
    int numOfEle = numOfRows * numOfCols;
    int tempArr[numOfEle];
    k = 0;
    for (i=0; i<numOfRows; i++){
        for (j=0; j<numOfCols; j++){
            tempArr[k] = (m->content)[i][j];
            k++;
        }
    }

    FILE* myFilePtr1 = fopen(output_file, "w");
    if (myFilePtr1 == NULL){
        return -1;
    }

    int numOfLines = numOfRows-1;
    int numOfSpaces = (numOfCols-1)*numOfRows;
    int totalSize = numOfEle + numOfLines + numOfSpaces;
    char content[totalSize];
    char ele[800];
    char space[1] = " ";
    char line[1] = "\n";
    strncpy(content, "", 1);


    for (i=0; i<numOfEle; i++){
        sprintf(ele, "%i", tempArr[i]);
        strncat(content, ele, 10);
        if ((i+1)%numOfCols == 0){
            strncat(content, line, 1);
        }
        else{
            strncat(content, space, 1);
        }
    }



    if (fwrite(content, 1, strlen(content), myFilePtr1)!=strlen(content)){
        return -1;
    }
    fclose(myFilePtr1);
    return 0;
}

int matrix_allocate_and_init_file(matrix_t *m, char *input_file) {
    size_t op;
    int location;
    location = -1;
    for (op=0; op<strlen(input_file); op++){
        if (input_file[op] == '.'){
            location = op;
        }
    }
    if (location == -1){
        //printf("invalid format\n");
        return -1;
    }
    if (location+4 != strlen(input_file)){
        //printf("invalid format\n");
        return -1;
    }
    if ((input_file[location+1] != 't') || (input_file[location+2] != 'x') || (input_file[location+3] != 't')){
        //printf("invalid format\n");
        return -1;
    }


    FILE* myFilePtr1 = fopen(input_file, "r");
    if (myFilePtr1 == NULL){
        return -1;
    }
    char myChar;
    if ((myChar = fgetc(myFilePtr1)) == EOF){
        //printf("empty file.......");
        return -1;
    }
    fclose(myFilePtr1);


    int i,j,k;
    char strNum[500];
    long num;
    char *endPtr;
    int numOfEle = 0;
    int numOfRows = 0;
    int numOfCols = 0;

    FILE* myFilePtr2 = fopen(input_file, "r");
    if (myFilePtr2 == NULL){
        return -1;
    }

    while (!feof(myFilePtr2)){
        fscanf(myFilePtr2, "%s", strNum);
        num = strtol(strNum, &endPtr, 10);
        if (*endPtr != '\0'){
            return -1;
        }
        if (errno == ERANGE){
                return -1;
        }
        numOfEle++;
    }
    numOfEle = numOfEle - 1;
    fclose(myFilePtr2);



    FILE* myFilePtr3 = fopen(input_file, "r");
    if (myFilePtr3 == NULL){
        return -1;
    }
    while (!feof(myFilePtr3)){
            if (fgets(strNum, 500, myFilePtr3)!=NULL){
                if (strNum[0] != '\n'){
                    numOfRows++;
                }
            }
    }
    fclose(myFilePtr3);



    FILE* myFilePtr4 = fopen(input_file, "r");
    if (myFilePtr4 == NULL){
        return -1;
    }
    while (!feof(myFilePtr4)){
        if (fgetc(myFilePtr4) == ' '){
            numOfCols++;
        }
    }
    numOfCols = (int)((numOfCols/numOfRows)+1);
    fclose(myFilePtr4);

    if (numOfRows*numOfCols != numOfEle){
        return -1;
    }


    FILE* myFilePtr5 = fopen(input_file, "r");
    if (myFilePtr5 == NULL){
        return -1;
    }
    if (matrix_allocate(m, numOfRows, numOfCols) != 0){
       return -1;
    }
    int tempArr[numOfEle];
    i = 0;
    while (!feof(myFilePtr5)){
        fscanf(myFilePtr5, "%s", strNum);
        num = strtol(strNum, &endPtr, 10);
        if (*endPtr != '\0'){
            return -1;
        }
        if (errno == ERANGE){
                return -1;
        }
        tempArr[i] = (int)num;
        i++;
    }
    fclose(myFilePtr5);
    k=0;
    for (i=0; i<numOfRows; i++){
        for (j=0; j<numOfCols; j++){
            (m->content)[i][j] = tempArr[k];
            k++;
        }
    }
    return 0;
}
