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

static float millisecondsFromString(const juce::String& text)
{
    float value = text.getFloatValue();
    if (!text.endsWithIgnoreCase("ms")) {
        if (text.endsWithIgnoreCase("s") || value < Parameters::minDelayTime) {
            return value * 1000.0f;
        }
    }
    return value;
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " db";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + " %";
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, mixParamID, mixParam);
    castParameter(apvts, feedbackParamID, feedbackParam);
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
                                                           juce::AudioParameterFloatAttributes()
                                                                .withStringFromValueFunction(stringFromMilliseconds)
                                                                .withValueFromStringFunction(millisecondsFromString)
                                                           ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(mixParamID,
                                                           "Mix",
                                                           juce::NormalisableRange<float>{ 0.0f, 100.0f, 1.0f },
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
                                                           ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(feedbackParamID,
                                                           "Feedback",
                                                           juce::NormalisableRange<float>{ -100.0f, 100.f, 1.0f },
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                .withStringFromValueFunction(stringFromPercent)
                                                           ));
    return layout;
}

// Sets up initial conditions and prepares the parameters for playback.
void Parameters::prepareToPlay(double sampleRate) noexcept
{
    // 0.02s adds 20 millisecond fade
    // this is acceptable at 48kHz
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    
    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
    
    mixSmoother.reset(sampleRate, duration);
    feedbackSmoother.reset(sampleRate, duration);
}

// Adds re-initializing param to be safe upon host stop/start
void Parameters::reset() noexcept
{
    gain = 0.0f;
    delayTime = 0.0f;
    
    // Loads current gain value into gainSmoother obj
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    mix = 1.0f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
    
    feedback = 0.0f;
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
}

// Updates the parameter values based on the current settings.
void Parameters::update() noexcept
{
    // Reads current gain value, converts decibels to linear units, uses new value
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    targetDelayTime = delayTimeParam->get();
    if (delayTime == 0.0f) {
        delayTime = targetDelayTime;
    }
    
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
}

// Reads current smoothed value and sets it to variable
// so the variable is always moving towards the latest parameter setting
void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    
    delayTime += ( targetDelayTime - delayTime ) * coeff;
    
    mix = mixSmoother.getNextValue();
    feedback = feedbackSmoother.getNextValue();
}
