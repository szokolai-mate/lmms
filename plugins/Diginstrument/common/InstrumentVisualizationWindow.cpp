#include "InstrumentVisualizationWindow.h"

using namespace QtDataVisualization;

void Diginstrument::InstrumentVisualizationWindow::setSurfaceData(QSurfaceDataArray * data)
{
    series->dataProxy()->resetArray(data);
}

void Diginstrument::InstrumentVisualizationWindow::setPartialData(std::vector<std::vector<Diginstrument::Component<float>>> partials, int startFrame, int sampleRate)
{
    //tmp: debug: if there are custom items, just remove them and dont add any
    //result: this "on/off" thing results in good behaviour: all items diseappear, and at the next press, all items appear
    //TODO: absolutely disgusting hacky "solution", but mutexes wont work, removing items one-by-one wont work, sleep wont work, im at my wits end
    if(graph->customItems().size()!=0)
    {
        graph->removeCustomItems();
        return;
    }

    const auto palette = Diginstrument::ColorPalette::generatePaletteTextures(partials.size());
    int colorIndex = 0;
    for(const auto & partial : partials)
    {
        int sampleIndex = startFrame;
        for(const auto & component : partial)
        {
            //calculate freq for visualization from diff of phase
            //TODO: const double freq = abs(((phase[i] - phase[i - 1]) * m_sampleBuffer.sampleRate()) / (2 * M_PI));
            //reduce points
            if(sampleIndex%((int)std::round(partial.size()/100.0f))==0)
            {
                //TODO: relative path? packaged resources?
                graph->addCustomItem(new QCustom3DItem("/home/mate/projects/lmms/plugins/Diginstrument/analyzer/resources/marker_mesh.obj",
                                                                QVector3D(component.frequency, component.amplitude, (float)sampleIndex / (float)sampleRate),
                                                                QVector3D(0.01f, 0.01f, 0.01f),
                                                                QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 45.0f),
                                                                palette[colorIndex]));
            }
            sampleIndex++;
        }
        colorIndex++;
    }	
}

Diginstrument::InstrumentVisualizationWindow::InstrumentVisualizationWindow(QObject * dataProvider)
{   
    //TODO: clear data when closed? lotsa memory
    connect(this, SIGNAL( requestDataUpdate(float, float, float, float, int, std::vector<float>) ), dataProvider, SLOT( updateVisualizationData(float, float, float, float, int, std::vector<float>) ));

    graph = new QtDataVisualization::Q3DSurface();
    container = QWidget::createWindowContainer(graph);
    series = new QSurface3DSeries;
    graph->addSeries(series);

    container->setMinimumWidth(400);
    container->setMinimumHeight(400);

    QHBoxLayout * outerLayout = new QHBoxLayout;
    QVBoxLayout * controlsLayout = new QVBoxLayout;
    QVBoxLayout * controlsContainerLayout = new QVBoxLayout;
    QWidget * controls = new QWidget;
    QWidget * controlsContainer = new QWidget;
    controls->setLayout(controlsLayout);
    controlsContainer->setLayout(controlsContainerLayout);
    outerLayout->addWidget(container);
    outerLayout->addWidget(controlsContainer);
    controlsContainerLayout->addWidget(controls);

    controlsContainer->setMaximumWidth(300);
    //todo:default values/limits
    controlsLayout->addWidget(new QLabel(QString("Coordinates")));
    coordinateSliderContainer = new QWidget;
    coordinateSliderContainer->setLayout(new QVBoxLayout);
    controlsLayout->addWidget(coordinateSliderContainer);

    controlsLayout->addWidget(new QLabel(QString("Limits")));
    startTimeSlider = new LabeledFieldSlider(0, 5000, 0);
    controlsLayout->addWidget(startTimeSlider);
    endTimeSlider = new LabeledFieldSlider(0,5000, 5000);
    controlsLayout->addWidget(endTimeSlider);
    startFreqSlider = new LabeledFieldSlider(20, 22000, 20);
    controlsLayout->addWidget(startFreqSlider);
    endFreqSlider = new LabeledFieldSlider(20, 22000, 22000);
    controlsLayout->addWidget(endFreqSlider);

    controlsLayout->addWidget(new QLabel(QString("Sample sizes")));
    timeSamples = new QLineEdit;
    timeSamples->setText("100");
    timeSamples->setValidator( new QIntValidator(2, 999) );
    controlsLayout->addWidget(timeSamples);

    QPushButton * refreshButton = new QPushButton("Refresh");
    controlsLayout->addWidget(refreshButton);
    connect( refreshButton, SIGNAL( clicked() ),
            this, SLOT( refresh() ));
    QWidget * autoRefreshContainer = new QWidget;
    QHBoxLayout * autoRefreshLayout = new QHBoxLayout;
    autoRefreshLayout->addWidget(new QLabel("Auto-refresh"));
    autoRefreshContainer->setLayout(autoRefreshLayout);
    autoRefreshCheckbox = new QCheckBox;
    autoRefreshCheckbox->setChecked(false);
    autoRefreshLayout->addWidget(autoRefreshCheckbox);
    controlsLayout->addWidget(autoRefreshContainer);

    this->setLayout(outerLayout);

    //tmp: graph style
    graph->axisX()->setRange(20, 22000);
    graph->axisX()->setFormatter(new QLogValue3DAxisFormatter);
    graph->axisY()->setRange(0, 1.1);
    graph->axisZ()->setRange(0, 3);
    graph->setAspectRatio(1.0);
    graph->setHorizontalAspectRatio(1.0);

    QLinearGradient gr;
    //TODO:maybe transparent? it seems bugged, may be just this old video card
    gr.setColorAt(0.0, Qt::transparent);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::green);
    gr.setColorAt(0.99, Qt::green);
    gr.setColorAt(1.0, Qt::red);

    //TODO: gradient seems to work weirdly: setColorAt is [0,1], is it automatically stretched to the values in data?
    graph->seriesList().at(0)->setBaseGradient(gr);
    graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    series->setDrawMode(QSurface3DSeries::DrawSurface);

    connect(startTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged()));
    connect(endTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged()));
    connect(startFreqSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged()));
    connect(endFreqSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged()));
}

