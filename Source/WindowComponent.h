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
#include "ProjectColours.h"

class InsideComponent: public GenericWindowComponent
{
public:
	InsideComponent()
	{
		getLookAndFeel().setColour(ComboBox::backgroundColourId, Colour(uint8(7), uint8(21), uint8(36)));
		getLookAndFeel().setColour(PopupMenu::backgroundColourId, Colour(uint8(7), uint8(21), uint8(36)));
		getLookAndFeel().setColour(PopupMenu::highlightedBackgroundColourId, projectColours.naranja);
        
		getLookAndFeel().setColour(ComboBox::ColourIds::buttonColourId, Colour(uint8(243), uint8(74), uint8(40)));
        getLookAndFeel().setColour(ComboBox::ColourIds::textColourId, projectColours.amarillo);
		getLookAndFeel().setColour(PopupMenu::ColourIds::textColourId, projectColours.amarillo);

		StringArray sexArray = { "Hombre", "Mujer" };
		StringArray ageArray = {"0 - 5", "6 - 11", "12 - 17", "18 - 28", "29 - 50", String(CharPointer_UTF8 ("60 o m\xc3\xa1s"))};
        StringArray comunaArray = { "Comuna 1: Popular", "Comuna 2: Santa Cruz", "Comuna 3: Manrique", "Comuna 4: Aranjuez", "Comuna 5: Castilla", "Comuna 6: 12 de Octubre", "Comuna 7: Robledo ", "Comuna 8: Villa Hermosa", "Comuna 9: Buenos Aires", "Comuna 10: La Candelaria", "Comuna 11: Laureles - Estadio", String(CharPointer_UTF8 ("Comuna 12: La Am\xc3\xa9rica")), "Comuna 13: San Javier", "Comuna 14: El Poblado", "Comuna 15: Guayabal", String(CharPointer_UTF8 ("Comuna 16: Bel\xc3\xa9n")), CharPointer_UTF8 ("\"Corregimiento 50: San Sebasti\xc3\xa1n de Palmitas\""), "Corregimiento 60: San Cristobal", "Corregimiento 70: Altavista", "Corregimiento 80: San Antonio de Prado", "Corregimiento 90: Santa Elena"};
		// ============================================================================
		        
        String introMessage = String::fromUTF8(BinaryData::intro_txt, BinaryData::intro_txtSize);
		// =============================================================================
		        
		auto desktopArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
		float desktopSize = desktopArea.getWidth() * desktopArea.getHeight();
        
        introText.reset (new Label());
        addAndMakeVisible (introText.get());
        introText->setText (introMessage, dontSendNotification);
        introText->setFont (Font(desktopSize * 0.000015));
        introText->setColour (Label::ColourIds::textColourId, projectColours.amarillo);
        introText->setJustificationType(Justification::horizontallyJustified);
        introText->setVisible(true);
        
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
        ageMenu->addItemList (ageArray, 1);
        ageMenu->setTextWhenNothingSelected ("Seleccionad tu rango de edad");
        ageMenu->setVisible(false);

        comunaMenu.reset (new ComboBox());
        addAndMakeVisible (comunaMenu.get());
        comunaMenu->addItemList (comunaArray, 1);
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
            case thirdState:
            {
                backgroundImage =ImageFileFormat::loadFrom (BinaryData::instrucciones_jpeg, BinaryData::instrucciones_jpegSize);
            }
            default:
                break;
        }
        
        g.drawImage(backgroundImage, area.toFloat());
	}

	void resized() override
	{
        introText->setBoundsRelative (0.2f, 0.45f, 0.60f, 0.30f);
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
                {
                    sexMenu->setVisible (true);
                    ageMenu->setVisible (true);
                    comunaMenu->setVisible (true);
                    String text = String::fromUTF8(BinaryData::datos_txt, BinaryData::datos_txtSize);
                    introText->setBoundsRelative (0.15f, 0.2f, 0.60f, 0.30f);
                    introText->setText(text, dontSendNotification);
                    initialButton->setButtonText ("Siguiente");
                    initialStates = states::secondState;
                break;
                }
            case secondState:
                {
                    sexMenu->setVisible (false);
                    ageMenu->setVisible (false);
                    comunaMenu->setVisible (false);
                    String text = String::fromUTF8(BinaryData::instrucciones_txt, BinaryData::instrucciones_txtSize);
                    introText->setText(text, dontSendNotification);
                    introText->setBoundsRelative (0.15f, 0.3f, 0.60f, 0.30f);
                    initialStates = states::thirdState;
                break;
                }
                case thirdState:
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
    ProjectColours projectColours;
    Image backgroundImage;
    enum states {firstState, secondState, thirdState} initialStates;
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
        
        String texto = "Tantas personas han sido asesinadas al rededor";
        
        nextButton.reset (new RoundedButton ());
		nextButton->setButtonText("Continuar");
        addAndMakeVisible (nextButton.get());
        nextButton->addListener (this);
        
        text.reset (new Label());
        addAndMakeVisible(text.get());
        text->setFont (Font(desktopSize * 0.000012));
        text->setText (texto, dontSendNotification);
        text->setJustificationType(Justification::horizontallyJustified);
        text->setColour(juce::Label::textColourId, projectColours.amarillo);
		text->setVisible(true);
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
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::distancia_jpeg, BinaryData::distancia_jpegSize);
			break;
		case secondState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::datos_jpeg, BinaryData::datos_jpegSize);
			break;
		case thirdState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::grabar_jpeg, BinaryData::grabar_jpegSize);
			break;
		case fourthState:
                backgroundImage = ImageFileFormat::loadFrom(BinaryData::gracias_jpeg, BinaryData::gracias_jpegSize);
			break;
		default:
			break;
		}
		juce::Rectangle<int> area = getLocalBounds();
		g.drawImage(backgroundImage, area.toFloat());
    }
    void resized() override
    {
        text->setBoundsRelative (0.20f, 0.2f, 0.6f, 0.5f);
        nextButton->setBoundsRelative (0.425, 0.8f, 0.15f, 0.075f);;
    }
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        switch (componentState)
        {
        case firstState:
                text->setText ("Numero de personas de tu edad han sido asesinadas", dontSendNotification);
                componentState = states::secondState;
            break;
        case secondState:
            {
                String texto = String::fromUTF8(BinaryData::grabacion_txt, BinaryData::grabacion_txtSize);
                text->setText (texto, dontSendNotification);
                componentState = states::thirdState;
                nextButton->setButtonText("Grabar");
                sendChangeMessage();
            }
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
            nextButton->setButtonText("Grabando... " + String(counter));
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
    enum states {firstState, secondState, thirdState, fourthState, fifthState} componentState;
    
private:
    int counter = 6;
    File outputFile;
    File parentDir;
    Image backgroundImage;
    std::unique_ptr<RoundedButton> nextButton;
    std::unique_ptr<Label>  text;
    ProjectColours projectColours;
    AudioRecorder audioRecorder;
	
    std::unique_ptr<AudioDeviceManager> audioDeviceManager;
};

