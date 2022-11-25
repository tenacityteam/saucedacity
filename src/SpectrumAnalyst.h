/**********************************************************************

  Audacity: A Digital Audio Editor

  SpectrumAnalyst.h

  Dominic Mazzoni
  Paul Licameli split from FreqWindow.h

**********************************************************************/

#ifndef __AUDACITY_SPECTRUM_ANALYST__
#define __AUDACITY_SPECTRUM_ANALYST__

#include <vector>
#include <wx/statusbr.h>

class FreqGauge;

class SAUCEDACITY_DLL_API SpectrumAnalyst
{
public:

   enum Algorithm {
      Spectrum,
      Autocorrelation,
      CubeRootAutocorrelation,
      EnhancedAutocorrelation,
      Cepstrum,

      NumAlgorithms
   };

   SpectrumAnalyst();
   ~SpectrumAnalyst();

   // Return true iff successful
   bool Calculate(Algorithm alg,
      int windowFunc, // see FFT.h for values
      size_t windowSize, double rate,
      const float *data, size_t dataLen,
      float *pYMin = nullptr, float *pYMax = nullptr, // outputs
      FreqGauge *progress = nullptr);

   [[nodiscard]] const float *GetProcessed() const;
   [[nodiscard]] int GetProcessedSize() const;

   [[nodiscard]] float GetProcessedValue(float freq0, float freq1) const;
   float FindPeak(float xPos, float *pY) const;

private:
   [[nodiscard]] static float CubicInterpolate(float y0, float y1, float y2, float y3, float x) ;
   static float CubicMaximize(float y0, float y1, float y2, float y3, float * max) ;

private:
   Algorithm mAlg;
   double mRate;
   size_t mWindowSize;
   std::vector<float> mProcessed;
};

class SAUCEDACITY_DLL_API FreqGauge final : public wxStatusBar
{
public:
   FreqGauge(wxWindow * parent, wxWindowID winid);

   void SetRange(int range, int bar = 12, int gap = 3);
   void SetValue(int value);
   void Reset();

private:
   wxRect mRect;
   int mRange;
   int mCur;
   int mLast;
   int mInterval;
   int mBar;
   int mGap;
   int mMargin;
};

#endif
