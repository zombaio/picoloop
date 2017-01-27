#!/bin/bash
set -x
#g++ -DFIXED -c osctab.c        -o osctab.o
#g++ -DFIXED -c lfotab.c        -o lfotab.o
#g++ -DFIXED -c coeff.c         -o coeff.o
#g++ -DFIXED -c envtab.c        -o envtab.o
#g++ -DFIXED -c synthengine.cpp -o synthengine.o
#g++ -DFIXED -c osc.cpp         -o osc.o
#g++ -DFIXED -c lfo.cpp         -o lfo.o
#g++ -DFIXED -c filter.cpp      -o filter.o
#g++ -DFIXED -c env.cpp         -o env.o
#g++ -DFIXED -c echo.cpp        -o echo.o
#g++ -DFIXED -c sdl.cpp         -o sdl.o
#g++ -DFIXED -c test.cpp        -o test.o
#g++ -DFIXED synthengine.o osc.o lfo.o test.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o
g++ -O3 -g -c cursynth_engine.cpp 	-o cursynth_engine.o
g++ -O3 -g -c wave.cpp		-o wave.o
g++ -O3 -g -c voice_handler.cpp	-o voice_handler.o
g++ -O3 -g -c value.cpp		-o value.o
g++ -O3 -g -c trigger_operators.cpp	-o trigger_operators.o
g++ -O3 -g -c step_generator.cpp	-o step_generator.o
g++ -O3 -g -c smooth_value.cpp	-o smooth_value.o
g++ -O3 -g -c smooth_filter.cpp	-o smooth_filter.o
g++ -O3 -g -c send_receive.cpp	-o send_receive.o
g++ -O3 -g -c processor_router.cpp	-o processor_router.o
g++ -O3 -g -c processor.cpp		-o processor.o
g++ -O3 -g -c oscillator.cpp	-o oscillator.o
g++ -O3 -g -c operators.cpp		-o operators.o
g++ -O3 -g -c mono_panner.cpp	-o mono_panner.o
g++ -O3 -g -c midi_lookup.cpp	-o midi_lookup.o
g++ -O3 -g -c linear_slope.cpp	-o linear_slope.o
g++ -O3 -g -c filter.cpp		-o filter.o
g++ -O3 -g -c feedback.cpp		-o feedback.o
g++ -O3 -g -c envelope.cpp		-o envelope.o
g++ -O3 -g -c delay.cpp		-o delay.o
g++ -O3 -g -c cursynth_strings.cpp	-o cursynth_strings.o
g++ -O3 -g -c main.cpp		-o main.o

#g++ cursynth_engine.o wave.o voice_handler.o value.o trigger_operators.o step_generator.o smooth_value.o smooth_filter.o send_receive.o processor_router.o processor.o oscillator.o operators.o mono_panner.o midi_lookup.o linear_slope.o filter.o feedback.o envelope.o delay.o cursynth_strings.o main.o -o main

g++ -O3 -c main.cpp -o main.o && g++ cursynth_engine.o wave.o voice_handler.o value.o trigger_operators.o step_generator.o smooth_value.o smooth_filter.o send_receive.o processor_router.o processor.o oscillator.o operators.o mono_panner.o midi_lookup.o linear_slope.o filter.o feedback.o envelope.o delay.o cursynth_strings.o main.o -o main
g++ -O3 -c bench.cpp -o bench.o && g++ cursynth_engine.o wave.o voice_handler.o value.o trigger_operators.o step_generator.o smooth_value.o smooth_filter.o send_receive.o processor_router.o processor.o oscillator.o operators.o mono_panner.o midi_lookup.o linear_slope.o filter.o feedback.o envelope.o delay.o cursynth_strings.o bench.o -o bench
