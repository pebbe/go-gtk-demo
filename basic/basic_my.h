#ifndef _BASIC_MY_H_
#define _BASIC_MY_H_

enum ID { idERROR, idREADY, idCLOSE, idDESTROY, idBUTTON, idTEXT };

int run(void);
void quit(void);

void update_label(void const *text);
void get_text(void);

#endif
