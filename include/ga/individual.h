#ifndef _GA_INDIVIDUAL_H_
#define _GA_INDIVIDUAL_H_

/* STRUCTS */
struct ga_individual {
    char *chromosome;
    float score;
};

/* FUNCTIONS */
struct ga_individual *ga_individual_create(char *chromosome);
void ga_individual_destroy(struct ga_individual *individual);

#endif
