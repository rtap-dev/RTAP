#include <Windows.h>
#include "RTAPgfx.h"

using namespace std;

RTAPgfx::RTAPgfx()
{	
	bInitialComputation = true;

	nextSample = 0;

#if 0
#if RTAP_REALTIME_PRIORITY_BUILD
	// Set RTAP to real-time priority process
	SetPriorityClass ( GetCurrentProcess(), REALTIME_PRIORITY_CLASS );
#elif RTAP_HIGH_PRIORITY_BUILD
	// Set RTAP to high priority process
	SetPriorityClass ( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
#elif RTAP_ABOVE_NORMAL_PRIORITY_BUILD
	// Set RTAP to above normal priority process
	SetPriorityClass ( GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS );
#else
	// Set RTAP to normal priority process
	SetPriorityClass ( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );
#endif
#endif

	/*
		Colour c#(Hue, Saturation, Brightness, Alpha)
		- Hue controls colour variation (0.0 - red, 0.15 - yellow, 0.25 - green, 0.5 - blue, 0.75 - purple
		- Saturation controls the purity of the colour (0.0 - original colour, 1.0 - grayscale colour dependent on brightness index i.e. 0.0 - black, 1.0 0 white)
		- Brightness is the measure of quantity of greyscale in the colour (0.0 - black: other 2 index useless, 1.0 - white / colour (depend on H & S)
		- Alpha is the transperancy of the colour. (0.0 - fully transparent, 0.5 - translucent (semi-transparent), 1.0 - Fully opaque)
	*/
	// Colour band defining high intensity signal i.e. red to low intensity i.e. yellow
	// Arrangement => #new num - #old num
	c[0] = Colour (0.66f, 1.0f, 0.35f, 1.0f);		
	c[1] = Colour (0.66f, 1.0f, 0.4f, 1.0f);		// 1:29-blue (dark transparent)
	c[2] = Colour (0.66f, 1.0f, 0.6f, 1.0f);		// 2:9
	c[3] = Colour (0.66f, 1.0f, 0.8f, 1.0f);		// 3-18
	c[4] = Colour (0.66f, 1.0f, 1.0f, 1.0f);		// 4-27
	c[5] = Colour (0.64f, 1.0f, 1.0f, 1.0f);		// 5-7
	c[6] = Colour (0.62f, 1.0f, 1.0f, 1.0f);		// 6-16
	c[7] = Colour (0.60f, 1.0f, 1.0f, 1.0f);		// 7:19
	c[8] = Colour (0.58f, 1.0f, 1.0f, 1.0f);		// 8-28
	c[9] = Colour (0.55f, 1.0f, 1.0f, 1.0f);		// 9-8
	c[10] = Colour (0.54f, 1.0f, 1.0f, 1.0f);		// 10-17
	c[11] = Colour (0.50f, 1.0f, 1.0f, 1.0f);		// 11-26
	c[12] = Colour (0.46f, 1.0f, 1.0f, 1.0f);		// 12-6
	c[13] = Colour (0.42f, 1.0f, 1.0f, 1.0f);		// 13-15
	c[14] = Colour (0.38f, 1.0f, 1.0f, 1.0f);		// 14-25
	c[15] = Colour (0.34f, 1.0f, 1.0f, 1.0f);		// 15-5
	c[16] = Colour (0.30f, 1.0f, 1.0f, 1.0f);		// 16-14
	c[17] = Colour (0.26f, 1.0f, 1.0f, 1.0f);		// 17-23
	c[18] = Colour (0.24f, 1.0f, 1.0f, 1.0f);		// 18-3
	c[19] = Colour (0.22f, 1.0f, 1.0f, 1.0f);		// 19-12
	c[20] = Colour (0.20f, 1.0f, 1.0f, 1.0f);		// 20-21
	c[21] = Colour (0.18f, 1.0f, 1.0f, 1.0f);		// 21-1
	c[22] = Colour (0.16f, 1.0f, 1.0f, 1.0f);		// 22-24
	c[23] = Colour (0.14f, 1.0f, 1.0f, 1.0f);		// 23-4
	c[24] = Colour (0.12f, 1.0f, 1.0f, 1.0f);		// 24-13
	c[25] = Colour (0.10f, 1.0f, 1.0f, 1.0f);		// 25-22
	c[26] = Colour (0.08f, 1.0f, 1.0f, 1.0f);		// 26-2
	c[27] = Colour (0.06f, 1.0f, 1.0f, 1.0f);		// 27-11
	c[28] = Colour (0.04f, 1.0f, 1.0f, 1.0f);		// 28:20-red (1/3 transparent)
	c[29] = Colour (0.02f, 1.0f, 1.0f, 1.0f);		// 29:10-red (2/3 opaque)
	c[30] = Colour (0.00f, 1.0f, 1.0f, 1.0f);		// 30:0-red (opaque)
	c[31] = Colour (0.0f, 0.0f, 0.0f, 0.0f);

	// Colour test
	c[32] = Colour (0.0f, 0.0f, 1.0f, 0.5f);
	c[33] = Colour (0.0f, 0.0f, 1.0f, 0.1f);

	Plot2Display = Image (Image::ARGB, IMGBUFFSIZE, IMGHEIGHT, true);

#if GFX_THREAD_ENABLE
	pthread_create(&hDrawPixThread[0], NULL, &RTAPgfx::StartRenderThreads, this);
#endif

	// load the timer counter
	pEar->uiTimerCount = 3000 / TIMEDURATION;

	// Set timer to 58 msec to accomodate 1280 samples for fs=22.05kHz
	startTimer(TIMEDURATION);
}

RTAPgfx::~RTAPgfx()
{
}

void RTAPgfx::paint(Graphics& g)
{
#if DISPLAY_PROCTIME
	dGfxPaintStartTime = Time::getMillisecondCounterHiRes();
#endif

	siDisplayWindowWidth = getWidth() + uiScrollSpeed;

	if (bPlayPauseStatus)
	{
		// first of all resume the Processing thread(s) - Multi-threaded algo
		pthread_mutex_lock (&PlayPauseMutex);
		pthread_cond_broadcast (&cvWaitOnPlayButton);
		pthread_mutex_unlock (&PlayPauseMutex);

		if (cAudioSource != AUDIO_SYSTEM)
		{
			pthread_mutex_lock (&ComputeAlgoMutex);
			pthread_cond_signal (&cvComputeAlgo);
			pthread_mutex_unlock (&ComputeAlgoMutex);
		}
	}

#if DISPLAY_PROCTIME
	dGfxPaintMidTime = Time::getMillisecondCounterHiRes();
#endif

	// signal background colour
	g.fillAll(Colours::black);

#if GFX_THREAD_ENABLE
	if (bScrollPlot)
	{
		// draw plot from left side of display window 1st
		g.drawImage (Plot2Display, siDisplayStartPoint, 0, getWidth(), IMGHEIGHT, uiDisplayDestX, 0, getWidth(), IMGHEIGHT);

		if ((IMGBUFFSIZE - uiDisplayDestX) < getWidth())
		{
			g.drawImage (Plot2Display, (IMGBUFFSIZE - uiDisplayDestX), 0, getWidth(), IMGHEIGHT, 0, 0, getWidth(), IMGHEIGHT);
		}

#if _DEBUG || VAR_FB
		uiGlobalShow2 = uiDisplayDestX;
		uiGlobalShow3 = siNumProcData2Display;//siDisplayStartPoint;
#endif
	}
	else
		g.drawImage (Plot2Display, 0, 0, getWidth(), IMGHEIGHT, 0, 0, getWidth(), IMGHEIGHT);
#endif

	// only increment counting semaphore when 'Play' button clicked
	if (bPlayPauseStatus)
	{
		int i;

		// erase the waveforms as it comes directly out from the right of the window (to accomodate y-axis labels)
		g.setColour (Colours::black);
		g.fillRect (1090, 0, getWidth()-1090, getHeight());
		// draw the y-axis labels
		g.setColour (Colours::aquamarine);
		for (i=0; i<uiNumYaxisLabels; i++)
			g.drawSingleLineText(cYaxisLabel[i], 1090, uiBFlabelYaxis[i]);
		// draw the x-axis labels
		for (i=0; i<NUMOFXLABELS; i++)
			g.drawSingleLineText(cXaxisLabel[i], uiTimeLabelXaxis[i], 485);
		
		dPlotDispSem++;
	}

#if DISPLAY_PROCTIME
	dGfxPaintEndTime = Time::getMillisecondCounterHiRes();
	dGfxPaintFuncTime = dGfxPaintEndTime - dGfxPaintStartTime;
	dGfxPaintTime = dGfxPaintEndTime - dGfxPaintMidTime;
	dSummedGfxPaintTime += dGfxPaintTime;
	dNumGfxPaintInvk++;
#endif
}

void* RTAPgfx::StartRenderThreads ( void* obj )
{
	reinterpret_cast<RTAPgfx*>(obj)->DrawPlotPixels();

	return 0;
}

void* RTAPgfx::DrawPlotPixels()
{
	UINT j, m, n, x, y, yAud, xWidth;
	// test variables for scroll
	UINT uiImgBuffReadTrack;

	// Line draw init start
	int i, j0;
	float x1, x2, y1, y2, delx, dely;
	float m1, error;

	x1 = x2 = 0;
	y1 = y2 = 0;

	while (bRTAPrunning)
	{
		pthread_mutex_lock (&DrawPlotMutex);
		pthread_cond_wait (&cvDrawPlot, &DrawPlotMutex);
		pthread_mutex_unlock (&DrawPlotMutex);

#if DISPLAY_PROCTIME
		dPlotRenderStartTime = Time::getMillisecondCounterHiRes();
#endif

		uiScrollSpeed = 12;//siNumProcData2Display;

		bRenderPlotInProgress = true;

		// check for RTAP configuration
		if (bRTAPsetBtnDispSync)
		{
			uiImgSrcX = uiImgBuffReadTrack = uiScreenStartRender = uiDestX = 0;
		}

		if (dPixRenderSem != dAlgoProcSem)
		{
			if (bScrollPlot && !bAutoRefreshPlot)
			{
				// calculate waveform display if starting point is x!=0
				if (siDisplayStartPoint > 0)
				{
					// only calculate for the time when display start not reached x=0
					siDisplayStartPoint -= uiScrollSpeed;
					//  move virtual end tracker to the left of screen while maintaining the virtual start tracker at the start (right)
					uiDestWidth = 1276;
				}
				else
				{
					siDisplayStartPoint = 0;
					uiDestWidth = 1276;
//					if (uiDestWidth < IMGBUFFSIZE)
//						uiDestWidth += uiScrollSpeed;
				}

				xWidth = 1276;//jmin ( siNumProcData2Display, getWidth());

				// scrolling spectrogram render segment
				if (cWaveformType == SPECTROGRAM_PLOT)
				{
#if 0
					// since there are 30 discrete colour band levels set aside, calculate each band level
					fColourDiscreteStep = (fMaxPixDisplayVal - fMinPixDisplayVal) / (NUMOFCOLOURBANDS-1);

					for (m=0; m<NUMOFCOLOURBANDS-1; m++)
					{
						// Handle the colour band size here
						if (!m)
							// if 1st 1, then simply assign the step size
							fColourDiscreteLevels[m] = fMinPixDisplayVal;
						else
							// keep incrementing and store them in array
							fColourDiscreteLevels[m] = fColourDiscreteLevels[m-1] + fColourDiscreteStep;
					}

					for (x=0; x<=xWidth; x+=PIXELWIDTH)
					{
						uiImgBuffReadTrack = (x + uiImgSrcX) % IMGTEMPSTORESIZE;
						uiDestX = (uiScreenStartRender + (x/*<<1*/)) % IMGBUFFSIZE;

						// Erase a segment of the image buffer so that there is no overlap of image when the buffer is circulated around
						for (n=IMGHEIGHT; n>0; n--)
						{
							Plot2Display.setPixelAt ( uiDestX, n, c[31] );
						}

						for (j=0; j<pEar->uiNumBFchannels; j++)
						{
							// variable used in the setting of pixels in images
							y = uiYstart[0] - (j*PIXELHEIGHT);

							for (m=0; m<NUMOFCOLOURBANDS; m++)
							{
								// handle anything lower than the 1st colour band
/*								if ((fImgBuff[j][uiImgBuffReadTrack] < fColourDiscreteLevels[m]) && !m)
								{
									Plot2Display.setPixelAt(uiDestX, y, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y, c[m]);

									Plot2Display.setPixelAt(uiDestX, y-1, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y-1, c[m]);

									Plot2Display.setPixelAt(uiDestX, y-2, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y-2, c[m]);

									Plot2Display.setPixelAt(uiDestX, y-3, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y-3, c[m]);
								}
								else */if ((fImgBuff[j][uiImgBuffReadTrack] >= fColourDiscreteLevels[m-1]) && (fImgBuff[j][uiImgBuffReadTrack] < fColourDiscreteLevels[m]))
								{
									Plot2Display.setPixelAt(uiDestX, y, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y, c[m]);

									Plot2Display.setPixelAt(uiDestX, y-1, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y-1, c[m]);

									Plot2Display.setPixelAt(uiDestX, y-2, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y-2, c[m]);

									Plot2Display.setPixelAt(uiDestX, y-3, c[m]);
									Plot2Display.setPixelAt(uiDestX+1, y-3, c[m]);
								}
							}
						}
					}

					uiScreenStartRender = (uiScreenStartRender + uiScrollSpeed) % IMGBUFFSIZE;
					siNumProcData2Display = 0;

					// update variable only when Play button is active & img buff has reached the left end of display window
					if (bPlayPauseStatus && (siDisplayStartPoint == 0))
						uiDisplayDestX = (uiDisplayDestX + uiScrollSpeed) % IMGBUFFSIZE;
#endif
				}
				// scrolling scatter & line plot render segment
				else
				{
					for (x=0; x<=xWidth; x++)
					{
						uiImgBuffReadTrack = (x + uiImgSrcX) % IMGTEMPSTORESIZE;
						uiDestX = (uiScreenStartRender + x) % IMGBUFFSIZE;

						// Erase a segment of the image buffer so that there is no overlap when the buffer is circulated around
						for (n=IMGHEIGHT; n>0; n--)
							Plot2Display.setPixelAt (uiDestX, n, c[31]);

						// compute y-coordinate for scrolling auditory stimulus
						yAud = (YENDPLOT - 400) - ((fAudioImgBuff[uiImgBuffReadTrack] + 1) * uiYspacing / 2);

						// Render pixel for ERB scaled plots 
						for (j=0; j<pEar->uiNumBFchannels; j++)
						{
							// formula to define the magnitude of the y-coordinate of the processed data
//							y = uiYstart[j] - ((fImgBuff[j][uiImgBuffReadTrack] - fMinDisp[j]) * uiYspacing / (fMaxDisp[j] - fMinDisp[j]));

							// this conditional check is required to correct the occasional 0 that comes out of IHCRP, NRR & ANSP function
							// ----->>>>> THIS IS ONLY VALID FOR LSR CHANNEL FOR NRR & ANSP. NEED TO ACCOMODATE HSR AS WELL!!! <<<<<-----
							if ((fImgBuff[j][uiImgBuffReadTrack] == 0.f) && (iFunctionToRun == DRNL2IHCRP_ST_OPT))
								fImgBuff[j][uiImgBuffReadTrack] = LOAXIS_IHCRP;
							else if ((fImgBuff[j][uiImgBuffReadTrack] == 0.f) && (iFunctionToRun == DRNL2IHCpresynapse_ST_OPT))
								fImgBuff[j][uiImgBuffReadTrack] = LOAXIS_NRR_LSR;
							else if ((fImgBuff[j][uiImgBuffReadTrack] == 0.f) && (iFunctionToRun == DRNL2ANsynapse_ST_OPT))
								fImgBuff[j][uiImgBuffReadTrack] = LOAXIS_ANSP_LSR;

							// read the data out and translate to onscreen y-coord (x-coord is an increment)
							y = uiYstart[j] - ((fImgBuff[j][uiImgBuffReadTrack] - fMinPixDisplayVal) * uiYspacing / (fMaxPixDisplayVal - fMinPixDisplayVal));
							
							// Noisy spike eliminator: Set fY to be the same as y so that there are no spike displays during image display startup
							if (bRTAPsetBtnDispSync)
							{
								// store y so that next instance renderer is called, plot draws from where it is left off instead of y = 0
								fPrevYposProcData[j] = y;
								// when handling last channel, reset noisy spike eliminator
								if (j == pEar->uiNumBFchannels-1)
									bRTAPsetBtnDispSync = false;		// reset RTAP set boolean flag to allow wave display update
							}

							// window boundary draw
							if (bWindowBoundaryDisp && (x == 0))
							{
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+4, c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+3, c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+2, c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+1, c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1), c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-1, c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-2, c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-3, c[16]);
								Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-4, c[16]);
							}

							// peak BF boundary draw for every window
							if (bPeakAmpDisp && ((uiMeanPeakBF[0] - 1) == j))
							{
								if ((x == 0) || (x == xWidth))
								{
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+4, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+3, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+2, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+1, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1), c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-1, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-2, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-3, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-4, c[15]);
								}
								else
								{
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)+4, c[15]);
									Plot2Display.setPixelAt (uiDestX, uiYstart[j]-(uiYspacing>>1)-4, c[15]);
								}
							}

							if (cWaveformType == SCATTER_PLOT)
							{
								Plot2Display.setPixelAt (uiDestX, yAud, c[15]);		// Draw audio signal
								Plot2Display.setPixelAt (uiDestX, y, c[21]);		// Draw processed data
							}
							// Bresenham line draw algo start for scroll display
							else if (cWaveformType == LINE_PLOT)
							{
								// Draw audio data signal line
								if (!j)
									DrawLine (fPrevXposAudData, fPrevYposAudData, uiDestX, yAud, YENDPLOT-400, 15);
								// Draw processed data line
								DrawLine (fPrevXposProcData[j], fPrevYposProcData[j], uiDestX, y, uiYstart[j], 21);
							}
							// assign start coord for next render
							fPrevXposAudData = uiDestX;
							fPrevYposAudData = yAud;
							fPrevXposProcData[j] = uiDestX;
							fPrevYposProcData[j] = y;
							// Bresenham line draw algo end
						}
					}

					uiScreenStartRender = (uiScreenStartRender + uiScrollSpeed) % IMGBUFFSIZE;
					siNumProcData2Display = 0;

					if (bPlayPauseStatus && (siDisplayStartPoint == 0))
						uiDisplayDestX = (uiDisplayDestX + uiScrollSpeed) % IMGBUFFSIZE;
