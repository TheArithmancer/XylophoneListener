#include "arduinoFFT.h"
#include <RCSwitch.h>


 
#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC


arduinoFFT FFT = arduinoFFT();
RCSwitch mySwitch = RCSwitch();
 
unsigned int sampling_period_us;
unsigned long microseconds;

const int Transmitter=2;  // Transmitter connected to pin 10
//const String MusicTarget_Full="B1 E1 G2 F#1 E1 B2 A2 F#1 E1 G2 F#1 D1 F1 B1 ";
const String MusicTarget2="G2 F#1 E1 ";   //"E1 G2 F#1 E1 A2 "; 
double vReal[SAMPLES];
double vImag[SAMPLES];

String MusicPlayed="";
String lastState="";
int lengthOfPlayed=0;


void setup() {
    Serial.begin(115200);
 
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));

    // Transmitter is connected to Arduino Pin #10  
    mySwitch.enableTransmit(Transmitter);
    mySwitch.setPulseLength(309); //320

}
 
void loop() {
   

    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!     
        vReal[i] = analogRead(A0);
        vImag[i] = 0;
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    /*PRINT RESULTS*/
    if (peak>54) {
      peak=round(peak);
      Serial.println(peak);

      if  (peak>=210 && peak<=222) {
        // G1 on xylophone
        if (lastState!="G1") {
            MusicPlayed=MusicPlayed+"G1 ";
        }
        lastState="G1"; 
        //Serial.println("G1");
      } else if  (peak==118 || peak==119) {
        // A1 on xylophone
        if (lastState!="A1") {
            MusicPlayed=MusicPlayed+"A1 ";
        }
        lastState="A1"; 
        //Serial.println("A1");
      } else if  (peak>=355 && peak <=360) {
        // G1 on xylophone
        if (lastState!="B1") {
            MusicPlayed=MusicPlayed+"B1 ";
        }
        lastState="B1"; 
        //Serial.println("B1");
//      } else if  (peak>=55 && peak <=56) {
 //       // C1 on xylophone
//        if (lastState!="C1") {
//            MusicPlayed=MusicPlayed+"C1 ";
//        }
//        lastState="C1"; 
        //Serial.println("C1");
      //} else if  (peak==122) {
      //  Serial.println("C#1");
      }  else if  (peak==254) {
        // D#1 on xylophone
        if (lastState!="D#1") {
            MusicPlayed=MusicPlayed+"D#1 ";
        }
        lastState="D#1"; 
        //Serial.println("D#1");
      } else if (peak==184 || peak==183) {
        // D1 on xylophone
        if (lastState!="D1") {
            MusicPlayed=MusicPlayed+"D1 ";
        }
        lastState="D1"; 
        //Serial.println("D1");
        
      } else if ( peak>=333 && peak<=338) {
        // E1 on xylophone
        if (lastState!="E1") {
            MusicPlayed=MusicPlayed+"E1 ";
        }
        lastState="E1"; 
        //Serial.println("E1");
      } else if (peak==409 || peak==408) {
                // F1 on xylophone
        if (lastState!="F1") {
            MusicPlayed=MusicPlayed+"F1 ";
        }
        lastState="F1"; 
        //Serial.println("F1");
      }  else if  (peak>=496 && peak <=500) {
        // F#1 on xylophone
        if (lastState!="F#1") {
            MusicPlayed=MusicPlayed+"F#1 ";
        }
        lastState="F#1"; 
        //Serial.println("F#1");
      } else if (peak>=422 && peak<=426) {
        // G2 on xylophone
        if (lastState!="G2") {
            MusicPlayed=MusicPlayed+"G2 ";
        }
        lastState="G2"; 
        //Serial.println("G2");
      //}  else if  (peak>=331) {
      //  Serial.println("G#2");
      } else if (peak==229 || peak==230) {
        // A2 on xylophone
        if (lastState!="A2") {
            MusicPlayed=MusicPlayed+"A2 ";
        }
        lastState="A2"; 
        //Serial.println("A2");
      }  else if  (peak==111) {
                // C2 on xylophone
        if (lastState!="C2") {
            MusicPlayed=MusicPlayed+"C2 ";
        }
        lastState="C2"; 
        //Serial.println("C2");

      //}  else if  (peak==121 || peak ==120) {
      //  Serial.println("A#2");
      //} else {
      //  Serial.println(peak);     //Print out what frequency is the most dominant.
      }
    }
    
    Serial.println(MusicPlayed);

    lengthOfPlayed=MusicPlayed.length()-MusicTarget.length();
    //Serial.println("
    if (lengthOfPlayed>=0) {
      if (MusicPlayed.endsWith(MusicTarget) || MusicPlayed.endsWith(MusicTarget2)) {
        Serial.println("SUCCESS");
        MusicPlayed="";  
        lastState="";
        lengthOfPlayed=0;
        mySwitch.send(6679343, 24);
        delay(1000);
      } else if (lengthOfPlayed>10) {
        MusicPlayed="";  
        lastState="";
        lengthOfPlayed=0;
        
      }
    }

    delay(100);  //Repeat the process every second OR:
    //while(1);       //Run code once
}


/*    for(int i=0; i<(SAMPLES/2); i++)
    {
        //View all these three lines in serial terminal to see which frequencies has which amplitudes
         
        //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
        //Serial.print(" ");
        //Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
    }*/
