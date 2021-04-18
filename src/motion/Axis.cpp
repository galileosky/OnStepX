// -----------------------------------------------------------------------------------
// Axis motion control
#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"
#include "../pinmaps/Models.h"
#include "../debug/Debug.h"
#include "../tasks/OnTask.h"
extern Tasks tasks;

#include "../telescope/Telescope.h"
extern Telescope telescope;
#include "Axis.h"

extern unsigned long periodSubMicros;

#if AXIS1_DRIVER_MODEL != OFF
  const AxisPins     Axis1StepPins = {AXIS1_STEP_PIN, AXIS1_DIR_PIN, AXIS1_ENABLE_PIN, false, false, true};
  const AxisSettings Axis1Settings = {AXIS1_STEPS_PER_DEGREE*RAD_DEG_RATIO, 0, AXIS1_DRIVER_REVERSE, { degToRad(AXIS1_LIMIT_MIN), degToRad(AXIS1_LIMIT_MAX) } };
  IRAM_ATTR void moveAxis1() { telescope.mount.axis1.move(AXIS1_STEP_PIN, AXIS1_DIR_PIN); }
  IRAM_ATTR void slewForwardAxis1() { telescope.mount.axis1.slewForward(AXIS1_STEP_PIN); }
  IRAM_ATTR void slewReverseAxis1() { telescope.mount.axis1.slewReverse(AXIS1_STEP_PIN); }
#endif
#if AXIS2_DRIVER_MODEL != OFF
  const AxisPins     Axis2StepPins = {AXIS2_STEP_PIN, AXIS2_DIR_PIN, AXIS2_ENABLE_PIN, false, false, true};
  const AxisSettings Axis2Settings = {AXIS2_STEPS_PER_DEGREE*RAD_DEG_RATIO, 0, AXIS2_DRIVER_REVERSE, { degToRad(AXIS2_LIMIT_MIN), degToRad(AXIS2_LIMIT_MAX) } };
  IRAM_ATTR void moveAxis2() { telescope.mount.axis2.move(AXIS2_STEP_PIN, AXIS2_DIR_PIN); }
  IRAM_ATTR void slewForwardAxis2() { telescope.mount.axis2.slewForward(AXIS2_STEP_PIN); }
  IRAM_ATTR void slewReverseAxis2() { telescope.mount.axis2.slewReverse(AXIS2_STEP_PIN); }
#endif
#if AXIS3_DRIVER_MODEL != OFF
  const AxisPins     Axis3StepPins = {AXIS3_STEP_PIN, AXIS3_DIR_PIN, AXIS3_ENABLE_PIN, false, false, true};
  const AxisSettings Axis3Settings = {AXIS3_STEPS_PER_DEGREE*RAD_DEG_RATIO, 0, AXIS3_DRIVER_REVERSE, { degToRad(AXIS3_LIMIT_MIN), degToRad(AXIS3_LIMIT_MAX) } };
  inline void moveAxis3() { telescope.rotator.axis.move(AXIS3_STEP_PIN, AXIS3_DIR_PIN); }
#endif
#if AXIS4_DRIVER_MODEL != OFF
  const AxisPins     Axis4StepPins = {AXIS4_STEP_PIN, AXIS4_DIR_PIN, AXIS4_ENABLE_PIN, false, false, true};
  const AxisSettings Axis4Settings = {AXIS4_STEPS_PER_DEGREE*RAD_DEG_RATIO, 0, AXIS4_DRIVER_REVERSE, { degToRad(AXIS4_LIMIT_MIN), degToRad(AXIS4_LIMIT_MAX) } };
  inline void moveAxis4() { telescope.focuser1.axis.move(AXIS4_STEP_PIN, AXIS4_DIR_PIN); }
#endif
#if AXIS5_DRIVER_MODEL != OFF
  const AxisPins     Axis5StepPins = {AXIS5_STEP_PIN, AXIS5_DIR_PIN, AXIS5_ENABLE_PIN, false, false, true};
  const AxisSettings Axis5Settings = {AXIS5_STEPS_PER_DEGREE*RAD_DEG_RATIO, 0, AXIS5_DRIVER_REVERSE, { degToRad(AXIS5_LIMIT_MIN), degToRad(AXIS5_LIMIT_MAX) } };
  inline void moveAxis5() { telescope.focuser2.axis.move(AXIS2_STEP_PIN, AXIS5_DIR_PIN); }
