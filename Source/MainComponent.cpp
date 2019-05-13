/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    setSize (900, 700);
    
    addMouseListener(this, true);
    
    addAndMakeVisible (mainLoader);

    mainLoader.addReference(*this);
    
    auto area = getBounds();
    
    mainLoader.setBounds (area.removeFromTop ((int)area.getHeight() * 0.05));

    //==============================================================================
    
    autoButton.reset(new TextButton());
    addAndMakeVisible(autoButton.get());
    autoButton->setBoundsRelative(0.5, 0.075, 0.1, 0.05);
    autoButton->setButtonText("Auto-Play");
    autoButton->setEnabled(false);
    autoButton->addListener(this);
    
    //==============================================================================
   

	hostLabel.reset(new Label());
	addAndMakeVisible(hostLabel.get());
	hostLabel->setText("Host: ", dontSendNotification);
	hostLabel->setColour(Label::textColourId, Colour(Colours::white));
	hostLabel->setBounds(getWidth() * 0.1, getHeight() * 0.15, getWidth() * 0.1, getHeight() * 0.05);
	hostLabel->setJustificationType(juce::Justification::centred);

	//==============================================================================

	hostEnter.reset(new TextEditor());
	addAndMakeVisible(hostEnter.get());
	hostEnter->setColour(TextEditor::backgroundColourId, Colour((uint8)255, (uint8)255, (uint8)250, (uint8)50));
	hostEnter->setBounds(getWidth() * 0.2, getHeight() * 0.15, getWidth() * 0.15, getHeight() * 0.05);
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
	portLabel->setBounds (getWidth() * 0.35, getHeight() * 0.15, getWidth() * 0.1, getHeight() * 0.05);
	portLabel->setJustificationType(juce::Justification::centred);

	//==============================================================================
    
    addressEnter.reset (new TextEditor());
    addAndMakeVisible (addressEnter.get());
	addressEnter->setColour (TextEditor::backgroundColourId, Colour((uint8) 255, (uint8) 255, (uint8) 250, (uint8) 50));
	addressEnter->setBounds (getWidth() * 0.45, getHeight() * 0.15, getWidth() * 0.15, getHeight() * 0.05);
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
	mainButton->setBounds (getWidth() * 0.65, getHeight() * 0.15, getWidth() * 0.1, getHeight() * 0.05 );
	mainButton->setEnabled(false);
	mainButton->setColour(TextButton::buttonColourId, Colour(0x81361b45));
	mainButton->addListener(this);

	//==============================================================================

	statusLabel.reset (new Label);
	addAndMakeVisible (statusLabel.get());
	statusLabel->setText ("Desconectado", dontSendNotification);
	statusLabel->setFont (Font(18.0f, 0));
	statusLabel->setColour (Label::textColourId, Colour(Colours::darkred));
	statusLabel->setBounds (getWidth() * 0.80, getHeight() * 0.15, getWidth() * 0.15, getHeight() * 0.05);
	statusLabel->setJustificationType(juce::Justification::centredLeft);

	//==============================================================================

	addAndMakeVisible (mainPlot);
	mainPlot.setBounds(0, getHeight() * 0.25, getWidth(), getHeight() * 0.75);
	mainPlot.setEnabled(false);

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
	mainButton->setEnabled(true);

	mainPlot.updatePlot (newDataSet, dataSetSize, true);
    
    float Min, Max;
    findMinAndMax(newDataSet, dataSetSize, Min, Max);
    normFactor = jmax(abs(Min), Max); 
}

void MainComponent::updateSliderPosistion (float newPosition)
{
    
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
    if ( e.originalComponent == &mainPlot && isConnected)
    {
        sender.send("/test", (mainPlot.dataValue / normFactor));
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
			}
		}
	}
    else if (button == autoButton.get())
    {
        startTimer(1000);
    }
}

// ==============================================================================
void MainComponent::paint(Graphics& g)
{
	g.fillAll(Colour(Colours::black));
}

void MainComponent::resized()
{
    
}
// ==============================================================================
void MainComponent::timerCallback()
{
    float autoStep = getWidth() / 10000;
    float position = cursorPosition / ((float)getWidth() * mainPlot.dataSetSize);
    mainPlot.interpolatePosition(position);
    cursorPosition += autoStep;
    if (cursorPosition >= getWidth())
    {
        stopTimer();
    }
}
