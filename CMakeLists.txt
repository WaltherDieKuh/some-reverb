cmake_minimum_required(VERSION 3.22)

# Projektname definieren
project(AUDIO_PLUGIN_TEMPLATE VERSION 0.0.1)

# JUCE Verzeichnis einbinden
add_subdirectory(JUCE)

# Das eigentliche Plugin erstellen
juce_add_plugin(some-reverb
    COMPANY_NAME "Example"
    PLUGIN_MANUFACTURER_CODE Exmp
    PLUGIN_CODE RVB1
    FORMATS VST3 Standalone
    PRODUCT_NAME "Some Reverb"
)

# JuceHeader.h generieren (für #include <JuceHeader.h>)
juce_generate_juce_header(some-reverb)

# C++ Standard festlegen
target_compile_features(some-reverb PRIVATE cxx_std_20)

# Include-Verzeichnis für JuceHeader.h hinzufügen BEVOR target_sources
target_include_directories(some-reverb PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/some-reverb_artefacts/JuceLibraryCode
)

# Source Files hinzufügen
target_sources(some-reverb PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginEditor.h
)

# JUCE Module verlinken
target_link_libraries(some-reverb PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_extra
    juce::juce_core
)