// -------------------------------------------------------------------------------------------------
// Pin map for OnStep PerseusPro v1.0 (Teensy 3.1)
#pragma once

#if defined(__MK20DX256__) || defined(_mk20dx128_h_) || defined(__MK20DX128__) || defined(__IMXRT1052__) || defined(__IMXRT1062__)

// Serial ports (see Pins.defaults.h for SERIAL_A)
// Serial1: RX1 Pin 0, TX1 Pin 1

#if SERIAL_A_BAUD_DEFAULT != OFF
  #define SERIAL_A              Serial
#endif
#if SERIAL_B_BAUD_DEFAULT != OFF
  #define SERIAL_B              Serial1
#endif
#if SERIAL_C_BAUD_DEFAULT != OFF
  #error "Configuration (Config.h): SerialC isn't supported, disable this option."
#endif
#if defined(USB_DUAL_SERIAL) || defined(USB_TRIPLE_SERIAL)
  #define SERIAL_D              SerialUSB1
  #define SERIAL_D_BAUD_DEFAULT 9600
#endif
#if defined(USB_TRIPLE_SERIAL)
  #define SERIAL_E              SerialUSB2
  #define SERIAL_E_BAUD_DEFAULT 9600
#endif

// The multi-purpose pins (Aux3..Aux8 can be analog pwm/dac if supported)
#define AUX0_PIN                5//19            // Status LED-  
#define AUX1_PIN                28//18           // RST/SLP of A4988_1,   ESP8266 GPIO0, SPI MISO/Fault
#define AUX2_PIN                28//5            // RST/SLP of A4988_2,   ESP8266 RST, SPI MISO/Fault
#define AUX3_PIN                3//4             // Limit SW, Home SW
#define AUX4_PIN                7//22            // Status LED2-,   Reticle LED, Home SW
#if !defined(_mk20dx128_h_) && !defined(__MK20DX128__) && !defined(__IMXRT1052__) && !defined(__IMXRT1062__)
  #define AUX5_PIN              DAC_PIN(A14)     // true analog output
#endif

#define AUX6_PIN                 8  // LED3_N
#define AUX7_PIN                29  // LED4_P

#define AUX8_PIN                33  // FOC_IN
#define AUX9_PIN                32  // FOC_OUT  (PWM)




// Misc. pins
#ifndef ONE_WIRE_PIN
  #define ONE_WIRE_PIN          29//24           // SCL1,   Default Pin for OneWire bus
#endif
#if PINMAP == MiniPCB13
  #define ADDON_GPIO0_PIN       2                // ESP8266 GPIO0 (Dir2)
  #define ADDON_RESET_PIN       AUX2_PIN         // ESP8266 RST
#else
  #define ADDON_GPIO0_PIN       31//AUX1_PIN     // N/A,  ESP8266 GPIO0 or SPI MISO/Fault
  #define ADDON_RESET_PIN       32//AUX2_PIN     // N/A,  ESP8266 RST or SPI MISO/Fault
#endif

// The PEC index sense is a logic level input, resets the PEC index on rising edge then waits for 60 seconds before allowing another reset
#define PEC_SENSE_PIN           2//23            // PEC Sense, analog or digital

// The status LED is a two wire jumper with a 10k resistor in series to limit the current to the LED
#define STATUS_LED_PIN          AUX4_PIN//7      // Default LED Cathode (-)
#define MOUNT_LED_PIN           AUX6_PIN//8      // Default LED Cathode (-)

#ifndef RETICLE_LED_PIN 
  #define RETICLE_LED_PIN       AUX0_PIN//8      // Default LED Cathode (-)
#endif

// For a piezo buzzer
#ifndef STATUS_BUZZER_PIN 
  #define STATUS_BUZZER_PIN     AUX7_PIN//29     // N/A,  Tone
#endif

// The PPS pin is a 3.3V logic input, OnStep measures time between rising edges and adjusts the internal sidereal clock frequency
#ifndef PPS_SENSE_PIN
  #define PPS_SENSE_PIN         33//28           // N/A,  PPS time source, GPS for example
#endif

#ifndef LIMIT_SENSE_PIN
  #define LIMIT_SENSE_PIN       AUX3_PIN         // The limit switch sense is a logic level input normally pull high (2k resistor,) shorted to ground it stops gotos/tracking
#endif

