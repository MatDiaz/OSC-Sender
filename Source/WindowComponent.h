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
		StringArray sexArray = { "Hombre", "Mujer" };
		StringArray ageArray = {"15 - 20", "21 - 25", "26 - 30", "31 - 35"};
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
        initialButton->setBoundsRelative (0.45f, 0.75f, 0.1f, 0.1f);
		        
        sexMenu->setBoundsRelative (0.1f, 0.5f, 0.2f, 0.05f);
        ageMenu->setBoundsRelative (0.4f, 0.5f, 0.2f, 0.05f);
        comunaMenu->setBoundsRelative (0.7f, 0.5f, 0.2f, 0.05f);
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
    SecondComponent()
    {
        auto desktopArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
        float desktopSize = desktopArea.getWidth() * desktopArea.getHeight();
        
        nextButton.reset (new TextButton ("Continuar"));
        addAndMakeVisible (nextButton.get());
        nextButton->addListener (this);
        
        title.reset (new Label());
        addAndMakeVisible (title.get());
        title->setFont (Font(desktopSize * 0.00015));
        title->setText ("Como tu", dontSendNotification);
        title->setJustificationType(Justification::centred);
        
        text.reset (new Label());
        addAndMakeVisible(text.get());
        text->setFont (Font(desktopSize * 0.00001));
        text->setText ("Tantas Personas de tu edad ___ anos y sexo ___  han sido asesinados", dontSendNotification);
        text->setJustificationType(Justification::centred);
        componentState = states::firstState; 
        audioDeviceManager.initialiseWithDefaultDevices (1, 1);
        audioDeviceManager.addAudioCallback (&audioRecorder);
    }
    ~SecondComponent()
    {
        nextButton = nullptr;
        title = nullptr;
        text = nullptr;
    }
    void paint(Graphics& g) override
    {
        
    }
    void resized() override
    {
        title->setBoundsRelative(0.0f, 0.0f, 1.0f, 0.5f);
        text->setBoundsRelative (0.1f, 0.5f, 0.8f, 0.25f);
        nextButton->setBoundsRelative (0.45f, 0.75f, 0.1f, 0.1f);
    }
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        switch (componentState)
        {
        case firstState:
                componentState = states::secondState;
            break;
        case secondState:
                title->setText("Une Mensaje Para la vida", dontSendNotification);
                nextButton->setButtonText("Grabar");
                componentState = states::thirdState;
            break;
        case thirdState:
                nextButton->setEnabled(false);
                parentDir = File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory);
                outputFile = parentDir.getNonexistentChildFile(Time::getCurrentTime().toString(true, true), ".wav");
                audioRecorder.startRecording(outputFile);
                startTimer(1000);
            break;
        case fourthState:
                sendChangeMessage();
            break;
        default:
            break;
        }
    }
    
    void timerCallback() override
    {
        if(++counter <= 4)
        {
            nextButton->setButtonText("Grabando...");
        }
        else
        {
            componentState = states::fourthState;
            nextButton->setButtonText("Terminar");
            nextButton->setEnabled(true);
            title->setText("Gracias!", dontSendNotification);
            counter = 0;
            audioRecorder.stop();
            outputFile = File();
            stopTimer();
        }
    }
    
private:
    int counter = 0;
    File outputFile;
    File parentDir;
    Image backgroundImage;
    std::unique_ptr<TextButton> nextButton;
    std::unique_ptr<Label> title, text;
    AudioRecorder audioRecorder;
    AudioDeviceManager audioDeviceManager;
    enum states {firstState, secondState, thirdState, fourthState} componentState;
};

