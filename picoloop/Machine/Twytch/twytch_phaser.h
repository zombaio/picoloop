/* Copyright 2013-2016 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef TWYTCH_PHASER_H
#define TWYTCH_PHASER_H

#include "twytch_processor_router.h"

namespace mopotwytchsynth {

  class Phaser : public ProcessorRouter {
    public:
      enum Inputs {
        kAudio,
        kMix,
        kOscFrequency,
        kOscWaveform,
        kFilterCutoffMidi,
        kFilterResonance,
        kSemitoneSweep,
        kReset,
        kNumInputs
      };

      Phaser(int num_passes = 8);

      virtual Processor* clone() const override { return new Phaser(*this); }
  };
} // namespace mopo

#endif // PHASER_H
