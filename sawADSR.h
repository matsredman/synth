#ifndef sawADSR_H
#define sawADSR_H

double sawADSR(double incrementSaw, double samplingFrequency, double attackSaw, 
                double decaySaw, double sustainSaw, double releaseSaw, 
                bool buttonSawPressed, int tempoSawON);

#endif