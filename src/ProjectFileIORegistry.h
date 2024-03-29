/**********************************************************************

  Audacity: A Digital Audio Editor

  ProjectFileIORegistry.h

  Paul Licameli

**********************************************************************/

#ifndef __AUDACITY_PROJECT_FILE_IO_REGISTRY__
#define __AUDACITY_PROJECT_FILE_IO_REGISTRY__

#include <functional>

class TenacityProject;
class XMLTagHandler;
class wxString;

namespace ProjectFileIORegistry {

// Type of functions returning objects that interpret a part of the saved XML
using TagHandlerFactory =
   std::function< XMLTagHandler *( TenacityProject & ) >;

// Typically statically constructed
struct TENACITY_DLL_API Entry{
   Entry( const wxString &tag, const TagHandlerFactory &factory );
};

TagHandlerFactory Lookup( const wxString &tag );

}

#endif
