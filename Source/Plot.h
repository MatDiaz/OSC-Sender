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

class Plot	: public Component
{
public:
    Plot(): // Constructor
		dataSetToPlot(nullptr) 
    {
        // Create data cursor
        addAndMakeVisible (mainDataCursor);
        
        xDataLabel.reset (new Label());
        addAndMakeVisible (xDataLabel.get());
        xDataLabel->setText (" ", dontSendNotification);
        xDataLabel->setColour (Label::textColourId, Colour (Colours::white));
        xDataLabel->setFont (20.0f);
        xDataLabel->setJustificationType (Justification::centred);
		xDataLabel->setEnabled(false);
        xDataLabel->setVisible(false);

		yDataLabel.reset (new Label());
		addAndMakeVisible (yDataLabel.get());
		yDataLabel->setColour (Label::textColourId, Colour(Colours::black));
		yDataLabel->setFont (50.0f);
		yDataLabel->setJustificationType (Justification::centred);

		plotNameLabel.reset (new Label());
		addAndMakeVisible(plotNameLabel.get());
		plotNameLabel->setColour (Label::textColourId, Colour(Colours::white));
		plotNameLabel->setFont (40.0f);
		plotNameLabel->setJustificationType (Justification::centred);
    }

    ~Plot()
    {	
		xDataLabel = nullptr;
		yDataLabel = nullptr;
		plotNameLabel = nullptr;
        pointsArray.clearQuick(false);
    }

	void paint (Graphics& g) override
	{
        // Set data to plot colour
        if (!imageCreated)
        {
            g.setColour (Colour((uint8)220, (uint8)220, (uint8)220, (uint8)15));
            Rectangle<float> area(0, 0, getWidth(), getHeight());
            g.drawRect (0, 0, getWidth(), getHeight(), 2);
			g.setColour (backgroundColour);
			g.fillRect (getLocalBounds().toFloat());
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
                
                g.strokePath (dataSetPlot, PathStrokeType(2.0f));
                
                mainDataCursor.shouldPaint = true;
            }
        }
        else
        {
            g.drawImageAt (backgroundImage, 0, 0, false);
        }
    }

    // This method will receive data from parent component
	void updatePlot (float* dataSet, int dataSize, bool Loaded, const Array<float>& ndataArray)
	{
		isLoaded = Loaded;
		dataSetSize = dataSize;
		dataSetToPlot = dataSet;
		dataArray = ndataArray;
        
		float divX = (getWidth() * (1 - offset)) / ((float) dataSetSize - 1);
		float Xoffset = getWidth() * offset;

		float Min, Max;

		findMinAndMax (dataSetToPlot, dataSetSize, Min, Max);

        Max = jmax(std::abs(Min), Max);
        
        float Height = Min < 0 ? getHeight() / 2 : getHeight();

		for (auto i = 0; i < dataSetSize; ++i)
		{
            pointsArray.insert(i, new Point<float>);

			pointsArray[i]->addXY((i * divX) + Xoffset, (Height - ((dataSetToPlot[i] / Max) * Height)));
		}
        
        repaint();
        
        if (!imageCreated)
        {
            xDataLabel->setVisible(false);
			yDataLabel->setVisible(false);
            backgroundImage = createComponentSnapshot(getLocalBounds());
            imageCreated = true;
            xDataLabel->setVisible(true);
			yDataLabel->setVisible(true);
        }
	}

    // This should be called when resizing in parent component happens
    // and dataSetToPlot is already in use
    void updatePlot()
    {
        if (dataSetToPlot != nullptr)
        {
            float divX = getWidth() / ((float)dataSetSize - 1);
            
            float Min, Max;
            
            findMinAndMax (dataSetToPlot, dataSetSize, Min, Max);
            
            Max = jmax (std::abs(Min), Max);
            
            float Height = Min < 0 ? getHeight() / 2 : getHeight();
            
            for (auto i = 0; i < dataSetSize; ++i)
            {
                pointsArray.insert(i, new Point<float>);
                
                pointsArray[i]->addXY((i * divX), (Height - ((dataSetToPlot[i] / Max) * Height)));
            }
            repaint();
            
            if (!imageCreated)
            {
                xDataLabel->setVisible(false);
				yDataLabel->setVisible(false);
                backgroundImage = createComponentSnapshot(getLocalBounds());
                imageCreated = true;
                xDataLabel->setVisible(true);
				yDataLabel->setVisible(true);
            }
        }
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        if(e.mods.isLeftButtonDown() && (e.getPosition().getX() >= 0) && isEnabled())
        {
            float position = (((float) e.getPosition().getX() / (float)getWidth()) * dataSetSize);
            // Position va a normalizar la posicion donde se encuentren los datos
            dataValue = position;
            
            updateCursor (position);
        }
    }

    void updateCursor (float position, const bool isNormalized = false)
    {
        position = isNormalized ? (position * dataSetSize) : position;

		yDataLabel->setText (String (dataArray [(int)round(position)]), dontSendNotification);

		int prevPosition = (int)floor(position) >= dataSetSize ? dataSetSize - 1 : (int)floor(position);
        
        int nextPosition = (int)ceil(position) >= dataSetSize ? dataSetSize - 1 : (int)ceil(position);
        
        float fraction = position - prevPosition;
        
        int realPositionX = (pointsArray[nextPosition]->getX() * fraction) + (pointsArray[prevPosition]->getX() * (1 - fraction)) - 7.5;
        int realPositionY = (pointsArray[nextPosition]->getY() * fraction) + (pointsArray[prevPosition]->getY() * (1 - fraction)) - 7.5;
        
        mainDataCursor.setBounds (realPositionX, 0, 7.5, getLocalBounds().getHeight());
    }

    void resized() override
    {
        repaint();
        xDataLabel->setBoundsRelative (0.7f, 0.9f, 0.3f, 0.1f);
		yDataLabel->setBoundsRelative(0.0f, 0.5f, 0.15f, 0.5f);
		plotNameLabel->setBoundsRelative (0.0f, 0.0f, 0.15f, 0.6f);
    }
    
    void addYDataToPlot (StringArray newDataToPlot)
    {
        yDataToPlot = newDataToPlot;
        yDataLoaded = true;
    }

	void setBackgroundColour(const Colour& colourToChange)
	{
		backgroundColour = colourToChange;
	}

	void setOffset(float setCurrentOffset)
	{
		offset = setCurrentOffset;
	}

	void setPlotName(const String& nameToAdd)
	{
		plotNameLabel->setText (nameToAdd, dontSendNotification);
	}

	OwnedArray<Point<float>> pointsArray;

	bool isLoaded = false, isConnected = false;
    
    float dataValue = 0;

	int dataSetSize = 0;

	float* dataSetToPlot;
	StringArray yDataToPlot;

private:
   
	std::unique_ptr<Label> xDataLabel, yDataLabel, plotNameLabel;

	float offset = 0;
	Colour backgroundColour = { Colours::black };
	Array<float> dataArray;
	Image backgroundImage;
    DataCursor mainDataCursor;
    bool yDataLoaded = false;
    bool imageCreated = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plot)
};
