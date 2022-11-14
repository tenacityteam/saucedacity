/**********************************************************************

  Saucedacity: A Digital Audio Editor

  EffectsRack.cpp

  Avery King split from EffectUI.h

**********************************************************************/

#ifdef EXPERIMENTAL_EFFECTS_RACK

#include "EffectsRack.h"

#include "../UndoManager.h"
#include "../ProjectHistory.h"
#include "Effect.h"
#include "EffectManager.h"
#include "EffectUI.h"
#include "RealtimeEffectManager.h"

#include <wx/dcmemory.h>
#include <wx/defs.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/timer.h>
#include <wx/tglbtn.h>

#include "../commands/CommandContext.h"
#include "../Project.h"
#include "../shuttle/ShuttleGui.h"
#include "../widgets/wxPanelWrapper.h"

// Saucedacity libraries
#include <lib-preferences/Prefs.h>

#include "../../images/EffectsRack/EffectsRackImages.h"

constexpr int COL_POWER  = 0;
constexpr int COL_EDITOR = 1;
constexpr int COL_UP     = 2;
constexpr int COL_DOWN   = 3;
constexpr int COL_FAV    = 4;
constexpr int COL_REMOVE = 5;
constexpr int COL_NAME   = 6;
constexpr int NUMCOLS    = 7;

constexpr int ID_BASE    = 20000;
constexpr int ID_RANGE   = 100;
constexpr int ID_POWER   = (ID_BASE + (COL_POWER * ID_RANGE));
constexpr int ID_EDITOR  = (ID_BASE + (COL_EDITOR * ID_RANGE));
constexpr int ID_UP      = (ID_BASE + (COL_UP * ID_RANGE));
constexpr int ID_DOWN    = (ID_BASE + (COL_DOWN * ID_RANGE));
constexpr int ID_FAV     = (ID_BASE + (COL_FAV * ID_RANGE));
constexpr int ID_REMOVE  = (ID_BASE + (COL_REMOVE * ID_RANGE));
constexpr int ID_NAME    = (ID_BASE + (COL_NAME * ID_RANGE));

BEGIN_EVENT_TABLE(EffectsRack, wxFrame)
   EVT_CLOSE(EffectsRack::OnClose)
   EVT_TIMER(wxID_ANY, EffectsRack::OnTimer)

   EVT_BUTTON(wxID_APPLY, EffectsRack::OnApply)
   EVT_TOGGLEBUTTON(wxID_CLEAR, EffectsRack::OnBypass)

   EVT_COMMAND_RANGE(ID_REMOVE, ID_REMOVE + 99, wxEVT_COMMAND_BUTTON_CLICKED, EffectsRack::OnRemove)
   EVT_COMMAND_RANGE(ID_POWER,  ID_POWER + 99,  wxEVT_COMMAND_BUTTON_CLICKED, EffectsRack::OnPower)
   EVT_COMMAND_RANGE(ID_EDITOR, ID_EDITOR + 99, wxEVT_COMMAND_BUTTON_CLICKED, EffectsRack::OnEditor)
   EVT_COMMAND_RANGE(ID_UP,     ID_UP + 99,     wxEVT_COMMAND_BUTTON_CLICKED, EffectsRack::OnUp)
   EVT_COMMAND_RANGE(ID_DOWN,   ID_DOWN + 99,   wxEVT_COMMAND_BUTTON_CLICKED, EffectsRack::OnDown)
   EVT_COMMAND_RANGE(ID_FAV,    ID_FAV + 99,    wxEVT_COMMAND_BUTTON_CLICKED, EffectsRack::OnFav)
END_EVENT_TABLE()

EffectsRack::EffectsRack( SaucedacityProject &project )
:  wxFrame( &GetProjectFrame( project ),
      wxID_ANY,
      _("Effects Rack"),
      wxDefaultPosition,
      wxDefaultSize,
      wxSYSTEM_MENU |
      wxCLOSE_BOX |
      wxCAPTION |
      wxFRAME_NO_TASKBAR |
      wxFRAME_FLOAT_ON_PARENT)
