/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyDistortionAudioProcessor::MyDistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        tree_state(*this, nullptr, "PARAMETER", create_parameter_layout())
#endif
{
    myDistMixWet = 0;
    myDistMixDry = 0;
    myDistDrive = 0;
    myDistIn = 0;
    myDistOut = 0;
    myDistMysteryMagic = 0;
}

MyDistortionAudioProcessor::~MyDistortionAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout MyDistortionAudioProcessor::create_parameter_layout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameters;

    parameters.add((std::make_unique<AudioParameterFloat>("myDistMixWetID",         "myDistMixWetName",         NormalisableRange<float>(0.0, 1.0, 0.05), 0.3)));
    parameters.add((std::make_unique<AudioParameterFloat>("myDistMixDryID",         "myDistMixDryName",         NormalisableRange<float>(0.0, 1.0, 0.05), 0.3)));
    parameters.add((std::make_unique<AudioParameterFloat>("myDistDriveID",          "myDistDriveName",          NormalisableRange<float>(0.0, 125.0, 1.0), 50.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("myDistInID",             "myDistInName",             NormalisableRange<float>(-60.0, 6.0, 1.0), 0.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("myDistOutID",            "myDistOutName",            NormalisableRange<float>(-60.0, 6.0, 1.0), 0.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("myDistMysteryMagicID",   "myDistMysteryMagicName",   NormalisableRange<float>(0.0, 1.0, 0.05), 0.3)));

    return parameters;
}


const juce::String MyDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MyDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MyDistortionAudioProcessor::updateParameters()
{
    myDistMixWet = tree_state.getRawParameterValue("myDistMixWetID")->load();
    myDistMixDry = tree_state.getRawParameterValue("myDistMixDryID")->load();
    myDistDrive  = tree_state.getRawParameterValue("myDistDriveID")->load();
    myDistIn = tree_state.getRawParameterValue("myDistInID")->load();
    myDistOut = tree_state.getRawParameterValue("myDistOutID")->load();
    myDistMysteryMagic = tree_state.getRawParameterValue("myDistMysteryMagicID")->load();
}


void MyDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    updateParameters();

    int numSampels = buffer.getNumSamples();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSampels);

    float drySample = 0, exponant = 0, toDistort = 0, sign = 0, magic = 0;
    
    const float drive = jmax(0.f, std::min(1.f, myDistDrive));
    const float range = jmax(1.f, myDistMysteryMagic);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < numSampels; sample++)
        {
            toDistort = channelData[sample];
            if (toDistort != 0)
            {
                //drySample = toDistort * myDistIn;
                //sign = toDistort / abs(toDistort);
                //exponant = expf(toDistort * drive);
                //toDistort = (1 - exponant) * sign;
                //channelData[sample] = ((drySample * myDistMixDry) + (toDistort * myDistMixWet)) * myDistOut;
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //drySample = toDistort * myDistIn;
                //toDistort = toDistort * (1 + drive);
                //if (toDistort >= 0.5)
                //    channelData[sample] = 0.5;
                //if (toDistort <= -0.5)
                //    channelData[sample] = -0.5;
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                float clippedInput = tanh(channelData[sample] * drive * range) / tanh(drive * range);
                channelData[sample] = (myDistMixDry * channelData[sample]) + (myDistMixWet * clippedInput);
            }
        }
        
    }
}



//In this example, we have added three parameters to our distortion effect : drive, mix, and range.
//drive controls the amount of distortion applied to the signal, with a range of 0 to 1. mix controls the mix between the 
//original signal and the distorted signal, with a range of 0 to 1. range controls the range of the soft clipping function, with a range of 1 or greater.
//We then apply a soft clipping function to the input signal using the std::tanh function, which is a common way to implement distortion
//effects.We use the driveand range parameters to control the shapeand intensity of the clipping.
//Finally, we mix the clipped signal with the original signal using the mix parameter, and write the resulting output to the output buffer.
//Note that in order to use this example in your own JUCE project, you will need to defineand initialize the driveParam, mixParam, and rangeParam variables, 
//which should be instances of juce::AudioParameterFloat.




//==============================================================================
bool MyDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyDistortionAudioProcessor::createEditor()
{
    return new MyDistortionAudioProcessorEditor(*this);
}

//==============================================================================
void MyDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MyDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyDistortionAudioProcessor();
}
