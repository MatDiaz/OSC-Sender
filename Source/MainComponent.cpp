/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    setSize (800, 600);
    
    addAndMakeVisible (mainLoader);
    
    mainLoader.addReference(*this);
    
    auto area = getBounds();
    
    mainLoader.setBounds (area.removeFromTop ((int)area.getHeight() * 0.2));

    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    
    g.fillAll (Colour(Colours::black));

}

void MainComponent::resized()
{
    
}