#endif
#if AXIS6_DRIVER_MODEL != OFF
  const AxisPins     Axis6StepPins = {AXIS6_STEP_PIN, AXIS6_DIR_PIN, AXIS6_ENABLE_PIN, false, false, true};
  const AxisSettings Axis6Settings = {AXIS6_STEPS_PER_DEGREE*RAD_DEG_RATIO, 0, AXIS6_DRIVER_REVERSE, { degToRad(AXIS6_LIMIT_MIN), degToRad(AXIS6_LIMIT_MAX) } };
  inline void moveAxis6() { telescope.focuser3.axis.move(AXIS6_STEP_PIN, AXIS6_DIR_PIN); }
#endif

static uint8_t pollingTaskHandle  = 0;
inline void pollAxes() {
  #if AXIS1_DRIVER_MODEL != OFF
    telescope.mount.axis1.poll();
  #endif
  #if AXIS2_DRIVER_MODEL != OFF
    telescope.mount.axis2.poll();
  #endif
  #if AXIS3_DRIVER_MODEL != OFF
    telescope.mount.axis3.poll();
  #endif
  #if AXIS4_DRIVER_MODEL != OFF
    telescope.mount.axis4.poll();
  #endif
  #if AXIS5_DRIVER_MODEL != OFF
    telescope.mount.axis5.poll();
  #endif
  #if AXIS6_DRIVER_MODEL != OFF
    telescope.mount.axis6.poll();
  #endif
}

void Axis::init(uint8_t axisNumber) {
  if (pollingTaskHandle == 0) {
    VF("MSG: Axis, starting axis polling task (rate 20ms priority 0)... ");
    pollingTaskHandle = tasks.add(20, 0, true, 0, pollAxes, "AxsPoll");
    if (pollingTaskHandle) VL("success"); else VL("FAILED!");
  }

  this->axisNumber = axisNumber;

  taskHandle = 0;
  #if AXIS1_DRIVER_MODEL != OFF
    if (axisNumber == 1) {
      pins = Axis1StepPins; settings = Axis1Settings;
      taskHandle = tasks.add(0, 0, true, 0, moveAxis1, "Axis1");
      if (!tasks.requestHardwareTimer(taskHandle, 1, 0)) VLF("WRN: Axis, didn't get h/w timer for Axis1 (using s/w timer)");
    }
  #endif
  #if AXIS1_DRIVER_MODEL != OFF
    if (axisNumber == 2) {
      pins = Axis2StepPins; settings = Axis2Settings;
      taskHandle = tasks.add(0, 0, true, 0, moveAxis2, "Axis2");
      if (!tasks.requestHardwareTimer(taskHandle, 2, 0)) VLF("WRN: Axis, didn't get h/w timer for Axis2 (using s/w timer)");
    }
  #endif
  #if AXIS3_DRIVER_MODEL != OFF
    pins = Axis3StepPins; settings = Axis3Settings;
    if (axisNumber == 3) taskHandle = tasks.add(0, 0, true, 0, moveAxis3, "Axis3");
  #endif
  #if AXIS4_DRIVER_MODEL != OFF
    pins = Axis4StepPins; settings = Axis4Settings;
    if (axisNumber == 4) taskHandle = tasks.add(0, 0, true, 0, moveAxis4, "Axis4");
  #endif
  #if AXIS5_DRIVER_MODEL != OFF
    pins = Axis5StepPins; settings = Axis5Settings;
    if (axisNumber == 5) taskHandle = tasks.add(0, 0, true, 0, moveAxis5, "Axis5");
  #endif
  #if AXIS6_DRIVER_MODEL != OFF
    pins = Axis6StepPins; settings = Axis6Settings;
    if (axisNumber == 6) taskHandle = tasks.add(0, 0, true, 0, moveAxis6, "Axis6");
  #endif
  
  VF("MSG: Axis, starting moveAxis"); V(axisNumber); VF(" task... ");
  if (taskHandle) VL("success"); else VL("FAILED!");

  driver.init(axisNumber);

  if (settings.reverse) invertDir = !invertDir;

  pinModeInitEx(pins.step, OUTPUT, !invertStep?LOW:HIGH);
  pinModeInitEx(pins.dir, OUTPUT, !invertDir?LOW:HIGH);
  pinModeEx(pins.enable, OUTPUT); enable(false);
}

