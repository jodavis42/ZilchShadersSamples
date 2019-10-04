///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Callbacks.hpp"

namespace Demo
{

void ZilchCompilerErrorCallback(Zilch::ErrorEvent* e)
{
  Zero::String msg = e->GetFormattedMessage(Zilch::MessageFormat::MsvcCpp);
  ZPrint("%s", msg.c_str());
  ZeroDebugBreak();
}

void ZilchTranslationErrorCallback(Zero::TranslationErrorEvent* e)
{
  Zero::String msg = e->mFullMessage;
  ZPrint("%s", msg.c_str());
  __debugbreak();
}

}//namespace Demo
