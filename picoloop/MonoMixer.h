//using namespace std;

#include "Machine.h"
#include "fixed.h"
#include "Machine/PBSynth/PBSynthMachine.h"

#if !defined(__PBSYNTHONLY__)
#include "Machine/Picosynth/PicosynthMachine.h"
#include "Machine/Picodrum/PicodrumMachine.h"
#include "Machine/Dbopl/DboplMachine.h"
#endif

#ifdef __RAM512MIB__
#include "Machine/Lgptsampler/LgptsamplerMachine.h"
#endif
#ifdef __VECTORFPU__
#include "Machine/Cursynth/CursynthMachine.h"
#include "Machine/Open303/Open303Machine.h"
#include "Machine/Twytch/TwytchsynthMachine.h"
#include "Machine/MDADrum/MDADrumMachine.h"
#include "Machine/SIDSynth/SIDSynthMachine.h"
#endif 

#ifdef __RTMIDI__
#include "Machine/MidiOutSystem/MidiOutMachine.h"
#endif
//#include "Effect.h"
#include "EffectDelay.h"
#include "EffectDisabled.h"
//#include <SDL/SDL_types.h>

#ifndef __MONOMIXER____
#define __MONOMIXER____

class MonoMixer
{
 public:
  MonoMixer();
  ~MonoMixer();
  void        init();
  void        setMachineType(int type);
  void        setAmplitude(int amp);
  //  void        setInput(Machine * M);
  void        setMonoMixerChannelNumber(int c);
  int         getMonoMixerChannelNumber();
  Machine *   getInput();
  Effect  *   getEffect();
  void        process();
  void        process_fixed(); 
  Sint16      tick();
  Fixed       tick_fixed();
  Fixed *     tick_fixed_buffer();
 private:
  int                   channel;
  
  Machine              *M;
  PBSynthMachine        PBS;

#if !defined(__PBSYNTHONLY__)
  PicosynthMachine      PS;
  PicodrumMachine       PD;
  dboplMachine          OPLM;
#endif
  
#ifdef __RAM512MIB__
  LgptsamplerMachine    LGPTSMPL;
#endif
#ifdef __VECTORFPU__
  CursynthMachine       CS;
  Open303Machine        O303;
  TwytchsynthMachine    TW;
  MDADrumMachine        MD;
  SIDSynthMachine       SS;
#endif

#ifdef __RTMIDI__
  MidiOutMachine        MIDIOUTM;
#endif

  Effect                *FX;
  EffectDelay           FXDelay;
  EffectDisabled        FXDisabled;

  Sint16                amplitude;
  Fixed                 amplitudeFixed;
  int                   machine_type;

  int                   fx_depth;
  int                   fx_speed;

  int                   index;
  Sint16                * buffer16;
  Sint32                * buffer32;
  Fixed                 * bufferfix;
};


#endif
