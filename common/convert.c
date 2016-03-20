/* Copyright 2016 Sean Gallagher
 * Part of the HPC MPC project.
 * Available under the Apache 2 License
 */
 #include <stdio.h>
 #include <stdlib.h>


 // Little helper to count file lines.
 int count_lines(FILE *file) {
   char line[100];
   int line_count = 0;
   rewind(file);
   while (fgets(line, 100, file) != NULL) {
     line_count++;
   }
   return line_count;
 }

int main(int argc, char **argv) {
  // Check args and give nicer names
  int dims;
  char *input_filename, *output_filename;
  if (argc != 4) {
    puts("Usage: convert DIMS INPUT_CSV OUTPUT_DAT");
    return 1;
  } else {
    dims = atol(argv[1]);
    input_filename = argv[2];
    output_filename = argv[3];
    if (dims < 2 || dims > 4) {
      perror("Sorry, but only 2, 3, and 4 dimensional spaces are supported.");
    }
  }

  // Open and read the file
  FILE *input_file = fopen(input_filename, "rt");
  if (input_file == NULL) {
    perror("Trouble reading from input file.");
  }

  // Make a matrix to put it in
  int line_count = count_lines(input_file);
  size_t matrix_size = line_count * dims * sizeof(float);
  FILE *output_file = fopen(output_filename, "wb");

  // Read it into the matrix
  rewind(input_file);
  fwrite(&line_count, sizeof(size_t), 1, output_file);
  fwrite(&dims, sizeof(size_t), 1, output_file);
  for (int line_i=0; line_i<line_count; line_i++) {
    for (int dim=0; dim<dims; dim++) {
      float component;
      fscanf(input_file, "%f%*[,\n ]", &component);
      fwrite(&component, sizeof(float), 1, output_file);
    }
  }

  // All done, clean up
  fclose(output_file);
  fclose(input_file);
}
