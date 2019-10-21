/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Plot.h"
#include "ProjectColours.h"
#include "InsideComponent.h"
#include "MainWindowComponent.h"
#include "SecondComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent,
					  private ChangeListener
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent();

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================
	void paint(Graphics& g) override;
	void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	//==============================================================================

	void readLocationFileData(const char* textFileData, int textFileSize, bool isData);

	int findData(String idToSearch);

	enum sequenceOrder { firstWindow, secondWindow, thirdWindow } order;

	void executeSequence(sequenceOrder actualPosition);

private:

	Typeface::Ptr tptr;
    std::unique_ptr<InsideComponent> initialComponent;
    std::unique_ptr<SecondComponent> secondComponent;
	std::unique_ptr<MainWindowComponent> mainWindowComponent;
    
    Array<float> locationArray;
    StringArray idArray;
    String currentGender, currentId;
    int deathNum, locationId;
    OSCSender sender;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
