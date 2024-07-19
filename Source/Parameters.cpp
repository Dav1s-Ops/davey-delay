/*
  ==============================================================================

    Parameters.cpp
    Created: 17 Jul 2024 1:49:51pm
    Author:  Davis Weimer

  ==============================================================================
*/

#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::ParameterID& id,
                          T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); // if parameter does not exist or is wrong type
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f) {
        return juce::String(value, 2) + " ms";
    } else if (value < 100.f) {
        return juce::String(value, 1) + " ms";
    } else if (value < 1000.f) {
        return juce::String(int(value)) + " ms";
    } else {
        return juce::String(value * 0.001f, 2) + " s";
    }
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " db";
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);
}
// Return type                                      // Function name
juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Arguments passed to the AudioFloatParameter constructor
    layout.add(std::make_unique<juce::AudioParameterFloat>(gainParamID,
                                                           // Name of the parameter
                                                           "Output Gain",
                                                           // Value range
                                                           juce::NormalisableRange<float> { -12.0f, 12.0f },
                                                           // Default value for range
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(delayTimeParamID,
                                                           "Delay Time",
                                                           juce::NormalisableRange<float>{ minDelayTime, maxDelayTime, 0.001f, 0.25f },
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)
                                                           ));
    return layout;
}

// Reads Output Gain parameter
void Parameters::update() noexcept
{
    // Reads current gain value, converts decibels to linear units, uses new value
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    delayTime = delayTimeParam->get();
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    // 0.02s adds 20 millisecond fade
    // this is acceptable at 48kHz
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
}

// Adds re-initializing to gain param to be safe upon host stop/start
void Parameters::reset() noexcept
{
    gain = 0.0f;
    delayTime = 0.0f;
    
    // Loads current gain value into gainSmoother obj
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
}

// Reads current smoothed value and sets it to the gain variable
// so the gain is always moving towards the latest parameter setting
void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
}
