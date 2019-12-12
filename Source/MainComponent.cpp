/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
using namespace std;

//==============================================================================
// Clase con el componente principal, esta clase se encargara de llamar en orden a los demas
// componentes
MainComponent::MainComponent()
{
	tptr = Typeface::createSystemTypefaceFor(BinaryData::SharpGroteskLight15_otf, BinaryData::SharpGroteskLight15_otfSize);

	LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(tptr);

	// =============================================================================
	juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	auto x = r.getWidth();
	auto y = r.getHeight();
	setSize(x, y);
	//==============================================================================

	readLocationFileData(BinaryData::_2019_grp2_txt, BinaryData::_2019_grp2_txtSize, false);
	readLocationFileData(BinaryData::_2019_grp2_datos_txt, BinaryData::_2019_grp2_datos_txtSize, true);

	setAudioChannels(2, 2);

#if JUCE_LINUX
	deviceManager.setCurrentAudioDeviceType("JACK", true);
#endif

	executeSequence(firstWindow);
	MemoryInputStream inputStream(BinaryData::sonidosvida_wav, BinaryData::sonidosvida_wavSize, true);
	audioFormatManager.registerBasicFormats();
	AudioFormatReader* audioReader =  audioFormatManager.createReaderFor(&inputStream);
	audioToRead.setSize(1, audioReader->lengthInSamples);
	audioReader->read(&audioToRead, 0, audioReader->lengthInSamples, 0, true, false);
	DBG(audioToRead.getNumChannels());
	DBG(audioToRead.getNumSamples());
	isReading = false;
	sampleIdx = 0;
    sender.connect("127.0.0.1", 9001);
}

MainComponent::~MainComponent()
{
    initialComponent = nullptr;
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	if (isReading)
	{
		for (auto i = 0; i < bufferToFill.buffer->getNumSamples(); ++i)
		{
			for (auto j = 0; j < bufferToFill.buffer->getNumChannels(); ++j)
			{
				float* audioSample = bufferToFill.buffer->getWritePointer(j);
				const float* audioValue = audioToRead.getReadPointer(0);

				audioSample[i] = audioValue[sampleIdx];
			}
			++sampleIdx;
			if (sampleIdx >= (audioToRead.getNumSamples()-1))
			{
				sampleIdx = 0;
				isReading = false;
				break;
			}
		}
	}
	else
	{
		bufferToFill.clearActiveBufferRegion();
	}
}

void MainComponent::releaseResources()
{
    
}

void MainComponent::readLocationFileData(const char *textFileData, int textFileSize, bool isData)
{
    String inputFile = String::fromUTF8(textFileData, textFileSize);
    std::stringstream stringStream (inputFile.toStdString());
    std::string token;
    
    while (std::getline (stringStream, token, '\n'))
    {
        std::string idData;
        std::istringstream inLine(token);
        float inValue;
        if (!isData)
        {
            inLine >> idData;
            idArray.add(idData);
        }
        else
        {
            inLine >> inValue;
            locationArray.add(inValue);
        }
    }
}

int MainComponent::findData(const String idToSearch)
{
    int deathAmount = 0;
    for (auto i = 0; i < idArray.size(); ++i)
    {
        if (idArray[i] == idToSearch)
        {
            deathAmount = locationArray[i];
            break;
        }
    }
    return deathAmount;
}

// ==============================================================================
void MainComponent::paint(Graphics& g)
{
	g.fillAll(Colour(Colours::black));
}

void MainComponent::resized()
{
	
}

void MainComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == initialComponent.get())
	{
		currentGender = initialComponent->selectedSex;
		currentId = initialComponent->selectedId;
		deathNum = findData(currentId);
		locationId = initialComponent->location;

		File parentDir;

		String newDirectory = File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory).getFullPathName();

	#if	JUCE_WINDOWS
		newDirectory += "\\ManifiestoDatos";
	#else
		newDirectory += "//ManifiestoDatos";
	#endif

		parentDir = File(newDirectory);
		parentDir.createDirectory();

		if (!parentDir.getChildFile("datos.txt").existsAsFile())
			parentDir.getChildFile("datos.txt").create();

		parentDir.getChildFile("datos.txt").appendText(currentId + " Time:" + Time::getCurrentTime().toString(true, true, true) + "\n");
	
		initialComponent.reset();
		executeSequence(secondWindow);
	}
	else if (source == mainWindowComponent.get())
	{
		mainWindowComponent.reset();
		executeSequence(thirdWindow);
	}
	else if (source == secondComponent.get())
	{
		if (secondComponent->componentState == SecondComponent::thirdState)
		{
			sender.send("/toggle3", 0.0f);
			sender.send("/toggle4", 0.0f);
			isReading = true;
		}
		else if (secondComponent->changeState)
		{
			sender.send("/toggle", 0.0f);
			sender.send("/toggle2", 0.0f);
			sender.send("/toggle3", 0.0f);
			sender.send("/toggle4", 0.0f);
		}
		else
		{
			secondComponent.reset();
			sender.send("/toggle", 0.0f);
			sender.send("/toggle2", 0.0f);
			sender.send("/toggle3", 0.0f);
			sender.send("/toggle4", 0.0f);
			executeSequence(firstWindow);
		}
	}
}

void MainComponent::executeSequence (sequenceOrder actualPosition)
{
	switch (actualPosition)
	{
	case firstWindow:
		initialComponent.reset(new InsideComponent());
		addAndMakeVisible(initialComponent.get());
		initialComponent->addChangeListener(this);
		initialComponent->setBounds(getBounds());
		break;
	case secondWindow:
		mainWindowComponent.reset(new MainWindowComponent(&sender));
		addAndMakeVisible(mainWindowComponent.get());
		mainWindowComponent->addChangeListener(this);
		mainWindowComponent->setBounds(getBounds());
		break;
	case thirdWindow:
		secondComponent.reset(new SecondComponent(currentGender, deathNum, locationId));
		addAndMakeVisible(secondComponent.get());
		secondComponent->addChangeListener(this);
		secondComponent->setBounds(getBounds());
		break;
	default:
		break;
	}
}

// ==============================================================================
