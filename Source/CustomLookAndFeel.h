#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "Utility/Identifiers.h"

namespace Gui {
    using namespace juce;

    enum CustomColourIDs
    {
        backgroundColour = 1,
        accentColour,
        voiceColour,
        bassVoiceColour,
        melodyVoiceColour,
        textColour
    };

    class CustomLookAndFeel : public LookAndFeel_V4
    {
    public:
        CustomLookAndFeel()
                : storedColours(IDs::ColourTheme)
        {
            // Background colour alternatives:
            // 0xFF212121 - pure grey
            // 0xFF27282C - neutral-ish grey
            // 0xFF26292B - more green
            // 0xFF28262B - more purple
            // 0xFF26272B - more more blue

            setColour(TextEditor::textColourId, Colours::white);

            InitialiseColours();

            // setDefaultSansSerifTypefaceName("Consolas");
        }
        static Colour getAccentColour() { return Colour(0xFFDAA520); }
        static Colour getCustomBackgroundColour() { return Colour(0xFF212121); }

    private:
        ValueTree storedColours;

    public:
        //Mostly copied from LookAndFeel_V4
        void InitialiseColours(bool recreateTheme = false)
        {


            //If we have a default theme file in the right location, init with that. Else, create a new default theme
            /*if(const auto defaultTheme = AppState::getInstance()->getDefaultTheme(); defaultTheme.existsAsFile() && !recreateTheme) {
                SetThemeFromFile(defaultTheme);
                return;
            }*/

            Colour accentColour = Colour(0xFFDAA520);
            Colour backgroundColour = Colour(0xFF212121);
            Colour textColour = Colour(0xFFFFFFFF);

            auto customColourScheme = getCurrentColourScheme();
            customColourScheme.setUIColour(ColourScheme::windowBackground, backgroundColour);
            customColourScheme.setUIColour(ColourScheme::widgetBackground, backgroundColour);
            customColourScheme.setUIColour(ColourScheme::menuBackground, backgroundColour);
            customColourScheme.setUIColour(ColourScheme::outline, Colours::transparentBlack);
            customColourScheme.setUIColour(ColourScheme::defaultText, textColour);
            customColourScheme.setUIColour(ColourScheme::defaultFill, accentColour);
            //currentColourScheme.setUIColour(ColourScheme::highlightedText, backgroundColour);
            //currentColourScheme.setUIColour(ColourScheme::hightlightedFill, backgroundColour);
            customColourScheme.setUIColour(ColourScheme::menuText, textColour);

            auto transparent = Colours::transparentBlack;

            storedColours = ValueTree(IDs::ColourTheme); //Resetting the value tree before adding colours
            {
                appendColour(createVTColour("CustomColourIDs::backgroundColour",CustomColourIDs::backgroundColour, backgroundColour.toString()));
                appendColour(createVTColour("CustomColourIDs::accentColour",CustomColourIDs::accentColour, accentColour.toString()));
                appendColour(createVTColour("CustomColourIDs::textColour",CustomColourIDs::textColour, textColour.toString()));
                appendColour(createVTColour("CustomColourIDs::bassVoiceColour",CustomColourIDs::bassVoiceColour, Colour(0xFF57A461).toString()));
                appendColour(createVTColour("CustomColourIDs::voiceColour",CustomColourIDs::voiceColour, Colour(0xFF747DDE).toString()));
                appendColour(createVTColour("CustomColourIDs::melodyVoiceColour",CustomColourIDs::melodyVoiceColour, Colour(0xFFAD8C39).toString()));

                appendColour(createVTColour("TextButton::buttonColourId",TextButton::buttonColourId, backgroundColour.brighter(0.1f).toString()));
                appendColour(createVTColour("TextButton::buttonOnColourId",TextButton::buttonOnColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).toString()));
                appendColour(createVTColour("TextButton::textColourOnId",TextButton::textColourOnId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedText).toString()));
                appendColour(createVTColour("TextButton::textColourOffId",TextButton::textColourOffId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("ToggleButton::textColourId",ToggleButton::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("ToggleButton::tickColourId", ToggleButton::tickColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("ToggleButton::tickDisabledColourId", ToggleButton::tickDisabledColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).withAlpha(0.5f).toString()));

                appendColour(createVTColour("TextEditor::backgroundColourId", TextEditor::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("TextEditor::textColourId", TextEditor::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("TextEditor::highlightColourId", TextEditor::highlightColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).withAlpha(0.4f).toString()));
                appendColour(createVTColour("TextEditor::highlightedTextColourId", TextEditor::highlightedTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedText).toString()));
                appendColour(createVTColour("TextEditor::outlineColourId", TextEditor::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("TextEditor::focusedOutlineColourId", TextEditor::focusedOutlineColourId, accentColour.toString()));
                appendColour(createVTColour("TextEditor::shadowColourId", TextEditor::shadowColourId, transparent.toString()));

                appendColour(createVTColour("CaretComponent::caretColourId", CaretComponent::caretColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).toString()));

                appendColour(createVTColour("Label::backgroundColourId", Label::backgroundColourId, transparent.toString()));
                appendColour(createVTColour("Label::textColourId", Label::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("Label::outlineColourId", Label::outlineColourId, transparent.toString()));
                appendColour(createVTColour("Label::textWhenEditingColourId", Label::textWhenEditingColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("ScrollBar::backgroundColourId", ScrollBar::backgroundColourId, transparent.toString()));
                appendColour(createVTColour("ScrollBar::thumbColourId", ScrollBar::thumbColourId, accentColour.toString()));
                appendColour(createVTColour("ScrollBar::trackColourId", ScrollBar::trackColourId, transparent.toString()));

                appendColour(createVTColour("TreeView::linesColourId", TreeView::linesColourId, transparent.toString()));
                appendColour(createVTColour("TreeView::backgroundColourId", TreeView::backgroundColourId, transparent.toString()));
                appendColour(createVTColour("TreeView::dragAndDropIndicatorColourId", TreeView::dragAndDropIndicatorColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("TreeView::selectedItemBackgroundColourId", TreeView::selectedItemBackgroundColourId, transparent.toString()));
                appendColour(createVTColour("TreeView::oddItemsColourId", TreeView::oddItemsColourId, transparent.toString()));
                appendColour(createVTColour("TreeView::evenItemsColourId", TreeView::evenItemsColourId, transparent.toString()));

                appendColour(createVTColour("PopupMenu::backgroundColourId", PopupMenu::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuBackground).toString()));
                appendColour(createVTColour("PopupMenu::textColourId", PopupMenu::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuText).toString()));
                appendColour(createVTColour("PopupMenu::headerTextColourId", PopupMenu::headerTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuText).toString()));
                appendColour(createVTColour("PopupMenu::highlightedTextColourId", PopupMenu::highlightedTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedText).toString()));
                appendColour(createVTColour("PopupMenu::highlightedBackgroundColourId", PopupMenu::highlightedBackgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).toString()));