void Axis::enable(bool value) {
  if (pins.enable != OFF) {
    if (value) digitalWrite(pins.enable, invertEnable?HIGH:LOW); else digitalWrite(pins.enable, invertEnable?LOW:HIGH);
  }
  enabled = value;
}

bool Axis::isEnabled() {
  return enabled;
}

double Axis::getStepsPerMeasure() {
  return settings.stepsPerMeasure;
}

int Axis::getStepsPerStepSlewing() {
  return stepsPerStepSlewing;
}

void Axis::setMotorCoordinate(double value) {
  long steps = lround(value*settings.stepsPerMeasure);
  setMotorCoordinateSteps(steps);
}

double Axis::getMotorCoordinate() {
  noInterrupts();
  long steps = motorSteps + backlashSteps;
  interrupts();
  return steps/settings.stepsPerMeasure;
}

void Axis::setMotorCoordinateSteps(long value) {
  indexSteps    = 0;
  noInterrupts();
  motorSteps    = value;
  targetSteps   = value;
  backlashSteps = 0;
  interrupts();
}

long Axis::getMotorCoordinateSteps() {
  noInterrupts();
  long steps = motorSteps + backlashSteps;
  interrupts();
  return steps;
}

long Axis::getInstrumentCoordinateSteps() {
  noInterrupts();
  long steps = motorSteps + indexSteps;
  interrupts();
  return steps;
}

void Axis::setInstrumentCoordinate(double value) {
  long steps = value*settings.stepsPerMeasure;
  noInterrupts();
  indexSteps = steps - motorSteps;
  interrupts();
}

double Axis::getInstrumentCoordinate() {
  noInterrupts();
  long steps = motorSteps + indexSteps;
  interrupts();
  return steps/settings.stepsPerMeasure;
}

void Axis::markOriginCoordinate() {
  noInterrupts();
  originSteps = motorSteps;
  interrupts();
}

void Axis::setTargetCoordinate(double value) {
  target = value;
  long steps = lround(value*settings.stepsPerMeasure);
  noInterrupts();
  targetSteps = steps - indexSteps;
  interrupts();
}

void Axis::setTargetCoordinateSteps(long value) {
  target = value/settings.stepsPerMeasure;
  noInterrupts();
  targetSteps = value - indexSteps;
  interrupts();
}

double Axis::getTargetCoordinate() {
  noInterrupts();
  long steps = targetSteps + indexSteps;
  interrupts();
  return steps/settings.stepsPerMeasure;
}

long Axis::getTargetCoordinateSteps() {
  noInterrupts();
  long steps = targetSteps + indexSteps;
  interrupts();
  return steps;
}

void Axis::incrementTargetCoordinate(double value) {
  target += value;
  setTargetCoordinate(value);
}

bool Axis::nearTarget() {
  return labs(motorSteps - targetSteps) <= step * 2;
}

double Axis::getOriginOrTargetDistance() {
  noInterrupts();
  long steps = motorSteps;
  interrupts();
  long distanceOrigin = labs(originSteps - steps);
  long distanceTarget = labs(targetSteps - steps);
  if (distanceOrigin < distanceTarget) return distanceOrigin/settings.stepsPerMeasure; else return distanceTarget/settings.stepsPerMeasure;
}

void Axis::setFrequencyMax(float frequency) {
  maxFreq = frequency;
  if (frequency != 0.0) minPeriodMicros = 1000000.0F/(maxFreq*settings.stepsPerMeasure); else minPeriodMicros = 0.0F;
}

void Axis::setSlewAccelerationRate(float mpsps) {
  slewMpspcs = mpsps/100.0F;
}

void Axis::setSlewAccelerationRateAbort(float mpsps) {
  abortMpspcs = mpsps/100.0F;
}

