# Evolve
Evolve contains Genetic Algorithm (GA) runners that help make running them easier.

## Function Summary

    int run_evolution(
        struct population **p,
        float (eval_func)(char *),
        float crossover_prob,
        float mutate_prob,
        struct evolve_monitor *m
    )
    

### run_evolution
#### Description
This function encapsulates the whole GA process into one function, with parameters that you can tweak to change the behaviour of the GA. Namely the evaulation function (or fitness function), crossover and mutation probabilities, and also an optional evolve_monitor.

#### Parameters

        struct population **p
            Population of the GA
            
        float (eval_func)(char *)
            The evaluation or fitness function
            
        float crossover_prob
            Crossover probability
        
        float mutate_prob
            Mutation probability
            
        struct evolve_monitor *m
            Evolution monitor. Optional can be **NULL**
