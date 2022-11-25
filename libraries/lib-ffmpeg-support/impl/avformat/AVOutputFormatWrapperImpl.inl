/**********************************************************************

  Audacity: A Digital Audio Editor

  AVOutputFormatWrapperImpl.inl

  Dmitry Vedenko

**********************************************************************/

class AVOutputFormatWrapperImpl : public AVOutputFormatWrapper
{
public:
   explicit AVOutputFormatWrapperImpl(const AVOutputFormat* wrapped)
      : AVOutputFormatWrapper(wrapped)
   {
   }

   [[nodiscard]] const char* GetName() const noexcept override
   {
      if (mAVOutputFormat != nullptr)
         return mAVOutputFormat->name;

      return {};
   }

   [[nodiscard]] const char* GetLongName() const noexcept override
   {
      if (mAVOutputFormat != nullptr)
         return mAVOutputFormat->long_name;

      return {};
   }

   [[nodiscard]] const char* GetMimeType() const noexcept override
   {
      if (mAVOutputFormat != nullptr)
         return mAVOutputFormat->mime_type;

      return {};
   }

   [[nodiscard]] const char* GetExtensions() const noexcept override
   {
      if (mAVOutputFormat != nullptr)
         return mAVOutputFormat->extensions;

      return {};
   }

   [[nodiscard]] AVCodecIDFwd GetAudioCodec() const noexcept override
   {
      if (mAVOutputFormat != nullptr)
         return mAVOutputFormat->audio_codec;

      return {};
   }

   [[nodiscard]] int GetFlags() const noexcept override
   {
      if (mAVOutputFormat != nullptr)
         return mAVOutputFormat->flags;

      return {};
   }

   [[nodiscard]] const struct AVCodecTag* const* GetCodecTag() const noexcept override
   {
      if (mAVOutputFormat != nullptr)
         return mAVOutputFormat->codec_tag;

      return {};
   }
};

std::unique_ptr<AVOutputFormatWrapper> CreateAVOutputFormatWrapper (const AVOutputFormat* wrapped)
{
   return std::make_unique<AVOutputFormatWrapperImpl>(wrapped);
}
