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
	mainSlider->setColour(Slider::thumbColourId, Colour(Colours::dimgrey));
	mainSlider->setColour(Slider::backgroundColourId, Colour(Colours::white));
	mainSlider->setColour(Slider::trackColourId, Colour(Colours::darkgrey));
    mainSlider->addListener (this);
    
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
	addressEnter->setJustification(juce::Justification::centred);

	//==============================================================================

	mainButton.reset (new TextButton());
	addAndMakeVisible (mainButton.get());
	mainButton->setButtonText ("Conectar");
	mainButton->setColour (TextButton::buttonColourId, Colour((uint8)255, (uint8)255, (uint8)250, (uint8)50));
	mainButton->setBounds (getWidth() * 0.65, getHeight() * 0.15, getWidth() * 0.1, getHeight() * 0.05 );
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
	dataSetTam = dataSetSize;
	  
	mainPlot.updatePlot (newDataSet, dataSetSize, true);
}


void MainComponent::sliderValueChanged(juce::Slider *slider)
{
	if (slider == mainSlider.get())
	{
		if (isConnected)
		{	
			float dataValue = dataSets[0][(int) mainSlider->getValue()];
			sender.send("/juce/message", dataValue);
		}
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
				statusLabel->setColour(Label::textColourId, Colour(Colours::darkgreen));
				statusLabel->setText("Conectado", dontSendNotification);
			}

			isConnected = true;
		
		}
	}
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
	g.fillAll(Colour(Colours::black));

}

void MainComponent::resized()
{
    
}
