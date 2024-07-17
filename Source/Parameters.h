/*
  ==============================================================================

    Parameters.h
    Created: 17 Jul 2024 1:49:51pm
    Author:  Davis Weimer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const juce::ParameterID gainParamID { "gain", 1 };

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::AudioParameterFloat* gainParam;
};
