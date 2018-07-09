#ifndef _MY_BASIC_H_
#define _MY_BASIC_H_

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