Diginstrument::InstrumentVisualizationWindow::~InstrumentVisualizationWindow()
{
    delete container;
}

void Diginstrument::InstrumentVisualizationWindow::refresh()
{
    std::vector<float> coordinates;
    coordinates.reserve(coordinateSliders.size());
    for(auto * slider : coordinateSliders)
    {
        coordinates.push_back(slider->value());
    }
    emit requestDataUpdate(startTimeSlider->value(),endTimeSlider->value(),startFreqSlider->value(),endFreqSlider->value(),timeSamples->text().toInt(), coordinates);
    graph->axisX()->setRange(startFreqSlider->value(), endFreqSlider->value());
    graph->axisZ()->setRange(startTimeSlider->value()/1000.0f, endTimeSlider->value()/1000.0f);
}

void Diginstrument::InstrumentVisualizationWindow::setDimensions(std::vector<Dimension> dimensions)
{
    for(auto * s : coordinateSliders)
    {
        delete s;
    }
    coordinateSliders.clear();

    for(auto & d : dimensions)
    {
        //TMP: bad solution to exclude time
        if(d.name == "time") continue;
        //TODO: clearing the coordinates does not remove the labels
        coordinateSliderContainer->layout()->addWidget(new QLabel(d.name.c_str()));
        LabeledFieldSlider * slider = new LabeledFieldSlider(d.min, d.max, 400 /*TMP*/);
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged()));
        coordinateSliders.push_back(slider);
        coordinateSliderContainer->layout()->addWidget(slider);
    }
}

void  Diginstrument::InstrumentVisualizationWindow::slidersChanged()
{
    if(autoRefreshCheckbox->isChecked()) refresh();
}

QtDataVisualization::QSurfaceDataArray * Diginstrument::InstrumentVisualizationWindow::getInstrumentSurfaceData(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, int sampleRate, std::vector<float> coordinates, const Diginstrument::Interpolator<float> & interpolator)
{
	//TODO: better/refactoring
    //TODO: coordinates are empty on first open
    //TODO: exception inside qtvis 3d renderer
    //TODO: freq resolution is now unused

    //TODO: this might be all messed up: why do i need time?
	//coordinates.push_back(0);

    const float stepZ = (maxTime - minTime) / float(timeSamples - 1);

	QSurfaceDataArray * data = new QSurfaceDataArray;
	data->reserve(timeSamples);
	for(int i = 0; i<timeSamples;i++)
	{
		float z = qMin(maxTime, (i * stepZ + minTime));
		unsigned int startFrame = sampleRate*z;
		//coordinates.back() = z;
		
        //get one frame of residuals, which contains all non-empty channels!
		const auto residualSlice = interpolator.getResidual(coordinates, startFrame, 1);
        QSurfaceDataRow *dataRow = new QSurfaceDataRow(residualSlice.get().size());;
		for (int j = 0; j < residualSlice.get().size(); j++)
        {
            for(const auto & frame : residualSlice.get()[j])
            {   
                //if the channel is in this frame
                if(frame.first == startFrame || frame.first == startFrame+1)
                {
                    (*dataRow)[j].setPosition(QVector3D(frame.second.frequency, frame.second.amplitude, z));
                }
                else
                {
                    (*dataRow)[j].setPosition(QVector3D(frame.second.frequency, 0, z));
                }
            }
		}
		*data<<dataRow;
	}

	return data;
}

std::vector<std::vector<Diginstrument::Component<float>>> Diginstrument::InstrumentVisualizationWindow::getInstrumentPartialData(float minTime, float maxTime, float minFreq, float maxFreq, int sampleRate, std::vector<float> coordinates, const Diginstrument::Interpolator<float> & inst)
{
    std::vector<std::vector<Diginstrument::Component<float>>> res;
    int startFrame = minTime * sampleRate;
    int frames = maxTime * sampleRate - startFrame;
    const auto partials = inst.getPartials(coordinates, startFrame, frames);
    for(const auto & partial : partials.get())
    {
        if(partial.front().frequency >= minFreq && partial.front().frequency <= maxFreq)
        {
            res.push_back(partial);
        }
    }
    return res;
}