#pragma once
// clang-format off

#define MACHINE_NAME            "Nodi.Box.1D"
#define N_AXIS 1
#define X_STEP_PIN              GPIO_NUM_27
#define X_DIRECTION_PIN         GPIO_NUM_14
#define X_LIMIT_PIN             GPIO_NUM_12
#define STEPPERS_DISABLE_PIN    GPIO_NUM_32 // UNDEFINED_PIN
#define SYS_RUNNING             GPIO_NUM_22  // system running
#define COOLANT_FLOOD_PIN       GPIO_NUM_23  // labeled Flood

//#define PROBE_PIN               GPIO_NUM_2  // labeled Probe
//#define CONTROL_FEED_HOLD_PIN   GPIO_NUM_36  // labeled Hold,  needs external pullup
//#define CONTROL_CYCLE_START_PIN GPIO_NUM_39  // labeled Start, needs external pullup

#define MACRO_BUTTON_0_PIN      GPIO_NUM_18
#define MACRO_BUTTON_1_PIN      GPIO_NUM_5
#define MACRO_BUTTON_2_PIN      GPIO_NUM_21
#define MACRO_BUTTON_3_PIN      GPIO_NUM_19