void Axis::autoSlewRateByDistance(float distance) {
  autoRate = AR_RATE_BY_DISTANCE;
  slewAccelerationDistance = distance;
  driver.modeDecaySlewing();
  VF("MSG: Axis::autoSlewRateByDistance(); Axis"); V(axisNumber); VLF(" slew started");
}

void Axis::autoSlewRateByDistanceStop() {
  driver.modeDecayTracking();
  autoRate = AR_NONE;
}

void Axis::autoSlew(Direction direction) {
  if (direction == DIR_NONE) return;
  if (direction == DIR_FORWARD) autoRate = AR_RATE_BY_TIME_FORWARD; else autoRate = AR_RATE_BY_TIME_REVERSE;
  driver.modeDecaySlewing();
  VF("MSG: Axis::autoSlew(); Axis"); V(axisNumber); VLF(" slew started");
}

void Axis::autoSlewStop() {
  if (autoRate != AR_NONE && autoRate != AR_RATE_BY_TIME_ABORT) {
    VF("MSG: Axis::autoSlewStop(); Axis"); V(axisNumber); VLF(" slew stopping");
    autoRate = AR_RATE_BY_TIME_END;
    poll();
  }
}

void Axis::autoSlewAbort() {
  if (autoRate != AR_NONE) {
    VF("MSG: Axis::autoSlewAbort(); Axis"); V(axisNumber); VLF(" slew aborting");
    autoRate = AR_RATE_BY_TIME_ABORT;
    poll();
  }
}

bool Axis::autoSlewActive() {
  return autoRate != AR_NONE;  
}

void Axis::poll() {
  // limits

  if (autoRate == AR_NONE) return;

  // acceleration
  if (autoRate == AR_RATE_BY_DISTANCE) {
    freq = (getOriginOrTargetDistance()/slewAccelerationDistance)*maxFreq + backlashFreq;
    if (freq < backlashFreq) freq = backlashFreq;
    if (freq > maxFreq) freq = maxFreq;
  } else
  if (autoRate == AR_RATE_BY_TIME_FORWARD) {
    freq += slewMpspcs;
    if (freq > maxFreq) freq = maxFreq;
  } else
  if (autoRate == AR_RATE_BY_TIME_REVERSE) {
    freq -= slewMpspcs;
    if (freq < -maxFreq) freq = -maxFreq;
  } else
  if (autoRate == AR_RATE_BY_TIME_END) {
    if (freq > slewMpspcs) freq -= slewMpspcs; else if (freq < -slewMpspcs) freq += slewMpspcs; else freq = 0.0F;
    if (fabs(freq) <= slewMpspcs) {
      driver.modeDecayTracking();
      autoRate = AR_NONE;
      freq = 0.0F;
      VF("MSG: Axis::poll(); Axis"); V(axisNumber); VLF(" slew stopped");
    }
  } else
  if (autoRate == AR_RATE_BY_TIME_ABORT) {
    if (freq > abortMpspcs) freq -= abortMpspcs; else if (freq < -abortMpspcs) freq += abortMpspcs; else freq = 0.0F;
    if (fabs(freq) <= abortMpspcs) {
      driver.modeDecayTracking();
      autoRate = AR_NONE;
      freq = 0.0F;
      VF("MSG: Axis::poll(); Axis"); V(axisNumber); VLF(" slew aborted");
    }
  } else freq = 0.0F;


  // ISR swap and microstep mode switching
  if (microstepModeControl == MMC_SLEWING) {
    if (fabs(freq) <= backlashFreq*1.2F || autoRate == AR_NONE) {
      if (driver.modeSwitchAllowed()) driver.modeMicrostepTracking();
      microstepModeControl = MMC_TRACKING;
      enableMoveFast(false);
      VF("MSG: Axis::poll(); Axis"); V(axisNumber); VF(" high speed ISR swapped out at "); VL(radToDeg(freq));
    }
  } else {
    if (fabs(freq) > backlashFreq*1.2F) {
      if (driver.modeSwitchAllowed()) {
        if (microstepModeControl == MMC_TRACKING) { microstepModeControl = MMC_SLEWING_REQUEST; return; } else
        if (microstepModeControl != MMC_SLEWING_READY) return;
        driver.modeMicrostepSlewing();
      }
      microstepModeControl = MMC_SLEWING;
      enableMoveFast(true);
      VF("MSG: Axis::poll(); Axis"); V(axisNumber); VF(" high speed ISR swapped in at "); VL(radToDeg(freq));
    }
  }

  setFrequency(freq);
}

