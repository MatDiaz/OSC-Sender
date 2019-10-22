/*
  ==============================================================================

    InitialWindow.h
    Created: 18 Sep 2019 9:41:03pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "WindowClass.h"
#include "ProjectColours.h"

class ComboboxModified : public LookAndFeel_V4
{
public:
	Font getComboBoxFont(ComboBox& /*box*/) override
	{
		return getCommonMenuFont();
	}

	Font getPopupMenuFont() override
	{
		return getCommonMenuFont();
	}

private:
	Font getCommonMenuFont()
	{
		return Font(25.f);
	}
};

class InsideComponent: public GenericWindowComponent
{
public:
	InsideComponent()
	{
		setLookAndFeel(&lookandf);
		getLookAndFeel().setColour(ComboBox::backgroundColourId, Colour(uint8(7), uint8(21), uint8(36)));
		getLookAndFeel().setColour(PopupMenu::backgroundColourId, Colour(uint8(7), uint8(21), uint8(36)));
		getLookAndFeel().setColour(PopupMenu::highlightedBackgroundColourId, projectColours.naranja);
        
		getLookAndFeel().setColour(ComboBox::ColourIds::buttonColourId, Colour(uint8(243), uint8(74), uint8(40)));
        getLookAndFeel().setColour(ComboBox::ColourIds::textColourId, projectColours.amarillo);
		getLookAndFeel().setColour(PopupMenu::ColourIds::textColourId, projectColours.amarillo);

		StringArray sexArray = { "Hombre", "Mujer" };
		StringArray ageArray = {"0 - 5", "6 - 11", "12 - 17", "18 - 28", "29 - 59", String(CharPointer_UTF8 ("60 o m\xc3\xa1s"))};
        StringArray comunaArray = { "Comuna 1: Popular", "Comuna 2: Santa Cruz", "Comuna 3: Manrique", "Comuna 4: Aranjuez", "Comuna 5: Castilla", "Comuna 6: 12 de Octubre", "Comuna 7: Robledo ", "Comuna 8: Villa Hermosa", "Comuna 9: Buenos Aires", "Comuna 10: La Candelaria", "Comuna 11: Laureles - Estadio", String(CharPointer_UTF8 ("Comuna 12: La Am\xc3\xa9rica")), "Comuna 13: San Javier", "Comuna 14: El Poblado", "Comuna 15: Guayabal", String(CharPointer_UTF8 ("Comuna 16: Bel\xc3\xa9n")), CharPointer_UTF8 ("Corregimiento 50: San Sebasti\xc3\xa1n de Palmitas"), "Corregimiento 60: San Cristobal", "Corregimiento 70: Altavista", "Corregimiento 80: San Antonio de Prado", "Corregimiento 90: Santa Elena"};
        StringArray lugaresArray = {"Parque de los sentidos", "Parque del Amor", "Biblioteca de la Floresta", "Parque Biblioteca San Javier"};
		// ============================================================================
		        
        String introMessage = String::fromUTF8(BinaryData::intro_txt, BinaryData::intro_txtSize);
		// =============================================================================
		        
		auto desktopArea = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
		float desktopSize = desktopArea.getWidth() * desktopArea.getHeight();
        
        introText.reset (new Label());
        addAndMakeVisible (introText.get());
        introText->setText (introMessage, dontSendNotification);
        introText->setFont (Font(35.0f));
        introText->setColour (Label::ColourIds::textColourId, projectColours.amarillo);
        introText->setJustificationType(Justification::horizontallyJustified);
        introText->setVisible(true);
        
		initialButton.reset (new RoundedButton());
		addAndMakeVisible (initialButton.get());
		initialButton->setButtonText ("Comenzar");
		initialButton->addListener (this);
		        
		// =============================================================================
		        
        sexMenu.reset (new ComboBox());
        addAndMakeVisible (sexMenu.get());
        sexMenu->addItemList (sexArray, 1);
        sexMenu->setTextWhenNothingSelected ("Selecciona tu sexo");
        sexMenu->setVisible(false);

        ageMenu.reset (new ComboBox());
        addAndMakeVisible (ageMenu.get());
        ageMenu->addItemList (ageArray, 1);
        ageMenu->setTextWhenNothingSelected ("Selecciona tu rango de edad");
        ageMenu->setVisible(false);

        comunaMenu.reset (new ComboBox());
        addAndMakeVisible (comunaMenu.get());
        comunaMenu->addItemList (comunaArray, 1);
        comunaMenu->setTextWhenNothingSelected ("Selecciona tu comuna");
        comunaMenu->setVisible(false);
        
        lugaresMenu.reset (new ComboBox());
        addAndMakeVisible (lugaresMenu.get());
        lugaresMenu->addItemList (lugaresArray, 1);
        lugaresMenu->setTextWhenNothingSelected ("Selecciona donde te encuentras en este momento");
        lugaresMenu->setVisible(false);
        
        initialStates = firstState;
		location = 1;
	}
    
