/*
  ==============================================================================

    Parameters.cpp
    Created: 17 Jul 2024 1:49:51pm
    Author:  Davis Weimer

  ==============================================================================
*/

#include "Parameters.h"

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    auto* param = apvts.getParameter(gainParamID.getParamID());
    gainParam = dynamic_cast<juce::AudioParameterFloat*>(param);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(gainParamID,
                                                           // Name of the parameter
                                                           "Output Gain",
                                                           // Value range
                                                           juce::NormalisableRange<float> { -12.0f, 12.0f },
                                                           // Default value for range
                                                           0.0f));
    return layout;
}

