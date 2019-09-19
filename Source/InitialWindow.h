/*
  ==============================================================================

    InitialWindow.h
    Created: 18 Sep 2019 9:41:03pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class insideComponent: public Component
{
public:
    insideComponent()
    {
        addAndMakeVisible(titleLabel.get());
        titleLabel.reset (new Label());
        
        addAndMakeVisible(initialButton.get());
        
        addAndMakeVisible(startButton.get());
        
        addAndMakeVisible(sexMenu.get());
        
        addAndMakeVisible(ageMenu.get());
        
        addAndMakeVisible(comunaMenu.get());
    }
    ~insideComponent()
    {
        
    }
    void resized() override
    {
        
    }
    void paint(Graphics& g) override
    {
        
    }
    
private:
    std::unique_ptr<TextButton> initialButton;
    std::unique_ptr<TextButton> startButton;
    std::unique_ptr<ComboBox> sexMenu, ageMenu, comunaMenu;
    std::unique_ptr<Label> titleLabel;
};

class InitialWindow: public ResizableWindow
{
public:
    InitialWindow (const String& name, bool addToDesktop):
    ResizableWindow (name, addToDesktop)
    {
        setVisible(true);
        setAlwaysOnTop(true);
        setBackgroundColour(Colour(Colours::black));
    }
    ~InitialWindow()
    {
        
    }
};
