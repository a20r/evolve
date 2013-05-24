#ifndef MUTATE_H
#define MUTATE_H

/* FUNCTIONS */
void mutate_str(char **str);
void mutator(
        struct chromosome_pair **c_pair,
        float mutate_chance,
        void (mutator)(char **)
);

#endif