#if _DEBUG || VAR_FB
					uiGlobalShow1 = uiDestX;
					uiGlobalShow4 = uiScreenStartRender;
					uiGlobalShow5 = uiImgBuffReadTrack;
					uiGlobalShow6 = uiImgSrcX;
#endif
				}
				UpdateXaxis();
			}
			// Static plot display
			else
			{
				xWidth = 1276;//jmin ( siNumProcData2Display, getWidth() );

				// Auto refresh plot display (ensure old plot is erased before rendering new one)
				if ((!bScrollPlot && bAutoRefreshPlot) || bErasePlot)
				{
					// Erase a segment of the image buffer so that there is no overlap when the buffer is circulated around
					Plot2Display.clear (juce::Rectangle<int>(proportionOfWidth (0.f),proportionOfHeight (0.f), proportionOfWidth (1.f),proportionOfHeight (1.f)), c[31]);

					bErasePlot = false;			// ok done with background erasing. Now let us draw...

					UpdateXaxis();
				}

				if (cWaveformType == SPECTROGRAM_PLOT)
				{
					// since there are 30 discrete colour band levels set aside, calculate each band level
					fColourDiscreteStep = (fMaxPixDisplayVal - fMinPixDisplayVal) / (NUMOFCOLOURBANDS-1);

					for (m=0; m<NUMOFCOLOURBANDS-1; m++)
					{
						// Handle the colour band size here
						if (!m)
							// if 1st 1, then simply assign the step size
							fColourDiscreteLevels[m] = fMinPixDisplayVal;
						else
							// keep incrementing and store them in array
							fColourDiscreteLevels[m] = fColourDiscreteLevels[m-1] + fColourDiscreteStep;
					}

					for (x=0; x<xWidth; x+=PIXELWIDTH)
					{
						// static display relies on fixed x-coord whilst scrolling display relies on variable 'uiImgBuffReadTrack'
						yAud = (YENDPLOT-400) - ((fAudioImgBuff[x]+1) * uiYspacing/2);
						Plot2Display.setPixelAt (x, yAud, c[15]);

						for (j=0; j<pEar->uiNumBFchannels; j++)
						{
							// variable used in the setting of pixels in images
							y = uiYstart[0]-(j*PIXELHEIGHT);

							for (m=0; m<NUMOFCOLOURBANDS; m++)
							{
								if ((fImgBuff[j][x] >= fColourDiscreteLevels[m-1]) && (fImgBuff[j][x] < fColourDiscreteLevels[m]))
								{
									Plot2Display.setPixelAt (x, y,c[m]);
									Plot2Display.setPixelAt (x+1, y,c[m]);

									Plot2Display.setPixelAt (x, y-1,c[m]);
									Plot2Display.setPixelAt (x+1, y-1,c[m]);

									Plot2Display.setPixelAt (x, y-2,c[m]);
									Plot2Display.setPixelAt (x+1, y-2,c[m]);

									Plot2Display.setPixelAt (x, y-3,c[m]);
									Plot2Display.setPixelAt (x+1, y-3,c[m]);
								}
							}
						}
					}

					siNumProcData2Display = 0;
				}
				else
				{
					for (x=0; x<xWidth; x++)
					{
						// static display relies on fixed x-coord whilst scrolling display relies on variable 'uiImgBuffReadTrack'
						yAud = (YENDPLOT-400) - ((fAudioImgBuff[x]+1) * uiYspacing/2);

						for (j=0; j<pEar->uiNumBFchannels; j++)
						{
							y = uiYstart[j] - ((fImgBuff[j][x] - fMinPixDisplayVal) * uiYspacing / (fMaxPixDisplayVal - fMinPixDisplayVal));
							
							// Static scatter plot display start
							if (cWaveformType == SCATTER_PLOT) 
							{
								Plot2Display.setPixelAt (x, yAud, c[8]);
								Plot2Display.setPixelAt (x, y, c[21]);
							}
							// Static scatter plot display end
							// Bresenham line draw algo start
							else if (cWaveformType == LINE_PLOT)
							{
								DrawLine (fPrevXposAudData, fPrevYposAudData, x, yAud, YENDPLOT-400, 8);
								DrawLine (fPrevXposProcData[j], fPrevYposProcData[j], x, y, uiYstart[j], 21);
							}
							// assign start coord for next render
							fPrevXposAudData = x;
							fPrevYposAudData = yAud;
							fPrevXposProcData[j] = x;
							fPrevYposProcData[j] = y;
							// Bresenham line draw algo end
						}
					}

					siNumProcData2Display = 0;
				}
			}
			// increment pix render semaphore
			dPixRenderSem++;
		}

		bRenderPlotInProgress = false;

