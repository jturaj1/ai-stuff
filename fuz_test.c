#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define C_RULEMAX	4
#define C_SETMAX	3

float fuz_tri(int arg, float center, float slope, float u){
	float y = slope * (u - center) + 1;

	if ( arg == 0 ) {
		if ( u > center ) {
			 y = slope * -(u - center) + 1;
		}
	}
	
	if ( arg == -1 ) {
		if ( u <= center ) {
			return 1;
		}
		if ( u > center ) {
			y = slope * -(u - center) + 1;
		}
	}

	if ( arg == 1 ) {
		if ( u >= center ) {
			return 1;
		}
	}

	if ( y < 0 ) {
		return 0;
	}

	if ( y > 1 ) {
		return 1;
	}

	return y;
}

int rules[C_RULEMAX][C_SETMAX] = {
	{0, 0, 0},
	{0, 1, 1},
	{1, 0, 1},
	{1, 1, 2}
};

struct t_fuz_set{
	int mode;
	float slope;
	float center;
};

static struct t_fuz_set fuzzy_set_in[C_RULEMAX][C_SETMAX];
static struct t_fuz_set fuzzy_set_out[C_RULEMAX][C_SETMAX];

float F[C_RULEMAX];

int main() {
	fuzzy_set_in[0][0].center = 0;
	fuzzy_set_in[0][0].slope = 0.1;
	fuzzy_set_in[0][0].mode = -1;

	fuzzy_set_in[0][1].center = 10;
	fuzzy_set_in[0][1].slope = 0.1;
	fuzzy_set_in[0][1].mode = 1;

	fuzzy_set_in[1][0].center = 0;
	fuzzy_set_in[1][0].slope = 0.1;
	fuzzy_set_in[1][0].mode = -1;

	fuzzy_set_in[1][1].center = 10;
	fuzzy_set_in[1][1].slope = 0.1;
	fuzzy_set_in[1][1].mode = 1;

	fuzzy_set_out[0][0].center = 0;
	fuzzy_set_out[0][0].slope = 0.1;
	fuzzy_set_out[0][0].mode = -1;

	fuzzy_set_out[0][1].center = 0;
	fuzzy_set_out[0][1].slope = 0.1;
	fuzzy_set_out[0][1].mode = 0;

	fuzzy_set_out[0][2].center = 100;
	fuzzy_set_out[0][2].slope = 0.1;
	fuzzy_set_out[0][2].mode = 1;
	

	int r, s;
	float element;
	float u[2];
	float fraction_top = 0.0;
	float fraction_bottom = 0.0;
	float output;
	printf("Enter first number: ");
	scanf("%f", &u[0]);
	printf("Enter second number: ");
	scanf("%f", &u[1]);

	for(r = 0; r < C_RULEMAX; r++){
		F[r] = 1.0;

		for(s = 0; s < 2; s++){
			int rule_set = rules[r][s];
			element = fuz_tri(fuzzy_set_in[s][rule_set].mode,
					 		  fuzzy_set_in[s][rule_set].center, 
					 		  fuzzy_set_in[s][rule_set].slope, 
					 		  u[s]);
			F[r] *= element;
		}
		int output_set = rules[r][2];

			fraction_top += F[r] * fuzzy_set_out[0][output_set].center;
			fraction_bottom += F[r];
}

	if(fraction_bottom != 0){
		output = fraction_top / fraction_bottom;
		printf("Result of Multiplication: %g x %g = %g\n", u[0], u[1], output);
	}


}
