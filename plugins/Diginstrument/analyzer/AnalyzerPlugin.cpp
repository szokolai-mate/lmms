#include "AnalyzerPlugin.h"

using namespace QtDataVisualization;

extern "C"
{

	Plugin::Descriptor PLUGIN_EXPORT diginstrument_spectral_analyzer_plugin_descriptor =
		{
			STRINGIFY(PLUGIN_NAME),
			"Diginstrument Spectral Analyzer",
			QT_TRANSLATE_NOOP("pluginBrowser",
							  "WIP"
							  "Test"),
			"Máté Szokolai",
			0x0110,
			Plugin::Tool,
			new PluginPixmapLoader("logo"),
			NULL,
			NULL};

	// necessary for getting instance out of shared lib
	PLUGIN_EXPORT Plugin *lmms_plugin_main(Model *model, void *)
	{
		return new AnalyzerPlugin();
	}
}

AnalyzerPlugin::AnalyzerPlugin() : ToolPlugin(&diginstrument_spectral_analyzer_plugin_descriptor, NULL)
{
	/*TODO */
}

AnalyzerPlugin::~AnalyzerPlugin() {}

PluginView *AnalyzerPlugin::instantiateView(QWidget *_parent)
{
	return new AnalyzerView(this);
}

void AnalyzerPlugin::loadSettings(const QDomElement &_this) {}
void AnalyzerPlugin::saveSettings(QDomDocument &_doc, QDomElement &_parent) {}

QString AnalyzerPlugin::nodeName() const
{
	return "TEST";
}

QString AnalyzerPlugin::fullDisplayName() const
{
	return "TEST";
}

//TMP
std::string AnalyzerPlugin::analyzeSample(const QString &_audio_file, vector<pair<string, float>> coordinates, double partialCutoffParameter, double partialHeightCutoffParameter, double partialMinDistance, double residualCutoffParameter)
{
	m_sampleBuffer.setAudioFile(_audio_file);
	std::vector<double> sample(m_sampleBuffer.frames());
	for (int i = 0; i < sample.size(); i++)
	{
		//tmp: left only
		sample[i] = m_sampleBuffer.data()[i][0];
	}

	//tmp:visualization
	visualization = new Diginstrument::InstrumentVisualizationWindow(this);

	const auto partials = subtractiveAnalysis(sample, m_sampleBuffer.sampleRate(), partialCutoffParameter, partialHeightCutoffParameter, partialMinDistance);
	//inst.add(PartialSet<double>(std::move(partials), coordinates, m_sampleBuffer.sampleRate()));
	const PartialSet<float> partialSet((partials), coordinates, m_sampleBuffer.sampleRate());
	inst.add(PartialSet<float>(std::move(partials), coordinates, m_sampleBuffer.sampleRate()));
	const auto res = residualAnalysis(sample, m_sampleBuffer.sampleRate(), residualCutoffParameter);
	inst.add(Residual<float>(res, coordinates));

	//tmp: synthesize
	std::vector<float> synth(sample.size(), 0);
	//partials
	for (const auto &partial : partialSet.get())
	{
		for (int i = 0; i < partial.size(); i++)
		{		
			synth[i] += cos(partial[i].phase) * partial[i].amplitude;
		}
	}

	for(const auto & channel : res)
    {
        for(const auto & frame : channel)
        {
            synth[frame.first] += cos(frame.second.phase) * frame.second.amplitude; 
        }
    }

	/*for(auto s : synth)
	{
		cout<<std::fixed<<s<<" ";
	}
	std::cout<<std::endl;*/

	//tmp: error signal
	std::vector<float> errorSignal(sample.size(), 0);
	for (int i = 0; i < sample.size(); i++)
	{
		//tmp: left only
		errorSignal[i] = m_sampleBuffer.data()[i][0] - synth[i];
	}
	for (auto &e : errorSignal)
	{
		e = pow(std::abs(e),2);
	}
	cout << "highest amp in error signal: " << *max_element(errorSignal.begin(), errorSignal.end()) << std::endl;
	cout << "mean amp in error signal: " << std::accumulate(errorSignal.begin(), errorSignal.end(), 0.0) / errorSignal.size() << std::endl;
	std::cout<<"estimated fundamentalFrequency: "<<partialSet.getFundamentalFrequency()<<std::endl;
	//tmp: show raw visualization
	visualization->show();

	return "TODO";
}

