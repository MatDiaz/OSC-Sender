/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
using namespace std;

//==============================================================================
MainComponent::MainComponent()
{
    addMouseListener(this, true);

    //==============================================================================
    
    autoButton.reset(new TextButton());
    addAndMakeVisible(autoButton.get());
    autoButton->setButtonText("Auto-Play");
    autoButton->setColour(TextButton::buttonOnColourId, Colour(Colours::grey));
    autoButton->setEnabled(false);
    autoButton->addListener(this);
    autoButton->setClickingTogglesState(true);
    
    //==============================================================================
    
    messageEnter.reset(new TextEditor());
    addAndMakeVisible(messageEnter.get());
    messageEnter->setText("/juce/message");
    messageEnter->setReadOnly(true);
    messageEnter->setEnabled(false);
    messageEnter->setColour(TextEditor::backgroundColourId, Colour(0x81361b45));
    messageEnter->setJustification(juce::Justification::centred);
    
    //==============================================================================
    
    messageLabel.reset(new Label());
    addAndMakeVisible(messageLabel.get());
    messageLabel->setText("Mensaje OSC: ", dontSendNotification);
    messageLabel->setColour(Label::textColourId, Colour(Colours::white));
    messageLabel->setJustificationType(juce::Justification::centred);
    
    //==============================================================================
    
    fileName.reset (new Label());
    addAndMakeVisible (fileName.get());
    fileName->setText ("Nombre: ", dontSendNotification);
    fileName->setColour (Label::textColourId, Colour(Colours::white));
    fileName->setJustificationType(juce::Justification::centred);
    
    //==============================================================================
   
	hostLabel.reset(new Label());
	addAndMakeVisible(hostLabel.get());
	hostLabel->setText("Host: ", dontSendNotification);
	hostLabel->setColour(Label::textColourId, Colour(Colours::white));
	hostLabel->setJustificationType(juce::Justification::centred);

	//==============================================================================

	hostEnter.reset(new TextEditor());
	addAndMakeVisible(hostEnter.get());
	hostEnter->setColour(TextEditor::backgroundColourId, Colour((uint8)255, (uint8)255, (uint8)250, (uint8)50));
	hostEnter->setText("127.0.0.1", false);
    hostEnter->setReadOnly(true);
	hostEnter->setColour(TextEditor::backgroundColourId, Colour(0x81361b45));
	hostEnter->setEnabled(false);
	hostEnter->setJustification(juce::Justification::centred);
	
	//==============================================================================

	portLabel.reset (new Label());
	addAndMakeVisible (portLabel.get());
	portLabel->setText ("Puerto: ", dontSendNotification);
	portLabel->setColour (Label::textColourId, Colour(Colours::white));
	portLabel->setJustificationType(juce::Justification::centred);

	//==============================================================================
    
    addressEnter.reset (new TextEditor());
    addAndMakeVisible (addressEnter.get());
	addressEnter->setColour (TextEditor::backgroundColourId, Colour((uint8) 255, (uint8) 255, (uint8) 250, (uint8) 50));
	addressEnter->setText("9001", false);
	addressEnter->setEnabled(false);
    addressEnter->setReadOnly(true);
	addressEnter->setColour(TextEditor::backgroundColourId, Colour(0x81361b45));
	addressEnter->setJustification(juce::Justification::centred);

	//==============================================================================

	mainButton.reset (new TextButton());
	addAndMakeVisible (mainButton.get());
	mainButton->setButtonText ("Conectar");
	mainButton->setColour (TextButton::buttonColourId, Colour((uint8)255, (uint8)255, (uint8)250, (uint8)50));
	mainButton->setEnabled(false);
	mainButton->setColour(TextButton::buttonColourId, Colour(0x81361b45));
	mainButton->addListener(this);

	//==============================================================================

	statusLabel.reset (new Label);
	addAndMakeVisible (statusLabel.get());
	statusLabel->setText ("Desconectado", dontSendNotification);
	statusLabel->setFont (Font(18.0f, 0));
	statusLabel->setColour (Label::textColourId, Colour(Colours::darkred));
	statusLabel->setJustificationType(juce::Justification::centredLeft);

	//==============================================================================

	addAndMakeVisible (mainPlot);
	mainPlot.setEnabled(false);
    
    //==============================================================================
    
    addAndMakeVisible (mainLoader);
    mainLoader.addReference(*this);

    setSize (900, 700);
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
	dataSetTam = dataSetSize;

	hostEnter->setEnabled(true);
    hostEnter->setReadOnly(false);
	
	addressEnter->setEnabled(true);
    addressEnter->setReadOnly(false);
	
	messageEnter->setEnabled(true);
	messageEnter->setReadOnly(false);
	
	mainButton->setEnabled(true);

	mainPlot.updatePlot (newDataSet, dataSetSize, true);
    isLoaded = true;
    
    float Min, Max;
    findMinAndMax(newDataSet, dataSetSize, Min, Max);
    normFactor = jmax(abs(Min), Max); 
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
    if ( e.originalComponent == &mainPlot && isConnected)
    {
		float normValue = 0;
		
		if (mainPlot.dataValue >= 0 && mainPlot.dataValue <= dataSetTam)
			normValue = (dataSets[0][(int)mainPlot.dataValue] / normFactor);
		
        sender.send(messageEnter->getTextValue().toString(), normValue);
    }
}

