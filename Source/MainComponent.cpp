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
    
    mainLoader.setBounds (area.removeFromTop ((int)area.getHeight() * 0.05));
    
    //==============================================================================
    
    mainSlider.reset (new Slider());
    addAndMakeVisible (mainSlider.get());
    mainSlider->setRange (0, 1, 0);
    mainSlider->setSliderSnapsToMousePosition(true);
    mainSlider->setBounds (area.removeFromTop((int) area.getHeight() *  0.2));
    mainSlider->addListener (this);
    
    //==============================================================================
    
    area.removeFromTop((int) area.getHeight() *  0.01);
    
    addressEnter.reset(new TextEditor());
    addAndMakeVisible(addressEnter.get());
    addressEnter->setBounds (area.removeFromLeft(area.getWidth() * 0.1));
    
    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    dataSets.clearQuick(false);
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

void MainComponent::receiveArray(float *newDataSet, int dataSetSize)
{
    dataSets.add (newDataSet);
    repaint();
}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{
    
}

void MainComponent::buttonClicked(Button *button)
{
    
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll(Colour(Colours::black));
}

void MainComponent::resized()
{
    
}
