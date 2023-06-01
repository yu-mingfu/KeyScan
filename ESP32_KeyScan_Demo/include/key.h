#ifndef __KEY_H
#define __KEY_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define     KEY1    16
#define     KEY2    4
#define     BOOT    0
void key_init();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif