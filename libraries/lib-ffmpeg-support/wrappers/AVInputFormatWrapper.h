/**********************************************************************

  Audacity: A Digital Audio Editor

  AVInputFormatWrapper.h

  Dmitry Vedenko

**********************************************************************/

#pragma once

struct FFmpegFunctions;
typedef struct AVInputFormat AVInputFormat;

class FFMPEG_SUPPORT_API AVInputFormatWrapper
{
public:
   AVInputFormatWrapper(const AVInputFormatWrapper&) = delete;
   AVInputFormatWrapper& operator=(AVInputFormatWrapper&) = delete;

   AVInputFormatWrapper(AVInputFormatWrapper&&) = delete;
   AVInputFormatWrapper& operator=(AVInputFormatWrapper&&) = delete;

   explicit AVInputFormatWrapper(AVInputFormat* wrapped) noexcept;

   AVInputFormat* GetWrappedValue() noexcept;
   [[nodiscard]] const AVInputFormat* GetWrappedValue() const noexcept;

   virtual ~AVInputFormatWrapper() = default;

   [[nodiscard]] virtual const char* GetName() const noexcept = 0;
   [[nodiscard]] virtual const char* GetLongName() const noexcept = 0;
   [[nodiscard]] virtual int GetFlags() const noexcept = 0;
   [[nodiscard]] virtual const char* GetExtensions() const noexcept = 0;
   [[nodiscard]] virtual const struct AVCodecTag* const* GetCodecTag() const noexcept = 0;
protected:
   AVInputFormat* mAVInputFormat { nullptr };
};
