/*
  ==============================================================================

    MainWindowComponent.h
    Created: 20 Oct 2019 3:24:53pm
    Author:  WATEO

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WindowClass.h"
#include "ProjectColours.h"
#include "Plot.h"

class MainWindowComponent : public GenericWindowComponent,
							public Timer
{
public:
	MainWindowComponent(OSCSender* oscControl)
	{
		addMouseListener(this, true);

		date.reset(new Label());
		addAndMakeVisible(date.get());
		date->setText("Fecha:--", dontSendNotification);
		date->setColour(Label::textColourId, ProjectColours::naranja);
		date->setJustificationType(Justification::centred);
		date->setFont(60.0f);	

		addAndMakeVisible(mainPlot);
		mainPlot.setBackgroundColour(ProjectColours::azulOscuro);
		mainPlot.setPlotName("Suicidio");
		mainPlot.setOffset(0.15f);
		mainPlot.setEnabled(true);

		addAndMakeVisible(secondPlot);
		secondPlot.setEnabled(true);
		secondPlot.setBackgroundColour(ProjectColours::amarillo);
		secondPlot.setPlotName("Homicidio");
		secondPlot.setOffset(0.15f);
		secondPlot.setEnabled(true);
		
		addAndMakeVisible(thirdPlot);
		thirdPlot.setBackgroundColour(ProjectColours::naranja);
		thirdPlot.setOffset(0.15f);
		thirdPlot.setPlotName(String(CharPointer_UTF8("Muertes por tr\xc3\xa1""fico")));
		thirdPlot.setEnabled(true);

		setSize(getParentWidth(), getParentHeight());

		readTextFileData(BinaryData::suicidio_txt, BinaryData::suicidio_txtSize, mainPlot, firstArray);
		readTextFileData(BinaryData::transporte_txt, BinaryData::transporte_txtSize, thirdPlot, thirdArray);
		readTextFileData(BinaryData::homicidio_txt, BinaryData::homicidio_txtSize, secondPlot, secondArray);

		sender.reset(oscControl);
		startTimer(30);
		oscControl->send("/toggle", 1.0f);
	}

	void mouseDrag(const MouseEvent& e) override
	{
		if (e.originalComponent == &mainPlot || e.originalComponent == &secondPlot || e.originalComponent == &thirdPlot)
		{
			cursorPosition = e.originalComponent->getMouseXYRelative().getX() / (float)e.originalComponent->getWidth();
		}
	}

	~MainWindowComponent()
	{
		sender.release();
		date = nullptr;
	}

	void resized() override
	{
		date->setBoundsRelative(0.0f, 0.0f, 1.0f, 0.1f);

		mainPlot.setBoundsRelative(0, 0.1, 1, 0.3);
		secondPlot.setBoundsRelative(0, 0.4, 1, 0.3);
		thirdPlot.setBoundsRelative(0, 0.7, 1, 0.3);
	}

	void readTextFileData(const char* textFileData, int textFileSize, Plot& plotToAdd, Array<float>& nArray)
	{
		StringArray months = { "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre" };

		Array<float> valueArray;
		StringArray dateArray;

		String inputFile = String::fromUTF8(textFileData, textFileSize);

		std::stringstream stringStream(inputFile.toStdString());
		std::string token;

		while (std::getline(stringStream, token, '\n'))
		{
			std::stringstream secondStream(token);
			std::string secondToken;
			int count = 0;

			String year = "";
			String month = "";

			while (std::getline(secondStream, secondToken, '-'))
			{
				if (count == 0)
				{
					year = String(secondToken);
				}
				else if (count == 1)
				{
					month = months[std::stoi(secondToken) - 1] + " ";
				}
				else if (count == 2)
				{
					std::string valueToken;
					std::stringstream thirdString(secondToken);
					bool state = false;
					while (std::getline(thirdString, valueToken, ','))
					{
						if (state)
							valueArray.add(std::stof(valueToken));
						state = !state;
					}
				}
				if (++count > 2) count = 0;
			}

			dateArray.add(month + year);
		}

		nArray = valueArray;

		plotToAdd.updatePlot(valueArray.getRawDataPointer(), valueArray.size(), true, valueArray);
		plotToAdd.addYDataToPlot(dateArray);
	}

	void interpolateData(float inValue, bool isNormalized, Array<float> nArray, const String& Message)
	{
		if (isNormalized) (inValue = inValue * nArray.size());
		int prevPosition = floor(inValue);
		int nextPosition = ceil(inValue);
		float fraction = inValue - prevPosition;
		float value = (nArray[nextPosition] * fraction) + (nArray[prevPosition] * (1 - fraction));
		float Min, Max;
		findMinAndMax(nArray.getRawDataPointer(), nArray.size(), Min, Max);
		float normFactor = jmax(abs(Min), Max);
		sender->send(Message, (value / normFactor));
	}

	void timerCallback() override
	{
		mainPlot.updateCursor(cursorPosition, true);
		secondPlot.updateCursor(cursorPosition, true);
		thirdPlot.updateCursor(cursorPosition, true);

		interpolateData(cursorPosition, true, firstArray, "/homicidio");
		interpolateData(cursorPosition, true, secondArray, "/suicidio");
		interpolateData(cursorPosition, true, thirdArray, "/transporte");

		date->setText(mainPlot.yDataToPlot[(int)round(cursorPosition * mainPlot.dataSetSize)], dontSendNotification);

		const float cycleTime = (2000 * 60) / 30;

		cursorPosition += 1.0f / (float)cycleTime;

		if (cursorPosition >= (1 - (1.0f / (float)cycleTime) * 5))
		{
			cursorPosition = 0;
			sender->send("/toggle2", 1.0f);
			stopTimer();
			sendChangeMessage();
		}
	}

private:
	std::unique_ptr<Label> date;
	std::unique_ptr<OSCSender> sender;
	Plot mainPlot, secondPlot, thirdPlot;
	Array<float> firstArray, secondArray, thirdArray;
	float cursorPosition = 0;
};
