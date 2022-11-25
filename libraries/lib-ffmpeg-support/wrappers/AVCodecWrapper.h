/**********************************************************************

  Audacity: A Digital Audio Editor

  AVCodecWrapper.h

  Dmitry Vedenko

**********************************************************************/

#pragma once

#include <cstdint>
#include "FFmpegTypes.h"

struct FFmpegFunctions;
typedef struct AVCodec AVCodec;
typedef struct AVRational AVRational;

class FFMPEG_SUPPORT_API AVCodecWrapper
{
public:
   AVCodecWrapper(const AVCodecWrapper&) = delete;
   AVCodecWrapper& operator=(AVCodecWrapper&) = delete;

   AVCodecWrapper(AVCodecWrapper&&) = delete;
   AVCodecWrapper& operator=(AVCodecWrapper&&) = delete;

   explicit AVCodecWrapper(const AVCodec* wrapped) noexcept;

   [[nodiscard]] const AVCodec* GetWrappedValue() const noexcept;

   virtual ~AVCodecWrapper() = default;

   [[nodiscard]] virtual const char* GetName() const noexcept = 0;
   [[nodiscard]] virtual const char* GetLongName() const noexcept = 0;
   [[nodiscard]] virtual AVMediaTypeFwd GetType() const noexcept = 0;
   [[nodiscard]] virtual AVCodecIDFwd GetId() const noexcept = 0;
   [[nodiscard]] virtual int GetCapabilities() const noexcept = 0;
   [[nodiscard]] virtual const AVRational* GetSupportedFramerates() const noexcept = 0;
   [[nodiscard]] virtual const AVMediaTypeFwd* GetPixFmts() const noexcept = 0;
   [[nodiscard]] virtual const int* GetSupportedSamplerates() const noexcept = 0;
   [[nodiscard]] virtual const AVSampleFormatFwd* GetSampleFmts() const noexcept = 0;
   [[nodiscard]] virtual const uint64_t* GetChannelLayouts() const noexcept = 0;
   [[nodiscard]] virtual uint8_t GetMaxLowres() const noexcept = 0;

   [[nodiscard]] virtual bool IsAudio() const noexcept = 0;

protected:
   const AVCodec* mAVCodec { nullptr };
};
