/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 14 Sep 2024 11:14:21am
    Author:  Davis Weimer

  ==============================================================================
*/

#include "LookAndFeel.h"

RotaryKnobLookAndFeel::RotaryKnobLookAndFeel()
{
    setColour(juce::Label::textColourId, Colors::Knob::label);
    setColour(juce::Slider::textBoxTextColourId, Colors::Knob::label);
}

void RotaryKnobLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                             int x, int y, int width, [[maybe_unused]] int height,
                                             float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                             juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();
    auto knobRect = bounds.reduced(10.0f, 10.0f);
    
    //==============================================================================
    // Knob Gradient + Drop Shadow
    //==============================================================================

    auto path = juce::Path();
    path.addEllipse(knobRect);
    dropShadow.drawForPath(g, path);
    
    g.setColour(Colors::Knob::outline);
    g.fillEllipse(knobRect);
    
    auto innerRect = knobRect.reduced(2.0f, 2.0f);
    auto gradient = juce::ColourGradient(Colors::Knob::gradientTop, 0.0f, innerRect.getY(),
                                         Colors::Knob::gradientBottom, 0.0f, innerRect.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillEllipse(innerRect);
    
    //==============================================================================
    // Knob track
    //==============================================================================
    
    auto center = bounds.getCentre();
    auto radius = bounds.getWidth() / 2.0f;
    auto lineWidth = 3.0f;
    auto arcRadius = radius - lineWidth/2.0f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(center.x,
                                center.y,
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                rotaryEndAngle,
                                true);
    
    auto strokeType = juce::PathStrokeType(lineWidth,
                                           juce::PathStrokeType::curved,
                                           juce::PathStrokeType::rounded);
    
    g.setColour(Colors::Knob::trackBackground);
    g.strokePath(backgroundArc, strokeType);
    
    //==============================================================================
    // Knob dial
    //==============================================================================
    
    auto dialRadius = innerRect.getHeight() / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    juce::Point<float> dialStart(center.x + 10.f * std::sin(toAngle),
                                 center.y - 10.f * std::cos(toAngle));
    juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle),
                               center.y - dialRadius * std::cos(toAngle));
    
    juce::Path dialPath;
    dialPath.startNewSubPath(dialStart);
    dialPath.lineTo(dialEnd);
    g.setColour(Colors::Knob::dial);
    g.strokePath(dialPath, strokeType);
}
