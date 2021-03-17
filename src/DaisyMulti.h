#ifndef DAISY_MULTI_H
#define DAISY_MULTI_H

#include "../include/PedalConfig.h"
#include "DaisyDisplay.h"
#include "../include/IEffect.h"
#include "../lib/Inputs/Button.h"
#include "../lib/Inputs/Knob.h"
#include "../lib/Effects/CleanBoost.h"
#include "../lib/Effects/DaisyChorus.h"
#include "../lib/Effects/DaisyFlanger.h"
#include "../lib/Effects/DaisyTremolo.h"
#include "../lib/Effects/Crush.h"
#include "../lib/Effects/Distortion.h"
#include "../lib/Effects/Drive.h"
#include "../lib/Effects/AutoWah.h"
#include "../lib/Effects/Echo.h"
#include "../lib/Effects/DaisyCompressor.h"
#include "../lib/Effects/Reverb.h"
#include "../lib/Effects/DaisyPhaser.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hw
DaisySeed *hw;
IEffect *currentEffect = new CleanBoost();

// Control Encoder
Encoder controlEncoder;

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