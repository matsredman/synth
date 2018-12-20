#include "mbed.h"
#include "synthFunctions.h"

// Function which gives back the lenght of each increment
// The frequency controls the value returned from the function
double incrementPhase(int frequency, int samplingFrequency){ 
    return 2 * MY_PI * frequency / samplingFrequency;   
}

// The squarewave oscillator
// The in parameter is the size of each increment
// One full phase is 2 * PI for easy implementation of a sinus wave
double squareWaveOscillator(double incrementSquare){
    static double phase = 0;
    double tone;
    if (phase >= 2 * MY_PI){
        phase -= 2 * MY_PI;
    }
    if (phase <= MY_PI){
        tone = 1;
    }
    else{
        tone = -1;
    }
    phase += incrementSquare;
    return tone;
}

// The sawwave oscillator
// The in parameter is the size of each increment
// One full phase is 2 * PI for easy implementation of a sinus wave
// The mapping between phase and frequency are linear
double sawWaveOscillator(double incrementSaw){
    static double sawPhase = 0;
    double tone;
    if (sawPhase >= 2 * MY_PI){
        sawPhase -= 2 * MY_PI;
        }
    double x = sawPhase;
    double in_min = 0.0;
    double in_max = 2.0 * MY_PI;
    double out_min = -1.0;
    double out_max = 1.0;
    tone = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    sawPhase += incrementSaw;
    return tone;
}

// Function for remapping the oscialling values between -1 to 1 ->
// To oscillating between 0 and 1
double mapToAnalogOut(double tone){
    return (tone + 1) / 2;
}  

// Remapping the frequency logarithmic
// The relation between frequency and tone are logarithmic 
double mapAnalogInToFrequency(double inPotValue){
  double min = 0.0;
  double max = 1.0;
  double lowestFrequency = 41.2;
  double highestFrequency = 329.63;
  double b = log(highestFrequency / lowestFrequency) / (max - min);
  double a = highestFrequency / exp(b);
  return a * exp(b * inPotValue);
}

// Read in the tempo for the main tempo oscillator and sync it with the 
// sampling frequency
int mapTempo(double analogInTempoPot, int samplingFrequency){
    double tempo;
    double x = analogInTempoPot;
    double in_min = 0.0;
    double in_max = 1.0;
    double out_max = samplingFrequency * 0.3;
    double out_min = samplingFrequency * 1.0;
    tempo = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return (int)tempo;
}

// Read in the tempo for the "slave" oscillator and sync it with the main 
// oscillator tempo
int mapTempoMultiplier(double analogInTempoPot){
    double tempo;
    double x = analogInTempoPot;
    double in_min = 0.0;
    double in_max = 1.0;
    double out_min = 1.0;
    double out_max = 12.2;
    tempo = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return (int)tempo;
}