/*
 * Copyright (c) 2015-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BandlimitedOscillatorExpSynth_h
#define BandlimitedOscillatorExpSynth_h

#include "Tonic.h"

using namespace Tonic;

class BandlimitedOscillatorExpSynth : public Synth
{

public:

	BandlimitedOscillatorExpSynth()
	{
		//ControlParameter blend = addParameter("blend").min(0).max(1);
		ControlParameter blend = addParameter("blend", 0.5f).min(0).max(1);

		//Generator freqSweep = SineWave().freq(0.25) * 1000 + 1080; //original
		//Generator freqSweep = SineWave().freq(1) * 50 + 100;
		Generator freqSweep = SineWave().freq(addParameter("lfo_freq", 5.0).min(0.1).max(10.0)) * addParameter("lfo_amp", 50.0).min(10.0).max(400.0) + addParameter("sweep_freq", 100.0).min(10.0).max(400.0);
		//Generator smoothBlend = blend.smoothed(); //original
		Generator smoothBlend = blend.smoothed(.05);

		//Generator output = (SquareWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SquareWaveBL().freq(freqSweep) * smoothBlend); //original
		//Generator output = (SineWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SineWave().freq(freqSweep) * smoothBlend);
		//Generator output = (TriangleWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SquareWaveBL().freq(freqSweep) * smoothBlend);
		Generator output = (SquareWave().freq(addParameter("base_freq", 80.0).min(10.0).max(400.0)) * (1.0 - smoothBlend)) + (SquareWave().freq(freqSweep) * smoothBlend);

		StereoDelay delay = StereoDelay(3.0f, 3.0f, 3.0f, 3.0f)
			//.delayTimeLeft(0.5 + SineWave().freq(0.2) * 0.01)
			.delayTimeLeft(addParameter("delay_left", 0.5).min(0.0).max(3.0) + SineWave().freq(0.2) * 0.01)
			//.delayTimeRight(0.55 + SineWave().freq(0.23) * 0.01)
			.delayTimeRight(addParameter("delay_right", 0.55).min(0.0).max(3.0) + SineWave().freq(0.23) * 0.01)
			//.feedback(0.3)
			.feedback(addParameter("delay_feedback", 0.5).min(0.0).max(1.0))
			.dryLevel(addParameter("delay_drylevel", 0.5).min(0.0).max(1.0))
			//.wetLevel(0.2);
			.wetLevel(addParameter("delay_wetlevel", 0.5).min(0.0).max(1.0));

		setOutputGen(delay.input(output * 0.25));
	}

};

//TONIC_REGISTER_SYNTH(BandlimitedOscillatorExpSynth);

#endif