QtDataVisualization::QSurfaceDataArray *AnalyzerPlugin::getSurfaceData(double minTime, double maxTime, double minFreq, double maxFreq, int timeSamples, int freqSamples)
{
	//TODO: fix this (but why?)
	const double stepX = (maxFreq - minFreq) / double(freqSamples - 1);
	const double stepZ = (maxTime - minTime) / double(timeSamples - 1);

	QSurfaceDataArray *data = new QSurfaceDataArray;
	data->reserve(timeSamples);
	for (int i = 0; i < timeSamples; i++)
	{
		QSurfaceDataRow *dataRow = new QSurfaceDataRow(freqSamples);
		/*double z = qMin(maxTime, (i * stepZ + minTime));
		int index = 0;
		//TODO: new instrument model invalidated this: needs coordinates now
		//TODO: residual and partial broke it
		auto it = std::lower_bound(spectra.begin(), spectra.end(), SplineSpectrum<double, 4>(std::vector<std::pair<std::string, double>>{std::make_pair("time", z)}));
		//TMP
		if (it == spectra.end())
			break;
		//TMP
		const auto spectrum = *it;
		for (int j = 0; j < freqSamples; j++)
		{
			double x = qMin(maxFreq, (j * stepX + minFreq));
			(*dataRow)[index++].setPosition(QVector3D(x, spectrum[x].amplitude, z));
		}
		//tmp: identical to discrete
		for (const auto &c : spectrum.getComponents(0))
		{
			//NOTE: BUGHUNT: missing inbetween piece causes segfault
			if (c.frequency <= minFreq || c.frequency >= maxFreq)
				continue;
			(*dataRow)[std::round((c.frequency - minFreq) / ((maxFreq - minFreq) / (double)freqSamples))].setPosition(QVector3D(c.frequency, c.amplitude, z));
		}*/

		*data << dataRow;
	}

	return data;
}

void AnalyzerPlugin::writeInstrumentToFile(std::string filename)
{
	ofstream file(filename);
	if (file.is_open())
	{
		//TMP: pretty printed
		file << fixed << inst.toString(4);
		file.close();
	}
}

std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<float>>>> AnalyzerPlugin::residualAnalysis(
	const std::vector<double> &signal,
	unsigned int sampleRate,
	double minProminence)
{
	//do CWT
	const int level = 12;
	CWT<float> transform("morlet", 6, level, sampleRate);
	transform(signal);

	std::vector<std::vector<float>> unwrappedPhases;
	std::vector<std::vector<float>> magnitudes;
	const float normCWT = transform.calculateMagnitudeNormalizationConstant(6);
	//construct magnitude and unwrapped phase matrices
	for (int j = 0; j < level * CWT<float>::octaves; j++)
	{
		const auto cfs = transform.getScaleCoefficients(j);
		std::vector<float> phase;
		phase.reserve(cfs.size());
		std::vector<float> mag;
		mag.reserve(cfs.size());

		for (const auto &c : cfs)
		{
			phase.emplace_back(std::arg(c));
			mag.emplace_back(std::abs(c * normCWT) / (sqrt(transform.getScale(j).first * sampleRate)));
		}

		Diginstrument::Phase<float>::unwrapInPlace(phase);
		magnitudes.emplace_back(std::move(mag));
		unwrappedPhases.emplace_back(std::move(phase));
	}

	//TODO: TMP: we have to synthesize and calculate mean-ratio to normalize
	//this is probably just a hack!
	//synthesis
	std::vector<float> synth(signal.size(), 0);
	for (int j = 0; j < level * CWT<float>::octaves; j++)
	{
		for (int i = 0; i < signal.size(); i++)
		{
			synth[i] += cos(unwrappedPhases[j][i]) * magnitudes[j][i];
		}
	}

	//calculate mean of absolute residual signal
	double absResidualMean = 0;
	for (const auto &e : signal)
	{
		absResidualMean += pow(std::abs(e), 2);
	}

	//calculate synthesized residual absolute mean
	double absSynthMean = 0;
	for (const auto &e : synth)
	{
		absSynthMean += pow(std::abs(e), 2);
	}

	//normalize
	const float norm = sqrt(absResidualMean / absSynthMean);
	
	for (int i = 0; i < synth.size(); i++)
	{
		for (int j = 0; j < level * CWT<float>::octaves; j++)
		{
			magnitudes[j][i] *= norm;
		}
	}

	//construct res
	std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<float>>>> res;
	for (int j = 0; j < level * CWT<float>::octaves; j++)
	{
		std::vector<std::pair<unsigned int, Diginstrument::Component<float>>> channel;
		for (int i = 0; i < signal.size(); i++) 
		{
			if (magnitudes[j][i] > minProminence)
			{
				channel.push_back(
					std::make_pair(
						i,
						Diginstrument::Component(
							1.0f / (float) transform.getScale(j).second,
							unwrappedPhases[j][i],
							magnitudes[j][i])
					)
				);
			}
		}
		if(channel.size()>0) res.emplace_back(std::move(channel));
	}

	//tmp: visualize
	unsigned int stepSize = 441;
	QSurfaceDataArray *data = new QSurfaceDataArray;
	data->reserve(synth.size()/stepSize);
	for(int i = 0; i<synth.size(); i+=stepSize)
	{
		QSurfaceDataRow *dataRow = new QSurfaceDataRow(level * CWT<float>::octaves);
		for (int j = 0; j < level * CWT<float>::octaves; j++)
		{
			(*dataRow)[j].setPosition(QVector3D(1.0 / transform.getScale(j).second, magnitudes[j][i], (double)i/(double)(m_sampleBuffer.sampleRate())));
		}
		*data << dataRow;
	}
	visualization->setSurfaceData(data);

	return res;
}

