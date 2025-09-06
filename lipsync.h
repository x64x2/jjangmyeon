/* 
lipsync.h
*/	

/**
@file lipsync.h
@brief declares classes and structures for automatic lipsync 

This file declares 4 classes:
- alignment_result. a data class holding the labels and timings returned by sapi
- lipsync. base class containing common processing.
- textbased_lipsync. aligns an audio file with a text transcript
- textless_lipsync. pulls phonemes and word time from an audio file 
*/

#ifndef _H_LIPSYNC
#define _H_LIPSYNC

// forwards 
class phoneme_estimator;

/**
    @brief This data class contains phoneme alignment/lipsync results

     lipsync and it's subclasses generate a list of alignment_result
	 objects.
	
	 The 'raw' form will contain the start and end time for each recognized
	 word, along with the orthography (the text of the word) and the
	 list of phonemes (alignment_result::m_phonemes)
	
	 alignment_result::m_phonemeEndTimes are not produced 
	 The actual phoneme times are estimated by this application. (see
	 lipsync::finalize_phoneme_alignment for details).

	 Applications can either use the raw word results or use the finalized
	 data.
	
  */
class alignment_result
{
public:
    /// start time in milliseconds of this alignment result
    long        m_msStart;
    /// end time in milliseconds of this alignment result
    long        m_msEnd;
    /// the text representing this result
    std::wstring m_orthography;
    /** @brief the phonemes representing this result 
		These are pulled from sapi. Each phoneme is a separate
		index in the phonemes array (for easier parsing) */
    std::vector<std::wstring> m_phonemes;
	std::vector<long> m_phonemeEndTimes;
};

/** @brief base class for lipsync
**/
class lipsync
{
public:
	/// default constructor
	lipsync();

     /// constructor with estimator object
    lipsync(phoneme_estimator* pEstimator);
    
    // destructor
    virtual ~lipsync();

    /// destroy objects
    virtual void close();

    /// this method performs common initialization of objects
    bool initializeObjects();

    /// this method loads the audio file into the ISPStream 
    bool loadAudio(const std::wstring& audioFile);

    /// retrieve the error string
    const std::wstring& getErrorString();

    /// returns true if subclass thinks we are finished with async lipsync
    virtual bool isDone() { return (m_bDone); }

	/// this method returns the current best phoneme alignment
	std::vector<alignment_result>& get_phoneme_alignment();

	/// pretties up the phoneme alignment
	virtual void finalize_phoneme_alignment();

	/// prints the current best results to the specified stream
    virtual void print_results(std::ostream& os);

    /** @brief pure virtual function implemented by subclasses<P>
        This will be called when sapi events occur. The processing
        will depend on the type of lipsync being performed. 
	**/
    virtual void callback() = 0;

	/**@brief converts a timestamp into a millisecond time
		@param ts - [in] microsecond time
		@return millisecond time 
	**/
    long time_to_milli(ULONGLONG ts)
    {
        return (long(ts / 10000));
    }
	/** @brief converts audio bytes into milliseconds using the sample rate of the audio stream 
		@param dwBytes - [in] byte position in the audio stream
		@return millisecond timing for the audio at the specified byte
	 **/
    long bytes_to_milli(DWORD dwBytes)
    {
        return (UINT)((dwBytes * 1000 )/ m_pWaveFmt->nAvgBytesPerSec); 
    }

protected:
    CComPtr<ISpRecognizer>		m_recog;
    /// the recognizer context COM object
    CComPtr<ISpRecoContext>		m_recogCntxt;
    /// the grammar COM object
    CComPtr<ISpRecoGrammar>		m_grammar;
    /// the phone converter object. Converts PHONEID into strings
    CComPtr<ISpPhoneConverter>	m_phnCvt;
    /// the audio source object
    CComPtr<ISpStream>			m_audioStream;


    /// wave format of the audio we are processing
    WAVEFORMATEX				*m_pWaveFmt;

	/**@brief the phoneme estimator used to heuristically 
	   spread phoneme timings across the word. Can be NULL.
	   The estimator is set by the constructor, where a pointer is passed in.
	   m_pPhnEstimator is only used in lipsync::finalize_phoneme_results.
	 **/
	phoneme_estimator			*m_pPhnEstimator;			

    /// error description. 
    std::wstring m_err;

	/// audio file path. Needed for printing .anno file
	std::wstring m_strAudioFile;

    /** @brief static method used to receive notifications from SAPI
     */
    static void _stdcall sapi_callback(WPARAM wParam, LPARAM lParam);


    /// results container
    std::vector<alignment_result> m_results;

    /// subclasses will set this when the lipsync is done.
    bool                          m_bDone;

};

class textbased_lipsync : public lipsync
{
public:
	/// constructor
	textbased_lipsync();

	/// constructor with estimator object
    textbased_lipsync(phoneme_estimator* pEstimator);

	/// destructor
	virtual ~textbased_lipsync();
    
    /// start the asyncronous lipsync process given a text file and an audio file
    virtual bool lipsync(const std::wstring& strAudioFile, const std::wstring& strText);


    /** @brief notifications from the sapi engine. */
    virtual void callback();

	/// override of sapi_lipsync::print_results
    virtual void print_results(std::ostream& os);

    // the method cleans the transription text for use in text based lipsync.
    static std::wstring preprocess_text(const std::wstring& in);
    
	/// the method filters a characters that cause performance degradation of
	/// text based processes
    static bool is_dirty_char(wchar_t in);

protected:
	/**@brief the raw text string results.
		This is used to decide whether or not to accept a hyphothesis. 
		If the result generates a longer string than m_strResults, we
		accept the hypothesis and generate phonemes, if not. we skip it */
    std::wstring                  m_strResults;

	/// input text. needed for printing anno file
	std::wstring				  m_strInputText;

};

/** @brief this class implements lipsync without a transcription

  This uses the Dictation grammar, and simply points the audio to it
  and waits for the results. 

  @see ::run_sapi_textless_lipsync for example code
  */
class textless_lipsync : public lipsync
{
public:
	/// constructor
	textless_lipsync();

	/// constructor with estimator object
    textless_lipsync(phoneme_estimator* pEstimator);

	/// destructor
	virtual ~textless_lipsync();
    
    /// start the asyncronous lipsync process given a text file and an audio file
    virtual bool lipsync(const std::wstring& strAudioFile);
    
    /** @brief notifications from the sapi engine. */
    virtual void callback();

};



