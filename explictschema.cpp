#include "explictschema.h"

void ExplictSchema::step()
{
	double h_r_2 = h_r*h_r;
	second_array[0] = first_array[0] + h_t*(4*k_c*(first_array[1] -
			first_array[0])/h_r_2 - alpha_cl*first_array[0]);
	for(int i = 1; i < I; i++)
		second_array[i] = first_array[i] + h_t*(k_c*((first_array[i+1] -
				2*first_array[i] + first_array[i-1])/h_r_2 + 0.5/(h_r-2*i)*
				(first_array[i+1]-first_array[i-1])) - alpha_cl*first_array[0]);
	second_array[I] = 0;
}
