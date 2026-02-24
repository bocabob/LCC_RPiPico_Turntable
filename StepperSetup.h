/*
 *  These are the standard stepper controller and motor definitions.
*/

  #ifndef STANDARD_STEPPERS_h
    #define STANDARD_STEPPERS_h

    // #include <Arduino.h>
    #include "AccelStepper.h"

    #define ULN2003_HALF_CW AccelStepper(AccelStepper::HALF4WIRE, A3, A1, A2, A0)
    #define ULN2003_HALF_CCW AccelStepper(AccelStepper::HALF4WIRE, A0, A2, A1, A3)
    #define ULN2003_FULL_CW AccelStepper(AccelStepper::FULL4WIRE, A3, A1, A2, A0)
    #define ULN2003_FULL_CCW AccelStepper(AccelStepper::FULL4WIRE, A0, A2, A1, A3)
    #define A4988 AccelStepper(AccelStepper::DRIVER, STEPPER_STEP_PIN, STEPPER_DIR_PIN, STEPPER_ENABLE_PIN)
    #define A4988_INV AccelStepper(AccelStepper::DRIVER, STEPPER_STEP_PIN, STEPPER_DIR_PIN, STEPPER_ENABLE_PIN, UNUSED_PIN, true, true)

  #endif
