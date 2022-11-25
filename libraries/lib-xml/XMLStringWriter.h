/**********************************************************************

  Audacity: A Digital Audio Editor

  XMLStringWriter.h

  Avery King split from XMLWriter.h

**********************************************************************/
#pragma once

#include <wx/string.h>
#include "XMLWriter.h"


/// Wrapper to output XML data to strings.
class XML_API XMLStringWriter final : public wxString, public XMLWriter
{
    public:
        XMLStringWriter(size_t initialSize = 0);
        ~XMLStringWriter() override;

        void Write(const wxString &data) override;
};
