/**********************************************************************

Audacity: A Digital Audio Editor

HitTestResult.h

Paul Licameli

**********************************************************************/

#ifndef __AUDACITY_HIT_TEST_RESULT__
#define __AUDACITY_HIT_TEST_RESULT__

#include <memory>
#include <utility>

// Saucedacity libraries
#include <lib-strings/Internat.h> // for TranslatableString

class wxCursor;

struct HitTestPreview
{
   HitTestPreview()
   {}

   HitTestPreview(TranslatableString message_, wxCursor *cursor_,
      TranslatableString tooltip_ = {})
      : message{std::move( message_ )}, cursor{ cursor_ }, tooltip{std::move( tooltip_ )}
   {}

   TranslatableString message {};
   wxCursor *cursor {};
   TranslatableString tooltip{};
};

#endif
