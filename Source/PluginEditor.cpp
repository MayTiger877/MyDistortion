/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define TRANSPARENT 0.5f
#define BOLD 1.0f
//==============================================================================

MyDistortionAudioProcessorEditor::MyDistortionAudioProcessorEditor (MyDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    myDistMixWetAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "myDistMixWetID", myDistMixWet);
    myDistMixDryAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "myDistMixDryID", myDistMixDry);
    myDistDriveAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "myDistDriveID", myDistDrive);
    myDistInAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "myDistInID", myDistIn);
    myDistOutAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "myDistOutID", myDistOut);
    myDistMysteryMagicAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "myDistMysteryMagicID", myDistMysteryMagic);

    setSize (620, 120);
    winSizeRatio = 3.0 / 1.0;
    setResizable(true, true);
    //setResizeLimits(600, juce::roundToInt(600.0 / winSizeRatio), 1200, juce::roundToInt(1200.0 / winSizeRatio));
    //getConstrainer()->setFixedAspectRatio(winSizeRatio);
    //setResizeLimits(600, 400, 1200, 800);
    initiateComponents(p);
}

MyDistortionAudioProcessorEditor::~MyDistortionAudioProcessorEditor()
{
}

void MyDistortionAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.setColour(Colours::maroon);
}

void MyDistortionAudioProcessorEditor::initiateComponents(AudioProcessor& p)
{
    addAndMakeVisible(myDistIn);
    myDistIn.setRange(-60.0f, 6.0f, 0.01f);
    myDistIn.setSkewFactorFromMidPoint(0);
    myDistIn.setValue(0.0f);
    myDistIn.setTextValueSuffix("db");
    myDistIn.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    myDistIn.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(myDistInLabel);
    myDistInLabel.setText("input", juce::dontSendNotification);

    addAndMakeVisible(myDistOut);
    myDistOut.setRange(-60.0f, 6.0f, 1.0f);
    myDistOut.setSkewFactorFromMidPoint(0);
    myDistOut.setValue(0.0f);
    myDistOut.setTextValueSuffix("db");
    myDistOut.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    myDistOut.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(myDistOutLabel);
    myDistOutLabel.setText("output", juce::dontSendNotification);

    addAndMakeVisible(myDistMixWet);
    myDistMixWet.setRange(0.0f, 1.0f, 0.05f);
    myDistMixWet.setValue(0.3f);
    myDistMixWet.setTextValueSuffix("%");
    myDistMixWet.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    myDistMixWet.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(myDistMixWetLabel);
    myDistMixWetLabel.setText("wet", juce::dontSendNotification);

    addAndMakeVisible(myDistMixDry);
    myDistMixDry.setRange(0.0f, 1.0f, 0.05f);
    myDistMixDry.setValue(0.3f);
    myDistMixDry.setTextValueSuffix("%");
    myDistMixDry.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    myDistMixDry.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(myDistMixDryLabel);
    myDistMixDryLabel.setText("dry", juce::dontSendNotification);

    addAndMakeVisible(myDistDrive);
    myDistDrive.setRange(0.0f, 125.0f, 1.0f);
    myDistDrive.setValue(50.0f);
    myDistDrive.setTextValueSuffix("C");
    myDistDrive.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    myDistDrive.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(myDistDriveLabel);
    myDistDriveLabel.setText("drive", juce::dontSendNotification);

    addAndMakeVisible(myDistMysteryMagic);
    myDistMysteryMagic.setRange(0.0f, 1.0f, 0.05f);
    myDistMysteryMagic.setValue(0.3f);
    myDistMysteryMagic.setTextValueSuffix("%");
    myDistMysteryMagic.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    myDistMysteryMagic.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(myDistMysteryMagicLabel);
    myDistMysteryMagicLabel.setText("magic", juce::dontSendNotification);
}

void MyDistortionAudioProcessorEditor::printComponents()
{

    myDistMixWet.setBounds(20, 5, 80, 80);
    myDistMixWetLabel.setBounds(30, 70, 80, 50);

    myDistMixDry.setBounds(120, 5, 80, 80);
    myDistMixDryLabel.setBounds(130, 70, 70, 50);

    myDistDrive.setBounds(220, 5, 80, 80);
    myDistDriveLabel.setBounds(230, 70, 80, 50);

    myDistIn.setBounds(320, 5, 80, 80);
    myDistInLabel.setBounds(330, 70, 70, 50);

    myDistOut.setBounds(420, 5, 80, 80);
    myDistOutLabel.setBounds(430, 70, 80, 50);

    myDistMysteryMagic.setBounds(520, 5, 80, 80);
    myDistMysteryMagicLabel.setBounds(530, 70, 70, 50);

}

void MyDistortionAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().toFloat().removeFromBottom(getHeight() * 0.9f).reduced(getWidth() * 0.06f, getHeight() * 0.25f);
    printComponents();
}