// hint that the driver mode pins are dedicated (not shared SPI bus except possibly MISO)
#define DEDICATED_MODE_PINS

// Axis1 RA/Azm step/dir driver
#define AXIS1_ENABLE_PIN        21//14
#define AXIS1_M0_PIN            18//15           // M0,   SPI MOSI
#define AXIS1_M1_PIN            19//16           // M1,   SPI SCK
#define AXIS1_M2_PIN            20//17           // M2,   SPI CS (UART TX)
#define AXIS1_M3_PIN            AUX1_PIN         // N/A,  SPI MISO (UART RX)
#define AXIS1_STEP_PIN          6//20           // STEP
#define AXIS1_DIR_PIN           4//21           // DIR
#define AXIS1_FAULT_PIN         AXIS1_M3_PIN     // N/A,  SPI MISO
#ifndef AXIS1_SENSE_HOME_PIN
  #define AXIS1_SENSE_HOME_PIN  AUX3_PIN         // N/A,
#endif
#define AXIS1_ENCODER_A_PIN     AXIS1_M0_PIN     // N/A,
#define AXIS1_ENCODER_B_PIN     AXIS1_M1_PIN     // N/A,
#define AXIS1_SERVO_PH1_PIN     AXIS1_DIR_PIN    // N/A,
#define AXIS1_SERVO_PH2_PIN     AXIS1_STEP_PIN   // N/A,

// Axis2 Dec/Alt step/dir driver
#define AXIS2_ENABLE_PIN        16//9
#define AXIS2_M0_PIN            13//8            // SPI MOSI
#define AXIS2_M1_PIN            14//7            // SPI SCK
#define AXIS2_M2_PIN            15//6            // SPI CS (UART TX)
#define AXIS2_M3_PIN            AUX1_PIN         // SPI MISO (UART RX)
#define AXIS2_STEP_PIN          12//3
#define AXIS2_DIR_PIN           11//2
#define AXIS2_FAULT_PIN         AXIS2_M3_PIN     // N/A
#ifndef AXIS2_SENSE_HOME_PIN
  #define AXIS2_SENSE_HOME_PIN  AUX4_PIN         // N/A
#endif
#define AXIS2_ENCODER_A_PIN     AXIS2_M0_PIN     // N/A,
#define AXIS2_ENCODER_B_PIN     AXIS2_M1_PIN     // N/A,
#define AXIS2_SERVO_PH1_PIN     AXIS2_DIR_PIN    // N/A,
#define AXIS2_SERVO_PH2_PIN     AXIS2_STEP_PIN   // N/A,

// For rotator stepper driver
#define AXIS3_ENABLE_PIN        OFF
#define AXIS3_M0_PIN            OFF              // SPI MOSI
#define AXIS3_M1_PIN            OFF              // SPI SCK
#define AXIS3_M2_PIN            OFF              // SPI CS (UART TX)
#define AXIS3_M3_PIN            OFF              // SPI MISO (UART RX)
#define AXIS3_STEP_PIN          30
#define AXIS3_DIR_PIN           33

// For focuser1 stepper driver
#define AXIS4_ENABLE_PIN        OFF
#define AXIS4_M0_PIN            OFF              // SPI MOSI
#define AXIS4_M1_PIN            OFF              // SPI SCK
#define AXIS4_M2_PIN            OFF              // SPI CS (UART TX)
#define AXIS4_M3_PIN            OFF              // SPI MISO (UART RX)
#define AXIS4_STEP_PIN          31
#define AXIS4_DIR_PIN           32

// For focuser2 stepper driver
#define AXIS5_ENABLE_PIN        OFF
#define AXIS5_M0_PIN            OFF              // SPI MOSI
#define AXIS5_M1_PIN            OFF              // SPI SCK
#define AXIS5_M2_PIN            OFF              // SPI CS (UART TX)
#define AXIS5_M3_PIN            OFF              // SPI MISO (UART RX)
#define AXIS5_STEP_PIN          30
#define AXIS5_DIR_PIN           33

// ST4 interface
#define ST4_RA_W_PIN            27//10           // ST4 RA- West
#define ST4_DEC_S_PIN           26//11           // ST4 DE- South
#define ST4_DEC_N_PIN           25//12           // ST4 DE+ North
#define ST4_RA_E_PIN            24//13           // ST4 RA+ East

#else
#error "Wrong processor for this configuration!"

#endif
