#ifndef _GO_GTK_DEMO_H_
#define _GO_GTK_DEMO_H_

enum ID { idERROR, idDELETE, idDESTROY, idBUTTON, idTEXT };

char *return_arg(int);
int init(int, void*);
void run(void);

void update_label(char *text);
void get_text(void);

#endif
