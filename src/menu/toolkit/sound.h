/*
data = 0x46464952;		fwrite(&data,4,1,wave); //RIFF
data = size+36;			fwrite(&data,4,1,wave); //Header size
data = 0x45564157;		fwrite(&data,4,1,wave); //WAVE
data = 0x20746D66;		fwrite(&data,4,1,wave); //fmt
data = 16;				fwrite(&data,4,1,wave); //fixed1
data = 1;				fwrite(&data,2,1,wave); //format tag
data = 1;				fwrite(&data,2,1,wave); //Channels
(mono)
data = freq;			fwrite(&data,4,1,wave); //Sample rate
data = freq*2;			fwrite(&data,4,1,wave); //Bytes per second
data = 2;				fwrite(&data,2,1,wave); //Bytes per
sample
data = 16;				fwrite(&data,2,1,wave); //Bits per
sample
data = 0x61746164;		fwrite(&data,4,1,wave); //data
data = size;			fwrite(&data,4,1,wave); //Audio data size


*/

//
// RIFF WAVE fmt file header, specifically for converted GTA2 samples
//
typedef struct {
  char[4] riff_chunk_id; // RIFF
  uint32_t riff_chunk_size;
  char[4] riff_type; // WAVE

  char[4] fmt_chunk_id;   // "fmt "
  char[1] fmt_16;         // 16
  char[1] fmt_format_tag; // 1
  char[1] fmt_channels;   // 1 (mono)
  uint32_t fmt_sample_rate;
  uint32_t fmt_bytes_per_second;
  uint32_t fmt_bytes_per_sample; // 2
  uint16_t fmt_significant_bits; // 16
  char[8] fmt_extra_format_bytes;
  uint32_t fmt_audio_data_size;

} tk_sdt2wav_header_t;
