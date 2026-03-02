#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define c_DIMMENSION         2
#define c_SWARM_SIZE        20
#define c_ITERATION_LIMIT   30
#define C1	1.23010
#define C2	2.49445
#define W	0.529
#define X_MIN		-20.0
#define X_MAX		20.0
#define V_MAX		5.0

struct t_particle {
	double position[c_DIMMENSION];
	double pbest_position[c_DIMMENSION];
	double pbest_fitness;
	double Q;
	double last_change[c_DIMMENSION];
} ;

double get_rand_double(double min, double max){
	return min + (rand() / (double)RAND_MAX) * (max - min);
}


double fitness_function(double x1, double x2){
		double fitness;
		fitness = 6 * exp(-(x1-2)*(x1-2)) + 3 * exp(-(x2-6)*(x2-6));
		return fitness;
}

struct t_particle p[c_SWARM_SIZE]; 

int main()
{
int a;
int iterations = 0;
double r1;
double r2;
double new_change[c_DIMMENSION];
double gbest_position[c_DIMMENSION];
double gbest_fitness = 0;
srand(time(NULL));
 
for(int n = 0; n<c_SWARM_SIZE; n++){
  		for(int d = 0; d<c_DIMMENSION; d++){
  			p[n].position[d] = get_rand_double(X_MIN, X_MAX);
  			p[n].pbest_position[d] = p[n].position[d];
  			p[n].last_change[d] = 0.0;
  		}
  		
  		p[n].pbest_fitness = fitness_function(p[n].position[0], p[n].position[1]);
  		
  		if (p[n].pbest_fitness > gbest_fitness) {
  			gbest_fitness = p[n].pbest_fitness;
  			for (int d = 0; d < c_DIMMENSION; d++){
  				gbest_position[d] = p[n].pbest_position[d];
  			}
  		}
  	}
  	
  	printf("---- INITIAL STATE ----\n");
  	for(int n = 0; n<c_SWARM_SIZE; n++){
		  	printf("I am Particle %d\n", n+1);
		  		for(int d = 0; d<c_DIMMENSION; d++){
		  				printf("position: %f\n", p[n].position[d]);
		  			}
		  	}
	printf("\n");


	while(iterations < c_ITERATION_LIMIT)
		{
		printf("--- ITERATION %d ---\n\n", iterations);
			for (int n = 0; n < c_SWARM_SIZE; n++)
				{
					r1 = get_rand_double(0,1);
					r2 = 	get_rand_double(0,1);
					for (int d = 0; d < c_DIMMENSION; d++)
						{
							// calculate position change
							new_change[d] = W * p[n].last_change[d];
							new_change[d] += r1 * C1 * (p[n].pbest_position[d] - p[n].position[d]);
							new_change[d] += r2 * C2 * (gbest_position[d] - p[n].position[d]);
							
			
							
							// position update
							p[n].position[d] += new_change[d];
							
				
							
							// velocity update
							p[n].last_change[d] = new_change[d];
						}
					
						p[n].Q = fitness_function(p[n].position[0], p[n].position[1]);
						
						
						if (p[n].Q  > p[n].pbest_fitness)
							{
								p[n].pbest_fitness = p[n].Q;
								
								for (int d = 0; d < c_DIMMENSION; d++)
									{
										p[n].pbest_position[d] = p[n].position[d];
									}
							}
						
						if (p[n].Q > gbest_fitness)
							{
								gbest_fitness = p[n].Q;
								for (int d = 0; d < c_DIMMENSION; d++)
									{
										gbest_position[d] = p[n].position[d];
									}
							}
				
				
				}
				printf("\n");
				for(int n = 0; n<c_SWARM_SIZE; n++){
				  	printf("I am Particle %d\n", n+1);
				  		for(int d = 0; d<c_DIMMENSION; d++){
				  				printf("position: %f\n", p[n].position[d]);
				  			}
				  	}
				printf("\n");
				iterations++;
		}
		
				printf("----FINAL STATE----\n");
				for(int n = 0; n<c_SWARM_SIZE; n++){
				  	printf("I am Particle %d\n", n+1);
				  		for(int d = 0; d<c_DIMMENSION; d++){
				  				printf("position: %f\n", p[n].position[d]);
				  			}
				  	}

				printf("\n");
				for(int d = 0; d < c_DIMMENSION; d++){
					printf("Best Position %d: %f\n", d + 1, gbest_position[d]);
				}
				printf("Best Result: %f\n", gbest_fitness);
		return 0;
} 

