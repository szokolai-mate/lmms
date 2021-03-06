#include "DiginstrumentView.h"

using namespace QtDataVisualization;

DiginstrumentView::DiginstrumentView( Instrument * _instrument, QWidget * _parent ) :
  InstrumentView(_instrument, _parent)
{
    m_openInstrumentFileButton = new QPushButton( "Load instrument from file", this);
    m_openInstrumentVisualizationButton = new QPushButton( "Show instrument visualization");
    m_openInstrumentFileButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    m_openInstrumentVisualizationButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    connect( m_openInstrumentFileButton, SIGNAL( clicked() ),
            this, SLOT( openInstrumentFile() ) );
    connect( m_openInstrumentVisualizationButton, SIGNAL( clicked() ),
            this, SLOT( showInstumentVisualization() ) );
    m_nameField = new QLineEdit("Name", this);
    m_nameField->setReadOnly(true);

    QWidget * infoContainer = new QWidget;
    infoContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QGridLayout * infoLayout = new QGridLayout;
    infoContainer->setLayout(infoLayout);
    infoLayout->addWidget(m_nameField, 0, 0, 1, 1);
    infoLayout->addWidget(m_openInstrumentFileButton, 0, 1, 1, 2);
    infoLayout->addWidget(m_openInstrumentVisualizationButton, 1, 1, 1, 2);

    coordinateSliderContainer = new QWidget;
    coordinateSliderContainer->setLayout(new QVBoxLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(infoContainer);
    layout->addWidget(coordinateSliderContainer);
    this->setLayout(layout);

    visualization = new Diginstrument::InstrumentVisualizationWindow(this);
    setDimensions();
    updateCoordinates();
}


DiginstrumentView::~DiginstrumentView()
{
}

void DiginstrumentView::modelChanged( void ){
    /*TODO */
    if(castModel<DiginstrumentPlugin>()->fileName != "")
    {
      m_nameField->setText(castModel<DiginstrumentPlugin>()->instrument.name.c_str());
    }
}

void DiginstrumentView::openInstrumentFile( void )
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Select instrument"), "", tr("*.json"));

	if( fileName != "" )
	{
    castModel<DiginstrumentPlugin>()->setInstrumentFile( fileName );
    castModel<DiginstrumentPlugin>()->loadInstrumentFile();
    setDimensions();
    updateCoordinates();
	}
}

void DiginstrumentView::showInstumentVisualization()
{
  //TODO: defaults/saving
  visualization->setDimensions(castModel<DiginstrumentPlugin>()->instrument.dimensions);
  updateVisualizationData(0,3000,20,22000,100/*TODO default values*/, {});
  visualization->show();
}

void DiginstrumentView::updateVisualizationData(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, std::vector<float> coordinates)
{
  //TODO: FIXME: TMP: smapleRate is fixed!
  const int sampleRate = 44100;
  visualization->setSurfaceData(visualization->getInstrumentSurfaceData(minTime/1000.0f,maxTime/1000.0f,minFreq,maxFreq,timeSamples, sampleRate, coordinates, castModel<DiginstrumentPlugin>()->interpolator));
  visualization->setPartialData(
    visualization->getInstrumentPartialData(minTime/1000.0f,maxTime/1000.0f,minFreq,maxFreq, sampleRate, coordinates, castModel<DiginstrumentPlugin>()->interpolator),
    (minTime/1000.0f)*(float)sampleRate,
    sampleRate
  );
}

void DiginstrumentView::setDimensions()
{
  const auto dimensions = castModel<DiginstrumentPlugin>()->instrument.dimensions;
  for(auto * s : coordinateSliders)
  {
      delete s;
  }

  for(auto & d : dimensions)
  {
      //TMP: bad solution to exclude time
      if(d.name == "time" || d.name == "pitch" || d.name == "frequency" ) continue;
      coordinateSliderContainer->layout()->addWidget(new QLabel(d.name.c_str()));
      LabeledFieldSlider * slider = new LabeledFieldSlider(d.min, d.max, d.currentValue);
      connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateCoordinates()));
      coordinateSliders.push_back(slider);
      coordinateSliderContainer->layout()->addWidget(slider);
  }
}

void DiginstrumentView::updateCoordinates()
{
  auto & coordinates = castModel<DiginstrumentPlugin>()->coordinates;
  coordinates.clear();
  coordinates.reserve(coordinateSliders.size());
  for(auto * slider : coordinateSliders)
  {
    coordinates.push_back(slider->value());
  }
}