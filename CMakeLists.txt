cmake_minimum_required(VERSION 3.22)

# Projektname definieren
project(SimpleGain VERSION 0.1.0)

# JUCE Verzeichnis einbinden
add_subdirectory(JUCE)

# Das eigentliche Plugin erstellen
juce_add_plugin(SimpleGain
        # Hier deine Formate wählen (z.B. VST3, AU, Standalone)
        COMPANY_NAME "Example"
        PLUGIN_MANUFACTURER_CODE Tmpl
        PLUGIN_CODE Gain
        FORMATS VST3 Standalone
        PRODUCT_NAME "Simple Gain"
)

# C++ Standard festlegen
target_compile_features(SimpleGain PRIVATE cxx_std_20)

# Source Files hinzufügen
target_sources(SimpleGain PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
)

# JUCE Module verlinken, die du brauchst
target_link_libraries(SimpleGain PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_gui_extra
)