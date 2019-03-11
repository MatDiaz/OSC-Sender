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
            g.fillEllipse (0, 0, 10, 10);
        }
    }
    
    void mouseDown (const MouseEvent& e) override
    {
//        myDragger.startDraggingComponent(this, e);
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
//        myDragger.dragComponent(this, e, nullptr);
    }

    void resized() override
    {
    }
    
    bool shouldPaint = false;
    
private:
    ComponentDragger myDragger;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataCursor)
};
