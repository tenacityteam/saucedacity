/**********************************************************************

  Audacity: A Digital Audio Editor

  AVInputFormatWrapperImpl.inl

  Dmitry Vedenko

**********************************************************************/

class AVInputFormatWrapperImpl : public AVInputFormatWrapper
{
public:
   explicit AVInputFormatWrapperImpl(AVInputFormat* wrapped)
      : AVInputFormatWrapper(wrapped)
   {}

   [[nodiscard]] const char* GetName() const noexcept override
   {
      if (mAVInputFormat != nullptr)
         return mAVInputFormat->name;

      return {};
   }

   [[nodiscard]] const char* GetLongName() const noexcept override
   {
      if (mAVInputFormat != nullptr)
         return mAVInputFormat->long_name;

      return {};
   }

   [[nodiscard]] int GetFlags() const noexcept override
   {
      if (mAVInputFormat != nullptr)
         return mAVInputFormat->flags;

      return {};
   }

   [[nodiscard]] const char* GetExtensions() const noexcept override
   {
      if (mAVInputFormat != nullptr)
         return mAVInputFormat->extensions;

      return {};
   }

   [[nodiscard]] const struct AVCodecTag* const* GetCodecTag() const noexcept override
   {
      if (mAVInputFormat != nullptr)
         return mAVInputFormat->codec_tag;

      return {};
   }
};

std::unique_ptr<AVInputFormatWrapper> CreateAVInputFormatWrapper (AVInputFormat* wrapped)
{
   return std::make_unique<AVInputFormatWrapperImpl>(wrapped);
}
