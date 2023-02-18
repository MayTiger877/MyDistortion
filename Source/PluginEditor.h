/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;
using namespace std;

class MyDistortionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:

    MyDistortionAudioProcessorEditor (MyDistortionAudioProcessor&);
    ~MyDistortionAudioProcessorEditor() override;

    void initiateComponents(AudioProcessor&);
    void printComponents();
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    double winSizeRatio;

    // Wet mix
    Slider myDistMixWet;
    Label myDistMixWetLabel;

    // Dry mix
    Slider myDistMixDry;
    Label myDistMixDryLabel;

    // Drive
    Slider myDistDrive;
    Label myDistDriveLabel;

    // Input
    Slider myDistIn;
    Label myDistInLabel;

    // Output
    Slider myDistOut;
    Label myDistOutLabel;

    //Mystery magic
    Slider myDistMysteryMagic;
    Label myDistMysteryMagicLabel;

public:

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myDistMixWetAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myDistMixDryAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myDistDriveAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myDistInAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myDistOutAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myDistMysteryMagicAttach;

    MyDistortionAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyDistortionAudioProcessorEditor)
};