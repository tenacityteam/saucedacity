/**********************************************************************

Audacity: A Digital Audio Editor

MeterPanelBase.h

Paul Licameli split from Meter.h

**********************************************************************/

#ifndef __AUDACITY_METER_PANEL_BASE__
#define __AUDACITY_METER_PANEL_BASE__

#include "wxPanelWrapper.h"

class SAUCEDACITY_DLL_API MeterPanelBase /* not final */
   : public wxPanelWrapper
{
public:
   using wxPanelWrapper::wxPanelWrapper;
   ~MeterPanelBase() override;

   virtual void Clear() = 0;
   virtual void Reset(double sampleRate, bool resetClipping) = 0;
   virtual void UpdateDisplay(unsigned numChannels,
                      int numFrames, float *sampleData) = 0;
   [[nodiscard]] virtual bool IsMeterDisabled() const = 0;
   [[nodiscard]] virtual float GetMaxPeak() const = 0;

   [[nodiscard]] virtual bool IsClipping() const = 0;
   [[nodiscard]] virtual int GetDBRange() const = 0;

   [[nodiscard]] bool AcceptsFocus() const override { return s_AcceptsFocus; }
   [[nodiscard]] bool AcceptsFocusFromKeyboard() const override { return true; }

private:
   static bool s_AcceptsFocus;
   struct Resetter { void operator () (bool *p) const { if(p) *p = false; } };
   using TempAllowFocus = std::unique_ptr<bool, Resetter>;

public:
   static TempAllowFocus TemporarilyAllowFocus();
};

#endif
