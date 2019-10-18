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
	tptr = Typeface::createSystemTypefaceFor(BinaryData::SharpGroteskLight15_otf, BinaryData::SharpGroteskLight15_otfSize);

	LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(tptr);

	auto defaultFont = Font(tptr);
	defaultFont.setExtraKerningFactor(0.5f);
	defaultFont.setHorizontalScale(1.0f);
	defaultFont.setHeight(40.0f);

	addMouseListener(this, true);

	date.reset(new Label());
	addAndMakeVisible(date.get());
	date->setText("Fecha:--", dontSendNotification);
	date->setColour(Label::textColourId, projectColours.naranja);
	date->setJustificationType(Justification::centred);
	date->setFont(defaultFont);

	//=============================================================================

	playbackSpeedSlider.reset(new Slider());
	addAndMakeVisible(playbackSpeedSlider.get());
	playbackSpeedSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	playbackSpeedSlider->setColour(Slider::thumbColourId, Colour(Colours::grey));
	playbackSpeedSlider->setColour(Slider::trackColourId, Colour(Colours::darkgrey));
	playbackSpeedSlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::grey));
	playbackSpeedSlider->setRange(0.25, 4, 0);
	playbackSpeedSlider->setSkewFactorFromMidPoint(1.0f);
	playbackSpeedSlider->setValue(1.0f, dontSendNotification);
	playbackSpeedSlider->setVisible(false);

	//==============================================================================

	speedLabel.reset(new Label());
	addAndMakeVisible(speedLabel.get());
	speedLabel->setText("Lento", dontSendNotification);
	speedLabel->setColour(Label::textColourId, Colour(Colours::white));
	speedLabel->setJustificationType(juce::Justification::centred);
	speedLabel->setVisible(false);

	speedLabel_Two.reset(new Label());
	addAndMakeVisible(speedLabel_Two.get());
	speedLabel_Two->setText("Velocidad", dontSendNotification);
	speedLabel_Two->setFont(Font(20.0f));
	speedLabel_Two->setColour(Label::textColourId, Colour(Colours::white));
	speedLabel_Two->setJustificationType(juce::Justification::centred);
	speedLabel_Two->setVisible(false);

	speedLabel_Three.reset(new Label());
	addAndMakeVisible(speedLabel_Three.get());
	speedLabel_Three->setText(CharPointer_UTF8("R\xc3\xa1pido"), dontSendNotification);
	speedLabel_Three->setColour(Label::textColourId, Colour(Colours::white));
	speedLabel_Three->setJustificationType(juce::Justification::centred);
	speedLabel_Three->setVisible(false);

	//==============================================================================

	addAndMakeVisible(mainPlot);
	mainPlot.setEnabled(true);
	mainPlot.setBackgroundColour(projectColours.azulOscuro);
	mainPlot.setPlotName("Suicidio");
	mainPlot.setOffset(0.15f);
	mainPlot.setEnabled(false);

	addAndMakeVisible(secondPlot);
	secondPlot.setEnabled(false);
	secondPlot.setBackgroundColour(projectColours.amarillo);
	secondPlot.setPlotName("Homicidio");
	secondPlot.setOffset(0.15f);
	secondPlot.setEnabled(false);

	addAndMakeVisible(thirdPlot);
	thirdPlot.setEnabled(false);
	thirdPlot.setBackgroundColour(projectColours.naranja);
	thirdPlot.setOffset(0.15f);
	thirdPlot.setPlotName(String(CharPointer_UTF8("Muertes de tr\xc3\xa1""fico")));
	thirdPlot.setEnabled(false);

	// =============================================================================
	juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	auto x = r.getWidth();
	auto y = r.getHeight();
	setSize(x, y);
	//==============================================================================

	readTextFileData(BinaryData::suicidio_txt, BinaryData::suicidio_txtSize, mainPlot, firstArray);
	readTextFileData(BinaryData::transporte_txt, BinaryData::transporte_txtSize, thirdPlot, thirdArray);
	readTextFileData(BinaryData::homicidio_txt, BinaryData::homicidio_txtSize, secondPlot, secondArray);

	readLocationFileData(BinaryData::_2019_grp2_txt, BinaryData::_2019_grp2_txtSize, false);
	readLocationFileData(BinaryData::_2019_grp2_datos_txt, BinaryData::_2019_grp2_datos_txtSize, true);

	setAudioChannels(2, 2);

	#if JUCE_LINUX
		deviceManager.setCurrentAudioDeviceType("JACK", true);
	#endif

	DBG(deviceManager.getCurrentAudioDeviceType());

    sender.connect("127.0.0.1", 9001);

	startTimer(1000);
}

