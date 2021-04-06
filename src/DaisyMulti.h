#ifndef DAISY_MULTI_H
#define DAISY_MULTI_H

#include "../include/PedalConfig.h"
#include "DaisyDisplay.h"
#include "../lib/DaisyEffects/IEffect.h"
#include "../lib/Inputs/Button.h"
#include "../lib/Inputs/Knob.h"
#include "../lib/DaisyEffects/Metronome.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hw
DaisySeed *hw;
IEffect *currentEffect = new Metronome();

// Controls
Encoder controlEncoder;
Button onOffButton;
Led onOffLed;
bool isEffectOn = true;

// OLED display
DaisyDisplay display;

// Output volume control
const float outputLevelIncrement = 0.4f;
const float outputLevelMin = 0.0f;
const float outputLevelMax = 10.0f;
float outputLevel = 5.0f;
float newOutputLevel = 5.0f;

/**
 * Audio callback to process each enabled effect
 */
void AudioCallback(float **in, float **out, size_t size);

/**
 * Initializes the input controls
 */
void InitializeControls();

/**
 * Initializes the effects
 */
void InitializeEffects();

/**
 * Reads the control encoder and triggers actions
 */
void ControlEncoderInterrupt();

#endif