#pragma once
// clang-format off


#define MACHINE_NAME            "ESP32_V4"
#define N_AXIS 2

#define X_STEP_PIN              GPIO_NUM_33
#define X_DIRECTION_PIN         GPIO_NUM_25
#define X_LIMIT_PIN             GPIO_NUM_34

#define Y_STEP_PIN              GPIO_NUM_26
#define Y_DIRECTION_PIN         GPIO_NUM_27
#define Y_LIMIT_PIN             GPIO_NUM_35

//#define Z_STEP_PIN              GPIO_NUM_26
//#define Z_DIRECTION_PIN         GPIO_NUM_25
//#define Z_LIMIT_PIN             GPIO_NUM_34


// OK to comment out to use pin for other features
#define STEPPERS_DISABLE_PIN    GPIO_NUM_13
#define DEFAULT_HOMING_ENABLE   1
#define HOMING_PIN   GPIO_NUM_21
#define UNLOCK_PIN   GPIO_NUM_19

//#define SPINDLE_TYPE            SpindleType::RELAY
//#define SPINDLE_OUTPUT_PIN      GPIO_NUM_19   // labeled SpinPWM
//#define SPINDLE_ENABLE_PIN      GPIO_NUM_19  // labeled SpinEnbl
//#define COOLANT_FLOOD_PIN       GPIO_NUM_34  // labeled Flood
//#define COOLANT_MIST_PIN        GPIO_NUM_34  // labeled Mist

//#define PROBE_PIN               GPIO_NUM_18  // labeled Probe
#define MACRO_BUTTON_0_PIN      GPIO_NUM_5
#define MACRO_BUTTON_1_PIN      GPIO_NUM_18
#define CONTROL_FEED_HOLD_PIN   GPIO_NUM_22  // labeled Hold,  needs external pullup
#define CONTROL_CYCLE_START_PIN GPIO_NUM_23  // labeled Start, needs external pullup


//#define CONTROL_SAFETY_DOOR_PIN GPIO_NUM_35  // labeled Door,  needs external pullup
//#define CONTROL_RESET_PIN       GPIO_NUM_35  // labeled Reset, needs external pullup

// Status pins
#define STATUS_IDLE_PIN GPIO_NUM_2
#define STATUS_RUN_PIN GPIO_NUM_4
#define STATUS_HOLD_PIN GPIO_NUM_16
#define STATUS_ALARM_PIN GPIO_NUM_17