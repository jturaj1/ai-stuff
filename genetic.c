#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define C_POPULATION 50
#define C_GENERATION 50
#define C_DIMMENSION 5
#define T_SIZE 5
#define X_MIN -20
#define X_MAX 20
#define ALPHA 0.5
#define P_MUTATION 0.05
#define PI 3.14159
#define R_CONSTANT 100.0

struct t_chromosome {
  double position[C_DIMMENSION];
  double quality;
};

struct t_chromosome pop[C_POPULATION];
struct t_chromosome new_pop[C_POPULATION];

double get_rand_double(double min, double max) {
  return min + (rand() / (double)RAND_MAX) * (max - min);
}

int rand_int(int min, int max) { return rand() % (max + 1 - min) + min; }

double quality_func(struct t_chromosome *c) {
  double quality = 0.0;
  for (int d = 0; d < C_DIMMENSION; d++) {
    quality += (c->position[d] * c->position[d]) -
               (10 * cos(2 * PI * c->position[d])) + 10;
  }
  return R_CONSTANT - quality;
}

void pop_init(struct t_chromosome *pop) {
  for (int n = 0; n < C_POPULATION; n++) {
    for (int d = 0; d < C_DIMMENSION; d++) {
      pop[n].position[d] = get_rand_double(X_MIN, X_MAX);
    }
    pop[n].quality = quality_func(&pop[n]);
  }
}

int select_parent(int pop_size, int t_size) {
  int winner_index = -1;
  double best_quality = -999.0;
  for (int i = 0; i < t_size; i++) {
    int candidate_index = rand_int(0, pop_size - 1);
    double candidate_quality = pop[candidate_index].quality;
    if (candidate_quality > best_quality) {
      best_quality = candidate_quality;
      winner_index = candidate_index;
    }
  }
  return winner_index;
}

void blend_crossover(struct t_chromosome *parent1, struct t_chromosome *parent2,
                     struct t_chromosome *offspring1,
                     struct t_chromosome *offspring2) {
  double min, max, I, low, high;
  for (int d = 0; d < C_DIMMENSION; d++) {
    min = fmin(parent1->position[d], parent2->position[d]);
    max = fmax(parent1->position[d], parent2->position[d]);
    I = max - min;

    low = min - ALPHA * I;
    high = max + ALPHA * I;

    offspring1->position[d] = get_rand_double(low, high);
    offspring2->position[d] = get_rand_double(low, high);
  }
}

void crossover(struct t_chromosome *parent_a,
			   struct t_chromosome *parent_b,
			   struct t_chromosome *offspring_a,
			   struct t_chromosome *offspring_b) {
	int r1 = rand_int(1, C_DIMMENSION - 2);
    int r2 = rand_int(1, C_DIMMENSION - 2);

    if (r1 > r2) {
        int temp = r1;
        r1 = r2;
        r2 = temp;
    }

    for (int n = 0; n < C_DIMMENSION; n++) {
        if (n <= r1 || n >= r2) {
            offspring_a->position[n] = parent_b->position[n];
            offspring_b->position[n] = parent_a->position[n];
        } 
        else {
            offspring_a->position[n] = parent_a->position[n];
            offspring_b->position[n] = parent_b->position[n];
        }
    }
}

void gene_mutation(struct t_chromosome *c) {
  for (int d = 0; d < C_DIMMENSION; d++) {
    if (get_rand_double(0.0, 1.0) < P_MUTATION) {
      c->position[d] = get_rand_double(X_MIN, X_MAX);
    }
  }
}

void calc_quality(struct t_chromosome *c) { c->quality = quality_func(c); }

void pop_status(struct t_chromosome c[C_POPULATION]) {
  for (int i = 0; i < C_POPULATION; i++) {
    for (int d = 0; d < C_DIMMENSION; d++) {
      printf("Best Position	%f\n", c[i].position[d]);
    }
    printf("Best Quality	%f\n\n", c[i].quality);
  }
}

void gbest_status(struct t_chromosome *c) {
  for (int d = 0; d < C_DIMMENSION; d++) {
    printf("Best Position	%f\n", c->position[d]);
  }
  printf("Best Quality	%f\n\n", c->quality);
}


int* best_indexes(int size, struct t_chromosome pop_array[C_POPULATION]) {
  int* array = (int*)malloc(size * sizeof(int));

  if (array == NULL) {
  	printf("Failed allocating memory\n");
	exit(1);
  }

  int best_index = 0;
  double best_quality = pop_array[0].quality;

  for (int k = 0; k < size; k++) {
  for (int i = 1; i < C_POPULATION; i++) {
    if (pop_array[i].quality > best_quality) {
      best_quality = pop_array[i].quality;
      best_index = i;
    }
 	array[k] = best_index;
	pop_array[best_index].quality = -999;
  }
  }

  return array;
}
 
int best_index(struct t_chromosome pop_array[C_POPULATION]) {
  int best_index = 0;
  double best_quality = pop_array[0].quality;

  for (int i = 1; i < C_POPULATION; i++) {
    if (pop_array[i].quality > best_quality) {
      best_quality = pop_array[i].quality;
      best_index = i;
    }
  }
  return best_index;
}

int main() {

  srand(time(NULL));
  pop_init(pop);
  int gen = 0;
  int best_idx = best_index(pop);
  struct t_chromosome gbest = pop[best_idx];
  gbest_status(&gbest);

  while (gen < C_GENERATION) {

    for (int i = 0; i < C_POPULATION / 2; i++) {

      int parent1_index = select_parent(C_POPULATION, T_SIZE);
      int parent2_index = select_parent(C_POPULATION, T_SIZE);

      while (parent1_index == parent2_index) {
        parent2_index = select_parent(C_POPULATION, T_SIZE);
      }

      struct t_chromosome parent1 = pop[parent1_index];
      struct t_chromosome parent2 = pop[parent2_index];
      struct t_chromosome offspring1;
      struct t_chromosome offspring2;

	  crossover(&parent1, &parent2, &offspring1, &offspring2);

      gene_mutation(&offspring1);
      gene_mutation(&offspring2);

      calc_quality(&offspring1);
      calc_quality(&offspring2);

      new_pop[2 * i] = offspring1;
      new_pop[2 * i + 1] = offspring2;
    }
    

    for (int n = 0; n < C_POPULATION; n++) {
      pop[n] = new_pop[n];
      if (pop[n].quality > gbest.quality) {
        gbest = pop[n];
      }
    }

    if (gen % 10 == 0 || gen == C_GENERATION - 1) {
      printf("Generation's %d Best\n", gen);
      gbest_status(&gbest);
    }

    gen++;
  } 

  return 0;
}
