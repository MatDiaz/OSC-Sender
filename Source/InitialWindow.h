/*
  ==============================================================================

    InitialWindow.h
    Created: 18 Sep 2019 9:41:03pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class InsideComponent: public Component,
                       public Button::Listener
{
public:
    InsideComponent()
    {
        StringArray sexArray = {"Hombre", "Mujer"};
        StringArray ageArray = {"15 - 20", "21 - 25", "26 - 30", "31 - 35"};
        StringArray comunaArray = { "Comuna 1", "Comuna 2", "Comuna 3", "Comuna 4" };
        
        // =============================================================================
        
        addAndMakeVisible (titleLabel.get());
        titleLabel.reset (new Label());
        titleLabel->setText ("!Escucha!", dontSendNotification);
        
        addAndMakeVisible (initialButton.get());
        initialButton.reset (new TextButton());
        initialButton->setButtonText ("Comenzar");
        initialButton->addListener (this);
        
        addAndMakeVisible (startButton.get());
        startButton.reset (new TextButton());
        startButton->setButtonText ("Iniciar");
        startButton->addListener (this);
        startButton->setEnabled (false);
        startButton->setVisible(false);
        
        // =============================================================================
        
        addAndMakeVisible (sexMenu.get());
        sexMenu.reset (new ComboBox());
        sexMenu->addItemList (sexArray, 1);
        
        addAndMakeVisible (ageMenu.get());
        ageMenu.reset (new ComboBox());
        ageMenu->addItemList (ageArray, 2);
        
        addAndMakeVisible (comunaMenu.get());
        comunaMenu.reset (new ComboBox());
        comunaMenu->addItemList (comunaArray, 3);
        
        // =============================================================================
    }
    
    ~InsideComponent()
    {
        initialButton = nullptr;
        startButton = nullptr;
        sexMenu = nullptr;
        ageMenu = nullptr;
        comunaMenu = nullptr;
        titleLabel = nullptr;
    }
    
    void resized() override
    {
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll (Colour(Colours::grey));
    }
    
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        if (buttonThatWasClicked == initialButton.get())
        {
            
        }
        else if (buttonThatWasClicked == startButton.get())
        {
            
        }
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
        setVisible (true);
        setAlwaysOnTop (true);
        setBackgroundColour (Colour(Colours::black));
        
        insideComponent.setBounds(0, 0, 1000, 500);
        setContentOwned (&insideComponent, true);
    }
    ~InitialWindow()
    {
        
    }
    
    InsideComponent insideComponent;
};
