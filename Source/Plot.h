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
		g.setColour(Colour((uint8)250, (uint8)250, (uint8)250, (uint8)35));

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

			g.strokePath (dataSetPlot, PathStrokeType(2.0f));
           
            mainDataCursor.shouldPaint = true;
		}
    }

	void updatePlot(float* dataSet, int dataSize, bool Loaded)
	{
		isLoaded = Loaded;
		dataSetSize = dataSize;
		dataSetToPlot = dataSet;

		float divX = getWidth() / (dataSetSize - 1);

		float Min, Max;

		findMinAndMax(dataSetToPlot, dataSetSize, Min, Max);

		auto height = Min < 0 ? getHeight() / 2 : getHeight();

		for (auto i = 0; i < dataSetSize; ++i)
		{
			pointsArray.add(new Point<float>);

			pointsArray[i]->addXY((i * divX) * 1.01, height - ((dataSetToPlot[i] / Max) * height));
		}

		repaint();
	}
    
    void mouseDown (const MouseEvent& e) override
    {
        
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        if(e.mods.isLeftButtonDown())
        {
            float position = (((float) e.getPosition().getX() / (float)getWidth()) * dataSetSize);

			int prevPosition = floor(position);
			int nextPosition = ceil(position);
			float fraction = position - prevPosition;

			float realPositionX = (pointsArray[nextPosition]->getX() * fraction) + (pointsArray[prevPosition]->getX() * (1 - fraction)) - 5;
			float realPositionY = (pointsArray[nextPosition]->getY() * fraction) + (pointsArray[prevPosition]->getY() * (1 - fraction)) - 5;

            mainDataCursor.setBounds(realPositionX, realPositionY, 10, 10);
        }
    }
    
    void resized() override
    {
        
    }

	OwnedArray<Point<float>> pointsArray;

	bool isLoaded = false;

	int dataSetSize = 0;

	float* dataSetToPlot;

	DataCursor mainDataCursor;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plot)
};
