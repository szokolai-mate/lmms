#include "DiginstrumentPlugin.h"

using namespace QtDataVisualization;

extern "C"
{

	Plugin::Descriptor PLUGIN_EXPORT diginstrument_plugin_descriptor =
		{
			STRINGIFY(PLUGIN_NAME),
			"Diginstrument",
			QT_TRANSLATE_NOOP("pluginBrowser",
							  "WIP"
							  "Test"),
			"Máté Szokolai",
			0x0110,
			Plugin::Instrument,
			new PluginPixmapLoader("logo"),
			NULL,
			NULL};

	// necessary for getting instance out of shared lib
	PLUGIN_EXPORT Plugin *lmms_plugin_main(Model *model, void *)
	{
		return new DiginstrumentPlugin(static_cast<InstrumentTrack *>(model));
	}
}

DiginstrumentPlugin::DiginstrumentPlugin(InstrumentTrack *_instrument_track) : Instrument(_instrument_track, &diginstrument_plugin_descriptor)
{
	/*TODO */
}

DiginstrumentPlugin::~DiginstrumentPlugin() {}

PluginView *DiginstrumentPlugin::instantiateView(QWidget *_parent)
{
	return new DiginstrumentView(this, _parent);
}

void DiginstrumentPlugin::loadSettings(const QDomElement &_this)
{
	setInstrumentFile(_this.attribute("fileName"));
	loadInstrumentFile();
}

void DiginstrumentPlugin::saveSettings(QDomDocument &_doc, QDomElement &_this)
{
	_this.setAttribute("fileName", fileName.c_str());
}

QString DiginstrumentPlugin::nodeName() const
{
	return "TEST";
}

void DiginstrumentPlugin::playNote(NotePlayHandle *noteHandle,
								   sampleFrame *_working_buf)
{
	/*TMP*/
	//const double startTime = noteHandle->totalFramesPlayed() / (double)Engine::mixer()->processingSampleRate();
	vector<float> coordinates = {noteHandle->frequency()};
	//tmp:
	//cout<<"fr: "<<coordinates.front()<<endl;
	//TODO: first coordinate is freq, might not be correct?
	coordinates.reserve(this->coordinates.size()+2);
	for(auto c : this->coordinates)
	{
		coordinates.emplace_back(c);
	}
	auto partials = interpolator.getPartials(coordinates, noteHandle->totalFramesPlayed(), noteHandle->framesLeftForCurrentPeriod());
	//coordinates.emplace_back(startTime);
	auto residual = interpolator.getResidual(coordinates, noteHandle->totalFramesPlayed(), noteHandle->framesLeftForCurrentPeriod());
	vector<float> audioData = this->synth.playNote(partials, noteHandle->framesLeftForCurrentPeriod(), noteHandle->totalFramesPlayed(), /*TMP*/ 44100);
	//vector<float> audioData(noteHandle->framesLeftForCurrentPeriod(),0);
	vector<float> residualData = this->synth.playResidual(residual, noteHandle->framesLeftForCurrentPeriod(), noteHandle->totalFramesPlayed(), /*TMP*/ 44100);

	//tmp
	for(int i = 0; i<residualData.size(); i++)
	{
		audioData[i]+=residualData[i];
	}

	/*tmp: stereo*/
	unsigned int counter = 0;
	unsigned int offset = noteHandle->noteOffset();
	for (auto frame : audioData)
	{
		_working_buf[counter + offset][0] = _working_buf[counter + offset][1] = frame;
		counter++;
	}
	applyRelease(_working_buf, noteHandle);
	instrumentTrack()->processAudioBuffer(_working_buf, audioData.size() + noteHandle->noteOffset(), noteHandle);
}

void DiginstrumentPlugin::deleteNotePluginData(NotePlayHandle *_note_to_play)
{
}

f_cnt_t DiginstrumentPlugin::beatLen(NotePlayHandle *_n) const
{
	return 0;
}

