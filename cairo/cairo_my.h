#ifndef _CAIRO_MY_H_
#define _CAIRO_MY_H_

enum ID {
    idERROR,
    idREADY,
    idCLOSE,
    idDESTROY,
};

int run(void);
void quit(void);

void update_image(int hour, int minute, int second);

#endif
