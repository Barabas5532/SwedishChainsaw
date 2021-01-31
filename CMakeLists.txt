cmake_minimum_required(VERSION 3.15)
project(SwedishChainsaw VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(JUCE)

juce_add_plugin(SwedishChainsaw
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE Swed
    FORMATS VST3 Standalone
    PRODUCT_NAME "Swedish Chainsaw")

juce_generate_juce_header(SwedishChainsaw)

target_sources(SwedishChainsaw
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/FMVDesign.cpp
        Source/HM2Filter.cpp
        Source/Waveshaper.cpp)


target_include_directories(SwedishChainsaw
    PRIVATE
        Source)

target_compile_definitions(SwedishChainsaw
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(SwedishChainsaw
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
