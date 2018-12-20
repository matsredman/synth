#include "mbed.h"
#include "synthFunctions.h"

// The audio output
AnalogOut output(PA_4);

// Square oscillator setup
DigitalIn btnSquare(PB_8);
AnalogIn attackSquarePot(PC_2);
AnalogIn decaySquarePot(PC_1);
AnalogIn sustainSquarePot(PB_0);
AnalogIn releaseSquarePot(PA_7);
AnalogIn frequencySquarePot(PC_3);
AnalogIn tempoSquarePot(PC_0);

// Saw oscillator setup
DigitalIn btnSaw(PC_9);
AnalogIn attackSawPot(PA_1);
AnalogIn decaySawPot(PA_0);
AnalogIn sustainSawPot(PB_13);
AnalogIn releaseSawPot(PB_1);
AnalogIn frequencySawPot(PA_6);
AnalogIn tempoSawPot(PB_11);

// Tempo buttons
DigitalIn btnSquareTempo(PC_10);
DigitalIn btnSawTempo(PC_12);

// Frequency sync button
DigitalIn btnFrequencySync(PC_8); //PC_8

Ticker playTicker;

int samplingFrequency = 11025;
double tickerTime = 1.0 / samplingFrequency;

// Variables for square oscillator
double incrementSquare;
double frequencySquare;
double attackSquare;
double decaySquare;
double sustainSquare;
double releaseSquare;
double squareTimer;
int tempoSquareMultiplier;

// Variables for saw oscillator
double incrementSaw;
double frequencySaw;
double attackSaw;
double decaySaw;
double sustainSaw;
double releaseSaw;
double sawTimer;
int tempoSawMultiplier;

// Variables for tempo
int tempoSquareON = 0;
int tempoSawON = 0;

// Variabel for frequency sync
bool frequencySync;

// Buttons on/off for the different oscillators
bool buttonSquarePressed;
bool buttonSawPressed;

// Variables for buffer
const int bufferSize = 2;
double audioBuffer[bufferSize] = {0};

// Generate new buffer
void generateAudioToBuffer(void){
    static int tickerCount = 0;
    double tempSquare, tempSaw;    
    tickerCount++;

    // Overrides on/off button for saw if tempo is on
    if (tempoSawON == 1){
        if (tickerCount > tempoSawMultiplier){
            tickerCount = 0;
            buttonSawPressed = 0;
        }
        if (tickerCount < tempoSawMultiplier * (2.0/4.0)){
            buttonSawPressed = 1;
        }
        else{
            buttonSawPressed = 0;    
        }
    }
    
    // Overrides on/off button for square if tempo is on     
    if (tempoSquareON == 1){
        if ((tickerCount % (tempoSawMultiplier / tempoSquareMultiplier)) 
                < (int)(0.5 * (tempoSawMultiplier / tempoSquareMultiplier))){
            buttonSquarePressed = 1;
        }
        else{
            buttonSquarePressed = 0;    
        }
    }
    
    // Fill buffer with new values
    for (int i = 0; i < bufferSize; i++){
            incrementSquare = incrementPhase(frequencySquare, samplingFrequency);
            incrementSaw = incrementPhase(frequencySaw, samplingFrequency);
            tempSquare = mapToAnalogOut(squareADSR(incrementSquare, 
                                        samplingFrequency, attackSquare, 
                                        decaySquare, sustainSquare, 
                                        releaseSquare, buttonSquarePressed, 
                                        tempoSquareON));
            tempSaw = (mapToAnalogOut(sawADSR(incrementSaw, samplingFrequency, 
                                                attackSaw, decaySaw, sustainSaw, 
                                                releaseSaw, buttonSawPressed, 
                                                tempoSawON)));
            audioBuffer[i]= (tempSquare + tempSaw) / 2;
    }
}

// Play sound from audio buffer
void playSound(){
    static int index = bufferSize - 1;
    output.write(audioBuffer[index]);
    index++;  
    // Generate new buffer when buffer is empty
    if (index > bufferSize - 1){
        generateAudioToBuffer();
        index = 0;
    }  
}

int main() {
    printf("\r\nProgram started \n\r");
    // This calls the playSound function,
    // as many times as required by the sampling frequency
    playTicker.attach(&playSound, tickerTime);
    wait(0.001);
    
    // Will run when not playSound() is running
    while (true) {
        // Read ADSR potentiometers for square oscillator 
        attackSquare = attackSquarePot.read();
        decaySquare = decaySquarePot.read();
        sustainSquare = sustainSquarePot.read();
        releaseSquare = releaseSquarePot.read();
        
        // Read ADSR potentiometers for saw oscillator
        attackSaw = attackSawPot.read();
        decaySaw = decaySawPot.read();
        sustainSaw = sustainSawPot.read();
        releaseSaw = releaseSawPot.read();
        
        // Read buttons for the different oscillators
        buttonSquarePressed = btnSquare.read();
        buttonSawPressed = btnSaw.read();
        
        // Read buttons for tempo on for the different oscillators
        tempoSquareON = btnSquareTempo.read();
        tempoSawON = btnSawTempo.read();
        
        // Read tempo from potentiometers
        tempoSquareMultiplier = mapTempoMultiplier(tempoSquarePot.read());
        tempoSawMultiplier = mapTempo(tempoSawPot.read(), samplingFrequency);
        
        // Read if frequency should be synced
        frequencySync = btnFrequencySync.read();
        
        // Read frequency from potentiometers
        frequencySaw = mapAnalogInToFrequency(frequencySawPot.read());
        if (frequencySync){
            frequencySquare = frequencySaw;
        }
        else {
            frequencySquare = mapAnalogInToFrequency(frequencySquarePot.read());
        }
    }
}