/*
  ==============================================================================

    Plot.h
    Created: 8 Mar 2019 2:23:34pm
    Author:  MATEO

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DataCursor.h"

//==============================================================================
/*
*/
class Plot    : public Component
{
public:
    Plot()
    {
        addAndMakeVisible (mainDataCursor);
    }

    ~Plot()
    {
        pointsArray.clearQuick(false);
    }

	void paint(Graphics& g) override
	{
		g.setColour(Colour((uint8)230, (uint8)230, (uint8)230, (uint8)15));

		g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 1.0f);

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
    
    void mouseDrag (const MouseEvent& e) override
    {
        if(e.mods.isLeftButtonDown() && (e.getPosition().getX() >= 0) && isConnected)
        {
            float position = (((float) e.getPosition().getX() / (float)getWidth()) * dataSetSize);

			int prevPosition = (int)floor(position) >= dataSetSize ? dataSetSize - 1 : (int)floor(position);

			int nextPosition = (int)ceil(position) >= dataSetSize ? dataSetSize - 1 : (int)ceil(position);

			float fraction = position - prevPosition;

			int realPositionX = (pointsArray[nextPosition]->getX() * fraction) + (pointsArray[prevPosition]->getX() * (1 - fraction)) - 5;
			int realPositionY = (pointsArray[nextPosition]->getY() * fraction) + (pointsArray[prevPosition]->getY() * (1 - fraction)) - 5;

            setCursorPosition(realPositionX, realPositionY);
        }
    }
    
    void setCursorPosition (int posX, int posY)
    {
        mainDataCursor.setBounds(posX, posY, 20, 20);
    }
    
    void resized() override
    {
        
    }

	OwnedArray<Point<float>> pointsArray;

	bool isLoaded = false, isConnected = false;

	int dataSetSize = 0;

	float* dataSetToPlot;

	DataCursor mainDataCursor;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plot)
};
