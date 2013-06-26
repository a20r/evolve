#ifndef _MUTATE_H_
#define _MUTATE_H_

/* FUNCTIONS */
void mutate_str(char **str);
void mutator(
        struct chromosome_pair **c_pair,
        float mutate_chance,
        void (mutator)(char **)
);

#endif
