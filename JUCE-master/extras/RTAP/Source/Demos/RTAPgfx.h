#ifndef __RTAPGFX_H__
#define __RTAPGFX_H__

#include "nconventions.h"
#include <complex>
#include <math.h>
#include <stdlib.h>
#include "globalvars.h"
#include <process.h>
#include "RTAPcompute.h"

class RTAPgfx :	public Component,
				public Timer,
				public RTAPcompute
{
public:
	RTAPgfx();
	~RTAPgfx();

	void paint (Graphics& g);

	void DrawSpectrogramColourBar (UINT, UINT);
	void timerCallback ();
	Image Plot2Display;
private:
	// POSIX threads declarations
	static void* StartRenderThreads (void*);
	void* DrawPlotPixels ();
	void DrawLine (float, float, float, float, UINT, UINT);
	void UpdateXaxis ();

	int nextSample;

	Colour c[34];
	bool bInitialComputation;
#if SINGLE_PRECISION
	float fColourDiscreteLevels[NUMOFCOLOURBANDS-1], fColourDiscreteStep;
#elif DOUBLE_PRECISION
	double fColourDiscreteLevels[NUMOFCOLOURBANDS-1], fColourDiscreteStep;
#endif

	RTAPgfx (const RTAPgfx&);
	RTAPgfx& operator= (const RTAPgfx&);
};

#endif