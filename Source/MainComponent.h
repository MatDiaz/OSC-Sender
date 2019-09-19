/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "UserInterfaceClass.h"
#include "InitialWindow.h"
#include "Plot.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Button::Listener,
                        public Timer
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
    
    void buttonClicked (Button *button) override;
    
    void mouseDrag (const MouseEvent& e) override;
    
    void updateSliderPosistion (float newPosition);
    
    void timerCallback() override;
    
    //==============================================================================
    
    void receiveArray (Array<float>& inArray, StringArray inStringArray, int dataSetSize);
    
    float interpolateData(float inValue);

private:
    
    std::unique_ptr<TextButton> mainButton;
    std::unique_ptr<TextButton> autoButton;
	std::unique_ptr<Slider> playbackSpeedSlider;
    
    std::unique_ptr<TextEditor> messageEnter;
    std::unique_ptr<TextEditor> addressEnter;
	std::unique_ptr<TextEditor> hostEnter;
    
	std::unique_ptr<Label> portLabel;
	std::unique_ptr<Label> hostLabel;
	std::unique_ptr<Label> statusLabel;
    std::unique_ptr<Label> messageLabel;
    std::unique_ptr<Label> fileName;
	std::unique_ptr<Label> speedLabel, speedLabel_Two, speedLabel_Three;

    UserInterfaceClass mainLoader;
    OwnedArray<float> dataSets;
    OSCSender sender;
	Plot mainPlot, secondPlot, thirdPlot;
    float normFactor = 1;
	bool isConnected = false;
	bool isLoaded = false;
    bool isAuto = false;
    float cursorPosition = 0;
	int numCycles = 0;
	int dataSetTam = 1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