void MainComponent::buttonClicked(Button *button)
{
	if (button == mainButton.get())
	{
		if (!isConnected)
		{
			if (sender.connect(hostEnter->getText(), addressEnter->getText().getFloatValue()))
			{	
				isConnected = true;
				mainPlot.isConnected = true;
                autoButton->setEnabled(true);
				statusLabel->setColour(Label::textColourId, Colour(Colours::darkgreen));
				statusLabel->setText("Conectado", dontSendNotification);
                mainButton->setButtonText("Desconectar");
			}
		}
		else
		{
			if (sender.disconnect())
			{
				isConnected = false;
				mainPlot.isConnected = false;
                autoButton->setEnabled(false);
				statusLabel->setColour(Label::textColourId, Colour(Colours::darkred));
				statusLabel->setText("Desconectado", dontSendNotification);
                mainButton->setButtonText("Conectar");
			}
		}
	}
    else if (button == autoButton.get())
    {
        if (autoButton->getToggleState())
        {
            startTimer(30);
        }
        else
        {
            stopTimer();
        }
    }
}

// ==============================================================================
void MainComponent::paint(Graphics& g)
{
	g.fillAll(Colour(Colours::black));
}

void MainComponent::resized()
{
    hostLabel->setBoundsRelative(0.05, 0.05, 0.1, 0.05);
    hostEnter->setBoundsRelative(0.15, 0.05, 0.15, 0.05);
    
    messageLabel->setBoundsRelative(0.35, 0.05, 0.1, 0.05);
    messageEnter->setBoundsRelative(0.45, 0.05, 0.15, 0.05);
    
    portLabel->setBoundsRelative(0.65, 0.05, 0.1, 0.05);
    addressEnter->setBoundsRelative(0.75, 0.05, 0.15, 0.05);

    autoButton->setBoundsRelative(0.05, 0.125, 0.1, 0.05);

    mainButton->setBoundsRelative(0.65, 0.125, 0.1, 0.05);
    statusLabel->setBoundsRelative(0.8, 0.125, 0.15, 0.05);
    
    fileName->setBoundsRelative(0.05, 0.2, 0.1, 0.05);
    
    mainPlot.setBoundsRelative(0, 0.25, 1, 0.75);
    mainLoader.setBoundsRelative(0, 0, 1.0f, 0.05);
    if(isLoaded)
    {
        mainPlot.updatePlot();
    }
}
// ==============================================================================
void MainComponent::timerCallback()
{
	mainPlot.interpolatePosition(cursorPosition);
	cursorPosition += 0.05;
	
	cursorPosition = cursorPosition > dataSetTam ? 0 : cursorPosition;
	
	float normValue = 0;
	
	normValue = (dataSets[0][(int)cursorPosition] / normFactor);
	
	sender.send(messageEnter->getTextValue().toString(), normValue);
}
