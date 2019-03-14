/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "UserInterfaceClass.h"
#include "Plot.h"

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
    std::unique_ptr<TextButton> mainButton;
    std::unique_ptr<TextEditor> addressEnter;
	std::unique_ptr<TextEditor> hostEnter;
	std::unique_ptr<Label> portLabel;
	std::unique_ptr<Label> hostLabel;
	std::unique_ptr<Label> statusLabel;

    UserInterfaceClass mainLoader;
    OwnedArray<float> dataSets;
    OSCSender sender;
	Plot mainPlot;
	bool isConnected = false;
	bool isLoaded = false;

	int dataSetTam = 1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