#if DISPLAY_PROCTIME
		dPlotRenderEndTime = Time::getMillisecondCounterHiRes();
		dPlotRenderTime = dPlotRenderEndTime - dPlotRenderStartTime;
		dSummedPlotRenderTime += dPlotRenderTime;
		dNumPlotRenderInvk++;
#endif
#if SHOW_COLOURBAR
		DrawSpectrogramColourBar( 100, 30 );
#endif
	}

	return 0;
}

void RTAPgfx::DrawLine (float x1, float y1, float x2, float y2, UINT Ystart, UINT uiColour)
{
	float error, m1, j0, delx, dely, i;

	i = x1;
	j0 = y1;
	delx = x2 - x1;
	dely = y2 - y1;

	if (bScrollPlot)
	{
		if (dely > (float)uiYspacing)
		{
			y2 = YENDPLOT - Ystart;//uiYspacing * (j+1);
			y1 = j0 = y2;
			dely = 0.f;
		}
		else if (dely < (uiYspacing*-1.f))
		{
			y2 = YENDPLOT - Ystart;//uiYspacing * y;
			y1 = j0 = y2;
			dely = 0.f;
		}
	}

	if (delx == 0.f)	// account for 0 in denominator
	{
		// ensure that the 'for' loop below is entered
		x2 = x1 + 1.f;

		// adjustment in accordance to ending at y2 in all circumstances
		if (dely > 0.f)
			m1 = dely;
		else
			m1 = dely + 1.f;
	}
	else
		m1 = dely / delx;

	// compute error based on either +ve or -ve gradient (for consistency - ending at the final coords i.e. x2, y2)
	if (m1 > 0.f)
		error = m1-1;
	else
		error = m1;

	// Draw the line
	for (i=x1; i<x2; i++)
	{
		Plot2Display.setPixelAt (i, j0, c[uiColour]);
		if (y2 >= y1)
		{
			while (error >= 0)
			{
				j0 += 1;
				error -= 1.f;
				Plot2Display.setPixelAt (i, j0, c[uiColour]);
			}
			error += m1;
		}
		else
		{
			while (error <= 0)
			{
				j0 -= 1;
				error += 1.f;
				Plot2Display.setPixelAt (i, j0, c[uiColour]);
			}
			error += m1;
		}
	}
}

