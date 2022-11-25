/**********************************************************************

  Audacity: A Digital Audio Editor

  AVIOContextWrapperImpl.inl

  Dmitry Vedenko

**********************************************************************/

class AVIOContextWrapperImpl : public AVIOContextWrapper
{
public:
   explicit AVIOContextWrapperImpl(const FFmpegFunctions& ffmpeg)
      : AVIOContextWrapper(ffmpeg)
   {}

   ~AVIOContextWrapperImpl()
   {
      if (mAVIOContext != nullptr)
         mFFmpeg.av_free(mAVIOContext->buffer);
   }

   [[nodiscard]] unsigned char* GetBuffer() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->buffer;

      return {};
   }

   [[nodiscard]] int GetBufferSize() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->buffer_size;

      return {};
   }

   [[nodiscard]] unsigned char* GetBufPtr() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->buf_ptr;

      return {};
   }

   [[nodiscard]] unsigned char* GetBufEnd() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->buf_end;

      return {};
   }

   [[nodiscard]] void* GetOpaque() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->opaque;

      return {};
   }

   void SetOpaque(void* opaque) noexcept override
   {
      if (mAVIOContext != nullptr)
         mAVIOContext->opaque = opaque;
   }

   [[nodiscard]] int64_t GetPos() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->pos;

      return {};
   }

   [[nodiscard]] int GetEofReached() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->eof_reached;

      return {};
   }

   [[nodiscard]] int GetWriteFlag() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->write_flag;

      return {};
   }

   void SetWriteFlag(int write_flag) noexcept override
   {
      if (mAVIOContext != nullptr)
         mAVIOContext->write_flag = write_flag;
   }

   [[nodiscard]] int GetError() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->error;

      return {};
   }

   void SetError(int error) noexcept override
   {
      if (mAVIOContext != nullptr)
         mAVIOContext->error = error;
   }

   [[nodiscard]] int GetSeekable() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->seekable;

      return {};
   }

   void SetSeekable(int seekable) noexcept override
   {
      if (mAVIOContext != nullptr)
         mAVIOContext->seekable = seekable;
   }

   [[nodiscard]] int GetDirect() const noexcept override
   {
      if (mAVIOContext != nullptr)
         return mAVIOContext->direct;

      return {};
   }

   void SetDirect(int direct) noexcept override
   {
      if (mAVIOContext != nullptr)
         mAVIOContext->direct = direct;
   }

   int Read(uint8_t* buf, int size) override
   {
      if (mpFile == nullptr)
         return AUDACITY_AVERROR(EINVAL);

#if LIBAVFORMAT_VERSION_MAJOR >= 58
      // At least starting from avformat 58 FFmpeg expects EOF to be returned here
      // instead of 0. This is critical for some codecs
      if (mpFile->Eof())
         return AUDACITY_AVERROR_EOF;
#endif
      return static_cast<int>(mpFile->Read(buf, size));
   }
};

std::unique_ptr<AVIOContextWrapper> CreateAVIOContextWrapper(const FFmpegFunctions& ffmpeg)
{
   return std::make_unique<AVIOContextWrapperImpl>(ffmpeg);
}
