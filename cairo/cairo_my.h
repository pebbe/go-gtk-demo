#ifndef _CAIRO_MY_H_
#define _CAIRO_MY_H_

enum ID
{
    idERROR,
    idREADY,
    idDELETE,
    idDESTROY,
};

void run(void);

void update_image(int hour, int minute, int second);

#endif
