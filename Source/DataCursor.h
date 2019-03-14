/*
  ==============================================================================

    DataCursor.h
    Created: 8 Mar 2019 3:41:08pm
    Author:  MATEO

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DataCursor    : public Component
{
public:

	ComponentBoundsConstrainer constrainer;

    DataCursor()
    {	
		
    }

    ~DataCursor()
    {
    }

    void paint (Graphics& g) override
    {
        if (shouldPaint)
        {
            g.setColour (Colour (Colours::dimgrey));
            g.fillEllipse (0, 0, 15, 15);
			int X = getWidth() * 0.5;
			int Y = getHeight() * 0.5;
        }
    }
  

    void resized() override
    {
    }
    
    bool shouldPaint = false;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataCursor)
};
