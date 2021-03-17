#include "DaisyMulti.h"

/**
 * Audio callback to process each enabled effect
 */
void AudioCallback(float **in, float **out, size_t size)
{
    // Interrupt handler for the control encoder
    ControlEncoderInterrupt();

    for (size_t i = 0; i < size; i++)
    {
        float wet = in[AUDIO_IN_CH][i];

        // Apply the effect
        wet = currentEffect->Process(wet);

        // Output the processed signal with the volume level control
        out[AUDIO_OUT_CH][i] = wet * outputLevel;
    }
}

/**
 * Initializes the input controls
 */
void InitializeControls()
{
    // Initialize the 4 knobs
    AdcChannelConfig adcConfig[4];
    adcConfig[KNOB_1_CHN].InitSingle(hw->GetPin(effectPotPin1));
    adcConfig[KNOB_2_CHN].InitSingle(hw->GetPin(effectPotPin2));
    adcConfig[KNOB_3_CHN].InitSingle(hw->GetPin(effectPotPin3));
    adcConfig[KNOB_4_CHN].InitSingle(hw->GetPin(effectPotPin4));
    hw->adc.Init(adcConfig, 4);
    hw->adc.Start();

    // TODO: Find a better way to do this?
    // Give the ADC time to start up
    System::Delay(500);

    // Initialize the controlEncoder
    controlEncoder.Init(hw->GetPin(effectSelectorPinA), hw->GetPin(effectSelectorPinB), hw->GetPin(effectSelectorPinSw), hw->AudioCallbackRate());
}

/**
 * Initializes the effects
 */
void InitializeEffects()
{
    // Initialize the effect
    currentEffect->Setup(hw, &display);
}

/**
 * Reads the control encoder and triggers actions
 */
void ControlEncoderInterrupt()
{
    // Check for a change in the selected effect
    int inc = controlEncoder.Increment();
    if (inc != 0)
    {
        // Increment the output volume
        newOutputLevel = outputLevel + ((float)inc * outputLevelIncrement);

        // Check if we have hit an edge
        if (newOutputLevel < outputLevelMin)
        {
            newOutputLevel = outputLevelMin;
        }
        else if (newOutputLevel > outputLevelMax)
        {
            newOutputLevel = outputLevelMax;
        }
    }
}

/**
 * Main Loop
 */
int main(void)
{
    // Configure and Initialize the Daisy Seed
    hw->Configure();
    hw->Init();

    // Initialize the OLED display
    initDisplay(hw, display);

    // Initialize debug printing (true = wait for COM connection before continuing)
    initDebugPrint(hw, WAIT_FOR_SERIAL);
    debugPrintln(hw, "Starting DaisyPedal...");

    // Update the block size and sample rate to minimize noise
    hw->SetAudioBlockSize(DAISY_BLOCKSIZE);
    hw->SetAudioSampleRate(DAISY_SAMPLE_RATE);

    // Initialize the input controls
    InitializeControls();

    // Initialize the effect objects
    InitializeEffects();

    // Start the audio processing
    debugPrintln(hw, "Starting Audio");
    hw->StartAudio(AudioCallback);

    // Turn on the onboard LED
    hw->SetLed(true);

    // Loop forever
    for (;;)
    {
        // Check for a change in output level
        if (outputLevel != newOutputLevel)
        {
            outputLevel = newOutputLevel;
            debugPrintlnF(hw, "Changed output level to: %.2f", outputLevel);
            updateOutputLevel(display, outputLevel);
        }

        // Call the effect's loop
        currentEffect->Loop(true);
    }
}