//TODO: current output: spectra over time
//problems: FULL output = samples*partials
//phase + magnitude
//TODO: maybe reduce samples by excluding places of linear phase? and linear mag? then i will need to include time? possibly useless? only zero magnitude?
std::vector<std::vector<Diginstrument::Component<float>>> AnalyzerPlugin::subtractiveAnalysis(
	std::vector<double> &signal,
	unsigned int sampleRate,
	double minProminence,
	double heightThreshold,
	double minDistance)
{
	//TODO: avg energy to detect broad-changing frequencies
	//calculate FFT magnitudes of the signal
	Diginstrument::FFT<float> fft(signal.size());
	const auto mags = fft(signal, m_sampleBuffer.sampleRate());
	//find peaks in FFT, indicating areas of significance
	const auto maxima = Extrema::Differential::maxima(
		mags.begin(), mags.end(), [minProminence](double fr) -> double { return minProminence; }, heightThreshold, minDistance);
	//TMP: visualize found frequencies
	QImage colorBlack = QImage(2, 2, QImage::Format_RGB32);
	colorBlack.fill(Qt::black);
	for (auto p : maxima)
	{
		const auto Y = Interpolation::CubicLagrange<float>(mags[p.index - 1].first, mags[p.index - 1].second, mags[p.index].first, mags[p.index].second, mags[p.index + 1].first, mags[p.index + 1].second, mags[p.index + 2].first, mags[p.index + 2].second, p.x);
		visualization->addCustomItem(new QCustom3DItem("/home/mate/projects/lmms/plugins/Diginstrument/analyzer/resources/marker_mesh.obj",
													   QVector3D(p.x, Y, 0),
													   QVector3D(0.025f, 0.025f, 0.025f),
													   QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 45.0f),
													   colorBlack));
	}

	vector<float> phase(signal.size(), 0);
	vector<float> mag(signal.size(), 0);
	std::vector<std::vector<Diginstrument::Component<float>>> res;
	//tmp: visualization
	const auto palette = Diginstrument::ColorPalette::generatePaletteTextures(maxima.size());

	constexpr double parameter = 6;
	const float normalizationConstant = CWT<float>::calculateMagnitudeNormalizationConstant(parameter);
	//for each selected frequency
	for (int j = 0; j < maxima.size(); j++)
	{
		const double fr = maxima[j].x;
		//TODO: tie wavelet parameters together
		const double scale = (parameter + sqrt(2 + parameter * parameter)) / (4 * M_PI * fr);
		//perform a single center frequency CWT
		const auto cfs = CWT<float>::singleScaleCWT(signal, scale, sampleRate);
		//for each instance in time
		for (int i = 0; i < cfs.size(); i++)
		{
			//process the complex coefficient
			const auto &c = cfs[i];
			//TODO: this has been the most successful equation, as it resulted in the same amp for both components in 440+50.wav
			//I have no idea where the constant comes from; probably ties into the wavelet parameter
			const float amp = (std::abs(normalizationConstant * c) / (sqrt(scale * sampleRate)));
			mag[i] = amp;
			phase[i] = std::arg(c);
		}
			
		//subtract extracted partial to get residual signal
		for (int i = 0; i < signal.size(); i++)
		{
			signal[i] -= cos(phase[i]) * mag[i];
		}
		//unwrap phase
		Diginstrument::Phase<float>::unwrapInPlace(phase);
		//trim magnitudes
		//TODO: is there a way to trim front too? include time?
		unsigned int end = mag.size()-1;
		//tmp
		const float partialCutoff = 0.0005;
		while(end>0 && mag[end]<partialCutoff) end--;
		//tmp:
		cout<<"resized partial "<<fr<<": "<<mag.size()<<" -> "<<end+1<<endl;

		//construct partial
		vector<Diginstrument::Component<float>> partial(end+1);
		for (int i = 0; i < end+1; i++)
		{
			partial[i] = Diginstrument::Component<float>(fr, phase[i], mag[i]);
			//tmp: visualization
			if (i % 440 == 1 && mag[i] > 0.001)
			{
				//calculate freq for visualization from diff of phase
				const double freq = abs(((phase[i] - phase[i - 1]) * m_sampleBuffer.sampleRate()) / (2 * M_PI));
				visualization->addCustomItem(new QCustom3DItem("/home/mate/projects/lmms/plugins/Diginstrument/analyzer/resources/marker_mesh.obj",
															   QVector3D(freq, mag[i], (double)i / (double)sampleRate),
															   QVector3D(0.01f, 0.01f, 0.01f),
															   QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 45.0f),
															   palette[j]));
			}
		}
		//add partial to set
		res.push_back(std::move(partial));
	}
	return res;
}
