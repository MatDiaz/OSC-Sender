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
    mainSlider->setBounds (area.removeFromTop((int) area.getHeight() *  0.1));
    mainSlider->addListener (this);
    
    //==============================================================================
    
    area.removeFromBottom((int) area.getHeight() *  0.95);
	area.removeFromLeft((int)area.getWidth() * 0.15);

	portLabel.reset (new Label());
	addAndMakeVisible (portLabel.get());
	portLabel->setText ("Puerto: ", dontSendNotification);
	portLabel->setColour (Label::textColourId, Colour(Colours::white));
	portLabel->setBounds (area.removeFromLeft(area.getWidth() * 0.1));
    
    addressEnter.reset (new TextEditor());
    addAndMakeVisible (addressEnter.get());
    addressEnter->setBounds (area.removeFromLeft(area.getWidth() * 0.2));
	addressEnter->setColour (TextEditor::backgroundColourId, Colour((uint8) 255, (uint8) 255, (uint8) 250, (uint8) 50));
    
	//==============================================================================
	area.removeFromLeft((int)area.getWidth() * 0.125);

	mainButton.reset (new TextButton());
	addAndMakeVisible (mainButton.get());
	mainButton->setButtonText ("Conectar");
	mainButton->setColour(TextButton::buttonColourId, Colour((uint8)255, (uint8)255, (uint8)250, (uint8)50));
	mainButton->setBounds(area.removeFromLeft(area.getWidth() * 0.25));
	mainButton->addListener(this);

	//==============================================================================

	area.removeFromLeft ((int)area.getWidth() * 0.125);
	statusLabel.reset (new Label);
	addAndMakeVisible (statusLabel.get());
	statusLabel->setText ("Desconectado", dontSendNotification);
	statusLabel->setFont (Font(18.0f, 0));
	statusLabel->setColour (Label::textColourId, Colour(Colours::darkred));
	statusLabel->setBounds (area.removeFromLeft(area.getWidth() * 0.3));


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
	mainSlider->setRange(0, (dataSetSize - 1), 1);
    repaint();
}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{
	if (slider == mainSlider.get())
	{
		if (isConnected)
		{	
			int dataValue = (int) mainSlider->getValue();
			sender.send("/juce/valeverga", dataValue);
		}
	}
}

void MainComponent::buttonClicked(Button *button)
{
	if (button == mainButton.get())
	{
		if (!isConnected)
		{
			if (sender.connect("127.0.0.1", addressEnter->getText().getFloatValue()))
			{
				statusLabel->setColour(Label::textColourId, Colour(Colours::darkgreen));
				statusLabel->setText("Conectado", dontSendNotification);
			}

			isConnected = true;
		
		}
	}
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll(Colour(Colours::black));
}

void MainComponent::resized()
{
    
}
