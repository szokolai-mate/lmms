#include "AnalyzerView.h"

AnalyzerView::AnalyzerView(ToolPlugin * _parent ) :
  ToolPluginView(_parent)
{
    /*TODO */
    m_nameField = new QLineEdit;
    m_partialCutoffField = new QLineEdit;
    m_partialCutoffField->setText("0.002");
    m_residualCutoffField = new QLineEdit;
    m_residualCutoffField->setText("0.0001");
    m_partialAbsCutoffField = new QLineEdit;
    m_partialAbsCutoffField->setText("0.0005");
    m_partialMinDistanceField = new QLineEdit;
    m_partialMinDistanceField->setText("15");
    m_openAudioFileButton = new QPushButton( "Add note from file with current coordinates", this);
    m_openAudioFileButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    m_openVisualizationButton = new QPushButton( "Show instrument visualization (broken)");
    m_openVisualizationButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    m_addDimensionButton = new QPushButton( "Add dimension");
    m_addDimensionButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    m_saveToFileButton = new QPushButton( "Save instrument");
    m_saveToFileButton->setCursor( QCursor( Qt::PointingHandCursor ) );

    connect( m_openAudioFileButton, SIGNAL( clicked() ),
            this, SLOT( openAudioFile() ) );
    connect( m_openVisualizationButton, SIGNAL( clicked() ),
            this, SLOT( showVisualization() ) );
    connect( m_addDimensionButton, SIGNAL( clicked() ),
            this, SLOT( addDimension() ) );
    connect( m_saveToFileButton, SIGNAL( clicked() ),
            this, SLOT( writeInstrumentToFile() ) );

    QWidget * infoContainer = new QWidget;
    infoContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QGridLayout * infoLayout = new QGridLayout;
    infoContainer->setLayout(infoLayout);
    infoLayout->addWidget(new QLabel("Instrument name"), 0, 0, 1, 1);
    infoLayout->addWidget(m_nameField, 1, 0, 1, 1);
    infoLayout->addWidget(m_openAudioFileButton, 0, 1, 1, 2);
    infoLayout->addWidget(m_openVisualizationButton, 1, 1, 1, 2);

    dimensionFieldsContainer = new QWidget;
    QVBoxLayout * coordinateLayout = new QVBoxLayout;
    dimensionFieldsContainer->setLayout(coordinateLayout);
    coordinateLayout->setMargin(0);
    coordinateLayout->setSpacing(0);
    coordinateLayout->setAlignment(Qt::AlignTop);
    coordinateLayout->addWidget(m_addDimensionButton);

    QWidget * parametersContainer = new QWidget;
    parametersContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout * parametersLayout = new QHBoxLayout;
    parametersContainer->setLayout(parametersLayout);
    QWidget * partialParameterContainer = new QWidget;
    partialParameterContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout * partialParameterLayout = new QVBoxLayout;
    partialParameterContainer->setLayout(partialParameterLayout);
    partialParameterLayout->addWidget(new QLabel("Partials min. prominence"));
    partialParameterLayout->addWidget(m_partialCutoffField);
    partialParameterLayout->addWidget(new QLabel("Partials height cutoff"));
    partialParameterLayout->addWidget(m_partialAbsCutoffField);
    partialParameterLayout->addWidget(new QLabel("Partials min. frequency distance"));
    partialParameterLayout->addWidget(m_partialMinDistanceField);
    QWidget * residualParameterContainer = new QWidget;
    residualParameterContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout * residualParameterLayout = new QVBoxLayout;
    residualParameterContainer->setLayout(residualParameterLayout);
    residualParameterLayout->addWidget(new QLabel("Residual min. prominence"));
    residualParameterLayout->addWidget(m_residualCutoffField);
    parametersLayout->addWidget(partialParameterContainer);
    parametersLayout->addWidget(residualParameterContainer);

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(infoContainer);
    layout->addWidget(parametersContainer);
    layout->addWidget(dimensionFieldsContainer);
    layout->addWidget(m_saveToFileButton);
    this->setLayout(layout);
    this->adjustSize();

    visualization = new Diginstrument::InstrumentVisualizationWindow(this);
}


AnalyzerView::~AnalyzerView()
{
}

void AnalyzerView::modelChanged( void ){
    /*TODO */
}

void AnalyzerView::openAudioFile( void )
{
	QString af = castModel<AnalyzerPlugin>()->m_sampleBuffer.
							openAudioFile();
	if( af != "" )
	{
    std::vector<std::pair<std::string, float>> coordinates;
    for(auto * p : dimensionFields)
    {
      const auto pair = p->getCoordinate();
      if(!pair.first.empty()) coordinates.push_back(pair);
    }
    std::string res = castModel<AnalyzerPlugin>()->analyzeSample( af , coordinates, m_partialCutoffField->text().toDouble(), m_partialAbsCutoffField->text().toDouble(), m_partialMinDistanceField->text().toDouble(), m_residualCutoffField->text().toDouble());
		//Engine::getSong()->setModified();
		//m_waveView->updateSampleRange();
	}
}

void AnalyzerView::updateVisualizationData(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, int freqSamples, std::vector<double> coordinates)
{
  visualization->setSurfaceData(castModel<AnalyzerPlugin>()->getSurfaceData(minTime/1000.0f,maxTime/1000.0f,minFreq,maxFreq,timeSamples,freqSamples));
}

void AnalyzerView::showVisualization()
{
  updateVisualizationData(0,3000,20,22000,100,100/*TODO default values*/, {});
  visualization->show();
}

void AnalyzerView::addDimension()
{
  dimensionFields.push_back(new DimensionField);
  dimensionFieldsContainer->layout()->addWidget(dimensionFields.back());
  connect(dimensionFields.back(), SIGNAL( deleteSelf(DimensionField *) ), this, SLOT( deleteDimensionField(DimensionField*) ));
}

void AnalyzerView::deleteDimensionField(DimensionField * field)
{
  dimensionFields.removeOne(field);
  delete field;
}

void AnalyzerView::writeInstrumentToFile()
{
  //tmp: set dimensions here, before saving
  std::vector<Diginstrument::Dimension> dimensions;
  for(const auto * f : dimensionFields)
  {
    dimensions.push_back(f->getDimension());
  }
  castModel<AnalyzerPlugin>()->inst.dimensions = dimensions;
  QString fileName = QFileDialog::getSaveFileName(NULL, "Save instrument to file",
                           NULL,
                           "*.json");
  if(fileName!="") castModel<AnalyzerPlugin>()->writeInstrumentToFile(fileName.toStdString());
}