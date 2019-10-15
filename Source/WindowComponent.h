/*
  ==============================================================================

    InitialWindow.h
    Created: 18 Sep 2019 9:41:03pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "WindowClass.h"
#include "AudioRecorder.h"

class InsideComponent: public GenericWindowComponent
{
public:
	InsideComponent()
	{

		getLookAndFeel().setColour(ComboBox::backgroundColourId, Colour(uint8(7), uint8(21), uint8(36)));
		getLookAndFeel().setColour(PopupMenu::backgroundColourId, Colour(uint8(7), uint8(21), uint8(36)));
		getLookAndFeel().setColour(PopupMenu::highlightedBackgroundColourId, Colour(uint8(243), uint8(74), uint8(40)));
		getLookAndFeel().setColour(ComboBox::ColourIds::buttonColourId, Colour(uint8(243), uint8(74), uint8(40)));
		getLookAndFeel().setColour(ComboBox::ColourIds::textColourId, Colour(uint8(248), uint8(173), uint8(88)));
		getLookAndFeel().setColour(PopupMenu::ColourIds::textColourId, Colour(uint8(248), uint8(173), uint8(88)));

		StringArray sexArray = { "Hombre", "Mujer" };
		StringArray ageArray = {"0 - 5", "6 - 11", "12 - 17", "18 - 28", "29 - 50", "60 o mas"};
		StringArray comunaArray = { "Comuna 1", "Comuna 2", "Comuna 3", "Comuna 4" };
		// ============================================================================
		        
		String introMessage = String::createStringFromData(BinaryData::TextEx_txt, BinaryData::TextEx_txtSize);
		// =============================================================================
		        
		auto desktopArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
		float desktopSize = desktopArea.getWidth() * desktopArea.getHeight();
        
        introText.reset (new Label());
        addAndMakeVisible (introText.get());
        introText->setText (introMessage, dontSendNotification);
        introText->setFont (Font(desktopSize * 0.00001));
        introText->setColour (Label::ColourIds::textColourId, Colour(Colours::white));
        introText->setJustificationType(Justification::horizontallyJustified);
        introText->setVisible(false);
        
		initialButton.reset (new RoundedButton());
		addAndMakeVisible (initialButton.get());
		initialButton->setButtonText ("Comenzar!");
		initialButton->addListener (this);
		        
		// =============================================================================
		        
        sexMenu.reset (new ComboBox());
        addAndMakeVisible (sexMenu.get());
        sexMenu->addItemList (sexArray, 1);
        sexMenu->setTextWhenNothingSelected ("Selecciona tu sexo");
        sexMenu->setVisible(false);

        ageMenu.reset (new ComboBox());
        addAndMakeVisible (ageMenu.get());
        ageMenu->addItemList (ageArray, 2);
        ageMenu->setTextWhenNothingSelected ("Seleccionad tu rango de edad");
        ageMenu->setVisible(false);

        comunaMenu.reset (new ComboBox());
        addAndMakeVisible (comunaMenu.get());
        comunaMenu->addItemList (comunaArray, 3);
        comunaMenu->setTextWhenNothingSelected ("Selecciona tu comuna");
        comunaMenu->setVisible(false);
        
        initialStates = states::firstState;
	}
    
	~InsideComponent()
    {
        sexMenu = nullptr;
        ageMenu = nullptr;
        comunaMenu = nullptr;
        introText = nullptr;
        initialButton = nullptr;
    }

	void paint(Graphics& g) override
	{
        juce::Rectangle<int> area = getLocalBounds();
        
        switch (initialStates)
        {
            case firstState:
            {
                backgroundImage = ImageFileFormat::loadFrom (BinaryData::intro_jpeg, BinaryData::intro_jpegSize);
                break;
            }
            case secondState:
            {
                backgroundImage = ImageFileFormat::loadFrom (BinaryData::registro_jpeg, BinaryData::registro_jpegSize);
                break;
            }
            default:
                break;
        }
        
        g.drawImage(backgroundImage, area.toFloat());
	}

	void resized() override
	{
        introText->setBoundsRelative (0.1f, 0.5f, 0.8f, 0.25f);
        initialButton->setBoundsRelative (0.425, 0.8f, 0.15f, 0.075f);
		        
        sexMenu->setBoundsRelative (0.025f, 0.45f, 0.3f, 0.065f);
        ageMenu->setBoundsRelative (0.35f, 0.45f, 0.3f, 0.065f);
        comunaMenu->setBoundsRelative (0.675f, 0.45f, 0.3f, 0.065f);
	}

	void buttonClicked(Button* buttonThatWasClicked) override
	{
        if (buttonThatWasClicked == initialButton.get())
        {	
			
            switch (initialStates) {
            case firstState:
                    sexMenu->setVisible (true);
                    ageMenu->setVisible (true);
                    comunaMenu->setVisible (true);
                    initialButton->setButtonText ("Siguiente");
                    initialStates = states::secondState;
                break;
            case secondState:
                    sendChangeMessage();
                break;
            default:
                break;
            }
            repaint();
        }
	}
	
private:
    std::unique_ptr<RoundedButton> initialButton;
    std::unique_ptr<ComboBox> sexMenu, ageMenu, comunaMenu;
    std::unique_ptr<Label> introText;
    Image backgroundImage;
    enum states {firstState, secondState} initialStates;
};

// ===============================================================

class SecondComponent: public GenericWindowComponent,
                       private Timer
{
public:
    SecondComponent(AudioDeviceManager* deviceManager)
    {
        auto desktopArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
        float desktopSize = desktopArea.getWidth() * desktopArea.getHeight();
        
        nextButton.reset (new RoundedButton ());
		nextButton->setButtonText("Continuar");
        addAndMakeVisible (nextButton.get());
        nextButton->addListener (this);
        
        text.reset (new Label());
        addAndMakeVisible(text.get());
        text->setFont (Font(desktopSize * 0.00001));
        text->setText ("Tantas Personas de tu edad ___ anos y sexo ___  han sido asesinados", dontSendNotification);
        text->setJustificationType(Justification::centred);
		text->setVisible(false);
        componentState = states::firstState; 

		audioDeviceManager.reset(deviceManager);
        audioDeviceManager->addAudioCallback (&audioRecorder);
    }
    ~SecondComponent()
    {
        nextButton = nullptr;
        text = nullptr;
		audioDeviceManager->removeAudioCallback(&audioRecorder);
		audioDeviceManager.release();
    }
    void paint(Graphics& g) override
    {
		switch (componentState)
		{
		case firstState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::comotu_jpeg, BinaryData::comotu_jpegSize);
			break;
		case secondState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::mensaje_jpeg, BinaryData::mensaje_jpegSize);
			break;
		case thirdState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::gracias_jpeg, BinaryData::gracias_jpegSize);
			break;
		case fourthState:
			break;
		default:
			break;
		}
		juce::Rectangle<int> area = getLocalBounds();
		g.drawImage(backgroundImage, area.toFloat());
    }
    void resized() override
    {
        text->setBoundsRelative (0.1f, 0.5f, 0.8f, 0.25f);
        nextButton->setBoundsRelative (0.425, 0.8f, 0.15f, 0.075f);;
    }
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        switch (componentState)
        {
        case firstState:
                componentState = states::secondState;
            break;
        case secondState:
                nextButton->setButtonText("Grabar");
                componentState = states::thirdState;
            break;
        case thirdState:
		{
			nextButton->setEnabled(false);
			parentDir = File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory);
			outputFile = parentDir.getNonexistentChildFile(Time::getCurrentTime().toISO8601(false), ".wav");
			audioRecorder.startRecording(outputFile);
			startTimer(1000);
			break;
		}
        case fourthState:
                sendChangeMessage();
            break;
        default:
            break;
        }
		repaint();
    }
    
    void timerCallback() override
    {
        if(--counter >= 1)
        {
            nextButton->setButtonText(String(counter));
        }
        else
        {
            componentState = states::fourthState;
            nextButton->setButtonText("Terminar");
            nextButton->setEnabled(true);
            counter = 0;
            audioRecorder.stop();
            outputFile = File();
			repaint();
            stopTimer();
        }
    }
    
private:
    int counter = 6;
    File outputFile;
    File parentDir;
    Image backgroundImage;
    std::unique_ptr<RoundedButton> nextButton;
    std::unique_ptr<Label>  text;
    AudioRecorder audioRecorder;
	
    std::unique_ptr<AudioDeviceManager> audioDeviceManager;
    enum states {firstState, secondState, thirdState, fourthState, fifthState} componentState;
};

