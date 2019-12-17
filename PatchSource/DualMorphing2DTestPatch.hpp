
#include "OscSelector.h"
#include "VoltsPerOctave.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz

#include "alien10752.h"

class DualMorphing2DTestPatch : public Patch {
	VoltsPerOctave hz;
private:
  OscSelector *morph1;
  OscSelector *morph2;
  
  SmoothFloat freqA;
  float FML;
  SmoothFloat freqR;
  float FMR;
  
public:
  DualMorphing2DTestPatch() {																																
	FloatArray bank(alien42[0], SAMPLE_LEN*NOF_SAMPLES*NOF_OSC);
	WTFactory *wtf = new WTFactory();

	morph1 = new OscSelector();
	morph2 = new OscSelector();
	for (int i ; i<NOF_OSC ; i++)  {
	morph1->setWaveTables(wtf, bank, baseFrequency, i);
	morph2->setWaveTables(wtf, bank, baseFrequency, i);
	}

	  registerParameter(PARAMETER_A, "TuneL");
	  registerParameter(PARAMETER_B, "FML");
	  registerParameter(PARAMETER_C, "MorphXL");
	  registerParameter(PARAMETER_D, "MorphYL");
	  registerParameter(PARAMETER_E, "TuneR");
	  registerParameter(PARAMETER_F, "FMR");
	  registerParameter(PARAMETER_G, "MorphXR");
	  registerParameter(PARAMETER_H, "MorphYR");
;  }
  void processAudio(AudioBuffer &buffer) {
    
    float npast;
    float FML = getParameterValue(PARAMETER_B);
    float FMR = getParameterValue(PARAMETER_F);
    float tune = getParameterValue(PARAMETER_A)*7.0 - 4.0;
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    hz.setTune(tune);
    float freqA = hz.getFrequency(left[0]);
    float morphC = getParameterValue(PARAMETER_C);  
    float morphD = getParameterValue(PARAMETER_D); 
    
    float tuneR = getParameterValue(PARAMETER_E)*7.0 - 4.0;
    hz.setTune(tuneR);
    float freqR = hz.getFrequency(right[0]);
    float morphG = getParameterValue(PARAMETER_G);  
    float morphH = getParameterValue(PARAMETER_H); 
    
    
    morph1->setMorphY(morphD);
    morph1->setFrequency((freqA/sampleRate)+FML);
    morph1->setMorphX(morphC);
    morph2->setMorphY(morphH);
    morph2->setFrequency((freqR/sampleRate)+FMR);
    morph2->setMorphX(morphG);
    //FML = getParameterValue(PARAMETER_B) - 0.4991;
    //FMR = getParameterValue(PARAMETER_F) - 0.4991;
    
    for(int n = 0; n<buffer.getSize(); n++){
	
	left[n] = (morph1->get2DOutput()) * 0.5;
	right[n] = morph2->get2DOutput() * 0.5;	
    morph1->updatePhases();				
    morph2->updatePhases();
		
	}
	
    debugMessage("out" , FML , FMR )	;	
	}
};
