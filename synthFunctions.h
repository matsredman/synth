#include "squareADSR.h"
#include "sawADSR.h"

#ifndef SYNTHFUNCTIONS_H
#define SYNTHFUNCTIONS_H
#define MY_PI 3.141592653589793238463

bool readButton(DigitalIn btn);
double readPot(AnalogIn pot);
double incrementPhase(int frequency, int samplingFrequency);
double squareWaveOscillator(double incrementSquare);
double sawWaveOscillator(double incrementSquare);
double mapToAnalogOut(double tone);
double mapAnalogInToFrequency(double inPotValue);
int mapTempo(double tempoTemp, int samplingFrequency);
int mapTempoMultiplier(double analogInTempoPot);

#endif