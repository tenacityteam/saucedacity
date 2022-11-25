/**********************************************************************

  Audacity: A Digital Audio Editor

  Clipboard.h

  Paul Licameli

**********************************************************************/

#ifndef __AUDACITY_CLIPBOARD__
#define __AUDACITY_CLIPBOARD__


#include <memory>
#include <wx/event.h> // to inherit wxEvtHandler

class SaucedacityProject;
class TrackList;

// An event emitted by the clipboard whenever its contents change.
wxDECLARE_EXPORTED_EVENT( SAUCEDACITY_DLL_API,
                          EVT_CLIPBOARD_CHANGE, wxCommandEvent );

class SAUCEDACITY_DLL_API Clipboard final
   : public wxEvtHandler
{
public:
   static Clipboard &Get();

   [[nodiscard]] const TrackList &GetTracks() const;

   [[nodiscard]] double T0() const { return mT0; }
   [[nodiscard]] double T1() const { return mT1; }
   [[nodiscard]] double Duration() const { return mT1 - mT0; }

   [[nodiscard]] const std::weak_ptr<SaucedacityProject> &Project() const { return mProject; }

   void Clear();
   
   void Assign(
     TrackList && newContents, double t0, double t1,
     const std::weak_ptr<SaucedacityProject> &pProject );

   Clipboard();
   ~Clipboard() override;

   void Swap( Clipboard &other );

private:

   std::shared_ptr<TrackList> mTracks;
   std::weak_ptr<SaucedacityProject> mProject{};
   double mT0{ 0 };
   double mT1{ 0 };
};

#endif
