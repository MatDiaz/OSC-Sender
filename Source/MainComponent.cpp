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
    
    addAndMakeVisible (initialWindow);
    juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    auto x = r.getWidth();
    auto y = r.getHeight();
    initialWindow = new InitialWindow("!Escucha!", true);
    initialWindow->setVisible (true);
    initialWindow->setSize(x, y);
    initialWindow->addChangeListener(this);
    
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
	speedLabel->setText ("Lento", dontSendNotification);
	speedLabel->setColour (Label::textColourId, Colour(Colours::white));
	speedLabel->setJustificationType (juce::Justification::centred);

	speedLabel_Two.reset (new Label());
	addAndMakeVisible (speedLabel_Two.get());
	speedLabel_Two->setText ("Velocidad", dontSendNotification);
    speedLabel_Two->setFont (Font(20.0f));
	speedLabel_Two->setColour (Label::textColourId, Colour(Colours::white));
	speedLabel_Two->setJustificationType (juce::Justification::centred);

	speedLabel_Three.reset (new Label());
	addAndMakeVisible (speedLabel_Three.get());
	speedLabel_Three->setText (CharPointer_UTF8 ("R\xc3\xa1pido"), dontSendNotification);
	speedLabel_Three->setColour (Label::textColourId, Colour(Colours::white));
	speedLabel_Three->setJustificationType (juce::Justification::centred);

	//==============================================================================

	addAndMakeVisible (mainPlot);
	mainPlot.setEnabled(true);
    
    addAndMakeVisible(secondPlot);
    secondPlot.setEnabled(false);
    
    addAndMakeVisible(thirdPlot);
    thirdPlot.setEnabled(false);
    
    //==============================================================================
    setSize(x, y);
    
    readTextFileData (BinaryData::homicidio_txt, BinaryData::homicidio_txtSize, mainPlot, firstArray);
    readTextFileData (BinaryData::suicidio_txt, BinaryData::suicidio_txtSize, secondPlot, secondArray);
    readTextFileData (BinaryData::transporte_txt, BinaryData::transporte_txtSize, thirdPlot, thirdArray);
    
    setAudioChannels (2, 2);
    sender.connect("127.0.0.1", 9001);
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
    
    mainPlot.updatePlot (inArray.getRawDataPointer(), dataSetSize, true);
    
    mainPlot.addYDataToPlot (inStringArray);
    
    isLoaded = true;

    float Min, Max;
    
    findMinAndMax(inArray.getRawDataPointer(), dataSetSize, Min, Max);
    normFactor = jmax(abs(Min), Max);
}

float MainComponent::interpolateData (float inValue, bool isNormalized, Array<float> nArray, const String& Message)
{
    if (isNormalized) (inValue = inValue * nArray.size());
    int prevPosition = floor (inValue);
    int nextPosition = ceil (inValue);
    float fraction = inValue - prevPosition;
    float value = (nArray[nextPosition] * fraction) + (nArray[prevPosition] * (1 - fraction));
    float Min, Max;
    findMinAndMax (nArray.getRawDataPointer(), nArray.size(), Min, Max);
    float normFactor = jmax (abs(Min), Max);
    sender.send (Message, (value / normFactor));
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
    if (e.originalComponent == &mainPlot || e.originalComponent == &secondPlot || e.originalComponent == &thirdPlot)
        cursorPosition = e.originalComponent->getMouseXYRelative().getX() / (float) e.originalComponent->getWidth();
}

void MainComponent::buttonClicked(Button *button)
{

}

// ==============================================================================
void MainComponent::paint(Graphics& g)
{
	g.fillAll(Colour(Colours::black));
}

void MainComponent::readTextFileData (const char *textFileData, int textFileSize, Plot& plotToAdd, Array<float>& nArray)
{
    StringArray months = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    
    Array<float> valueArray;
    StringArray dateArray;
    
    String inputFile = String::fromUTF8 (textFileData, textFileSize);
    
    std::stringstream stringStream (inputFile.toStdString());
    std::string token;
    
    while (std::getline (stringStream, token, '\n'))
    {
        std::stringstream secondStream(token);
        std::string secondToken;
        int count = 0;
        
        String outDate = "";
        
        while(std::getline(secondStream, secondToken, '-'))
        {
            if (count == 0)
            {
                outDate += String (secondToken) + " ";
            }
            else if (count == 1)
            {
                outDate += months[std::stoi (secondToken) - 1];
            }
            else if (count == 2)
            {
                std::string valueToken;
                std::stringstream thirdString(secondToken);
                bool state = false;
                while(std::getline(thirdString, valueToken, ','))
                {
                    if (state)
                        valueArray.add (std::stof(valueToken));
                    state = !state;
                }
            }
            if (++count > 2) count = 0;
        }
        
        dateArray.add (outDate);
    }
    
    nArray = valueArray;

    plotToAdd.updatePlot (valueArray.getRawDataPointer(), valueArray.size(), true);
    plotToAdd.addYDataToPlot (dateArray);
}

void MainComponent::resized()
{
	playbackSpeedSlider->setBoundsRelative (0.25, 0.05, 0.5, 0.05);

	speedLabel->setBoundsRelative(0.20, 0.05, 0.05, 0.05);
	speedLabel_Two->setBoundsRelative(0.45, 0, 0.1, 0.05);
	speedLabel_Three->setBoundsRelative(0.75, 0.05, 0.1, 0.05);
    
    mainPlot.setBoundsRelative (0, 0.1, 1, 0.3);
    secondPlot.setBoundsRelative (0, 0.4, 1, 0.3);
    thirdPlot.setBoundsRelative (0, 0.7, 1, 0.3);

//    if(isLoaded) mainPlot.updatePlot();
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == initialWindow)
    {
        initialWindow.deleteAndZero();
        startTimer(30);
    }
}

// ==============================================================================
void MainComponent::timerCallback()
{
	mainPlot.updateCursor (cursorPosition, true);
    secondPlot.updateCursor (cursorPosition, true);
    thirdPlot.updateCursor (cursorPosition, true);
	
	const float cycleTime = (1000 * 60) / (30 * playbackSpeedSlider->getValue());

	cursorPosition += 1.0f / (float) cycleTime;
	
    if (cursorPosition >= 1)
    {
        cursorPosition = 0;
        stopTimer();
        
        addAndMakeVisible (initialWindow);
        juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
        auto x = r.getWidth();
        auto y = r.getHeight();
        initialWindow = new InitialWindow("!Escucha!", true);
        initialWindow->setVisible (true);
        initialWindow->setSize(x, y);
        initialWindow->addChangeListener(this);
    }
    
    interpolateData(cursorPosition, true, firstArray, "/homicidio");
    interpolateData(cursorPosition, true, secondArray, "/suicidio");
    interpolateData(cursorPosition, true, thirdArray, "/transporte");
}
// ==============================================================================
