/*
  ==============================================================================

    RotaryKnob.h
    Created: 23 Jul 2024 10:30:26am
    Author:  Davis Weimer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String& text,
               juce::AudioProcessorValueTreeState& apvts,
               const juce::ParameterID& parameterID,
               );
    ~RotaryKnob() override;

    void resized() override;
    
    juce::Slider slider;
    juce::Label label;
    
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

private:
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
