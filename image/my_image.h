#ifndef _MY_BASIC_H_
#define _MY_BASIC_H_

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
