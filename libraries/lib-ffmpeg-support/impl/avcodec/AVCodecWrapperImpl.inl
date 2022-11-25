/**********************************************************************

  Audacity: A Digital Audio Editor

  AVCodecWrapperImpl.inl

  Dmitry Vedenko

**********************************************************************/
class AVCodecWrapperImpl : public AVCodecWrapper
{
public:
   explicit
   AVCodecWrapperImpl(const AVCodec* wrapped) noexcept
      : AVCodecWrapper(wrapped)
   {
   }

   [[nodiscard]] const char* GetName() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->name;

      return {};
   }

   [[nodiscard]] const char* GetLongName() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->long_name;

      return {};
   }

   [[nodiscard]] AVMediaTypeFwd GetType() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->type;

      return {};
   }

   [[nodiscard]] AVCodecIDFwd GetId() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->id;

      return {};
   }

   [[nodiscard]] int GetCapabilities() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->capabilities;

      return {};
   }

   [[nodiscard]] const AVRational* GetSupportedFramerates() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->supported_framerates;

      return {};
   }

   [[nodiscard]] const AVPixelFormatFwd* GetPixFmts() const noexcept override
   {
      static_assert(sizeof(AVPixelFormat) == sizeof(AVPixelFormatFwd));
      if (mAVCodec != nullptr)
         return reinterpret_cast<const AVPixelFormatFwd*>(mAVCodec->pix_fmts);

      return {};
   }

   [[nodiscard]] const int* GetSupportedSamplerates() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->supported_samplerates;

      return {};
   }

   [[nodiscard]] const AVSampleFormatFwd* GetSampleFmts() const noexcept override
   {
      static_assert(sizeof(AVSampleFormat) == sizeof(AVSampleFormatFwd));

      if (mAVCodec != nullptr)
         return reinterpret_cast<const AVSampleFormatFwd*>(mAVCodec->sample_fmts);

      return {};
   }

   [[nodiscard]] const uint64_t* GetChannelLayouts() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->channel_layouts;

      return {};
   }

   [[nodiscard]] uint8_t GetMaxLowres() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->max_lowres;

      return {};
   }

   [[nodiscard]] bool IsAudio() const noexcept override
   {
      if (mAVCodec != nullptr)
         return mAVCodec->type == AVMEDIA_TYPE_AUDIO;

      return {};
   }
};

std::unique_ptr<AVCodecWrapper>CreateAVCodecWrapper(const AVCodec* obj)
{
   return std::make_unique<AVCodecWrapperImpl>(obj);
}
