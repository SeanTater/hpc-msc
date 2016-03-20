/* Copyright 2016 Sean Gallagher
 * Part of the HPC MSC project.
 * Available under the Apache 2 License
 */
#ifndef __MATRIX
#define __MATRIX

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

/*
 * Let's make our lives easier by defining Matrices to be pretty convenient.
 */
typedef union {
  struct {
    float x, y;
  };
  float v[2];
} Vec2;
typedef union {
  struct {
    float x, y, z;
  };
  float v[3];
} Vec3;
typedef union {
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
  }
}

typedef struct Box2 {
  Vec2 start, end;
}
typedef struct Box3 {
  Vec3 start, end;
}
typedef struct Box4 {
  Vec4 start, end;
}

/** Create a new Matrix */
inline Matrix mat_new(int rows, int cols) {
  Matrix m;
  m.rows = rows;
  m.cols = cols;
  size_t size_bytes = sizeof(float) * mat.rows * mat.cols;
  if (posix_memalign((void **) &mat.mat, 32, size_bytes) != 0) {
    perror("Could not allocate memory to read matrix.");
  }
  return m;
}

/*
 * Read and write matrices
 */

/** Read a matrix  from a file in a very simple platfrom dependent format. */
Matrix mat_read(char* filename) {
  FILE *file = fopen(filename, "rb");
  Matrix mat;
  fread(&mat.rows, sizeof(size_t), 1, file);
  fread(&mat.cols, sizeof(size_t), 1, file);

  size_t size_bytes = sizeof(float) * mat.rows * mat.cols;
  if (posix_memalign((void **) &mat.mat, 32, size_bytes) != 0) {
    perror("Could not allocate memory to read matrix.");
  }
  fread(&mat.mat, size_bytes, 1, file);
  fclose(file);
  return mat;
}

/** Read a matrix  from a file in a very simple platfrom dependent format. */
void mat_write(Matrix *mat, char* filename) {
  FILE *file = fopen(filename, "wb");

  // write it out
  fwrite(&mat.rows, sizeof(size_t), 1, file);
  fwrite(&mat.cols, sizeof(size_t), 1, file);
  fwrite(mat.mat, sizeof(float), mat.rows*mat.cols, output_file);
  fclose(file);
}


// Little helper to count file lines for mat_read
struct {int rows, cols;} MatSize;
static MatSize get_mat_text_size(FILE *file) {
 char line[100];
 MatSize size = {0,0};
 rewind(file);
 while (fgets(line, 100, file) != NULL) {
   rows++;
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
 return line_count;
}

/* Read a CSV text file */
Matrix mat_read_text(char* input_filename) {
  // Open and read the file
  FILE *input_file = fopen(input_filename, "rt");
  if (input_file == NULL) {
    perror("Trouble reading from input file.");
  }
  MatSize size = get_size(input_file);

  // Make a matrix to put it in
  Matrix out = mat_new(size.rows, size.cols);

  // Read it into the matrix
  rewind(input_file);
  for (int row=0; row<size.rows; row++) {
    for (int col=0; col<cols; col++) {
      fscanf(input_file, "%f%*[,\n ]", &out.mat[row * size.cols + col]);
    }
  }

  // All done, clean up
  fclose(input_file);
  return out;
}

/* Write a CSV text file */
void mat_write_text(Matrix mat, char* output_filename) {
  // Open and read the file
  FILE *output_file = fopen(output_filename, "wt");
  if (output_file == NULL) {
    perror("Trouble writing to output file.");
  }

  // Read it into the matrix
  for (int row=0; row<size.rows; row++) {
    for (int col=0; col<size.cols; col++) {
      fprintf(output_file, "%f", &out.mat2[row].v[col]);
      fputs(col == size.cols-1 ? "\n" : ",", output_file);
    }
  }

  // All done, clean up
  fclose(output_file);
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
inline Vec2 add(Vec2 a, Vec2 b) {
  Vec3 c = a;
  c.x += b.x;
  c.y += b.y;
  return c;
}
inline Vec3 add(Vec3 a, Vec3 b) {
  Vec3 c = a;
  c.x += b.x;
  c.y += b.y;
  c.z += c.z;
  return c;
}
inline Vec4 add(Vec4 a, Vec4 b) {
  Vec3 c = a;
  c.x += b.x;
  c.y += b.y;
  c.z += b.z;
  c.t += b.t;
  return c;
}

#endif
