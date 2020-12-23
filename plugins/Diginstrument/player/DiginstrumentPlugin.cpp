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

DiginstrumentPlugin::DiginstrumentPlugin(InstrumentTrack *_instrument_track) : Instrument(_instrument_track, &diginstrument_plugin_descriptor) {}

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
	return instrument.name.c_str();
}

QString DiginstrumentPlugin::fullDisplayName() const
{
	return instrument.name.c_str();
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
	vector<float> audioData = this->synth.playPartials(partials, noteHandle->framesLeftForCurrentPeriod(), noteHandle->totalFramesPlayed(), /*TMP*/ 44100);
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

//TODO: note plugin data could be the place to assign coordinates to notes!
void DiginstrumentPlugin::deleteNotePluginData(NotePlayHandle *_note_to_play) {}

f_cnt_t DiginstrumentPlugin::beatLen(NotePlayHandle *_n) const
{
	//TODO: what does this do?
	return 0;
}

void DiginstrumentPlugin::sampleRateChanged() {}

bool DiginstrumentPlugin::setInstrumentFile(const QString & fileName)
{
	this->fileName = fileName.toStdString();
	return true;
}

/**
 * Construct the instrument from the file given with setInstrumentFile()
 **/
bool DiginstrumentPlugin::loadInstrumentFile()
{
	QFile file(QString{fileName.c_str()});
	if(file.open(QIODevice::ReadOnly))
	{
		QByteArray arr = file.readAll();
		file.close();
		//TODO: separate into loading from file and loading saved
		//TODO: read file piece-by-piece
		//TODO: exceptions?
		instrument = Diginstrument::Instrument<float>::fromJSON(ordered_json::parse(arr.toStdString()));
		//tmp: populate both instrument and interpolator
		interpolator.clear();
		interpolator.addResiduals(instrument.getResiduals());
		interpolator.addPartialSets(instrument.getPartialSets());
		interpolator.setDimensions(instrument.dimensions);
		return true;
	}
	else return false;
}