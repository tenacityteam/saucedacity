/**********************************************************************

  Audacity: A Digital Audio Editor

  SplashDialog.cpp

  James Crook

********************************************************************//**

\class SplashDialog
\brief The SplashDialog shows help information for Audacity when
Audacity starts up.

It was written for the benefit of NEW users who do not want to
read the manual.  The text of the dialog is kept short to increase the
chance of it being read.  The content is designed to reduce the
most commonly asked questions about Audacity.

*//********************************************************************/



#include "SplashDialog.h"



#include <wx/dialog.h>
#include <wx/frame.h>
#include <wx/html/htmlwin.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/intl.h>
#include <wx/image.h>

#include "FileNames.h"
#include "Project.h"
#include "ShuttleGui.h"
#include "widgets/AudacityMessageBox.h"
#include "widgets/HelpSystem.h"

#include "AllThemeResources.h"
#include "Prefs.h"
#include "HelpText.h"

// DA: Logo for Splash Dialog (welcome dialog)
#ifdef EXPERIMENTAL_DA
#include "../images/DarkAudacityLogoWithName.xpm"
#else
#include "../images/SaucedacityLogoWithName.xpm"
#endif

SplashDialog * SplashDialog::pSelf=NULL;

/*enum
{
   DontShowID=1000,
};*/

BEGIN_EVENT_TABLE(SplashDialog, wxDialogWrapper)
   EVT_BUTTON(wxID_OK, SplashDialog::OnOK)
//   EVT_CHECKBOX( DontShowID, SplashDialog::OnDontShow )
END_EVENT_TABLE()

IMPLEMENT_CLASS(SplashDialog, wxDialogWrapper)

void SplashDialog::DoHelpWelcome( AudacityProject &project )
{
   Show2( &GetProjectFrame( project ) );
}

SplashDialog::SplashDialog(wxWindow * parent)
   :  wxDialogWrapper(parent, -1, XO("Welcome to Saucedacity!"),
      wxPoint( -1, 60 ), // default x position, y position 60 pixels from top of screen.
      wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
   SetName();
   m_pLogo = NULL; //v
   ShuttleGui S( this, eIsCreating );
   Populate( S );
   Fit();
   this->Centre();
   int x,y;
   GetPosition( &x, &y );
   Move( x, 60 );

   // Alpha/beta notice dialog
   #if defined(IS_ALPHA) || defined(IS_BETA)
   bool ShouldShow;
   gPrefs->Read(wxT("/GUI/ShowSplashScreen"), &ShouldShow, true );

   if (ShouldShow)
   {
     AudacityMessageBox(XO("The version of Saucedacity you are using is a pre-release version. Use of "
                        "these releases in production environments is discouraged. This version "
                        "of Saucedacity might contain (unknown) bugs along with possible data "
                        "loss."),
                        XO("Saucedacity Pre-Release"),
                        wxOK);
   }
   #endif
}

void SplashDialog::OnChar(wxMouseEvent &event)
{
   if ( event.ShiftDown() && event.ControlDown() )
      wxLaunchDefaultBrowser("https://saucedacity.github.io");
}

void SplashDialog::Populate( ShuttleGui & S )
{
   bool bShow;
   gPrefs->Read(wxT("/GUI/ShowSplashScreen"), &bShow, true );
   S.StartVerticalLay(1);

   //v For now, change to AudacityLogoWithName via old-fashioned ways, not Theme.
   //m_pLogo = std::make_unique<wxBitmap>((const char **) SaucedacityLogoWithName_xpm); //v


   // JKC: Resize to 50% of size.  Later we may use a smaller xpm as
   // our source, but this allows us to tweak the size - if we want to.
   // It also makes it easier to revert to full size if we decide to.
   //const float fScale=0.5f;// smaller size.
   //wxImage RescaledImage( m_pLogo->ConvertToImage() );
   //wxColour MainColour( 
   //   RescaledImage.GetRed(1,1), 
   //   RescaledImage.GetGreen(1,1), 
   //   RescaledImage.GetBlue(1,1));
   //this->SetBackgroundColour(MainColour);

   // wxIMAGE_QUALITY_HIGH not supported by wxWidgets 2.6.1, or we would use it here.
   // GP: ANSWER-ME: Don't we already require wxWidgets 3.1.3 (Audacity's version)? If
   // so, then shouldn't we have this option already? Please make an issue if you have
   // an answer or make a comment here.
   //RescaledImage.Rescale( (int)(LOGOWITHNAME_WIDTH * fScale), (int)(LOGOWITHNAME_HEIGHT *fScale), wxIMAGE_QUALITY_HIGH );
   //wxBitmap RescaledBitmap( RescaledImage );
   //wxStaticBitmap *const icon =
   //    safenew wxStaticBitmap(S.GetParent(), -1,
                          //*m_pLogo, //v theTheme.Bitmap(bmpAudacityLogoWithName),
   //                       RescaledBitmap,
   //                       wxDefaultPosition,
   //                       wxSize((int)(LOGOWITHNAME_WIDTH*fScale), (int)(LOGOWITHNAME_HEIGHT*fScale)));

   S.Prop(0)
#if  (0)
      .ConnectRoot( wxEVT_LEFT_DOWN, &SplashDialog::OnChar)
#endif
      /*.AddWindow( icon )*/;

   mpHtml = safenew LinkingHtmlWindow(S.GetParent(), -1,
                                         wxDefaultPosition,
                                         wxSize(506, 280),
                                         wxHW_SCROLLBAR_AUTO | wxSUNKEN_BORDER );
   mpHtml->SetPage(HelpText( wxT("welcome") ));
   S.Prop(1)
      .Position( wxEXPAND )
      .AddWindow( mpHtml );
   {
      S.SetBorder( 5 );
      S.SetBorder( 5 );

      S.Id(wxID_OK)
         .Prop(0)
         .AddButton(XXO("OK"), wxALIGN_RIGHT | wxALL, true);
   }
   S.EndVerticalLay();
}

SplashDialog::~SplashDialog()
{
}

void SplashDialog::OnOK(wxCommandEvent & WXUNUSED(event))
{
   gPrefs->Write(wxT("/GUI/ShowSplashScreen"), false );
   gPrefs->Flush();
   Show( false );
}

void SplashDialog::Show2( wxWindow * pParent )
{
   if( pSelf == NULL )
   {
      // pParent owns it
      wxASSERT(pParent);
      pSelf = safenew SplashDialog( pParent );
   }
   pSelf->mpHtml->SetPage(HelpText( wxT("welcome") ));
   pSelf->Show( true );
}
