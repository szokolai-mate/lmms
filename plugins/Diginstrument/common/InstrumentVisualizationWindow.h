#pragma once

#include <QtWidgets>
#include <QtDataVisualization>

#include "Qt/LabeledFieldSlider.h"
#include "../common/Qt/ColorPalette.hpp"

#include "Dimension.h"
#include "Interpolator.h"

namespace Diginstrument
{
class InstrumentVisualizationWindow : public QWidget
{
  Q_OBJECT
  public:
    QtDataVisualization::QSurfaceDataArray * getInstrumentSurfaceData(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, int sampleRate, std::vector<float> coordinates, const Diginstrument::Interpolator<float> & inst);
    std::vector<std::vector<Diginstrument::Component<float>>> getInstrumentPartialData(float minTime, float maxTime, float minFreq, float maxFreq, int sampleRate, std::vector<float> coordinates, const Diginstrument::Interpolator<float> & inst);

    void setSurfaceData(QtDataVisualization::QSurfaceDataArray * data);
    void setPartialData(std::vector<std::vector<Diginstrument::Component<float>>> partials, int startFrame, int sampleRate);
    void setDimensions(std::vector<Dimension> dimensions);
    int addCustomItem(QtDataVisualization::QCustom3DItem *item);
    void removeCustomItems();

    InstrumentVisualizationWindow(QObject * dataProvider);
    ~InstrumentVisualizationWindow();

  signals:
    void requestDataUpdate(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, std::vector<float> coordinates = {});
  private slots:
    void refresh();
    void slidersChanged();

  private:
    QtDataVisualization::Q3DSurface *graph;
    QWidget * container;
    QtDataVisualization::QSurface3DSeries *series;
    //UI elements
    //TODO: log-scale toggle
    LabeledFieldSlider *startTimeSlider, *endTimeSlider, *startFreqSlider, *endFreqSlider;
    QLineEdit * timeSamples;
    std::vector<LabeledFieldSlider*> coordinateSliders;
    QWidget * coordinateSliderContainer;
    QCheckBox * autoRefreshCheckbox;
};
};