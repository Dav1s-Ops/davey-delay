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

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
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
                                                           0.0f));
    return layout;
}

// Reads Output Gain parameter
void Parameters::update() noexcept
{
    gain = juce::Decibels::decibelsToGain(gainParam->get());
}