void Axis::setFrequency(float frequency) {
  if (frequency < 0.0F) {
    frequency = -frequency;
    noInterrupts(); trackingStep = -1; interrupts();
  } else {
    noInterrupts(); trackingStep =  1; interrupts();
  }

  lastFreq = frequency;
  // frequency in measures per second to period in microsecond counts per step
  float period = 1000000.0F/(frequency*settings.stepsPerMeasure);
  if (period < minPeriodMicros) period = minPeriodMicros;
  if (STEP_WAVE_FORM == SQUARE) period /= 2.0F;
  if (!isnan(period) && fabs(period) <= 134000000.0F) {
    // convert microsecond counts to sub-microsecond counts
    period *= 16.0F;
    lastPeriodSet = (unsigned long)lround(period);

    // adjust period for MCU clock inaccuracy
    period *= (SIDEREAL_PERIOD/periodSubMicros);
    // if this is the active period, just return
    if (lastPeriod == (unsigned long)lround(period)) return;
    lastPeriod = (unsigned long)lround(period);
  } else { period = 0.0; lastPeriodSet = 0; lastPeriod = 0; }
  tasks.setPeriodSubMicros(taskHandle, lastPeriod);
}

float Axis::getFrequency() {
  return getFrequencySteps()/settings.stepsPerMeasure;
}

float Axis::getFrequencySteps() {
  if (lastPeriodSet == 0) return 0;
  #if STEP_WAVE_FORM == SQUARE
    return 16000000.0F/(lastPeriodSet*2.0F);
  #else
    return 16000000.0F/lastPeriodSet;
  #endif
}

void Axis::setTracking(bool state) {
  this->tracking = state;
}

bool Axis::getTracking() {
  return tracking;
}

void Axis::setBacklash(float value) {
  noInterrupts();
  backlashAmountSteps = value*settings.stepsPerMeasure;
  interrupts();
  settings.backlashAmountSteps = backlashAmountSteps;
}

float Axis::getBacklash() {
  noInterrupts();
  long b = backlashSteps;
  interrupts();
  return b/settings.stepsPerMeasure;
}

bool Axis::inBacklash() {
  noInterrupts();
  long b = backlashSteps;
  interrupts();
  if (b == 0 || b == backlashAmountSteps) return false; else return true;
}

void Axis::disableBacklash() {
  noInterrupts();
  backlashStepsStore = backlashSteps;
  motorSteps += backlashSteps;
  backlashSteps = 0;
  interrupts();
}

void Axis::enableBacklash() {
  noInterrupts();
  backlashSteps = backlashStepsStore;
  motorSteps -= backlashSteps;
  interrupts();
  backlashStepsStore = 0;
}

bool Axis::motionForwardError() {
  return error.motorFault ||
         error.driverFault ||
         error.maxExceeded ||
         error.maxLimitSensed;
}

bool Axis::motionReverseError() {
  return error.motorFault ||
         error.driverFault ||
         error.minExceeded ||
         error.minLimitSensed;
}

bool Axis::motionError() {
  return motionForwardError() || motionReverseError();
}

void Axis::enableMoveFast(const bool fast) {
  #if AXIS1_DRIVER_MODEL != OFF && AXIS2_DRIVER_MODEL != OFF
    if (fast) {
      disableBacklash();
      if (axisNumber == 1) {
        if (direction == DIR_FORWARD) tasks.setCallback(taskHandle, slewForwardAxis1); else tasks.setCallback(taskHandle, slewReverseAxis1);
      }
      if (axisNumber == 2) {
        if (direction == DIR_FORWARD) tasks.setCallback(taskHandle, slewForwardAxis2); else tasks.setCallback(taskHandle, slewReverseAxis2);
      }
    } else {
      if (axisNumber == 1) tasks.setCallback(taskHandle, moveAxis1);
      if (axisNumber == 2) tasks.setCallback(taskHandle, moveAxis2);
      enableBacklash();
    }
  #endif
 }

