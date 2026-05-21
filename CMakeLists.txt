cmake_minimum_required(VERSION 3.22)

# Projektname definieren
project(AUDIO_PLUGIN_TEMPLATE VERSION 0.0.1)

# JUCE Verzeichnis einbinden
add_subdirectory(JUCE)

# Das eigentliche Plugin erstellen
juce_add_plugin(MeinAudioPlugin
        # Hier deine Formate wählen (z.B. VST3, AU, Standalone)
        FORMATS VST3 Standalone
        PRODUCT_NAME "Mein Audio Plugin"
)

# C++ Standard festlegen
target_compile_features(MeinAudioPlugin PRIVATE cxx_std_20)

# Source Files hinzufügen (erstellen wir im nächsten Schritt)
target_sources(MeinAudioPlugin PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
)

# JUCE Module verlinken, die du brauchst
target_link_libraries(MeinAudioPlugin PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_gui_extra
)