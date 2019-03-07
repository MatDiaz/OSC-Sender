/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "UserInterfaceClass.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Slider::Listener,
                        public Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged (Slider *slider) override;
    
    void buttonClicked (Button *button) override;
    
    //==============================================================================
    
    void receiveArray(float* newDataSet, int dataSetSize);
    

private:
    
    std::unique_ptr<Slider> mainSlider;
    std::unique_ptr<Button> mainButton;
    std::unique_ptr<TextEditor> addressEnter;
    UserInterfaceClass mainLoader;
    OwnedArray<float> dataSets;
    OSCSender sender;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
