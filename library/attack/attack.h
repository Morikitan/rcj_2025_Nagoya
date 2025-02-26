#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void Attack();
void LineMove();
void ChaseBall(float angle,bool isMakao);
void Makao(bool isClockwise,int TargetAngle);

#ifdef __cplusplus
}
#endif