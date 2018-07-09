#ifndef _BASIC_MY_H_
#define _BASIC_MY_H_

enum ID
{
    idERROR,
    idREADY,
    idDELETE,
    idDESTROY
};

void run(void);

void update_image(void *data, int size);

#endif
