/* 
mate.h
*/	

/**
@file phone_estimate.h
@brief declaration of the phoneme timing estimator to improve results 
**/

#ifndef _H_PHONE_CVT
#define _H_PHONE_CVT

// forward declaration
class engine_phoneme_spec;

/**
@brief the phoneme estimator class is used to estimate the
timing of phonemes in a word.

The phoneme estimator abstracts that process. the software will still work
without a specification for all the phonemes in the engine, but it can't
use heuristics to generate the timing information, instead it will just
evenly spread the phonemes across the word. 

The default phoneme_estimator uses the English set. 

The phoneme estimator also has the ability to transform the phonemes
based on information in the engine_phoneme_spec. This is kind of a hack.
I want to transform the phonemes into the \ref anno40 so that they can
be better viewed in \ref lipsync_tool. Other implementations may want
to change this.
  
@todo
It would be nice to load the engine_phoneme_spec from an auxillary file
so that we wouldn't need to change the code to support other SAPI speech 
engines
@see ::run_textbased_lipsync for example code
@see ::run_textless_lipsync for example code
**/
class phoneme_estimator
{
public:
	/// default constructor. Assumes SAPI 5.1 english phoneme set
	phoneme_estimator();

	/// construct the phoneme estimator with the list of expected phonemes.
	phoneme_estimator(engine_phoneme_spec* pSpec);
	
	/// This method estimates the phoneme durations given an alignment result
	void EstimatePhonemeAlignment(alignment_result& align);

	/// This static method estimates the phoneme durations given an alignment result
	static void TrivialPhonemeAlignment(alignment_result& align);
protected:
	/// the list of phonemes we expect to see in EstimatePhonemeAlignment
	engine_phoneme_spec* m_pSpec;
};


/** 
@brief This data class describes an engine phoneme.

As described above, in order to produce better spreading of phonemes
within the word alignment, we can use heuristics, but in order to
use them, we need some information about the phoneme, whether it
is voiced, unvoiced, a dipthong, etc. These classes typically are
the same duration. By classifying the phoneme, we are able to do
a little better job. 
*/
class engine_phoneme_spec
{
public:
	/// engine phoneme
	std::wstring enginePhoneme;
	/// output phoneme
	std::wstring outputPhoneme;
	/// description
	std::wstring desc;

	/** @brief type of phoneme  
		
		When we go to pick durations for phonemes we take the
		type of phoneme into account, generally unvoiced phonemes
		have the shortest hold, followed by voiced phonemes, and
		dipthongs generally hold the longest */
	typedef enum phoneme_type
	{
		silence,	///< phoneme is a silence phoneme
		unvoiced,	///< phoneme is an unvoiced phoneme
		voiced,		///< phoneme is a voiced phoneme
		dipthong	///< phoneme is a dipthong (two voiced sounds)
	};
	/// the type of phoneme one of engine_phoneme_spec::phoneme_type
	phoneme_type m_type;

	/// paramaterized constructor
	engine_phoneme_spec(const wchar_t * _enginePhn, wchar_t * _outputPhn, wchar_t* _desc, phoneme_type _pt)
	{
		enginePhoneme = _enginePhn;
		outputPhoneme = _outputPhn;
		desc = _desc;
		m_type = _pt;		
	}
};
