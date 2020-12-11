#include "AnalyzerView.h"

AnalyzerView::AnalyzerView(ToolPlugin * _parent ) :
  ToolPluginView(_parent)
{
    //create all GUI elements
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
    m_openVisualizationButton = new QPushButton( "Show instrument visualization");
    m_openVisualizationButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    m_addDimensionButton = new QPushButton( "Add dimension");
    m_addDimensionButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    m_saveToFileButton = new QPushButton( "Save instrument");
    m_saveToFileButton->setCursor( QCursor( Qt::PointingHandCursor ) );
    //connect buttons
    connect( m_openAudioFileButton, SIGNAL( clicked() ),
            this, SLOT( openAudioFile() ) );
    connect( m_openVisualizationButton, SIGNAL( clicked() ),
            this, SLOT( showVisualization() ) );
    connect( m_addDimensionButton, SIGNAL( clicked() ),
            this, SLOT( addDimension() ) );
    connect( m_saveToFileButton, SIGNAL( clicked() ),
            this, SLOT( writeInstrumentToFile() ) );

    //build widget hierarchy
    //instrument info
    QWidget * infoContainer = new QWidget;
    infoContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout * infoLayout = new QVBoxLayout;
    infoContainer->setLayout(infoLayout);
    infoLayout->addWidget(new QLabel("Instrument name"));
    infoLayout->addWidget(m_nameField);
    //dimension list
    dimensionFieldsContainer = new QWidget;
    QVBoxLayout * coordinateLayout = new QVBoxLayout;
    dimensionFieldsContainer->setLayout(coordinateLayout);
    coordinateLayout->setMargin(0);
    coordinateLayout->setSpacing(0);
    coordinateLayout->setAlignment(Qt::AlignTop);
    coordinateLayout->addWidget(m_addDimensionButton);
    QScrollArea * dimensionScrollArea = new QScrollArea;
    dimensionScrollArea->setWidget(dimensionFieldsContainer);
    dimensionScrollArea->setWidgetResizable(true);
    //analysis parameters
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
    //note list
    noteFieldsContainer = new QWidget;
    QVBoxLayout * notesLayout = new QVBoxLayout;
    noteFieldsContainer->setLayout(notesLayout);
    notesLayout->setMargin(0);
    notesLayout->setSpacing(0);
    notesLayout->setAlignment(Qt::AlignTop);
    notesLayout->addWidget(m_openAudioFileButton);
    QScrollArea * notesScrollArea = new QScrollArea;
    notesScrollArea->setWidget(noteFieldsContainer);
    notesScrollArea->setWidgetResizable(true);
    //assemble main layout
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(infoContainer);
    layout->addWidget(parametersContainer);
    layout->addWidget(dimensionScrollArea);
    layout->addWidget(notesScrollArea);
    layout->addWidget(m_openVisualizationButton);
    layout->addWidget(m_saveToFileButton);
    this->setLayout(layout);
    this->setMinimumSize(350,500);
    this->adjustSize();

    visualization = new Diginstrument::InstrumentVisualizationWindow(this);
}

AnalyzerView::~AnalyzerView() {}

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
    castModel<AnalyzerPlugin>()->analyzeSample( af , coordinates, m_partialCutoffField->text().toDouble(), m_partialAbsCutoffField->text().toDouble(), m_partialMinDistanceField->text().toDouble(), m_residualCutoffField->text().toDouble());
    lockDimensions();
    updateNoteFields();
	}
}

void AnalyzerView::updateVisualizationData(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, std::vector<float> coordinates)
{
  //TODO: TMP: smapleRate
  const int sampleRate = 44100;
  visualization->setSurfaceData(visualization->getInstrumentSurfaceData(minTime/1000.0f,maxTime/1000.0f,minFreq,maxFreq,timeSamples, sampleRate, coordinates, castModel<AnalyzerPlugin>()->interpolator));
  visualization->setPartialData(
    visualization->getInstrumentPartialData(minTime/1000.0f,maxTime/1000.0f,minFreq,maxFreq, sampleRate, coordinates, castModel<AnalyzerPlugin>()->interpolator),
    (minTime/1000.0f)*(float)sampleRate,
    sampleRate
  );
}

void AnalyzerView::showVisualization()
{
  std::vector<Diginstrument::Dimension> dimensions;
  for(const auto * f : dimensionFields)
  {
    dimensions.push_back(f->getDimension());
  }
  castModel<AnalyzerPlugin>()->buildInterpolator(dimensions);
  visualization->setDimensions(dimensions);
  updateVisualizationData(0,3000,20,22000,100/*TODO default values*/, {});
  visualization->show();
}

void AnalyzerView::addDimension()
{
  dimensionFields.push_back(new DimensionField);
  dimensionFieldsContainer->layout()->addWidget(dimensionFields.back());
  connect(dimensionFields.back(), SIGNAL( deleteSelf(DimensionField *) ), this, SLOT( deleteDimensionField(DimensionField*) ));
}

void AnalyzerView::lockDimensions()
{
  for(auto * f : dimensionFields)
  {
    f->lock();
  }
  m_addDimensionButton->setEnabled(false);
}

void AnalyzerView::unlockDimensions()
{
  for(auto * f : dimensionFields)
  {
    f->unlock();
  }
  m_addDimensionButton->setEnabled(true);
}

void AnalyzerView::deleteDimensionField(DimensionField * field)
{
  dimensionFields.removeOne(field);
  delete field;
}

void AnalyzerView::writeInstrumentToFile()
{
  std::vector<Diginstrument::Dimension> dimensions;
  for(const auto * f : dimensionFields)
  {
    dimensions.push_back(f->getDimension());
  }
  QString fileName = QFileDialog::getSaveFileName(NULL, "Save instrument to file",
                           NULL,
                           "*.json");
  if(fileName!="") castModel<AnalyzerPlugin>()->writeInstrumentToFile(fileName.toStdString(), dimensions);
}

void AnalyzerView::updateNoteFields()
{
  for(auto * field : noteFields)
  {
    delete field;
  }
  noteFields.clear();
  int i = 0;
  for(const auto & note : castModel<AnalyzerPlugin>()->notes)
  {
    std::string label = "";
    for(const auto & c : note.first.getLabels())
    {
      label+=c.first+"="+std::to_string(c.second)+", ";
    }
    noteFields.push_back(new NoteField(label, i));
    i++;
    noteFieldsContainer->layout()->addWidget(noteFields.back());
    connect(noteFields.back(), SIGNAL( deleteSelf(NoteField *) ), this, SLOT( deleteNote(NoteField*) ));
  }
}

void AnalyzerView::deleteNote(NoteField * field)
{
  auto it = castModel<AnalyzerPlugin>()->notes.begin();
  for(int j = 0; j<field->index ; j++)
  {
    it++;
  }
  castModel<AnalyzerPlugin>()->notes.erase(it);
  if(castModel<AnalyzerPlugin>()->notes.empty()) unlockDimensions();
  updateNoteFields();
}