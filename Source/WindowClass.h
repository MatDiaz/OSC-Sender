/*
  ==============================================================================

    WindowClass.h
    Created: 26 Sep 2019 11:46:49am
    Author:  WATEO

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "RoundedButton.h"

class GenericWindowComponent: public Component,
                             public Button::Listener,
                            public ChangeBroadcaster
{
public:
    GenericWindowComponent(){}
    ~GenericWindowComponent(){}
    
};

class InitialWindow: public ResizableWindow,
public ChangeBroadcaster,
private ChangeListener
{
public:
    InitialWindow (const String& name, bool addToDesktop, GenericWindowComponent* nComponent):
    ResizableWindow (name, addToDesktop)
    {
        setVisible (true);
        setAlwaysOnTop (true);
        toFront(true);
        setBackgroundColour (Colour(Colours::black));
        insideComponent.reset (nComponent);
        
        auto x = Desktop::getInstance().getDisplays().getMainDisplay().userArea.getWidth();
        auto y = Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight();
        insideComponent->setBounds(0,0, x, y);
        insideComponent->addChangeListener(this);
        setContentOwned (insideComponent.get(), true);
    }
    
    ~InitialWindow()
    {
        insideComponent->removeChangeListener(this);
        insideComponent.release();
    }
    
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
        if (source == insideComponent.get())
        {
            sendChangeMessage();
        }
    }
private:
    std::unique_ptr<GenericWindowComponent> insideComponent;
};
