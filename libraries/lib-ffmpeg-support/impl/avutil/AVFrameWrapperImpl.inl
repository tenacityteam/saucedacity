/**********************************************************************

  Audacity: A Digital Audio Editor

  AVFrameWrapperImpl.inl

  Dmitry Vedenko

**********************************************************************/

class AVFrameWrapperImpl : public AVFrameWrapper
{
public:
   explicit
   AVFrameWrapperImpl(const FFmpegFunctions& ffmpeg)
      : AVFrameWrapper(ffmpeg)
   {
   }

   [[nodiscard]] int GetNumDataPointers() const noexcept override
   {
      return AV_NUM_DATA_POINTERS;
   }

   [[nodiscard]] uint8_t* GetData(int index) const noexcept override
   {
      if (mAVFrame == nullptr)
         return {};

      if (index < 0 || index >= AV_NUM_DATA_POINTERS)
         return {};

      return mAVFrame->data[index];
   }

   [[nodiscard]] int GetLineSize(int index) const noexcept override
   {
      if (mAVFrame == nullptr)
         return {};

      if (index < 0 || index >= AV_NUM_DATA_POINTERS)
         return {};

      return mAVFrame->linesize[index];
   }

   [[nodiscard]] uint8_t* GetExtendedData(int index) const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->extended_data[index];

      return {};
   }

   [[nodiscard]] int GetWidth() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->width;

      return {};
   }

   [[nodiscard]] int GetHeight() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->height;

      return {};
   }

   [[nodiscard]] int GetSamplesCount() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->nb_samples;

      return {};
   }

   void SetSamplesCount(int count) noexcept override
   {
      if (mAVFrame != nullptr)
         mAVFrame->nb_samples = count;
   }

   [[nodiscard]] AVSampleFormatFwd GetFormat() const noexcept override
   {
      if (mAVFrame != nullptr)
         return static_cast<AVSampleFormatFwd>(mAVFrame->format);

      return {};
   }

   void SetFormat(AVSampleFormatFwd format) noexcept override
   {
      if (mAVFrame != nullptr)
         mAVFrame->format = format;
   }

   [[nodiscard]] int GetKeyFrame() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->key_frame;

      return {};
   }

   [[nodiscard]] AudacityAVRational GetSampleAspectRatio() const noexcept override
   {
      if (mAVFrame != nullptr)
         return { mAVFrame->sample_aspect_ratio.num,
               mAVFrame->sample_aspect_ratio.den };

      return {};
   }

   [[nodiscard]] int64_t GetPresentationTimestamp() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->pts;

      return {};
   }

   [[nodiscard]] int64_t GetPacketPresentationTimestamp() const noexcept override
   {
      if (mAVFrame != nullptr)
#if LIBAVUTIL_VERSION_MAJOR <= 56
         return mAVFrame->pkt_pts;
#else
         return mAVFrame->pts;
#endif

      return {};
   }

   [[nodiscard]] int64_t GetPacketDecompressionTimestamp() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->pkt_dts;

      return {};
   }

   [[nodiscard]] int GetCodedPictureNumber() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->coded_picture_number;

      return {};
   }

   [[nodiscard]] int GetDisplayPictureNumber() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->display_picture_number;

      return {};
   }

   [[nodiscard]] int GetQuality() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->quality;

      return {};
   }

   [[nodiscard]] void* GetOpaque() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->opaque;

      return {};
   }

   void SetOpaque(void* opaque) noexcept override
   {
      if (mAVFrame != nullptr)
         mAVFrame->opaque = opaque;
   }

   [[nodiscard]] int GetRepeatPict() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->repeat_pict;

      return {};
   }

   [[nodiscard]] int GetInterlacedFrame() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->interlaced_frame;

      return {};
   }

   [[nodiscard]] int GetTopFieldFirst() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->top_field_first;

      return {};
   }

   [[nodiscard]] int GetPaletteHasChanged() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->palette_has_changed;

      return {};
   }

   [[nodiscard]] int64_t GetReorderedOpaque() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->reordered_opaque;

      return {};
   }

   [[nodiscard]] int GetSampleRate() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->sample_rate;

      return {};
   }

   [[nodiscard]] uint64_t GetChannelLayout() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->channel_layout;

      return {};
   }

   void SetChannelLayout(uint64_t layout) noexcept override
   {
      if (mAVFrame != nullptr)
      {
         mAVFrame->channel_layout = layout;
#if LIBAVUTIL_VERSION_MAJOR >= 56
         mAVFrame->channels =
            mFFmpeg.av_get_channel_layout_nb_channels(layout);
#endif
      }
   }

   [[nodiscard]] int GetSideDataCount() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->nb_side_data;

      return {};
   }

   [[nodiscard]] int GetFlags() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->flags;

      return {};
   }

   [[nodiscard]] int64_t GetBestEffortTimestamp() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->best_effort_timestamp;

      return {};
   }

   [[nodiscard]] int64_t GetPacketPos() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->pkt_pos;

      return {};
   }

   [[nodiscard]] int64_t GetPacketDuration() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->pkt_duration;

      return {};
   }

   [[nodiscard]] AVDictionaryWrapper GetMetadata() const noexcept override
   {
      if (mAVFrame != nullptr)
         return AVDictionaryWrapper(mFFmpeg, mAVFrame->metadata);

      return AVDictionaryWrapper(mFFmpeg);
   }

   [[nodiscard]] int GetDecodeErrorFlags() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->decode_error_flags;

      return {};
   }

   [[nodiscard]] int GetChannels() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->channels;

      return {};
   }

   [[nodiscard]] int GetPacketSize() const noexcept override
   {
      if (mAVFrame != nullptr)
         return mAVFrame->pkt_size;

      return {};
   }
};

std::unique_ptr<AVFrameWrapper> CreateAVFrameWrapper(const FFmpegFunctions& ffmpeg)
{
   return std::make_unique<AVFrameWrapperImpl>(ffmpeg);
}
