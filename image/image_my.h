#ifndef _BASIC_MY_H_
#define _BASIC_MY_H_

enum ID { idERROR, idREADY, idCLOSE, idDESTROY };

int run(void);
void quit(void);

void update_image(void *data, int size);

#endif