, mProject{ project }
{
   mBypassing = false;
   mNumEffects = 0;
   mLastLatency = std::chrono::milliseconds();
   mTimer.SetOwner(this);

   mPowerPushed = CreateBitmap(power_on_16x16_xpm, false, false);
   mPowerRaised = CreateBitmap(power_off_16x16_xpm, true, false);
   mSettingsPushed = CreateBitmap(settings_up_16x16_xpm, false, true);
   mSettingsRaised = CreateBitmap(settings_down_16x16_xpm, true, true);
   mUpDisabled = CreateBitmap(up_9x16_xpm, true, true);
   mUpPushed = CreateBitmap(up_9x16_xpm, false, true);
   mUpRaised = CreateBitmap(up_9x16_xpm, true, true);
   mDownDisabled = CreateBitmap(down_9x16_xpm, true, true);
   mDownPushed = CreateBitmap(down_9x16_xpm, false, true);
   mDownRaised = CreateBitmap(down_9x16_xpm, true, true);
   mFavPushed = CreateBitmap(fav_down_16x16_xpm, false, false);
   mFavRaised = CreateBitmap(fav_up_16x16_xpm, true, false);
   mRemovePushed = CreateBitmap(remove_16x16_xpm, false, true);
   mRemoveRaised = CreateBitmap(remove_16x16_xpm, true, true);

   mPanel = safenew wxPanelWrapper(this, wxID_ANY);

   ShuttleGui S(this, eIsCreating);
   S.StartVerticalLay();
   {

      // Setup the 'top' panel. This panel is used for the "Add Effect...",
      // "Apply", and "Bypass" buttons
      wxPanel* topPanel = safenew wxPanelWrapper(this, wxID_ANY);

      ShuttleGui topPanelGui(topPanel, eIsCreating);
      topPanelGui.StartHorizontalLay();
      {
         topPanelGui.AddButton(XO("Apply"));
         mLatency = topPanelGui.AddVariableText(XO("Latency: 0"));
         topPanelGui.AddWindow(safenew wxToggleButton(topPanel, wxID_CLEAR, _("&Bypass")));
      }
      topPanelGui.EndHorizontalLay();

      S.AddWindow(topPanel);


      // Setup the other panel, which will list the effects in the effects rack.
      ShuttleGui PanelGui(mPanel, eIsCreating);

      PanelGui.StartVerticalLay();
      {
         PanelGui.AddWindow(safenew wxStaticLine(mPanel, wxID_ANY));

         // Setup a flex grid sizer
         {
            auto uMainSizer = std::make_unique<wxFlexGridSizer>(7);
            uMainSizer->AddGrowableCol(6);
            uMainSizer->SetHGap(0);
            uMainSizer->SetVGap(0);
            PanelGui.GetSizer()->Add((mMainSizer = uMainSizer.release()), 1, wxEXPAND);
         }
      }
      PanelGui.EndHorizontalLay();
   }
   S.EndHorizontalLay();
   S.AddWindow(mPanel);

   wxString oldPath = gPrefs->GetPath();
   gPrefs->SetPath(wxT("/EffectsRack"));
   size_t cnt = gPrefs->GetNumberOfEntries();
   gPrefs->SetPath(oldPath);

   EffectManager & em = EffectManager::Get();
   for (size_t i = 0; i < cnt; i++)
   {
      wxString slot;
      gPrefs->Read(wxString::Format(wxT("/EffectsRack/Slot%02d"), i), &slot);

      Effect *effect = em.GetEffect(slot.AfterFirst(wxT(',')));
      if (effect)
      {
         Add(effect, slot.BeforeFirst(wxT(',')) == wxT("1"), true);
      }
   }

   Fit();
}

EffectsRack::~EffectsRack()
{
   gPrefs->DeleteGroup(wxT("/EffectsRack"));

   for (size_t i = 0, cnt = mEffects.size(); i < cnt; i++)
   {
      if (mFavState[i])
      {
         Effect *effect = mEffects[i];
         gPrefs->Write(wxString::Format(wxT("/EffectsRack/Slot%02d"), i),
                       wxString::Format(wxT("%d,%s"),
                                        mPowerState[i],
                                        effect->GetID()));
      }
   }
}

