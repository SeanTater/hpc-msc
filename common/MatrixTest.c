/* Copyright 2016 Sean Gallagher
 * Part of the HPC MSC project.
 * Available under the Apache 2 License
 */
#include <assert.h>
#include <Matrix.c>

int testReadText() {
 Matrix mat = mat_read_text("6mat.csv");
 assert(mat.rows == 2);
 assert(mat.cols == 3);
 assert(mat.mat[0] == 5.0);
 assert(mat.mat[1] == 6.0);
 assert(mat.mat[2] == 7.0);
 assert(mat.mat[3] == 3.0);
 assert(mat.mat[4] == 2.0);
 assert(mat.mat[5] == 1.0);
 mat_del(mat);
}

int testWriteText() {
 Matrix mat = mat_new(2, 3);
 mat.rows = 2;
 mat.cols = 3;
 mat.mat[0] = 5.0;
 mat.mat[1] = 6.0;
 mat.mat[2] = 7.0;
 mat.mat[3] = 3.0;
 mat.mat[4] = 2.0;
 mat.mat[5] = 1.0;
 mat_write_text(mat, "6mat_new.csv");

 FILE *file = fopen("6mat_new.csv", "rt");
 char buffer[24];
 fread(buffer, 1, 24, file);
 assert(memcmp(buffer, "5.0,6.0,7.0\n3.0,2.0,1.0\n", 24));
 fclose(file);
 mat_del(mat);
}
