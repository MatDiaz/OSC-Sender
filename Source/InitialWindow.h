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
                       public Button::Listener,
                       public ChangeBroadcaster
{
public:
    InsideComponent()
    {
        StringArray sexArray = {"Hombre", "Mujer"};
        StringArray ageArray = {"15 - 20", "21 - 25", "26 - 30", "31 - 35"};
        StringArray comunaArray = { "Comuna 1", "Comuna 2", "Comuna 3", "Comuna 4" };
        // ============================================================================
        
        String introMessage = String::createStringFromData(BinaryData::TextEx_txt, BinaryData::TextEx_txtSize);
        // =============================================================================
        
        auto desktopArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
        float desktopSize = desktopArea.getWidth() * desktopArea.getHeight();
        
        titleLabel.reset (new Label());
        addAndMakeVisible (titleLabel.get());
        titleLabel->setText ("!Escucha!", dontSendNotification);
		titleLabel->setFont (Font(desktopSize * 0.0002));
		titleLabel->setColour (Label::ColourIds::textColourId, Colour(Colours::white));
        titleLabel->setJustificationType(Justification::centred);
        
        introText.reset (new Label());
        addAndMakeVisible (introText.get());
        introText->setText (introMessage, dontSendNotification);
        introText->setFont (Font(desktopSize * 0.00001));
        introText->setColour (Label::ColourIds::textColourId, Colour(Colours::white));
        introText->setJustificationType(Justification::horizontallyJustified);
        
        initialButton.reset (new TextButton());
        addAndMakeVisible (initialButton.get());
        initialButton->setButtonText ("Comenzar!");
        initialButton->addListener (this);
        
        // =============================================================================
        
        sexMenu.reset (new ComboBox());
        addAndMakeVisible (sexMenu.get());
        sexMenu->addItemList (sexArray, 1);
        sexMenu->setTextWhenNothingSelected ("Selecciona tu sexo");
        sexMenu->setVisible(false);
        
        ageMenu.reset (new ComboBox());
        addAndMakeVisible (ageMenu.get());
        ageMenu->addItemList (ageArray, 2);
        ageMenu->setTextWhenNothingSelected ("Seleccionad tu rango de edad");
        ageMenu->setVisible(false);
        
        comunaMenu.reset (new ComboBox());
        addAndMakeVisible (comunaMenu.get());
        comunaMenu->addItemList (comunaArray, 3);
        comunaMenu->setTextWhenNothingSelected ("Selecciona tu comuna");
        comunaMenu->setVisible(false);
        
        initialStates = states::firstState;
        
        // =============================================================================
    }
    
    ~InsideComponent()
    {
        sexMenu = nullptr;
        ageMenu = nullptr;
        comunaMenu = nullptr;
        titleLabel = nullptr;
        introText = nullptr;
    }
    
    void resized() override
    {
		titleLabel->setBoundsRelative (0.0f, 0.0f, 1.0f, 0.5f);
        introText->setBoundsRelative (0.1f, 0.5f, 0.8f, 0.25f);
        initialButton->setBoundsRelative (0.45f, 0.75f, 0.1f, 0.1f);
        
        sexMenu->setBoundsRelative (0.1f, 0.5f, 0.2f, 0.05f);
        ageMenu->setBoundsRelative (0.4f, 0.5f, 0.2f, 0.05f);
        comunaMenu->setBoundsRelative (0.7f, 0.5f, 0.2f, 0.05f);
    }
    
    void paint(Graphics& g) override
    {

    }
    
    void buttonClicked (Button* buttonThatWasClicked) override
    {
        if (buttonThatWasClicked == initialButton.get())
        {
            switch (initialStates) {
            case firstState:
                    initialStates = states::secondState;
                    sexMenu->setVisible (true);
                    ageMenu->setVisible (true);
                    comunaMenu->setVisible (true);
                    introText = nullptr;
                break;
            case secondState:
                    sendChangeMessage();
                break;
            default:
                break;
            }
        }
    }
    
private:
    std::unique_ptr<TextButton> initialButton;
    std::unique_ptr<ComboBox> sexMenu, ageMenu, comunaMenu;
    std::unique_ptr<Label> titleLabel, introText;
    enum states {firstState, secondState};
    states initialStates;
};

class InitialWindow: public ResizableWindow,
                    private ChangeListener
{
public:
    InitialWindow (const String& name, bool addToDesktop):
    ResizableWindow (name, addToDesktop)
    {
        setVisible (true);
        setAlwaysOnTop (true);
        setBackgroundColour (Colour(Colours::black));
        
        insideComponent.setBounds(Desktop::getInstance().getDisplays().getMainDisplay().userArea);
        insideComponent.addChangeListener(this);
        setContentOwned (&insideComponent, true);
    }
    
    ~InitialWindow()
    {
        insideComponent.removeChangeListener(this);
    }
    
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
        if (source == &insideComponent)
        {
            delete this;
        }
    }
    
    InsideComponent insideComponent;
};
