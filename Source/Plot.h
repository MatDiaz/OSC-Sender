/*
  ==============================================================================

    Plot.h
    Created: 8 Mar 2019 2:23:34pm
    Author:  MATEO

  ==============================================================================
*/

#pragma once

class MainComponent;

#include "../JuceLibraryCode/JuceHeader.h"
#include "DataCursor.h"
#include <cmath>

//==============================================================================
/*
*/

class Plot    : public Component
{
public:
    Plot() // Constructor
    {
        // Create data cursor
        addAndMakeVisible (mainDataCursor);
        
        yDataLabel.reset (new Label());
        addAndMakeVisible (yDataLabel.get());
        yDataLabel->setText (" ------- ", dontSendNotification);
        yDataLabel->setColour (Label::textColourId, Colour (Colours::white));
        yDataLabel->setFont (20.0f);
		yDataLabel->setColour(Label::backgroundColourId, Colour(uint8(169), uint8(169), uint8(169), 0.5f));
        yDataLabel->setJustificationType (Justification::centred);
    }

    ~Plot()
    {
        pointsArray.clearQuick(false);
    }

	void paint(Graphics& g) override
	{
        // Set data to plot colour
		g.setColour (Colour((uint8)220, (uint8)220, (uint8)220, (uint8)15));
        Rectangle<float> area(0, 0, getWidth(), getHeight());
		g.drawRect (0, 0, getWidth(), getHeight(), 2);
        g.fillCheckerBoard (area, getWidth()*0.025, getHeight()*0.05, Colour((uint8)220, (uint8)220, (uint8)220, (uint8)15), Colour((uint8)210, (uint8)210, (uint8)210, (uint8)15));

		if (isLoaded)
		{
			Path dataSetPlot;
			 
			g.setColour(Colour(Colours::white));

			for (auto i = 0; i < dataSetSize; ++i)
			{
				if (!i)
                    dataSetPlot.startNewSubPath (*pointsArray[i]);
				else
                    dataSetPlot.lineTo (*pointsArray[i]);
			}

			g.strokePath (dataSetPlot, PathStrokeType(1.0f));
           
            mainDataCursor.shouldPaint = true;
		}
    }
    // This method will receive data from parent component
	void updatePlot(float* dataSet, int dataSize, bool Loaded)
	{
		isLoaded = Loaded;
		dataSetSize = dataSize;
		dataSetToPlot = dataSet;
        
		float divX = getWidth() / ((float)dataSetSize - 1);

		float Min, Max;

		findMinAndMax(dataSetToPlot, dataSetSize, Min, Max);

		Max = jmax(abs(Min), Max);
        
        float Height = Min < 0 ? getHeight() / 2 : getHeight();

		for (auto i = 0; i < dataSetSize; ++i)
		{
            pointsArray.insert(i, new Point<float>);

			pointsArray[i]->addXY((i * divX), (Height - ((dataSetToPlot[i] / Max) * Height)));
		}
        
		repaint();
	}
    // This should be called when resizing in parent component happens
    // and dataSetToPlot is already in use
    void updatePlot()
    {
        if (dataSetToPlot != nullptr)
        {
            float divX = getWidth() / ((float)dataSetSize - 1);
            
            float Min, Max;
            
            findMinAndMax(dataSetToPlot, dataSetSize, Min, Max);
            
            Max = jmax(abs(Min), Max);
            
            float Height = Min < 0 ? getHeight() / 2 : getHeight();
            
            for (auto i = 0; i < dataSetSize; ++i)
            {
                pointsArray.insert(i, new Point<float>);
                
                pointsArray[i]->addXY((i * divX), (Height - ((dataSetToPlot[i] / Max) * Height)));
            }
            repaint();
        }
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        if(e.mods.isLeftButtonDown() && (e.getPosition().getX() >= 0) && isConnected)
        {
            float position = (((float) e.getPosition().getX() / (float)getWidth()) * dataSetSize);
            // Position va a normalizar la posicion donde se encuentren los datos
            dataValue = position;
            
            interpolatePosition(position);
        }
    }
    // Interpolacion lineal
    void interpolatePosition (float position)
    {
		if (yDataLoaded) { yDataLabel->setText(yDataToPlot[round(position)], dontSendNotification); }

		int prevPosition = (int)floor(position) >= dataSetSize ? dataSetSize - 1 : (int)floor(position);
        
        int nextPosition = (int)ceil(position) >= dataSetSize ? dataSetSize - 1 : (int)ceil(position);
        
        float fraction = position - prevPosition;
        
        int realPositionX = (pointsArray[nextPosition]->getX() * fraction) + (pointsArray[prevPosition]->getX() * (1 - fraction)) - 5;
        int realPositionY = (pointsArray[nextPosition]->getY() * fraction) + (pointsArray[prevPosition]->getY() * (1 - fraction)) - 5;
        
        mainDataCursor.setBounds (realPositionX, realPositionY, 20, 20);
    }
    
    void resized() override
    {
        repaint();
        yDataLabel->setBoundsRelative (0.7f, 0.0f, 0.3f, 0.1f);
    }
    
    void addYDataToPlot (StringArray newDataToPlot)
    {
        yDataToPlot = newDataToPlot;
        yDataLoaded = true;
    }

	OwnedArray<Point<float>> pointsArray;

	bool isLoaded = false, isConnected = false;
    
    float dataValue = 0;

	int dataSetSize = 0;

	float* dataSetToPlot;
    
    std::unique_ptr<Label> yDataLabel;

	DataCursor mainDataCursor;

private:
    
    StringArray yDataToPlot;
    bool yDataLoaded = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plot)
};