void EffectsRack::Add(Effect *effect, bool active, bool favorite)
{
   if (mEffects.end() != std::find(mEffects.begin(), mEffects.end(), effect))
   {
      return;
   }

   wxBitmapButton *bb;
 
   wxASSERT(mPanel); // To justify safenew
   bb = safenew wxBitmapButton(mPanel, ID_POWER + mNumEffects, mPowerRaised);
   bb->SetBitmapSelected(mPowerRaised);
   bb->SetName(_("Active State"));
   bb->SetToolTip(_("Set effect active state"));
   mPowerState.push_back(active);
   if (active)
   {
      bb->SetBitmapLabel(mPowerPushed);
      bb->SetBitmapSelected(mPowerPushed);
   }
   else
   {
      bb->SetBitmapLabel(mPowerRaised);
      bb->SetBitmapSelected(mPowerRaised);
   }
   mMainSizer->Add(bb, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

   bb = safenew wxBitmapButton(mPanel, ID_EDITOR + mNumEffects, mSettingsRaised);
   bb->SetBitmapSelected(mSettingsPushed);
   bb->SetName(_("Show/Hide Editor"));
   bb->SetToolTip(_("Open/close effect editor"));
   mMainSizer->Add(bb, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

   bb = safenew wxBitmapButton(mPanel, ID_UP + mNumEffects, mUpRaised);
   bb->SetBitmapSelected(mUpPushed);
   bb->SetBitmapDisabled(mUpDisabled);
   bb->SetName(_("Move Up"));
   bb->SetToolTip(_("Move effect up in the rack"));
   mMainSizer->Add(bb, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

   bb = safenew wxBitmapButton(mPanel, ID_DOWN + mNumEffects, mDownRaised);
   bb->SetBitmapSelected(mDownPushed);
   bb->SetBitmapDisabled(mDownDisabled);
   bb->SetName(_("Move Down"));
   bb->SetToolTip(_("Move effect down in the rack"));
   mMainSizer->Add(bb, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

   bb = safenew wxBitmapButton(mPanel, ID_FAV + mNumEffects, mFavRaised);
   bb->SetBitmapSelected(mFavPushed);
   bb->SetName(_("Favorite"));
   bb->SetToolTip(_("Mark effect as a favorite"));
   mFavState.push_back(favorite);
   if (favorite)
   {
      bb->SetBitmapLabel(mFavPushed);
      bb->SetBitmapSelected(mFavPushed);
   }
   else
   {
      bb->SetBitmapLabel(mFavRaised);
      bb->SetBitmapSelected(mFavRaised);
   }
   mMainSizer->Add(bb, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

   bb = safenew wxBitmapButton(mPanel, ID_REMOVE + mNumEffects, mRemoveRaised);
   bb->SetBitmapSelected(mRemovePushed);
   bb->SetName(_("Remove"));
   bb->SetToolTip(_("Remove effect from the rack"));
   mMainSizer->Add(bb, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

   wxStaticText *text = safenew wxStaticText(mPanel, ID_NAME + mNumEffects,
      effect->GetName().Translation() );
   text->SetToolTip(_("Name of the effect"));
   mMainSizer->Add(text, 0, wxEXPAND | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

   mMainSizer->Layout();
   SetSize(GetMinSize());
   Fit();
   Update();

   mEffects.push_back(effect);
   mNumEffects++;

   if (!mTimer.IsRunning())
   {
      mTimer.Start(1000);
   }

   if (active)
   {
      UpdateActive();
   }
}

void EffectsRack::OnClose(wxCloseEvent & evt)
{
   Show(false);
   evt.Veto();
}

void EffectsRack::OnTimer(wxTimerEvent & WXUNUSED(evt))
{
   auto latency = RealtimeEffectManager::Get().GetRealtimeLatency();
   if (latency != mLastLatency)
   {
      mLatency->SetLabel(wxString::Format(_("Latency: %4d"), latency.count()));
      mLatency->Refresh();
      mLastLatency = latency;
   }
}

void EffectsRack::OnApply(wxCommandEvent & WXUNUSED(evt))
{
   SaucedacityProject *project = &mProject;

   bool success = false;
   auto state = UndoManager::Get( *project ).GetCurrentState();
   auto cleanup = finally( [&] {
      if(!success)
         // This is like a rollback of state
         ProjectHistory::Get( *project ).SetStateTo( state, false );
   } );

   for (size_t i = 0, cnt = mEffects.size(); i < cnt; i++)
   {
      if (mPowerState[i])
      {
         if (!EffectUI::DoEffect(mEffects[i]->GetID(),
                           *project,
                           EffectManager::kConfigured))
            // If any effect fails (or throws), then stop.
            return;
      }
   }

   success = true;

   // Only after all succeed, do the following.
   for (size_t i = 0, cnt = mEffects.size(); i < cnt; i++)
   {
      if (mPowerState[i])
      {
         mPowerState[i] = false;

         wxBitmapButton *btn =
            static_cast<wxBitmapButton *>(FindWindowById(ID_POWER + i));
         btn->SetBitmapLabel(mPowerRaised);
         btn->SetBitmapSelected(mPowerRaised);
      }
   }

   UpdateActive();
}

void EffectsRack::OnBypass(wxCommandEvent & evt)
{
   mBypassing = evt.GetInt() != 0;
   UpdateActive();
}

void EffectsRack::OnPower(wxCommandEvent & evt)
{
   wxBitmapButton *btn =  static_cast<wxBitmapButton *>(evt.GetEventObject());

   int index = GetEffectIndex(btn);
   mPowerState[index] = !mPowerState[index];
   if (mPowerState[index])
   {
      btn->SetBitmapLabel(mPowerPushed);
      btn->SetBitmapSelected(mPowerPushed);
   }
   else
   {
      btn->SetBitmapLabel(mPowerRaised);
      btn->SetBitmapSelected(mPowerRaised);
   }

   UpdateActive();
}

void EffectsRack::OnEditor(wxCommandEvent & evt)
{
   wxBitmapButton *btn =  static_cast<wxBitmapButton *>(evt.GetEventObject());

   evt.Skip();

   int index = GetEffectIndex(btn);
   if (index < 0)
   {
      return;
   }

   auto pEffect = mEffects[index];
   pEffect->ShowInterface( *GetParent(), EffectUI::DialogFactory,
      pEffect->IsBatchProcessing() );
}

void EffectsRack::OnUp(wxCommandEvent & evt)
{
   wxBitmapButton *btn =  static_cast<wxBitmapButton *>(evt.GetEventObject());

   evt.Skip();

   int index = GetEffectIndex(btn);
   if (index <= 0)
   {
      return;
   }

   MoveRowUp(index);
}

void EffectsRack::OnDown(wxCommandEvent & evt)
{
   wxBitmapButton *btn =  static_cast<wxBitmapButton *>(evt.GetEventObject());

   evt.Skip();

   int index = GetEffectIndex(btn);
   if (index < 0 || index == (mMainSizer->GetChildren().GetCount() / NUMCOLS) - 1)
   {
      return;
   }

   MoveRowUp(index + 1);
}

void EffectsRack::OnFav(wxCommandEvent & evt)
{
   wxBitmapButton *btn =  static_cast<wxBitmapButton *>(evt.GetEventObject());

   int index = GetEffectIndex(btn);
   mFavState[index] = !mFavState[index];
   if (mFavState[index])
   {
      btn->SetBitmapLabel(mFavPushed);
      btn->SetBitmapSelected(mFavPushed);
   }
   else
   {
      btn->SetBitmapLabel(mFavRaised);
      btn->SetBitmapSelected(mFavRaised);
   }
}

void EffectsRack::OnRemove(wxCommandEvent & evt)
{
   wxBitmapButton *btn =  static_cast<wxBitmapButton *>(evt.GetEventObject());

   evt.Skip();

   int index = GetEffectIndex(btn);
   if (index < 0)
   {
      return;
   }

   mEffects.erase(mEffects.begin() + index);
   mPowerState.erase(mPowerState.begin() + index);
   mFavState.erase(mFavState.begin() + index);

   if (mEffects.size() == 0)
   {
      if (mTimer.IsRunning())
      {
         mTimer.Stop();
      }
   }

   index *= NUMCOLS;

   for (int i = 0; i < NUMCOLS; i++)
   {
      std::unique_ptr<wxWindow> w {mMainSizer->GetItem(index)->GetWindow()};
      mMainSizer->Detach(index);
   }

   mMainSizer->Layout();
   Fit();

   UpdateActive();
}

wxBitmap EffectsRack::CreateBitmap(const char *const xpm[], bool up, bool pusher)
{
   wxMemoryDC dc;
   wxBitmap pic(xpm);

   wxBitmap mod(pic.GetWidth() + 6, pic.GetHeight() + 6);
   dc.SelectObject(mod);
#if defined( __WXGTK__ )
   wxColour newColour = wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND );
#else
   wxColour newColour = wxSystemSettings::GetColour( wxSYS_COLOUR_3DFACE );
#endif
   dc.SetBackground(wxBrush(newColour));
   dc.Clear();

   int offset = 3;
   if (pusher)
   {
      if (!up)
      {
         offset += 1;
      }
   }
   dc.DrawBitmap(pic, offset, offset, true);

   dc.SelectObject(wxNullBitmap);

   return mod;
}

int EffectsRack::GetEffectIndex(wxWindow *win)
{
   int col = (win->GetId() - ID_BASE) / ID_RANGE;
   int row;
   int cnt = mMainSizer->GetChildren().GetCount() / NUMCOLS;
   for (row = 0; row < cnt; row++)
   {
      wxSizerItem *si = mMainSizer->GetItem((row * NUMCOLS) + col);
      if (si->GetWindow() == win)
      {
         break;
      }
   }

   if (row == cnt)
   {
      return -1;
   }

   return row;
}

void EffectsRack::MoveRowUp(int row)
{
   Effect *effect = mEffects[row];
   mEffects.erase(mEffects.begin() + row);
   mEffects.insert(mEffects.begin() + row - 1, effect);

   int state = mPowerState[row];
   mPowerState.erase(mPowerState.begin() + row);
   mPowerState.insert(mPowerState.begin() + row - 1, state);

   state = mFavState[row];
   mFavState.erase(mFavState.begin() + row);
   mFavState.insert(mFavState.begin() + row - 1, state);

   row *= NUMCOLS;

   for (int i = 0; i < NUMCOLS; i++)
   {
      wxSizerItem *si = mMainSizer->GetItem(row + NUMCOLS - 1);
      wxWindow *w = si->GetWindow();
      int flags = si->GetFlag();
      int border = si->GetBorder();
      int prop = si->GetProportion();
      mMainSizer->Detach(row + NUMCOLS - 1);
      mMainSizer->Insert(row - NUMCOLS, w, prop, flags, border);
   }

   mMainSizer->Layout();
   Refresh();

   UpdateActive();
}

void EffectsRack::UpdateActive()
{
   mActive.clear();

   if (!mBypassing)
   {
      for (size_t i = 0, cnt = mEffects.size(); i < cnt; i++)
      {
         if (mPowerState[i])
         {
            mActive.push_back(mEffects[i]);
         }
      }
   }

   RealtimeEffectManager::Get().RealtimeSetEffects(
      { mActive.begin(), mActive.end() }
   );
}

namespace
{
SaucedacityProject::AttachedWindows::RegisteredFactory sKey{
   []( SaucedacityProject &parent ) -> wxWeakRef< wxWindow > {
      auto result = safenew EffectsRack( parent );
      result->CenterOnParent();
      return result;
   }
};
}

EffectsRack &EffectsRack::Get( SaucedacityProject &project )
{
   return project.AttachedWindows::Get< EffectsRack >( sKey );
}

#endif
