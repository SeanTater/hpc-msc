/* Copyright 2016 Sean Gallagher
 * Part of the HPC MSC project.
 * Available under the Apache 2 License
 */
#include <stdlib.h>
#include "Matrix.h"
Matrix msc_naive_3d(Matrix cloud, Matrix seeds, Vec3 distances, int min_drops) {
  Matrix centers = mat_copy(seeds);

  // Continue until the movement is small enough
  float total_movement = 0;
  do {
    total_movement = 0;

    // For each center
    for (int center_i=0; center_i<seeds.rows; center_i++) {
      Vec3 center = {0,0,0};
      int count = 0;

      // For each cloud droplet
      for (int drop_i=0; drop_i<cloud.rows; drop_i++) {
        Vec3 *drop = &cloud.mat3[drop_i];

        // For each dimension
        for (int dim=0; dim < 3; dim++) {

          // Filter to only those that match the requirements
          if (abs(drop->v[dim] - center.v[dim]) < distances.v[dim]) {
            // Add them to the average
            center = add3(center, *drop);
            count++;
          }
        }
      }
      if (count >= min_drops) {
        center = scale3(center, 1.0/count);
        total_movement += euclid3(centers[center_i], center);
        centers[center_i] = center;
      }
    }
  } while (total_movement > 0.002);
  
  return centers;
}