	~InsideComponent()
    {	
		setLookAndFeel(nullptr);
        sexMenu = nullptr;
        ageMenu = nullptr;
        comunaMenu = nullptr;
        introText = nullptr;
        initialButton = nullptr;
    }

	void paint(Graphics& g) override
	{
        juce::Rectangle<int> area = getLocalBounds();
        
        switch (initialStates)
        {
            case firstState:
            {
                backgroundImage = ImageFileFormat::loadFrom (BinaryData::intro_png, BinaryData::intro_pngSize);
                break;
            }
            case secondState:
            {
                backgroundImage = ImageFileFormat::loadFrom (BinaryData::Fondo_png, BinaryData::Fondo_pngSize);
                break;
            }
            case thirdState:
            {
                backgroundImage = ImageFileFormat::loadFrom (BinaryData::Fondo_png, BinaryData::Fondo_pngSize);
            }
            default:
                break;
        }
        
        g.drawImage(backgroundImage, area.toFloat());
	}

	void resized() override
	{
        introText->setBoundsRelative (0.1f, 0.45f, 0.80f, 0.30f);
        initialButton->setBoundsRelative (0.425, 0.8f, 0.15f, 0.075f);
		        
        sexMenu->setBoundsRelative (0.025f, 0.40f, 0.3f, 0.065f);
        ageMenu->setBoundsRelative (0.35f, 0.40f, 0.3f, 0.065f);
        comunaMenu->setBoundsRelative (0.675f, 0.40f, 0.3f, 0.065f);
        lugaresMenu->setBoundsRelative (0.35f, 0.55f, 0.3f, 0.065f);
	}
    
    String getId()
    {
        selectedSex = sexMenu->getSelectedId() == 1 ? "Hombre" : "Mujer";
        
        int ageRank = ageMenu->getSelectedId();
        String ageRankString;
        
        if (ageRank == 1) ageRankString = "(0, 5]";
        else if (ageRank == 2) ageRankString = "(5,11]";
        else if (ageRank == 3) ageRankString = "(11,17]";
        else if (ageRank == 4) ageRankString = "(17,28]";
        else if (ageRank == 5) ageRankString = "(28,59]";
        else if (ageRank == 6) ageRankString = "(59,100]";
        else ageRankString = "(11, 17]";
        
        int comunaRank = comunaMenu->getSelectedId();
        String comunaRankString;
        
        if (comunaRank <= 16) comunaRankString = String(comunaRank) + ".0";
        else if (comunaRank == 17) comunaRankString = "60.0";
        else if (comunaRank == 18) comunaRankString = "70.0";
        else if (comunaRank == 19) comunaRankString = "80.0";
        else if (comunaRank == 20) comunaRankString = "90.0";
        else comunaRankString = "10.0";
     
        return (selectedSex + ageRankString + comunaRankString);
    }

	void buttonClicked(Button* buttonThatWasClicked) override
	{
        if (buttonThatWasClicked == initialButton.get())
        {
            switch (initialStates) {
            case firstState:
                {
                    sexMenu->setVisible (true);
                    ageMenu->setVisible (true);
                    comunaMenu->setVisible (true);
                    lugaresMenu->setVisible (true);
                    String text = String::fromUTF8(BinaryData::datos_txt, BinaryData::datos_txtSize);
                    introText->setFont(45.0f);
                    introText->setBoundsRelative (0.20f, 0.2f, 0.60f, 0.65f);
                    introText->setText(text, dontSendNotification);
                    introText->setJustificationType(Justification::horizontallyCentred);
                    initialButton->setButtonText ("Siguiente");
                    initialStates = states::secondState;
                break;
                }
            case secondState:
                {
                    if (sexMenu->getSelectedId() && ageMenu->getSelectedId() && comunaMenu->getSelectedId() && lugaresMenu->getSelectedId())
                    {
                        sexMenu->setVisible (false);
                        ageMenu->setVisible (false);
                        comunaMenu->setVisible (false);
                        lugaresMenu->setVisible (false);
                        String text = String::fromUTF8(BinaryData::instrucciones_txt, BinaryData::instrucciones_txtSize);
                        introText->setText(text, dontSendNotification);
                        introText->setFont(45.0f);
                        introText->setBoundsRelative (0.2f, 0.3f, 0.60f, 0.4f);
                        selectedId = getId();
                        location = lugaresMenu->getSelectedId();
                        initialStates = states::thirdState;
                    }
                break;
                }
                case thirdState:
                    sendChangeMessage();
                    break;
            default:
                break;
            }
            repaint();
        }
	}
    
    String selectedSex, selectedId;
    int location;
	
private:
	ComboboxModified lookandf;
    std::unique_ptr<RoundedButton> initialButton;
    std::unique_ptr<ComboBox> sexMenu, ageMenu, comunaMenu, lugaresMenu;
    std::unique_ptr<Label> introText;
    ProjectColours projectColours;
    Image backgroundImage;
    enum states {firstState, secondState, thirdState} initialStates;
};

// ===============================================================

