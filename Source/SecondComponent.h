/*
  ==============================================================================

    SecondComponent.h
    Created: 20 Oct 2019 3:26:43pm
    Author:  WATEO

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "WindowClass.h"
#include "AudioRecorder.h"
#include "ProjectColours.h"

class SecondComponent : public GenericWindowComponent,
						private Timer
{
public:
	SecondComponent(AudioDeviceManager* deviceManager, String gender, int deaths, int location)
	{
		auto desktopArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
		float desktopSize = desktopArea.getWidth() * desktopArea.getHeight();
		Location = location;
		String texto = String(CharPointer_UTF8("Detr\xc3\xa1s de cada dato hay una historia,\n"
			"un nombre, una voz,\n"
			"como la tuya.\n \n"));

		String plural = gender == "Hombre" ? "s" : "es";
		String femeninos = gender == "Hombre" ? "asesinados" : "asesinadas";

		texto = texto + String(deaths) + " " + gender + plural + " de tu edad, en tu comuna \nfueron " + femeninos;

		texto = texto + String(CharPointer_UTF8(" en el \xc3\xbaltimo a\xc3\xb1o."));

		nextButton.reset(new RoundedButton());
		nextButton->setButtonText("Continuar");
		addAndMakeVisible(nextButton.get());
		nextButton->addListener(this);

		text.reset(new Label());
		addAndMakeVisible(text.get());
		text->setFont(Font(50.0f));
		text->setText(texto, dontSendNotification);
		text->setJustificationType(Justification::centred);
		text->setColour(juce::Label::textColourId, projectColours.amarillo);
		text->setJustificationType(Justification::centredTop);
		text->setVisible(true);
		componentState = states::firstState;

		audioDeviceManager.reset(deviceManager);
		audioDeviceManager->addAudioCallback(&audioRecorder);
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
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::Fondo_png, BinaryData::Fondo_pngSize);
			break;
		case secondState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::Fondo_png, BinaryData::Fondo_pngSize);
			break;
		case thirdState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::grabar_png, BinaryData::grabar_pngSize);
			break;
		case fourthState:
			backgroundImage = ImageFileFormat::loadFrom(BinaryData::Final_png, BinaryData::Final_pngSize);
			break;
		default:
			break;
		}

		juce::Rectangle<int> area = getLocalBounds();
		g.drawImage(backgroundImage, area.toFloat());

		if (imageIsCreated)
		{
			Image mapImage;
			if (Location == 1)
				mapImage = ImageFileFormat::loadFrom(BinaryData::sentidos_png, BinaryData::sentidos_pngSize);
			else if (Location == 2)
				mapImage = ImageFileFormat::loadFrom(BinaryData::parque_amor_png, BinaryData::parque_amor_pngSize);
			else if (Location == 3)
				mapImage = ImageFileFormat::loadFrom(BinaryData::bib_floresta_png, BinaryData::bib_floresta_pngSize);
			else if (Location == 4)
				mapImage = ImageFileFormat::loadFrom(BinaryData::san_javier_png, BinaryData::san_javier_pngSize);

			juce::Rectangle<float> imageRec(area.getWidth() * 0.15, area.getHeight() * 0.225, area.getWidth() * 0.7, area.getHeight() * 0.60);

			g.drawImage(mapImage, imageRec);

			imageIsCreated = false;
		}
	}
	void resized() override
	{
		text->setBoundsRelative(0.10f, 0.2f, 0.8f, 0.6f);
		nextButton->setBoundsRelative(0.425, 0.85f, 0.15f, 0.075f);
	}
	void buttonClicked(Button* buttonThatWasClicked) override
	{
		switch (componentState)
		{
		case firstState:
		{
			int deathCount = 0;
			if (Location == 1) deathCount = 4;
			else if (Location == 2) deathCount = 2;
			else if (Location == 3) deathCount = 4;
			else if (Location == 4) deathCount = 5;
			String outText = String(CharPointer_UTF8("Aqu\xc3\xad. \n A 300 metros de distancia fueron asesinadas ")) + String(deathCount) + String(CharPointer_UTF8(" personas en el \xc3\xbaltimo a\xc3\xb1o")); 
			text->setText(outText, dontSendNotification);
			text->setFont(40.0f);
			text->setBoundsRelative(0.0f, 0.05f, 1.0f, 0.15f);
			imageIsCreated = true;
			componentState = states::secondState;
			break;
		}
		case secondState:
		{
			String texto = String::fromUTF8(BinaryData::grabacion_txt, BinaryData::grabacion_txtSize);
			text->setText(texto, dontSendNotification);
			text->setBoundsRelative(0.2f, 0.05f, 0.6f, 0.70f);
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
			text->setText("Grabando!", dontSendNotification);
			text->setJustificationType(Justification::verticallyCentred);
			text->setFont(120.0f);
			changeState = true;
			sendChangeMessage();
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
		if (counter-- >= 1)
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
			changeState = false;
			String textoo = String(CharPointer_UTF8("Para conocer m\xc3\xa1s de este proyecto y asistir a socializaciones visita: laboratoriodelsonido.com.co"));
			text->setText(textoo, dontSendNotification);
			text->setFont(35.0f);
			text->setJustificationType(Justification::centredBottom);
			text->setBoundsRelative(0.2f, 0.0f, 0.6f, 0.60f);
			repaint();
			stopTimer();
		}
	}
	bool changeState = false;
	enum states { firstState, secondState, thirdState, fourthState, fifthState } componentState;

private:
	int counter = 10;
	File outputFile;
	File parentDir;
	Image backgroundImage;
	std::unique_ptr<RoundedButton> nextButton;
	std::unique_ptr<Label>  text;
	ProjectColours projectColours;
	AudioRecorder audioRecorder;
	bool imageIsCreated = false;
	int Location;
	std::unique_ptr<AudioDeviceManager> audioDeviceManager;
};