MainComponent::~MainComponent()
{
    initialComponent = nullptr;
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

void MainComponent::interpolateData (float inValue, bool isNormalized, Array<float> nArray, const String& Message)
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

void MainComponent::mouseDrag (const MouseEvent& e)
{
	if (e.originalComponent == &mainPlot || e.originalComponent == &secondPlot || e.originalComponent == &thirdPlot)
	{
		cursorPosition = e.originalComponent->getMouseXYRelative().getX() / (float)e.originalComponent->getWidth();
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
        
        String year = "";
		String month = "";

        while(std::getline(secondStream, secondToken, '-'))
        {
            if (count == 0)
            {
                year = String (secondToken);
            }
            else if (count == 1)
            {
                month = months[std::stoi (secondToken) - 1] + " ";
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
        
        dateArray.add (month + year);
    }
    
    nArray = valueArray;

    plotToAdd.updatePlot (valueArray.getRawDataPointer(), valueArray.size(), true, valueArray);
    plotToAdd.addYDataToPlot (dateArray);
}

void MainComponent::resized()
{
	playbackSpeedSlider->setBoundsRelative (0.25, 0.05, 0.5, 0.05);

	speedLabel->setBoundsRelative(0.20, 0.05, 0.05, 0.05);
	speedLabel_Two->setBoundsRelative(0.45, 0, 0.1, 0.05);
	speedLabel_Three->setBoundsRelative(0.75, 0.05, 0.1, 0.05);

	date->setBoundsRelative(0.0f, 0.0f, 1.0f, 0.1f);
    
    mainPlot.setBoundsRelative (0, 0.1, 1, 0.3);
    secondPlot.setBoundsRelative (0, 0.4, 1, 0.3);
    thirdPlot.setBoundsRelative (0, 0.7, 1, 0.3);
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == initialWindow && initialComponent != nullptr)
    {
        currentGender = initialComponent->selectedSex;
        currentId = initialComponent->selectedId;
        deathNum = findData(currentId);
        locationId = initialComponent->location;
        initialComponent.reset();
        initialWindow.deleteAndZero();
        startTimer(30);
		sender.send("/toggle", 1.0f);
    }
    else if (source == initialWindow && secondComponent != nullptr)
    {
        if (secondComponent->componentState == SecondComponent::states::thirdState)
        {
            sender.send("/toggle3", 1.0f);
            sender.send("/toggle4", 1.0f);
			if (secondComponent->changeState)
			{
				sender.send("/toggle", 0.0f);
				sender.send("/toggle2", 0.0f);
				sender.send("/toggle3", 0.0f);
				sender.send("/toggle4", 0.0f);
			}
        }
        else
        {
            secondComponent.reset();
            initialWindow.deleteAndZero();
            executeSequence(true);
        }
    }
}

void MainComponent::executeSequence (bool init)
{
    if (initialWindow == nullptr && init)
    {
        addAndMakeVisible (initialWindow);
        juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
        auto x = r.getWidth();
        auto y = r.getHeight();
        initialComponent.reset (new InsideComponent());
        initialWindow = new InitialWindow("!Escucha!", true, initialComponent.get());
        initialWindow->setVisible (true);
        initialWindow->setSize(x, y);
		initialWindow->toFront(false);
        initialWindow->addChangeListener(this);
    }
    else if (initialWindow == nullptr && !init)
    {
        addAndMakeVisible (initialWindow);
        juce::Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
        auto x = r.getWidth();
        auto y = r.getHeight();
        secondComponent.reset (new SecondComponent(&deviceManager, currentGender, deathNum, locationId));
        initialWindow = new InitialWindow("!Escucha!", true, secondComponent.get());
        initialWindow->setVisible (true);
		initialWindow->toFront(false);
        initialWindow->setSize(x, y);
        initialWindow->addChangeListener(this);
    }
}

// ==============================================================================
void MainComponent::timerCallback()
{
	if (!init)
	{
		executeSequence(true);
		stopTimer();
		init = !init;
	}
	else
	{
		mainPlot.updateCursor(cursorPosition, true);
		secondPlot.updateCursor(cursorPosition, true);
		thirdPlot.updateCursor(cursorPosition, true);

		interpolateData(cursorPosition, true, firstArray, "/homicidio");
		interpolateData(cursorPosition, true, secondArray, "/suicidio");
		interpolateData(cursorPosition, true, thirdArray, "/transporte");

		date->setText(mainPlot.yDataToPlot[(int)round(cursorPosition * mainPlot.dataSetSize)], dontSendNotification);

		const float cycleTime = (2000 * 60) / (30 * playbackSpeedSlider->getValue());

		cursorPosition += 1.0f / (float)cycleTime;

		if (cursorPosition >= (1 - (1.0f / (float)cycleTime) * 5))
		{
			cursorPosition = 0;
			executeSequence(false);
			sender.send("/toggle2", 1.0f);
			stopTimer();
		}
	}
}
// ==============================================================================
