/**********************************************************************

  Audacity: A Digital Audio Editor

  AboutDialog.h

  Dominic Mazzoni

**********************************************************************/

#ifndef __AUDACITY_ABOUT_DLG__
#define __AUDACITY_ABOUT_DLG__

#include <vector>
#include "widgets/wxPanelWrapper.h" // to inherit

class wxStaticBitmap;
class wxTextOutputStream;

class ShuttleGui;

struct AboutDialogCreditItem {
   AboutDialogCreditItem( TranslatableString str, int r )
      : description{ std::move( str ) }, role{ r }
   {}
   TranslatableString description;
   int role;
};

using AboutDialogCreditItemsList = std::vector<AboutDialogCreditItem>;

class SAUCEDACITY_DLL_API AboutDialog final : public wxDialogWrapper {
   DECLARE_DYNAMIC_CLASS(AboutDialog)

 public:
   explicit AboutDialog(wxWindow * parent);
   ~ AboutDialog() override;

   static AboutDialog *ActiveInstance();

   void OnOK(wxCommandEvent & event);

   wxStaticBitmap *icon;

   DECLARE_EVENT_TABLE()

 private:
   enum Role {
      // Saucedacity Roles
      roleSaucedacityTeamMember,
      roleSaucedacityContributor,
      roleSaucedacityGraphics,
      roleSaucedacityLibrary,
      roleSaucedacityThanks,

      // Tenacity Roles
      roleTenacityTeamMember,
      roleTenacityThanks,
      roleTenacityLibrary,

      // Regular Audacity team member roles
      roleAudacityTeamMember,
      roleAudacityEmeritusTeam,
      roleAudacityDeceased,
      roleAudacityContributor,
      roleAudacityGraphics,
      roleAudacityLibrary,
      roleAudacityThanks
   };

   AboutDialogCreditItemsList creditItems;
   void PopulateAudacityPage( ShuttleGui & S );
   static void PopulateLicensePage( ShuttleGui & S );
   void PopulateInformationPage (ShuttleGui & S );

   void CreateCreditsList();
   void AddCredit( const wxString &name, Role role );
   void AddCredit( const wxString &name, const TranslatableString& format, Role role );
   wxString GetCreditsByRole(AboutDialog::Role role);

   static void AddBuildinfoRow( wxTextOutputStream *str, const wxChar * libname,
      const TranslatableString &libdesc, const TranslatableString &status);
   static void AddBuildinfoRow( wxTextOutputStream *str,
      const TranslatableString &description, const wxChar *spec);
};
#endif
