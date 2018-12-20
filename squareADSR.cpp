#include "mbed.h"
#include "synthFunctions.h"

// Return value of square oscillator scaled by ADSR
double squareADSR(double incrementSquare, double samplingFrequency,
                  double attackSquare, double decaySquare, double sustainLevel, 
                  double releaseSquare, bool buttonSquarePressed, 
                  int tempoSquareON){
    static bool attackBool = 1;
    static bool decayBool = 0;
    static double currentAmp = 0;
    double attackTime;
    double decayTime;
    double releaseTime;

    
    // If the button isn't pressed, the release will occur
    // Will always set the amplitude to zero if the release is finished
    if (!buttonSquarePressed){
        attackBool = 1;
        releaseTime = 15000 * releaseSquare + 1; //Need to remap
        double numberOfSteps = releaseTime / sustainLevel;
        double amplitudeStep = 1.0 / numberOfSteps;
        currentAmp -= amplitudeStep;
        if(currentAmp < 0.0001){
            currentAmp = 0;
        }
    } 
    // Start a tone with the attack if button is pressed
    // AttackBool flag is set to one in previous if-statement
    else if (attackBool == 1){
        attackTime = 15000 * attackSquare + 1; //Need to remap
        double numberOfSteps = attackTime;
        double amplitudeStep = 1.0 / numberOfSteps;
        currentAmp += amplitudeStep;
        if(currentAmp > 0.99){
            currentAmp = 1.0;
            attackBool = 0;
            decayBool = 1;
        }
    } 
    //If the attack goes all the way up to one, the decay will occur
    else if (decayBool == 1){
        decayTime = 15000 * decaySquare + 1; //Need to remap
        double numberOfSteps = decayTime / (1 - sustainLevel);
        double amplitudeStep = 1.0 / numberOfSteps;
        currentAmp -= amplitudeStep;
        if(currentAmp < sustainLevel){
            currentAmp = sustainLevel;
            decayBool = 0;
        }
    } 
    //If button is pressed but no attack or decay, sustain will occur
    else {
        currentAmp = sustainLevel;
    }
    //Return the amplitude times squareWaveOscillator
    //Amplitude is set by ADSR
    return currentAmp * squareWaveOscillator(incrementSquare);
}