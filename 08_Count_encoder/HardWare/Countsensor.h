#ifndef COUNT_H
#define COUNT_H

#include <stdint.h>

void Init_Count(void);
extern volatile  uint32_t count;
int16_t Get_Count(void);

#endif
