/**********************************************************************

  Audacity: A Digital Audio Editor

  AVPacketWrapperImpl.inl

  Dmitry Vedenko

**********************************************************************/

class AVPacketWrapperImpl : public AVPacketWrapper
{
public:
   explicit
   AVPacketWrapperImpl(const FFmpegFunctions& ffmpeg) noexcept
      : AVPacketWrapper(ffmpeg)
   {
      if (mFFmpeg.av_packet_alloc != nullptr)
      {
         // A library defining one is assumed to define the other
         // assert(ffmpeg->av_packet_free != nullptr);
         mAVPacket = mFFmpeg.av_packet_alloc();
         mUseAVFree = false;
      }
      else
      {
         mAVPacket =
            static_cast<AVPacket*>(mFFmpeg.av_malloc(sizeof(AVPacket)));
         mUseAVFree = true;
      }

      mFFmpeg.av_init_packet(mAVPacket);
   }

   [[nodiscard]] AudacityAVBufferRef* GetBuf() const noexcept override
   {
      if (mAVPacket != nullptr)
         return reinterpret_cast<AudacityAVBufferRef*>(mAVPacket->buf);

      return {};
   }

   [[nodiscard]] int64_t GetPresentationTimestamp() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->pts;

      return {};
   }

   [[nodiscard]] int64_t GetDecompressionTimestamp() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->dts;

      return {};
   }

   [[nodiscard]] uint8_t* GetData() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->data;

      return {};
   }

   [[nodiscard]] int GetSize() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->size;

      return {};
   }

   bool OffsetPacket(size_t offset) noexcept override
   {
      if (mAVPacket == nullptr)
         return false;

      if (offset >= mAVPacket->size)
      {
         mAVPacket->data += mAVPacket->size;
         mAVPacket->size = 0;

         return false;
      }
      else
      {
         mAVPacket->data += offset;
         mAVPacket->size -= offset;

         return true;
      }
   }

   void ResetData() noexcept override
   {
      if (mAVPacket == nullptr)
         return;

      mAVPacket->data = nullptr;
      mAVPacket->size = 0;
   }

   void ResetTimestamps() noexcept override
   {
      if (mAVPacket == nullptr)
         return;

      mAVPacket->dts = AV_NOPTS_VALUE;
      mAVPacket->pts = AV_NOPTS_VALUE;
   }

   [[nodiscard]] int GetStreamIndex() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->stream_index;

      return {};
   }

   void SetStreamIndex(int index) noexcept override
   {
      if (mAVPacket != nullptr)
         mAVPacket->stream_index = index;
   }

   [[nodiscard]] int GetFlags() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->flags;

      return {};
   }

   [[nodiscard]] int GetDuration() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->duration;

      return {};
   }

   [[nodiscard]] int64_t GetPos() const noexcept override
   {
      if (mAVPacket != nullptr)
         return mAVPacket->pos;

      return {};
   }

   [[nodiscard]] int64_t GetConvergenceDuration() const noexcept override
   {
      if (mAVPacket != nullptr)
#if LIBAVFORMAT_VERSION_MAJOR <= 58
         return mAVPacket->convergence_duration;
#else
         // From FFmpeg docs: "Same as the duration field, but as int64_t."
         // duration is int64_t now, convergence_duration is removed
         return mAVPacket->duration;
#endif

      return {};
   }

   [[nodiscard]] std::unique_ptr<AVPacketWrapper> Clone() const noexcept override
   {
      // Guarantee non-null return, which is assumed elsewhere
      auto copy = std::make_unique<AVPacketWrapperImpl>(mFFmpeg);
      if (mAVPacket == nullptr)
         // should happen only if mFFmpeg is null, and should result in
         // a clone with null for mAVPacket too
         return copy;

      // Make mAVPacket in this and the clone share resources
      mFFmpeg.av_packet_ref(copy->mAVPacket, mAVPacket);

      return std::move(copy);
   }

   void RescalePresentationTimestamp(AudacityAVRational bq, AudacityAVRational cq)
   noexcept override
   {
      if (mAVPacket == nullptr)
         return;

      mFFmpeg.av_rescale_q(mAVPacket->pts, { bq.num, bq.den }, { cq.num, cq.den });
   }
   void RescaleDecompressionTimestamp(AudacityAVRational bq, AudacityAVRational cq)
   noexcept override
   {
      if (mAVPacket == nullptr)
         return;

      mFFmpeg.av_rescale_q(mAVPacket->dts, { bq.num, bq.den }, { cq.num, cq.den });
   }
   void RescaleDuration(AudacityAVRational bq, AudacityAVRational cq)
   noexcept override
   {
      if (mAVPacket == nullptr)
         return;

      mFFmpeg.av_rescale_q(mAVPacket->duration, { bq.num, bq.den }, { cq.num, cq.den });
   }
};



std::unique_ptr<AVPacketWrapper> CreateAVPacketWrapper(const FFmpegFunctions& fns)
{
   return std::make_unique<AVPacketWrapperImpl>(fns);
}

