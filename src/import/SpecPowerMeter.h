/**********************************************************************

  Audacity: A Digital Audio Editor

  SpecPowerMeter.h

  Philipp Sibler

**********************************************************************/

#ifndef __AUDACITY_SPECPOWERMETER_H_
#define __AUDACITY_SPECPOWERMETER_H_

#include <cstddef>

// Saucedacity libraries
#include <lib-math/SampleFormat.h>

class SpecPowerCalculation
{
   const size_t mSigLen;
   
   Floats mSigI;
   Floats mSigFR;
   Floats mSigFI;

   static float CalcBinPower(const float* sig_f_r, const float* sig_f_i, int loBin, int hiBin);
   int Freq2Bin(float fc) const;
public:
   SpecPowerCalculation(size_t sigLen);
   ~SpecPowerCalculation();
   
   float CalcPower(float* sig, float fc, float bw);
};

#endif