void RTAPgfx::UpdateXaxis ()
{
	UINT i, uiTimeInterval, uiHours, uiMinutes, uiSeconds, uiMilliseconds, uiRTAPduration = 0;

	// update x-axis labels for scroll plot display
	if (bScrollPlot && !bAutoRefreshPlot)
	{
		if (uiRTAPprocTime < uiXintPix)
			// compute time elapsed since algo proc initiated
			uiRTAPprocTime = uiRTAPprocTime + uiScrollSpeed;
		else
		{
			uiRTAPprocTime = 0;						// reset counter
			uiTimeAcc++;							// increment time counter display

			// _itoa used in conformance to ISO C++ build
			if (siDisplayStartPoint)
			{
				if (siDisplayStartPoint <= uiTimeLabelXaxis[1])
				{
					_itoa_s (uiTimeAcc-4, cXaxisLabel[0], 10);
					_itoa_s (uiTimeAcc-3, cXaxisLabel[1], 10);
					_itoa_s (uiTimeAcc-2, cXaxisLabel[2], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[3], 10);
					_itoa_s (uiTimeAcc, cXaxisLabel[4], 10);
				}
				else if (siDisplayStartPoint <= uiTimeLabelXaxis[2])
				{
					_itoa_s (uiTimeAcc-3, cXaxisLabel[0], 10);
					_itoa_s (uiTimeAcc-3, cXaxisLabel[1], 10);
					_itoa_s (uiTimeAcc-2, cXaxisLabel[2], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[3], 10);
					_itoa_s (uiTimeAcc, cXaxisLabel[4], 10);
				}
				else if (siDisplayStartPoint <= uiTimeLabelXaxis[3])
				{
					_itoa_s (uiTimeAcc-2, cXaxisLabel[0], 10);
					_itoa_s (uiTimeAcc-2, cXaxisLabel[1], 10);
					_itoa_s (uiTimeAcc-2, cXaxisLabel[2], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[3], 10);
					_itoa_s (uiTimeAcc, cXaxisLabel[4], 10);
				}
				else if (siDisplayStartPoint <= uiTimeLabelXaxis[4])
				{
					_itoa_s (uiTimeAcc-1, cXaxisLabel[0], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[1], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[2], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[3], 10);
					_itoa_s (uiTimeAcc, cXaxisLabel[4], 10);
				}
				else if (siDisplayStartPoint <= uiTimeLabelXaxis[5])
				{
					_itoa_s (uiTimeAcc-1, cXaxisLabel[0], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[1], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[2], 10);
					_itoa_s (uiTimeAcc-1, cXaxisLabel[3], 10);
					_itoa_s (uiTimeAcc, cXaxisLabel[4], 10);
				}
			}
			else
			{
				// based on display window size of 674 pixels, project only 5 x-axis labels
				for (i=0; i<NUMOFXLABELS-1; i++)
					_itoa_s (uiTimeAcc+i-4, cXaxisLabel[i], 10);
			}
		
			cXaxisLabel[NUMOFXLABELS-2][0] = 0x73;			// 's'
			cXaxisLabel[NUMOFXLABELS-2][1] = 0x65;			// 'e'
			cXaxisLabel[NUMOFXLABELS-2][2] = 0x63;			// 'c'
			cXaxisLabel[NUMOFXLABELS-2][3] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-2][4] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-2][5] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-2][6] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-2][7] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-2][8] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-2][9] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][0] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][1] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][2] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][3] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][4] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][5] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][6] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][7] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][8] = 0x20;			// <space>
			cXaxisLabel[NUMOFXLABELS-1][9] = 0x20;			// <space>
		}
	}
	// update x-axis labels for auto display refresh
	else if (!bScrollPlot && bAutoRefreshPlot || bRefreshXaxis)
	{
		uiRTAPduration = Time::getMillisecondCounterHiRes() - uiRTAPplayStart;
		uiTimeAcc = 0;
		// 58ms difference between plot start & end based on 44100Hz, 2560 samples (58ms)
		uiTimeInterval = 1000 * (2560/NUMOFXLABELS) / 44100;
		for (i=0; i<NUMOFXLABELS; i++)
		{
			uiMilliseconds = uiRTAPduration + uiTimeAcc;
			uiSeconds = uiMilliseconds / 1000;
			uiMinutes = uiSeconds / 60;
			uiHours = uiMinutes / 60;
			_itoa_s (uiMilliseconds, cXaxisLabel[i], 10);
			uiTimeAcc += uiTimeInterval;				// increment to point to the next x-axis label based on time
		}
		cXaxisLabel[4][0] = 0x6D;					// 'm'
		cXaxisLabel[4][1] = 0x73;					// 's'
		cXaxisLabel[4][2] = 0x20;					// <space>
		cXaxisLabel[4][3] = 0x20;					// <space>
		cXaxisLabel[4][4] = 0x20;					// <space>
		cXaxisLabel[4][5] = 0x20;					// <space>
		cXaxisLabel[4][6] = 0x20;					// <space>
		cXaxisLabel[4][7] = 0x20;					// <space>
		cXaxisLabel[4][8] = 0x20;					// <space>
		cXaxisLabel[4][9] = 0x20;					// <space>
		cXaxisLabel[5][0] = 0x20;					// <space>
		cXaxisLabel[5][1] = 0x20;					// <space>
		cXaxisLabel[5][2] = 0x20;					// <space>
		cXaxisLabel[5][3] = 0x20;					// <space>
		cXaxisLabel[5][4] = 0x20;					// <space>
		cXaxisLabel[5][5] = 0x20;					// <space>
		cXaxisLabel[5][6] = 0x20;					// <space>
		cXaxisLabel[5][7] = 0x20;					// <space>
		cXaxisLabel[5][8] = 0x20;					// <space>
		cXaxisLabel[5][9] = 0x20;					// <space>

		bRefreshXaxis = false;
	}
}

void RTAPgfx::DrawSpectrogramColourBar (UINT uiX, UINT uiY)
{
	UINT i, j, m, xPos, yPos;

	xPos = uiX;
	yPos = uiY;

	for (m=0; m<NUMOFCOLOURBANDS; m++)
	{
		for (j=0; j<COLOURBARPIXHEIGHT; j++) for (i=0; i<COLOURBARPIXWIDTH; i++) Plot2Display.setPixelAt (xPos+i, yPos+j, c[m]);

		xPos += COLOURBARPIXWIDTH;
	}
}

void RTAPgfx::timerCallback()
{
	if (bPlayPauseStatus && bInitialComputation)
	{
		if (pEar->uiTimerCount > 0)
		{
			pEar->uiTimerCount--;
		}
		else
		{
//			fMaxPixDisplayVal = pEar->fMaxAcc / pEar->uiMaxCount;
//			fMinPixDisplayVal = pEar->fMinAcc / pEar->uiMinCount;
			bInitialComputation = false;
			// reload the timer counter
			pEar->uiTimerCount = 3000 / TIMEDURATION;
		}
	}
	repaint();
}