#if STEP_WAVE_FORM == SQUARE
  IRAM_ATTR void Axis::move(const int8_t stepPin, const int8_t dirPin) {
    if (microstepModeControl == MMC_SLEWING_READY) return;
    if (takeStep) {
      if (direction == DIR_FORWARD) {
        if (backlashSteps < backlashAmountSteps) backlashSteps += step; else motorSteps += step;
        digitalWriteF(stepPin, HIGH);
      } else
      if (direction == DIR_REVERSE) {
        if (backlashSteps > 0) backlashSteps -= step; else motorSteps -= step;
        digitalWriteF(stepPin, HIGH);
      }
    } else {
      if (tracking) targetSteps += trackingStep;
      if (motorSteps + backlashSteps < targetSteps) {
        direction = DIR_FORWARD;
        digitalWriteF(dirPin, invertDir?HIGH:LOW);
      } else if (motorSteps + backlashSteps > targetSteps) {
        direction = DIR_REVERSE;
        digitalWriteF(dirPin, invertDir?LOW:HIGH);
      } else direction = DIR_NONE;
      digitalWriteF(stepPin, LOW);
      if (microstepModeControl == MMC_SLEWING_REQUEST && (motorSteps + backlashSteps)%stepsPerStepSlewing == 0) microstepModeControl = MMC_SLEWING_READY;
    }
    takeStep = !takeStep;
  }
  IRAM_ATTR void Axis::slewForward(const int8_t stepPin, const int8_t dirPin) {
    if (takeStep) {
      if (tracking) targetSteps += trackingStep;
      if (motorSteps < targetSteps) { motorSteps++; digitalWriteF(stepPin, HIGH); }
    } else digitalWriteF(stepPin, LOW);
    takeStep = !takeStep;
  }
  IRAM_ATTR void Axis::slewReverse(const int8_t stepPin, const int8_t dirPin) {
    if (takeStep) {
      if (tracking) targetSteps += trackingStep;
      if (motorSteps > targetSteps) { motorSteps--; digitalWriteF(stepPin, HIGH); }
    } else digitalWriteF(stepPin, LOW);
    takeStep = !takeStep;
  }
#endif
#if STEP_WAVE_FORM == PULSE
  IRAM_ATTR void Axis::move(const int8_t stepPin, const int8_t dirPin) {
    if (microstepModeControl == MMC_SLEWING_READY) return;
    digitalWriteF(stepPin, LOW);
    if (tracking) targetSteps += trackingStep;
    if (motorSteps + backlashSteps < targetSteps) {
      digitalWriteF(dirPin, invertDir?HIGH:LOW);
      direction = DIR_FORWARD;
      if (backlashSteps < backlashAmountSteps) backlashSteps += step; else motorSteps += step;
    } else 
    if (motorSteps + backlashSteps > targetSteps) {
      digitalWriteF(dirPin, invertDir?LOW:HIGH);
      direction = DIR_REVERSE;
      if (backlashSteps > 0) backlashSteps -= step; else motorSteps -= step;
    } else { direction = DIR_NONE; return; }
    digitalWriteF(stepPin, HIGH);
    if (microstepModeControl == MMC_SLEWING_REQUEST && (motorSteps + backlashSteps)%stepsPerStepSlewing == 0) microstepModeControl = MMC_SLEWING_READY;
  }
  IRAM_ATTR void Axis::slewForward(const int8_t stepPin) {
    digitalWriteF(stepPin, LOW);
    if (tracking) targetSteps += trackingStep;
    if (motorSteps < targetSteps) { motorSteps++; digitalWriteF(stepPin, HIGH); }
  }
  IRAM_ATTR void Axis::slewReverse(const int8_t stepPin) {
    digitalWriteF(stepPin, LOW);
    if (tracking) targetSteps += trackingStep;
    if (motorSteps > targetSteps) { motorSteps--; digitalWriteF(stepPin, HIGH); }
  }
#endif
