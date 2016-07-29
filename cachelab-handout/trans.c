/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, k, l;
  int v1,v2,v3,v4,v5,v6,v7,v8;
  if(M == 32){
    l = 8;
    for(i = 0; i < N; i += l){
      for(j = 0; j < M; j += l){
        for(k = 0; k < l; k++){
          v1 = A[i + k][j];
          v2 = A[i + k][j + 1];
          v3 = A[i + k][j + 2];
          v4 = A[i + k][j + 3];
          v5 = A[i + k][j + 4];
          v6 = A[i + k][j + 5];
          v7 = A[i + k][j + 6];
          v8 = A[i + k][j + 7];

          B[j][i + k] = v1;
          B[j + 1][i + k] = v2;
          B[j + 2][i + k] = v3;
          B[j + 3][i + k] = v4;
          B[j + 4][i + k] = v5;
          B[j + 5][i + k] = v6;
          B[j + 6][i + k] = v7;
          B[j + 7][i + k] = v8;
        }
      }
    }
  }
  else{
    l = 4;
    for(i = 0; i < N; i += l * 2){
      for(j = 0; j < M; j += l * 2){
        v5 = i;
        v6 = j;
        for(k = 0; k < l; k++){
                  v1 = A[v5 + k][v6];
                  v2 = A[v5 + k][v6 + 1];
                  v3 = A[v5 + k][v6 + 2];
                  v4 = A[v5 + k][v6 + 3];

                  B[v6][v5 + k] = v1;
                  B[v6 + 1][v5 + k] = v2;
                  B[v6 + 2][v5 + k] = v3;
                  B[v6 + 3][v5 + k] = v4;

            }

        v5 = i + l;
        v6 = j;
        for(k = 0; k < l; k++){
                  v1 = A[v5 + k][v6];
                  v2 = A[v5 + k][v6 + 1];
                  v3 = A[v5 + k][v6 + 2];
                  v4 = A[v5 + k][v6 + 3];

                  B[v6][v5 + k] = v1;
                  B[v6 + 1][v5 + k] = v2;
                  B[v6 + 2][v5 + k] = v3;
                  B[v6 + 3][v5 + k] = v4;

            }

          v5 = i + l;
          v6 = j + l;
          for(k = 0; k < l; k++){
                    v1 = A[v5 + k][v6];
                    v2 = A[v5 + k][v6 + 1];
                    v3 = A[v5 + k][v6 + 2];
                    v4 = A[v5 + k][v6 + 3];

                    B[v6][v5 + k] = v1;
                    B[v6 + 1][v5 + k] = v2;
                    B[v6 + 2][v5 + k] = v3;
                    B[v6 + 3][v5 + k] = v4;

              }

            v5 = i;
            v6 = j + l;
            for(k = 0; k < l; k++){
                      v1 = A[v5 + k][v6];
                      v2 = A[v5 + k][v6 + 1];
                      v3 = A[v5 + k][v6 + 2];
                      v4 = A[v5 + k][v6 + 3];

                      B[v6][v5 + k] = v1;
                      B[v6 + 1][v5 + k] = v2;
                      B[v6 + 2][v5 + k] = v3;
                      B[v6 + 3][v5 + k] = v4;

                }

      }
    }
  }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
