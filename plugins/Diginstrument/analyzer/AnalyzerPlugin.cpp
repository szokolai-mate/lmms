#include "AnalyzerPlugin.h"

using namespace QtDataVisualization;

extern "C"
{

	Plugin::Descriptor PLUGIN_EXPORT diginstrument_spectral_analyzer_plugin_descriptor =
		{
			STRINGIFY(PLUGIN_NAME),
			"Diginstrument Spectral Analyzer and Instument Creator",
			QT_TRANSLATE_NOOP("pluginBrowser",
							  "A generic instrument model"),
			"Máté Szokolai <szokolai.mate@gmail.com>",
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

AnalyzerPlugin::AnalyzerPlugin() : ToolPlugin(&diginstrument_spectral_analyzer_plugin_descriptor, NULL) {}

AnalyzerPlugin::~AnalyzerPlugin() {}

PluginView *AnalyzerPlugin::instantiateView(QWidget *_parent)
{
	return new AnalyzerView(this);
}

void AnalyzerPlugin::loadSettings(const QDomElement &_this) {}
void AnalyzerPlugin::saveSettings(QDomDocument &_doc, QDomElement &_parent) {}

QString AnalyzerPlugin::nodeName() const
{
	return "Diginstrument Analyzer";
}

QString AnalyzerPlugin::fullDisplayName() const
{
	return "Diginstrument Spectral Analyzer and Instrument Creator";
}

void AnalyzerPlugin::buildInterpolator(std::vector<Diginstrument::Dimension> dimensions)
{
	interpolator.clear();
	for(const auto & note : this->notes)
	{
		interpolator.addResidual(note.second);
		interpolator.addPartialSet(note.first);
	}
	interpolator.setDimensions(dimensions);
}

bool AnalyzerPlugin::analyzeSample(
	const QString &_audio_file,
	vector<pair<string, float>> coordinates,
	double partialCutoffParameter,
	double partialHeightCutoffParameter,
	double partialMinDistance,
	double residualCutoffParameter)
{
	m_sampleBuffer.setAudioFile(_audio_file);
	//tmp: make mono: left only
	std::vector<double> sample(m_sampleBuffer.frames());
	for (int i = 0; i < sample.size(); i++)
	{
		sample[i] = m_sampleBuffer.data()[i][0];
	}

	const PartialSet<float> partialSet(
		subtractiveAnalysis(sample, m_sampleBuffer.sampleRate(), partialCutoffParameter, partialHeightCutoffParameter, partialMinDistance),
		coordinates,
		m_sampleBuffer.sampleRate()
	);
	const Residual<float> residual(
		residualAnalysis(sample, m_sampleBuffer.sampleRate(), residualCutoffParameter),
		coordinates
	);

	//synthesize sample
	std::vector<float> synth(sample.size(), 0);
	//partials
	for (const auto &partial : partialSet.get())
	{
		for (int i = 0; i < partial.size(); i++)
		{
			synth[i] += cos(partial[i].phase) * partial[i].amplitude;
		}
	}
	//residual
	for (const auto &channel : residual.get())
	{
		for (const auto &frame : channel)
		{
			synth[frame.first] += cos(frame.second.phase) * frame.second.amplitude;
		}
	}

	//calculate error signal
	//tmp: mono: left only
	std::vector<float> errorSignal(sample.size(), 0);
	for (int i = 0; i < sample.size(); i++)
	{
		errorSignal[i] = m_sampleBuffer.data()[i][0] - synth[i];
	}
	for (auto &e : errorSignal)
	{
		e = pow(e, 2);
	}
	//print error metrics
	cout << "highest energy in error signal: " << *max_element(errorSignal.begin(), errorSignal.end()) << std::endl;
	cout << "mean energy in error signal: " << std::accumulate(errorSignal.begin(), errorSignal.end(), 0.0) / errorSignal.size() << std::endl;
	//tmp: print fundamental frequency estimation
	cout << "estimated fundamental frequency: " << partialSet.getFundamentalFrequency() << std::endl;

	//store note
	this->notes.push_back(std::make_pair(std::move(partialSet),	std::move(residual)));

	return true;
}

void AnalyzerPlugin::writeInstrumentToFile(std::string filename, std::vector<Diginstrument::Dimension> dimensions)
{
	ofstream file(filename);
	if (file.is_open())
	{
		Diginstrument::Instrument<float> inst;
		inst.dimensions = dimensions;
		for(const auto & note : this->notes)
		{
			inst.add(note.first);
			inst.add(note.second);
		}
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
							1.0f / (float)transform.getScale(j).second,
							unwrappedPhases[j][i],
							magnitudes[j][i])));
			}
		}
		if (channel.size() > 0)
			res.emplace_back(std::move(channel));
	}

	return res;
}

std::vector<std::vector<Diginstrument::Component<float>>> AnalyzerPlugin::subtractiveAnalysis(
	std::vector<double> &signal,
	unsigned int sampleRate,
	double minProminence,
	double heightThreshold,
	double minDistance)
{
	//calculate FFT magnitudes of the signal
	Diginstrument::FFT<float> fft(signal.size());
	const auto mags = fft(signal, m_sampleBuffer.sampleRate());
	//find peaks in FFT, indicating areas of significance
	const auto maxima = Extrema::Differential::maxima(
		mags.begin(), mags.end(), [minProminence](double fr) -> double { return minProminence; }, heightThreshold, minDistance);

	vector<float> phase(signal.size(), 0);
	vector<float> mag(signal.size(), 0);
	std::vector<std::vector<Diginstrument::Component<float>>> res;

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
			//Current theory is that the bins overlap, therefore some energy is counted multiple times
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
		unsigned int end = mag.size() - 1;
		//tmp
		const float partialCutoff = 0.0005;
		while (end > 0 && mag[end] < partialCutoff)
			end--;
		//tmp:
		cout << "resized partial " << fr << ": " << mag.size() << " -> " << end + 1 << endl;

		//construct partial
		vector<Diginstrument::Component<float>> partial(end + 1);
		for (int i = 0; i < end + 1; i++)
		{
			partial[i] = Diginstrument::Component<float>(fr, phase[i], mag[i]);
		}
		//add partial to set
		res.push_back(std::move(partial));
	}
	return res;
}
