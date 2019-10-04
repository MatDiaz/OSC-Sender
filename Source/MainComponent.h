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
                        private ChangeListener,
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
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    //==============================================================================
    
    void receiveArray (Array<float>& inArray, StringArray inStringArray, int dataSetSize);
    
    float interpolateData (float inValue);
    
    void readTextFileData (const char *textFileData, int textFileSize, Plot& plotToAdd);
    
    void executeSequence();

private:
    
	std::unique_ptr<Slider> playbackSpeedSlider;

	std::unique_ptr<Label> speedLabel, speedLabel_Two, speedLabel_Three;
    
    juce::Component::SafePointer<InitialWindow> initialWindow;

    OwnedArray<float> dataSets;
    
    Array<float> nArray;
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
