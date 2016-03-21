/* Copyright 2016 Sean Gallagher
 * Part of the HPC MSC project.
 * Available under the Apache 2 License
 */
#include <assert.h>
#include "Matrix.h"

void test_read_text() {
 Matrix mat = mat_read_text("examples/6mat.csv");
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

void test_write_text() {
 Matrix mat = mat_new(2, 3);
 mat.rows = 2;
 mat.cols = 3;
 mat.mat[0] = 5.0;
 mat.mat[1] = 6.0;
 mat.mat[2] = 7.0;
 mat.mat[3] = 3.0;
 mat.mat[4] = 2.0;
 mat.mat[5] = 1.0;
 mat_write_text(mat, "examples/6mat_new.csv");

 FILE *file = fopen("examples/6mat_new.csv", "rt");
 char buffer[54];
 fread(buffer, 1, 54, file);
 assert(memcmp(buffer, "5.000000,6.000000,7.000000\n3.000000,2.000000,1.000000\n", 54) == 0);
 fclose(file);
 mat_del(mat);
}

void test_read() {
 Matrix mat = mat_read("examples/6mat.dat");
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

void test_write() {
 Matrix mat = mat_new(2, 3);
 mat.rows = 2;
 mat.cols = 3;
 mat.mat[0] = 5.0;
 mat.mat[1] = 6.0;
 mat.mat[2] = 7.0;
 mat.mat[3] = 3.0;
 mat.mat[4] = 2.0;
 mat.mat[5] = 1.0;
 mat_write(mat, "examples/6mat_new.dat");

 FILE *file = fopen("examples/6mat_new.dat", "rb");
 char buffer[32];
 fread(buffer, 1, 32, file);
 assert(memcmp(buffer, "\x02\x00\x00\x00\x03\x00\x00\x00\x00\x00\xa0@\x00\x00\xc0@\x00\x00\xe0@\x00\x00@@\x00\x00\x00@\x00\x00\x80?", 32) == 0);
 fclose(file);
 mat_del(mat);
}

void test_matrix() {
  test_read_text();
  test_write_text();
  test_read();
  test_write();
}
