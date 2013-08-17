# Crossover
Crossover is one of the Genetic Algorithm (GA) operators.

## Function Summary

    int one_ptr_crossover(void **chromo_1, void **chromo_2, int index)
    void crossover(
            void **child_1,
            void **child_2,
            int index,
            int (*crossover_func)(void **chromo_1, void **chromo_2, int index),
            float crossover_prob
    )
    

### one_pt_crossover
#### Parameters

	void **chromo_1
		Pointer to pointer pointing at chromosome 1
		
	void **chromo_2
		Pointer to pointer pointing at chromosome 1

    int index
    	Crossover pivot index
    	
#### Returns
	
	0 for success, -1 for failure

    	
    	
### crossover
#### Description

#### Parameters

	void **child_1
		Pointer to pointer pointing at child_1
		
	void **child_2
		Pointer to pointer pointing at child_2

    int index
    	Crossover pivot index
    
    int (*crossover_func)(void **chromo_1, void **chromo_2, int index)
    	Pointer to crossover function
    	
    float crossover_prob
    	Crossover probability
