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
    
    addAndMakeVisible (initialWindow.get());
    juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    auto x = r.getWidth();
    auto y = r.getHeight();
    initialWindow.reset(new InitialWindow("!Escucha!", true));
    initialWindow->setVisible (true);
    initialWindow->setSize(x, y);
    

    //==============================================================================
    
    autoButton.reset (new TextButton());
    addAndMakeVisible (autoButton.get());
    autoButton->setButtonText ("Auto-Play");
    autoButton->setColour (TextButton::buttonOnColourId, Colour(Colours::grey));
    autoButton->setEnabled (false);
    autoButton->addListener (this);
    autoButton->setClickingTogglesState (true);

	//=============================================================================

	playbackSpeedSlider.reset (new Slider());
	addAndMakeVisible(playbackSpeedSlider.get());
	playbackSpeedSlider->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
	playbackSpeedSlider->setColour (Slider::thumbColourId, Colour(Colours::grey));
	playbackSpeedSlider->setColour (Slider::trackColourId, Colour(Colours::darkgrey));
	playbackSpeedSlider->setColour (Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::grey));	
	playbackSpeedSlider->setRange (0.25, 4, 0);
	playbackSpeedSlider->setSkewFactorFromMidPoint(1.0f);
	playbackSpeedSlider->setValue(1.0f, dontSendNotification);
    //==============================================================================

	speedLabel.reset (new Label());
	addAndMakeVisible (speedLabel.get());
	speedLabel->setText("0.25x", dontSendNotification);
	speedLabel->setColour(Label::textColourId, Colour(Colours::white));
	speedLabel->setJustificationType(juce::Justification::horizontallyJustified);

	speedLabel_Two.reset (new Label());
	addAndMakeVisible(speedLabel_Two.get());
	speedLabel_Two->setText("1.0x", dontSendNotification);
	speedLabel_Two->setColour(Label::textColourId, Colour(Colours::white));
	speedLabel_Two->setJustificationType(juce::Justification::horizontallyJustified);

	speedLabel_Three.reset(new Label());
	addAndMakeVisible(speedLabel_Three.get());
	speedLabel_Three->setText("4.0x", dontSendNotification);
	speedLabel_Three->setColour(Label::textColourId, Colour(Colours::white));
	speedLabel_Three->setJustificationType(juce::Justification::centredRight);

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
    
    addAndMakeVisible(secondPlot);
    secondPlot.setEnabled(false);
    
    addAndMakeVisible(thirdPlot);
    thirdPlot.setEnabled(false);
    //==============================================================================
    
    setSize (900, 700);
    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    dataSets.clearQuick(false);
    initialWindow = nullptr;
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

void MainComponent::receiveArray (Array<float> &inArray, StringArray inStringArray, int dataSetSize)
{
    dataSets.add (inArray.getRawDataPointer());
	dataSetTam = dataSetSize;

	hostEnter->setEnabled(true);
    hostEnter->setReadOnly(false);
	
	addressEnter->setEnabled(true);
    addressEnter->setReadOnly(false);
	
	messageEnter->setEnabled(true);
	messageEnter->setReadOnly(false);
	
	mainButton->setEnabled(true);
    
    mainPlot.updatePlot (inArray.getRawDataPointer(), dataSetSize, true);
    
    mainPlot.addYDataToPlot (inStringArray);
    
    isLoaded = true;

    float Min, Max;
    findMinAndMax(inArray.getRawDataPointer(), dataSetSize, Min, Max);
    normFactor = jmax(abs(Min), Max);
}

float MainComponent::interpolateData(float inValue)
{
    int prevPosition = floor (inValue);
    int nextPosition = ceil (inValue);
    float fraction = inValue - prevPosition;
    return (dataSets[0][nextPosition] * fraction) + (dataSets[0][prevPosition] * (1 - fraction));
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
    if ( e.originalComponent == &mainPlot && isConnected)
    {
		float normValue = 0;
		
		if (mainPlot.dataValue >= 0 && mainPlot.dataValue <= dataSetTam)
            normValue = interpolateData (mainPlot.dataValue) / normFactor;
        
        std::cout << normValue << std::endl;
        
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

    autoButton->setBoundsRelative (0.05, 0.125, 0.1, 0.05);
	playbackSpeedSlider->setBoundsRelative (0.2, 0.125, 0.4, 0.05);

	speedLabel->setBoundsRelative(0.2, 0.1, 0.25, 0.05);
	speedLabel_Two->setBoundsRelative(0.38, 0.1, 0.25, 0.05);
	speedLabel_Three->setBoundsRelative(0.5, 0.1, 0.1, 0.05);

    mainButton->setBoundsRelative(0.65, 0.125, 0.1, 0.05);
    statusLabel->setBoundsRelative(0.8, 0.125, 0.1, 0.05);
    
    fileName->setBoundsRelative(0.05, 0.2, 0.1, 0.05);
    
    mainPlot.setBoundsRelative(0, 0.25, 1, 0.75);

    if(isLoaded)
    {
        mainPlot.updatePlot();
    }
}
// ==============================================================================
void MainComponent::timerCallback()
{
	mainPlot.interpolatePosition(cursorPosition);
	
	const float cycleTime = (1000.0f * 60.0f) / (30.0f * playbackSpeedSlider->getValue());

	cursorPosition += (float) dataSetTam / (float) cycleTime;
	
	cursorPosition = cursorPosition > dataSetTam ? 0 : cursorPosition;
    
    interpolateData(cursorPosition);
	
	float normValue = interpolateData (cursorPosition) / normFactor;
    
	sender.send(messageEnter->getTextValue().toString(), normValue);
}
// ==============================================================================

