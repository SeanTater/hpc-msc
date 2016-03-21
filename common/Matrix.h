/* Copyright 2016 Sean Gallagher
 * Part of the HPC MSC project.
 * Available under the Apache 2 License
 */
#ifndef __MATRIX
#define __MATRIX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

/*
 * Let's make our lives easier by defining Matrices to be pretty convenient.
 */
typedef union Vec2 {
  struct {
    float x, y;
  };
  float v[2];
} Vec2;
typedef union Vec3 {
  struct {
    float x, y, z;
  };
  float v[3];
} Vec3;
typedef union Vec4 {
  struct {
    float x, y, z, t;
  };
  float v[4];
} Vec4;

typedef struct Matrix {
  int rows, cols;
  union {
    float *mat;
    Vec2 *mat2;
    Vec3 *mat3;
    Vec4 *mat4;
  };
} Matrix;

typedef struct Box2 {
  Vec2 start, end;
} Box2;
typedef struct Box3 {
  Vec3 start, end;
} Box3;
typedef struct Box4 {
  Vec4 start, end;
} Box4;

/** Create a new Matrix */
Matrix mat_new(int rows, int cols) {
  Matrix mat;
  mat.rows = rows;
  mat.cols = cols;
  size_t size_bytes = sizeof(float) * mat.rows * mat.cols;
  if (posix_memalign((void **) &mat.mat, 32, size_bytes) != 0) {
    perror("Could not allocate memory for matrix.");
  }
  return mat;
}

/*
 * Read and write matrices
 */

/** Read a matrix  from a file in a very simple platfrom dependent format. */
Matrix mat_read(char* filename) {
  FILE *file = fopen(filename, "rb");
  Matrix mat;
  fread(&mat.rows, sizeof(int), 1, file);
  fread(&mat.cols, sizeof(int), 1, file);
  size_t size_bytes = sizeof(float) * mat.rows * mat.cols;
  if (posix_memalign((void **) &mat.mat, 32, size_bytes) != 0) {
    perror("Could not allocate memory to read matrix.");
  }
  fread(mat.mat, 1, size_bytes, file);
  fclose(file);
  return mat;
}

/** Read a matrix  from a file in a very simple platfrom dependent format. */
void mat_write(Matrix mat, char* filename) {
  FILE *file = fopen(filename, "wb");

  // write it out
  fwrite(&mat.rows, sizeof(int), 1, file);
  fwrite(&mat.cols, sizeof(int), 1, file);
  fwrite(mat.mat, sizeof(float), mat.rows*mat.cols, file);
  fclose(file);
}


// Little helper to count file lines for mat_read
typedef struct MatSize {int rows, cols;} MatSize;
static MatSize get_mat_text_size(FILE *file) {
 char line[100];
 MatSize size = {0,0};
 rewind(file);
 while (fgets(line, 100, file) != NULL) {
   size.rows++;
   int commas = 0;
   for (int i=0; i < 100 && line[i] != '\00' && line[i] != '\n'; i++) {
     if (line[i] == ',') {
       commas++;
     }
   }
   if (commas + 1 > size.cols) {
     size.cols = commas + 1;
   }
 }
 return size;
}

/* Read a CSV text file */
Matrix mat_read_text(char* input_filename) {
  // Open and read the file
  FILE *input_file = fopen(input_filename, "rt");
  if (input_file == NULL) {
    perror("Trouble reading from input file.");
  }
  MatSize size = get_mat_text_size(input_file);

  // Make a matrix to put it in
  Matrix out = mat_new(size.rows, size.cols);

  // Read it into the matrix
  rewind(input_file);
  for (int row=0; row<size.rows; row++) {
    for (int col=0; col<size.cols; col++) {
      fscanf(input_file, "%f%*[,\n ]", &out.mat[row * size.cols + col]);
    }
  }

  // All done, clean up
  fclose(input_file);
  return out;
}

/* Write a CSV text file */
void mat_write_text(Matrix mat, char* filename) {
  // Open and read the file
  FILE *file = fopen(filename, "wt");
  if (file == NULL) {
    perror("Trouble writing to output file.");
  }

  // Read it into the matrix
  for (int row=0; row<mat.rows; row++) {
    for (int col=0; col<mat.cols; col++) {
      fprintf(file, "%f", mat.mat[row*mat.cols + col]);
      fputs(col == mat.cols-1 ? "\n" : ",", file);
    }
  }

  // All done, clean up
  fclose(file);
}

/*
 * Operations on Matrices
 */

/** Zero out a matrix */
Matrix mat_zero(Matrix in) {
  const int size = in.cols * in.rows;
  for (int i=0; i<size; i++) {
    in.mat[i] = 0;
  }
  return in;
}

/* Delete a matrix. */
void mat_del(Matrix mat) {
  free(mat.mat);
}

/* Add two vectors */
inline Vec2 add2(Vec2 a, Vec2 b) {
  Vec2 c = a;
  c.x += b.x;
  c.y += b.y;
  return c;
}
inline Vec3 add3(Vec3 a, Vec3 b) {
  Vec3 c = a;
  c.x += b.x;
  c.y += b.y;
  c.z += c.z;
  return c;
}
inline Vec4 add4(Vec4 a, Vec4 b) {
  Vec4 c = a;
  c.x += b.x;
  c.y += b.y;
  c.z += b.z;
  c.t += b.t;
  return c;
}

#endif
