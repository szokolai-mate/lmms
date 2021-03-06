INCLUDE(BuildPlugin)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Qt5DataVisualization REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_definitions(${QT_DEFINITIONS})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

BUILD_PLUGIN(diginstrument_spectral_analyzer
 ../common/PeakMatcher.cpp AnalyzerPlugin.cpp AnalyzerView.cpp  ../common/InstrumentVisualizationWindow.cpp ../common/Qt/LabeledSlider.cpp ../common/Qt/LabeledFieldSlider.cpp ../common/Interpolator.cpp
 CWT.hpp Extrema.hpp ../common/Spectrum.hpp ../common/Instrument.hpp ../common/Qt/DimensionField.hpp ../common/Qt/NoteField.hpp ../common/MultidimensionalNeighbourMap.hpp
 AnalyzerPlugin.h AnalyzerView.h ../common/InstrumentVisualizationWindow.h ../common/Qt/LabeledSlider.h ../common/Qt/LabeledFieldSlider.h ../common/Interpolator.h
 ../resources/resources.qrc
 MOCFILES AnalyzerPlugin.h AnalyzerView.h EMBEDDED_RESOURCES *.png)
set_property(TARGET diginstrument_spectral_analyzer PROPERTY CXX_STANDARD 17)
target_link_libraries(diginstrument_spectral_analyzer wavelib)
target_link_libraries(diginstrument_spectral_analyzer nlohmann_json::nlohmann_json)
target_link_libraries(diginstrument_spectral_analyzer fftw3)
qt5_use_modules(diginstrument_spectral_analyzer DataVisualization)