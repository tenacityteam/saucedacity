/*!********************************************************************

Saucedacity: A Digital Audio Editor

@file GenericUIAssert.cpp
@brief Implementation for GenericUI::Assert()

Avery King

**********************************************************************/

#include "GenericUIAssert.h"

#include <utility>

namespace
{

GenericUI::AssertFn __AssertFn;

}

namespace GenericUI
{

void SetAssertFn(AssertFn fn)
{
    __AssertFn = std::move(fn);
}

void Assert(bool condition)
{
    __AssertFn(condition);
}

}
