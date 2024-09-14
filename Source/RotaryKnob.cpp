/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 23 Jul 2024 10:30:26am
    Author:  Davis Weimer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text,
                       juce::AudioProcessorValueTreeState& apvts,
                       const juce::ParameterID& parameterID)
    : attachment(apvts, parameterID.getParamID(), slider)
{
    float pi = juce::MathConstants<float>::pi;
    
    slider.setRotaryParameters(1.25f * pi, 2.75f * pi, true);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    slider.setBounds(0, 0, 70, 86);
    addAndMakeVisible(slider);
    
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{ 0, 0, 2, 0 });
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
    
    setLookAndFeel(RotaryKnobLookAndFeel::get());
    
    setSize(70, 110);

}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::resized()
{
    slider.setTopLeftPosition(0, 24);
}