                appendColour(createVTColour("ComboBox::buttonColourId",ComboBox::buttonColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("ComboBox::outlineColourId",ComboBox::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("ComboBox::textColourId",ComboBox::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("ComboBox::backgroundColourId",ComboBox::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("ComboBox::arrowColourId",ComboBox::arrowColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("ComboBox::focusedOutlineColourId",ComboBox::focusedOutlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("PropertyComponent::backgroundColourId", PropertyComponent::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("PropertyComponent::labelTextColourId", PropertyComponent::labelTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("TextPropertyComponent::backgroundColourId",TextPropertyComponent::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("TextPropertyComponent::textColourId",TextPropertyComponent::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("TextPropertyComponent::outlineColourId",TextPropertyComponent::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("BooleanPropertyComponent::backgroundColourId",BooleanPropertyComponent::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("BooleanPropertyComponent::outlineColourId",BooleanPropertyComponent::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("ListBox::backgroundColourId",ListBox::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("ListBox::outlineColourId",ListBox::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("ListBox::textColourId",ListBox::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("Slider::backgroundColourId",Slider::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("Slider::thumbColourId",Slider::thumbColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).toString()));
                appendColour(createVTColour("Slider::trackColourId",Slider::trackColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).brighter(0.1f).toString()));
                appendColour(createVTColour("Slider::rotarySliderFillColourId",Slider::rotarySliderFillColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).toString()));
                appendColour(createVTColour("Slider::rotarySliderOutlineColourId",Slider::rotarySliderOutlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("Slider::textBoxTextColourId",Slider::textBoxTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("Slider::textBoxBackgroundColourId",Slider::textBoxBackgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).withAlpha(0.0f).toString()));
                appendColour(createVTColour("Slider::textBoxHighlightColourId",Slider::textBoxHighlightColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).withAlpha(0.4f).toString()));
                appendColour(createVTColour("Slider::textBoxOutlineColourId",Slider::textBoxOutlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("ResizableWindow::backgroundColourId", ResizableWindow::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::windowBackground).toString()));

                appendColour(createVTColour("DocumentWindow::textColourId", DocumentWindow::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("AlertWindow::backgroundColourId",AlertWindow::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("AlertWindow::textColourId",AlertWindow::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("AlertWindow::outlineColourId",AlertWindow::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("ProgressBar::backgroundColourId", ProgressBar::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("ProgressBar::foregroundColourId", ProgressBar::foregroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).toString()));

                appendColour(createVTColour("TooltipWindow::backgroundColourId",TooltipWindow::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).toString()));
                appendColour(createVTColour("TooltipWindow::textColourId",TooltipWindow::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedText).toString()));
                appendColour(createVTColour("TooltipWindow::outlineColourId",TooltipWindow::outlineColourId, transparent.toString()));

                appendColour(createVTColour("TabbedComponent::backgroundColourId",TabbedComponent::backgroundColourId, transparent.toString()));
                appendColour(createVTColour("TabbedComponent::outlineColourId",TabbedComponent::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("TabbedButtonBar::tabOutlineColourId",TabbedButtonBar::tabOutlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).withAlpha(0.5f).toString()));
                appendColour(createVTColour("TabbedButtonBar::frontOutlineColourId",TabbedButtonBar::frontOutlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("Toolbar::backgroundColourId",Toolbar::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).withAlpha(0.4f).toString()));
                appendColour(createVTColour("Toolbar::separatorColourId",Toolbar::separatorColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("Toolbar::buttonMouseOverBackgroundColourId",Toolbar::buttonMouseOverBackgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).contrasting(0.2f).toString()));
                appendColour(createVTColour("Toolbar::buttonMouseDownBackgroundColourId",Toolbar::buttonMouseDownBackgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).contrasting(0.5f).toString()));
                appendColour(createVTColour("Toolbar::labelTextColourId",Toolbar::labelTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("Toolbar::editingModeOutlineColourId",Toolbar::editingModeOutlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("DrawableButton::textColourId",DrawableButton::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("DrawableButton::textColourOnId",DrawableButton::textColourOnId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedText).toString()));
                appendColour(createVTColour("DrawableButton::backgroundColourId",DrawableButton::backgroundColourId, transparent.toString()));
                appendColour(createVTColour("DrawableButton::backgroundOnColourId",DrawableButton::backgroundOnColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).toString()));

                appendColour(createVTColour("HyperlinkButton::textColourId", HyperlinkButton::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).interpolatedWith(Colours::blue, 0.4f).toString()));

                appendColour(createVTColour("GroupComponent::outlineColourId", GroupComponent::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));
                appendColour(createVTColour("GroupComponent::textColourId", GroupComponent::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("BubbleComponent::backgroundColourId", BubbleComponent::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("BubbleComponent::outlineColourId", BubbleComponent::outlineColourId, customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("DirectoryContentsDisplayComponent::highlightColourId", DirectoryContentsDisplayComponent::highlightColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).toString()));
                appendColour(createVTColour("DirectoryContentsDisplayComponent::textColourId", DirectoryContentsDisplayComponent::textColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuText).toString()));
                appendColour(createVTColour("DirectoryContentsDisplayComponent::highlightedTextColourId", DirectoryContentsDisplayComponent::highlightedTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::highlightedText).toString()));

                appendColour(createVTColour("LassoComponent::lassoFillColourId", 0x1000440,customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).toString()));
                appendColour(createVTColour("LassoComponent::lassoOutlineColourId", 0x1000441,customColourScheme.getUIColour(ColourScheme::UIColour::outline).toString()));

                appendColour(createVTColour("MidiKeyboardComponent::whiteNoteColourId",MidiKeyboardComponent::whiteNoteColourId, backgroundColour.brighter().toString()));
                appendColour(createVTColour("MidiKeyboardComponent::blackNoteColourId",MidiKeyboardComponent::blackNoteColourId, Colour(0xff000000).toString()));
                appendColour(createVTColour("MidiKeyboardComponent::keySeparatorLineColourId",MidiKeyboardComponent::keySeparatorLineColourId, Colour(0x66000000).toString()));
                appendColour(createVTColour("MidiKeyboardComponent::mouseOverKeyOverlayColourId",MidiKeyboardComponent::mouseOverKeyOverlayColourId, accentColour.toString()));
                appendColour(createVTColour("MidiKeyboardComponent::keyDownOverlayColourId",MidiKeyboardComponent::keyDownOverlayColourId, accentColour.toString()));
                appendColour(createVTColour("MidiKeyboardComponent::textLabelColourId",MidiKeyboardComponent::textLabelColourId, Colour(0xff000000).toString()));
                appendColour(createVTColour("MidiKeyboardComponent::upDownButtonBackgroundColourId",MidiKeyboardComponent::upDownButtonBackgroundColourId, Colour(0xffd3d3d3).toString()));
                appendColour(createVTColour("MidiKeyboardComponent::upDownButtonArrowColourId",MidiKeyboardComponent::upDownButtonArrowColourId, Colour(0xff000000).toString()));
                appendColour(createVTColour("MidiKeyboardComponent::shadowColourId",MidiKeyboardComponent::shadowColourId, accentColour.toString()));

                appendColour(createVTColour("CodeEditorComponent::backgroundColourId",0x1004500,customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("CodeEditorComponent::highlightColourId",0x1004502,customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).withAlpha(0.4f).toString()));
                appendColour(createVTColour("CodeEditorComponent::defaultTextColourId",0x1004503,customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("CodeEditorComponent::lineNumberBackgroundId",0x1004504,customColourScheme.getUIColour(ColourScheme::UIColour::highlightedFill).withAlpha(0.5f).toString()));
                appendColour(createVTColour("CodeEditorComponent::lineNumberTextId",0x1004505,customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).toString()));

                appendColour(createVTColour("ColourSelector::backgroundColourId",0x1007000,customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("ColourSelector::labelTextColourId",0x1007001,customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("KeyMappingEditorComponent::backgroundColourId",0x100ad00,customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("KeyMappingEditorComponent::textColourId",0x100ad01,customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("FileSearchPathListComponent::backgroundColourId", FileSearchPathListComponent::backgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuBackground).toString()));

                appendColour(createVTColour("FileChooserDialogBox::titleTextColourId", FileChooserDialogBox::titleTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));

                appendColour(createVTColour("SidePanel::backgroundColour",SidePanel::backgroundColour, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).toString()));
                appendColour(createVTColour("SidePanel::titleTextColour",SidePanel::titleTextColour, customColourScheme.getUIColour(ColourScheme::UIColour::defaultText).toString()));
                appendColour(createVTColour("SidePanel::shadowBaseColour",SidePanel::shadowBaseColour, customColourScheme.getUIColour(ColourScheme::UIColour::widgetBackground).darker().toString()));
                appendColour(createVTColour("SidePanel::dismissButtonNormalColour",SidePanel::dismissButtonNormalColour, customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).toString()));
                appendColour(createVTColour("SidePanel::dismissButtonOverColour",SidePanel::dismissButtonOverColour, customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).darker().toString()));
                appendColour(createVTColour("SidePanel::dismissButtonDownColour",SidePanel::dismissButtonDownColour, customColourScheme.getUIColour(ColourScheme::UIColour::defaultFill).brighter().toString()));

                appendColour(createVTColour("FileBrowserComponent::currentPathBoxBackgroundColourId",FileBrowserComponent::currentPathBoxBackgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuBackground).toString()));
                appendColour(createVTColour("FileBrowserComponent::currentPathBoxTextColourId",FileBrowserComponent::currentPathBoxTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuText).toString()));
                appendColour(createVTColour("FileBrowserComponent::currentPathBoxArrowColourId",FileBrowserComponent::currentPathBoxArrowColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuText).toString()));
                appendColour(createVTColour("FileBrowserComponent::filenameBoxBackgroundColourId",FileBrowserComponent::filenameBoxBackgroundColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuBackground).toString()));
                appendColour(createVTColour("FileBrowserComponent::filenameBoxTextColourId",FileBrowserComponent::filenameBoxTextColourId, customColourScheme.getUIColour(ColourScheme::UIColour::menuText).toString()));
            }

            for(int i = 0; i < storedColours.getNumChildren(); i++) {
                setColourFromVT(storedColours.getChild(i));
            }
            const auto coloursAsXml = storedColours.createXml();

            /*if(auto themesFolder = AppState::getInstance()->getThemePath(); themesFolder.exists()) {
                auto generatedTheme = themesFolder.getChildFile("DefaultTheme (auto-generated).xml");
                generatedTheme.create();
                coloursAsXml->writeTo(generatedTheme);
                return;
            }
                //TODO - prompt the user for a valid themes location at this point...
            jassertfalse; //if we hit this, it means that we couldn't store the default theme in a valid location*/
        }

        bool SetThemeFromFile(const File& path) 	{
            if (path.existsAsFile()) {

                auto coloursVT = ValueTree::fromXml(path.loadFileAsString());


                if(coloursVT.isValid() && coloursVT.getType() == IDs::ColourTheme) {

                    storedColours = coloursVT;
                    for(int i = 0; i < storedColours.getNumChildren(); i++) {
                        setColourFromVT(storedColours.getChild(i));
                    }
                }
                return true;
            }
            return false;
        }
        void storeColour(int id, Colour colour)
        {
            auto storedColour = storedColours.getChildWithProperty(IDs::id, id);
            if(storedColour.isValid()) {
                storedColour.setProperty(IDs::colour, colour.toString(), nullptr);
                setColour(id, colour);
            }
        }
        bool SaveThemeToFile(const File& path)
        {
            auto coloursAsXml = storedColours.createXml();
            File testFile(path);
            testFile.create();
            return coloursAsXml->writeTo(testFile);
        }

    private:

        //Creates a ValueTree element that can be stored
        static ValueTree createVTColour(String name, int64 id, String colour)
        {
            return ValueTree(IDs::COLOUR, { {IDs::id,id}, {IDs::name, name}, {IDs::colour, colour} });
        };

        //Helper method for appending a colour to the CustomLookAndFeel
        void appendColour(ValueTree colour)
        {
            storedColours.appendChild(colour, nullptr);
        }

        //Helper method for setting a colour
        void setColourFromVT(ValueTree colourVT)
        {
            int id = colourVT.getProperty(IDs::id);
            Colour colour = Colour::fromString(colourVT.getProperty(IDs::colour).toString());
            setColour(id, colour);
        }
    };
}