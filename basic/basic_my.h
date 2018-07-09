#ifndef _BASIC_MY_H_
#define _BASIC_MY_H_

enum ID
{
    idERROR,
    idREADY,
    idDELETE,
    idDESTROY,
    idBUTTON,
    idTEXT
};

void run(void);

void update_label(void const *text);
void get_text(void);

#endif
