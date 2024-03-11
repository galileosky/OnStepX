// Platform setup ------------------------------------------------------------------------------------
#pragma once

// We define a more generic symbol, in case more STM32 boards based on different lines are supported
#define __ARM_STM32__

// Analog read and write
#ifndef ANALOG_READ_RANGE
  #define ANALOG_READ_RANGE 1023
#endif
#ifndef ANALOG_WRITE_RANGE
  #define ANALOG_WRITE_RANGE 255
#endif
#ifndef ANALOG_WRITE_PWM_BITS
  #define ANALOG_WRITE_PWM_BITS 8 // up to 16 bits
#endif

// Base rate for critical task timing
#define HAL_FRACTIONAL_SEC 500.0F

// Lower limit (fastest) step rate in uS for this platform (in SQW mode) and width of step pulse
#define HAL_MAXRATE_LOWER_LIMIT 1.5
#define HAL_PULSE_WIDTH 0  // effectively disable pulse mode
#define HAL_FAST_PROCESSOR
#define HAL_VFAST_PROCESSOR

#include <HardwareTimer.h>

// Interrupts
#define cli() noInterrupts()
#define sei() interrupts()

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire
#ifndef HAL_WIRE_CLOCK
  #define HAL_WIRE_CLOCK 100000
#endif

// Non-volatile storage ------------------------------------------------------------------------------
#undef E2END
#if NV_DRIVER == NV_DEFAULT
  #define E2END 4095
  #define NV_ADDRESS 0x57
  #include "../lib/nv/NV_24XX.h"
  #define HAL_NV_INIT() nv.init(E2END + 1, true, 0, false, &HAL_Wire, NV_ADDRESS)
#endif

//--------------------------------------------------------------------------------------------------
// General purpose initialize for HAL

#define HAL_INIT() { \
  analogWriteResolution(ANALOG_WRITE_PWM_BITS); \
  HAL_Wire.setSDA(PB9); \
  HAL_Wire.setSCL(PB8); \
  HAL_Wire.setClock(HAL_WIRE_CLOCK); \
  HAL_Wire.begin(); \
}

//--------------------------------------------------------------------------------------------------
// Internal MCU temperature (in degrees C)
#define HAL_TEMP() ( NAN )

//---------------------------------------------------------------------------------------------------
// Misc. includes to support this processor's operation

// MCU reset
#define HAL_RESET() NVIC_SystemReset()

// a really short fixed delay (none needed)
#define HAL_DELAY_25NS()

// stand-in for delayNanoseconds()
#define delayNanoseconds(ns) delayMicroseconds(ceilf(ns/1000.0F))
