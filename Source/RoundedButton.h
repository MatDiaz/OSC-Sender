/*
  ==============================================================================

    RoundedButton.h
    Created: 12 Oct 2019 12:47:28pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ProjectColours.h"

class RoundedButton: public TextButton
{
public:
    RoundedButton(){}
    ~RoundedButton(){}
    void paint(Graphics& g) override
    {
		juce::Rectangle<int> area = getLocalBounds();
        g.setColour(Colour(uint8(0), uint8(115), uint8(178)));
		g.fillRoundedRectangle(area.toFloat(), ((area.getWidth() + area.getHeight()) / 2) * 0.3);

		g.setColour(projectColours.amarillo);
		g.setFont(Font(((area.getWidth() + area.getHeight()) / 2) * 0.15));
		g.drawText(getButtonText(), area, Justification::centred, false);
    }
private:
    ProjectColours projectColours;
    
};
