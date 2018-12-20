#include "mbed.h"
#include "synthFunctions.h"

// Return value of saw oscillator scaled by ADSR
double sawADSR(double incrementSaw, double samplingFrequency, double attackSaw, 
                double decaySaw, double sustainLevel, 
                double releaseSaw, bool buttonSawPressed, int tempoSawON){
                    
    static double currentSawAmp = 0;
    static bool attackSawBool = 1;
    static bool decaySawBool = 0;
    double attackTime;
    double decayTime;
    double releaseTime;

    
    //If the button isn't pressed, the release will occur
    //Will always set the amplitude to zero if the release is finished
    if (!buttonSawPressed){
        attackSawBool = 1;
        releaseTime = 15000 * releaseSaw + 1; //Need to remap
        double numberOfSteps = releaseTime / sustainLevel;
        double amplitudeStep = 1.0 / numberOfSteps;
        currentSawAmp -= amplitudeStep;
        if(currentSawAmp < 0.0001){
            currentSawAmp = 0;
        }
    } 
    //Start a tone with the attack if button is pressed
    //attackSawBool flag is set to one in previous if-statement
    else if (attackSawBool == 1){
        attackTime = 15000 * attackSaw + 1; //Need to remap
        double numberOfSteps = attackTime;
        double amplitudeStep = 1.0 / numberOfSteps;
        currentSawAmp += amplitudeStep;
        if(currentSawAmp > 0.99){
            currentSawAmp = 1.0;
            attackSawBool = 0;
            decaySawBool = 1;
        }
    } 
    //If the attack goes all the way up to one, the decay will occur
    else if (decaySawBool == 1){
        decayTime = 15000 * decaySaw + 1; //Need to remap
        double numberOfSteps = decayTime / (1 - sustainLevel);
        double amplitudeStep = 1.0 / numberOfSteps;
        currentSawAmp -= amplitudeStep;
        if(currentSawAmp < sustainLevel){
            currentSawAmp = sustainLevel;
            decaySawBool = 0;
        }
    } 
    //If button is pressed but no attack or decay, sustain will occur
    else {
        //sustainLevel = sustainSaw;
        currentSawAmp = sustainLevel;
        
    }
    //Return the amplitude times sawWaveOscillator
    //Amplitude is set by ADSR
    return currentSawAmp * sawWaveOscillator(incrementSaw);
}