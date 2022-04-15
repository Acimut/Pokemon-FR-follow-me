#ifndef GUARD_FIELD_SCREEN_H
#define GUARD_FIELD_SCREEN_H

#include "global.h"

// Exported type declarations

// Exported RAM declarations

// Exported ROM declarations
void pal_fill_black(void);
void UpdateWeatherPerDay(u16 days);

void SetPlayerVisibility(bool8 visible);
void Task_Teleport2Warp(u8 taskId);//pokeemerald:   Task_WarpAndLoadMap

#endif //GUARD_FIELD_SCREEN_H
