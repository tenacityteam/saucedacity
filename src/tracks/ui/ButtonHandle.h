/**********************************************************************

Audacity: A Digital Audio Editor

ButtonHandle.h

Paul Licameli

**********************************************************************/

#ifndef __AUDACITY_BUTTON_HANDLE__
#define __AUDACITY_BUTTON_HANDLE__

#include "../../UIHandle.h"

class wxMouseEvent;
class wxMouseState;

class Track;
class TranslatableString;


/// \brief A UIHandle for a TrackPanel button, such as the Mute and Solo 
/// buttons.
class SAUCEDACITY_DLL_API ButtonHandle /* not final */ : public UIHandle
{
   ButtonHandle(const ButtonHandle&) = delete;

public:
   [[nodiscard]] std::shared_ptr<Track> GetTrack() const { return mpTrack.lock(); }
   [[nodiscard]] bool IsClicked() const { return mIsClicked; }

protected:
   explicit ButtonHandle
      ( const std::shared_ptr<Track> &pTrack, const wxRect &rect );

   ButtonHandle &operator=(const ButtonHandle&) = default;

   ~ButtonHandle() override;

   // This NEW abstract virtual simplifies the duties of further subclasses.
   // This class will decide whether to refresh the clicked cell for button state
   // change.
   // Subclass can decide to refresh other things and the results will be ORed.
   virtual Result CommitChanges
      (const wxMouseEvent &event, SaucedacityProject *pProject, wxWindow *pParent) = 0;

   // Define a message for the status bar and tooltip.
   virtual TranslatableString Tip(
      const wxMouseState &state, SaucedacityProject &project) const = 0;

   void Enter(bool forward, SaucedacityProject *) final ;

   Result Click
      (const TrackPanelMouseEvent &event, SaucedacityProject *pProject)
      final ;

   Result Drag
      (const TrackPanelMouseEvent &event, SaucedacityProject *pProject)
      final ;

   HitTestPreview Preview
      (const TrackPanelMouseState &state, SaucedacityProject *pProject)
      final ;

   Result Release
      (const TrackPanelMouseEvent &event, SaucedacityProject *pProject,
       wxWindow *pParent) final ;

   Result Cancel(SaucedacityProject *pProject) final ;

   std::weak_ptr<Track> mpTrack;
   wxRect mRect;
   bool mWasIn{ true };
   bool mIsClicked{};
};

#endif
