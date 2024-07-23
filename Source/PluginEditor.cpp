/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(delayTimeKnob);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 330);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void DelayAudioProcessorEditor::resized()
{
    delayTimeKnob.setTopLeftPosition(20, 10);
    mixKnob.setTopLeftPosition(delayTimeKnob.getRight() + 20, 10);
    gainKnob.setTopLeftPosition(mixKnob.getRight() + 20, 10);
}
