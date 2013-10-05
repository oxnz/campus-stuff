#ifndef __X_COMMON_H__
#define __X_COMMON_H__

typedef unsigned short Choice;

void mainMenu(void);
void subMenu(void);
Choice getConfirm(char *prompt);
Choice getOpt(void(*menu)(void),int range);

#endif
