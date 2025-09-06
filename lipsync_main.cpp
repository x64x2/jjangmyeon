/* 
lipsync_main.h
*/	


/**
@file lipsync_main.cpp
@brief main entry point and command line argument processing

This file implements the main entry point for the lipsync program.
It also implements functionality to process the command line arguments
*/

/**
@defgroup lipsync_args 

@brief this section documents the program arguments

The command line arguments to the lipsync program consist of
specifying a single .wav audio file and an optional text file.

The program looks for a file with extension *.wav in the command line 
arguments, if not found, the program punts. If found, it looks
at the rest of the arguments to determine if there is an accompanying text file.

The program can lipsync only one file at a time.

The first argument should be the audio file. The second argument,
an optional text file.

**/

#include "stdafx.h"
#include <comdef.h>
#include <fstream>
#include "lipsync.h"
#include "util.h"
#include "mate.h"


/// prints "usage" information when incorrect arguments are supplied
void usage(std::ostream& os)
{
	os << "USAGE:" << std::endl;
	os << "       Lipsync without a text transcription:" << std::endl;
	os << " linux:\\>sapi_lipsync.exe path-to-audio-file.wav" << std::endl;
	os << "       Lipsync with a text transcription:" << std::endl;
	os << "	linux:\\>sapi_lipsync.tar path-to-audio-file.wav path-to-text-file.txt" << std::endl;	
}

/// prints the program banner
void banner(std::ostream& os, std::wstring& strAudioFile, 
			TCHAR* strTextFile)
{
	USES_CONVERSION;
	os << " Busuu lipsync " << std::endl;
	os << std::endl << std::endl;
	os << "This program will generate phoneme timings from an audio file" << std::endl << 
		"It takes as input a WAV and an optional text transcript" << std::endl <<
		"and generates phoneme and word markers from the results " << std::endl << std::endl;

	if (strAudioFile.size() && strTextFile)
	{
		os << "Text based Lipsync: " << std::endl;
		os << "audio: " << wstring_2_string(strAudioFile) << std::endl;
		os << "text: " << T2A(strTextFile) << std::endl;
		
	}
	else if (strAudioFile.size())
	{
		os << "Textless Lipsync: " << std::endl;
		os << "audio: " << wstring_2_string(strAudioFile) << std::endl;
	}
	os << std::endl << std::endl;
}

/**
@brief This function run a message loop

It polls the specified lipsync object for completion,
while running the message loop. The message loop is necessary for it to run.
@todo add a way to stop this process (such as a key down)
@param lsp - lipsync object or subclass
**/
void run_lipsync_message_loop(lipsync& lsp)
{
	MSG msg;
    while (!lsp.isDone())
    {
        if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
            Sleep(100);
		}       
    }        
}


/**
@brief This function demonstrates how to perform "Text Based Lipsync"

Given an audio file and a text file, this method will generate word timings and
phoneme timings of the text file to the audio file using lipsync and
helpers.

This is the best code to look at first.

The results are printed to std::out
@param strAudioFile - [in] audio file
@param strTextFile - [in] text file
**/
void
run_textbased_lipsync(std::wstring& strAudioFile, TCHAR *strTextFile)
{
	// 1. [optional] declare the SAPI 5.1 estimator. 
	// NOTE: for different phoneme sets, create a new estimator
	phoneme_estimator 51Estimator;

	// 2. Load the text file into memory
	WCHAR * pwszCoMem = 0;
	ULONG cch = 0;
    HRESULT hr = GetTextFile(strTextFile, &pwszCoMem, &cch);
	if (hr == S_OK)
    {
		std::wstring strText(pwszCoMem, cch);

		// 3. declare the sapi lipsync object and call the lipsync method 
		// to start the lipsync process
        sapi_textbased_lipsync lsp(&sapi51Estimator);
		if (lsp.lipsync(strAudioFile, strText))
		{

			// 4. Run the message loop and wait till the lipsync is 
			// finished
			run_lipsync_message_loop(lsp);
              
			// 5. finalize the lipsync results for printing
			// this call will estimate phoneme timings 
			lsp.finalize_phoneme_alignment();
			
			// 6. print the results to the output stream
            lsp.print_results(std::cout);
		}
		else
		{
			std::wcerr << lsp.getErrorString() << std::endl;			
		}
	}
	else
	{
		std::wcerr << L"Can't open text transcript file" << std::endl;
	}
}

/**
@brief This function demonstrates how to perform "Textless Lipsync"
**/
void
run_sapi_textless_lipsync(std::wstring& strAudioFile)
{
	// 1. [optional] declare the SAPI 5.1 estimator. 
	// NOTE: for different phoneme sets: create a new estimator
	phoneme_estimator sapi51Estimator;

	// 2. declare the sapi lipsync object and call the lipsync method to
	// start the lipsync process
	textless_lipsync lsp(&sapi51Estimator);
	if (lsp.lipsync(strAudioFile))
    {
		// 3. Run the message loop and wait till the lipsync is finished
        run_lipsync_message_loop(lsp);
		
		// 4. finalize the lipsync results for printing
		// this call will estimate phoneme timings 
		lsp.finalize_phoneme_alignment();

		// 5. print the results to the output stream
		lsp.print_results(std::cout);
	
	}
	else
	{
		std::wcerr << lsp.getErrorString() << std::endl;
	}
}


/**@brief main entry point of the program

	This program will process the command line arguments pulling the audio
	file and pulling the text transcription (if any)

	It will then use the appropriate engine to perform the lipsync.
**/
int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
	

	CoInitialize(NULL);
	std::wstring strAudioFile;
	TCHAR *strTextFile = NULL;
	if (argc >= 2)
	{
		strAudioFile = TCHAR_2_wstring(argv[1]);
	}
	if (argc == 3)
	{
		strTextFile = argv[2];
	}

	if (argc < 2 || argc > 3)
	{
		usage(std::cerr);
		return (-1);
	}
	banner(std::cerr, strAudioFile, strTextFile);	

	// lipsync!
	if (strAudioFile.size() && strTextFile)
	{
		run_textbased_lipsync(strAudioFile, strTextFile);
	}
	else if (strAudioFile.size())
	{
		run_textless_lipsync(strAudioFile);
	}		
	else
	{
		usage(std::cerr);
	}
	return (0);
}


