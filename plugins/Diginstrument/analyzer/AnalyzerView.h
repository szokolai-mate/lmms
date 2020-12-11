#pragma once

#include "ToolPluginView.h"
#include "AnalyzerPlugin.h"
#include "../common/InstrumentVisualizationWindow.h"
#include "../common/Qt/DimensionField.hpp"
#include "../common/Qt/NoteField.hpp"

#include <QPushButton>
#include <QTextEdit>

/**
 * @brief The view class of the analyzer plugin.
 * 
 * Provides the GUI for the analyzer plugin with QT.
 */
class AnalyzerView : public ToolPluginView
{
    Q_OBJECT
  public:
    AnalyzerView( ToolPlugin * _parent );
	  virtual ~AnalyzerView();

  protected slots:
    void openAudioFile();
    void writeInstrumentToFile();
    void showVisualization();
    void addDimension();
    void deleteDimensionField(DimensionField * field);
    void deleteNote(NoteField * field);
    void updateVisualizationData(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, std::vector<float> coordinates);

  private:
    QPushButton * m_openAudioFileButton;
    QPushButton * m_openVisualizationButton;
    QPushButton * m_addDimensionButton;
    QLineEdit * m_nameField;
    QLineEdit * m_partialCutoffField;
    QLineEdit * m_partialAbsCutoffField;
    QLineEdit * m_partialMinDistanceField;
    QLineEdit * m_residualCutoffField;
    QList<DimensionField*> dimensionFields;
    QWidget * dimensionFieldsContainer;
    QPushButton * m_saveToFileButton;
    QList<NoteField*> noteFields;
    QWidget * noteFieldsContainer;
    
    Diginstrument::InstrumentVisualizationWindow * visualization;

    void lockDimensions();
    void unlockDimensions();
    void updateNoteFields();
};