QString DiginstrumentPlugin::fullDisplayName() const
{
	return "TEST";
}

void DiginstrumentPlugin::sampleRateChanged()
{
	/*TODO*/
}

bool DiginstrumentPlugin::setInstrumentFile(const QString & fileName)
{
	this->fileName = fileName.toStdString();
	return true;
}

/**
 * Construct the instrument from the file given with setInstrumentFile()
 **/
//TODO: what fields to populate, information to display
bool DiginstrumentPlugin::loadInstrumentFile()
{
	QFile file(QString{fileName.c_str()});
	if(file.open(QIODevice::ReadOnly))
	{
		QByteArray arr = file.readAll();
		file.close();
		//TODO: separate into loading from file and loading saved
		//TODO: catch?
		instrument = Diginstrument::Instrument<float>::fromJSON(ordered_json::parse(arr.toStdString()));
		//tmp:
		interpolator.clear();
		interpolator.addResiduals(instrument.getResiduals());
		interpolator.addPartialSets(instrument.getPartialSets());
		interpolator.setDimensions(instrument.dimensions);
		return true;
	}
	else return false;
}

QtDataVisualization::QSurfaceDataArray * DiginstrumentPlugin::getInstrumentSurfaceData(float minTime, float maxTime, float minFreq, float maxFreq, int timeSamples, int freqSamples, std::vector<float> coordinates)
{
	//TODO: better/refactoring
	coordinates.push_back(0);

	const float stepX = (maxFreq - minFreq) / float(freqSamples - 1);
    const float stepZ = (maxTime - minTime) / float(timeSamples - 1);

	QSurfaceDataArray * data = new QSurfaceDataArray;
	data->reserve(timeSamples);
	for(int i = 0; i<timeSamples;i++)
	{
		QSurfaceDataRow *dataRow = new QSurfaceDataRow(freqSamples);
		float z = qMin(maxTime, (i * stepZ + minTime));
		unsigned int startFrame = /*TMP: sampleRate*/ 44100*z;
		//coordinates.back() = z;
		
		int index = 0;
		//TODO: fix after residual change
		//const auto spectrum = interpolator.getSpectrum(coordinates);
		/*const auto residualSlice = interpolator.getResidual(coordinates, startFrame, 1);
		for (int j = 0; j < freqSamples; j++) {
			float x = qMin(maxFreq, (j * stepX + minFreq));
			(*dataRow)[index++].setPosition(QVector3D(x, 0, z));
		}
		if(residualSlice.size()>0)
		{
			for(const auto & c : residualSlice.get().front().second)
			{
				if(c.frequency<=minFreq || c.frequency>=maxFreq) continue;
				(*dataRow)[std::round((c.frequency-minFreq)/((maxFreq-minFreq)/(float)freqSamples))].setPosition(QVector3D(c.frequency,c.amplitude, z));
			}
		}*/
		*data<<dataRow;
	}

	return data;
}

std::vector<Diginstrument::Component<float>> DiginstrumentPlugin::getPartialVisualization(float minTimeMilisec, float maxTimeMilisec, float minFreq, float maxFreq, int pointsPerSeconds, std::vector<float> coordinates)
{
	std::vector<Diginstrument::Component<float>> res;
	//TODO: sample rate is needed here
	const int sampleRate = 44100;
	const auto partials = interpolator.getPartials(coordinates, (minTimeMilisec/1000.0)*sampleRate, ((maxTimeMilisec-minTimeMilisec)/1000.0)*sampleRate).get();
	for(const auto & partial : partials)
	{
		for(int i = 1; i<partial.size(); i+=sampleRate/pointsPerSeconds)
		{
			
			const double freq = abs(((partial[i].phase - partial[i-1].phase) * sampleRate) / (2*M_PI));
			//pretty bad, i use phase as time
			res.emplace_back(freq, (double)i/(double)sampleRate, partial[i].amplitude);
		}
	}
	return res;
}