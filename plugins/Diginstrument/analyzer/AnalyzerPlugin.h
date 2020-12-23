#pragma once

#include "AnalyzerView.h"

//LMMS plugin includes
#include "ToolPlugin.h"
#include "plugin_export.h"
#include "embed.h"

//LMMS class includes
#include "SampleBuffer.h"

//QT5 includes
#include <QtDataVisualization>

#include "CWT.hpp"
#include "FFT.hpp"
#include "Phase.hpp"
#include "Extrema.hpp"
#include "../common/Instrument.hpp"
#include "../common/Interpolator.h"

//I/O includes
#include <iostream>
#include <fstream>

class AnalyzerPlugin : public ToolPlugin
{
  Q_OBJECT
public:
  AnalyzerPlugin();
  virtual ~AnalyzerPlugin();

  virtual PluginView *instantiateView(QWidget *_parent);

  virtual void saveSettings(QDomDocument &_doc, QDomElement &_parent);
  virtual void loadSettings(const QDomElement &_this);

  virtual QString nodeName() const;

  virtual QString fullDisplayName() const;

  /**
   * @brief Save the stored notes into an instrument file.
   * 
   * @param filename the full path of the output file
   * @param dimensions the dimensions assigned to the instrument
   */
  void writeInstrumentToFile(std::string filename, std::vector<Diginstrument::Dimension> dimensions);

  /**
   * @brief Construct and store a note model instance from an audio file.
   * 
   * Use the method described in the paper for the time-frequency analysis and representation of the selected audio file.
   * Partial selection using FFT, then partial extraction using single-scale CWTs, then finally residual analysis using CWT.
   * The note is separated into a PartialSet and a Residual representation.
   * 
   * @param _audio_file full path to the audio sample file
   * @param coordinates key-value pairs of the supplied coordinates
   * @param partialMinProminence minimum prominence of the peaks during partial selection (\f$p\f$)
   * @param partialHeightCutoffParameter minimum height cutoff of the peaks during partial trimming (\f$c_p\f$)
   * @param partialMinDistance minimum frequency distance between selected partials (\f$d\f$)
   * @param residualMinProminence minimum magnitude cutoff for residual samples (\f$c_r\f$)
   * 
   * @return whether the analysis was finished successfully
   * */
  bool analyzeSample(const QString &_audio_file,
                     vector<pair<string, float>> coordinates,
                     double partialMinProminence = 0,
                     double partialHeightCutoffParameter = 0,
                     double partialMinDistance = 0,
                     double residualMinProminence = 0);

private:
  friend class AnalyzerView;

  std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<float>>>> residualAnalysis(const std::vector<double> &signal,
                                                                                                      unsigned int sampleRate,
                                                                                                      double minProminence = 0);

  std::vector<std::vector<Diginstrument::Component<float>>> subtractiveAnalysis(std::vector<double> &signal,
                                                                                unsigned int sampleRate,
                                                                                double minProminence = 0,
                                                                                double heightThreshold = 0,
                                                                                double minDistance = 0);

  void buildInterpolator(std::vector<Diginstrument::Dimension> dimensions);

  SampleBuffer m_sampleBuffer;
  list<pair<PartialSet<float>, Residual<float>>> notes;
  Diginstrument::Interpolator<float> interpolator;
};