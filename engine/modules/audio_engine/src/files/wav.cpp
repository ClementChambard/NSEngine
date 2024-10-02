#include "wav.h"
#include "memory.h"
#include <asserts.h>
#include <platform/filesystem.h>

namespace ns::audio {

struct [[gnu::packed]] WAV_Block {
    char block_id[4];
    u32 block_size;
    byte data[];
};

struct [[gnu::packed]] WAV_HeaderBlock {
    char block_id[4];
    u32 block_size;
    char file_format_id[4];
};

struct [[gnu::packed]] WAV_FormatBlock {
    char block_id[4];
    u32 block_size;
    u16 audio_format;
    u16 channel_cnt;
    u32 sample_rate;
    u32 bytes_per_sec;
    u16 bytes_per_block;
    u16 bits_per_sample;
};

struct [[gnu::packed]] WAV_DataBlock {
    char block_id[4];
    u32 block_size;
    byte data[];
};

static WAV_Block* wav_read_block(ns::fs::File* f) {
    WAV_Block temp;
    usize bytes_read;
    ns::fs::read(f, sizeof(WAV_Block), &temp, &bytes_read);
    WAV_Block *out = reinterpret_cast<WAV_Block*>(ns::alloc_raw(sizeof(WAV_Block) + temp.block_size));
    ns::mem_copy(out, &temp, sizeof(WAV_Block));
    ns::fs::read(f, temp.block_size, out->data, &bytes_read);
    return out;
}

WAV_File::WAV_File(cstr file_name) {
    ns::fs::File f;
    ns::fs::open(file_name, ns::fs::Mode::READ, true, &f);
    WAV_HeaderBlock header;
    usize bytes_read;
    ns::fs::read(&f, sizeof(WAV_HeaderBlock), &header, &bytes_read);
    NS_ASSERT(header.block_id[0] == 'R' && header.block_id[1] == 'I' &&
              header.block_id[2] == 'F' && header.block_id[3] == 'F');
    NS_ASSERT(header.file_format_id[0] == 'W' && header.file_format_id[1] == 'A' &&
              header.file_format_id[2] == 'V' && header.file_format_id[3] == 'E');
    usize to_read = header.block_size + sizeof(WAV_Block) - sizeof(WAV_HeaderBlock);
    while (to_read > 0) {
        WAV_Block* block = wav_read_block(&f);
        to_read -= (block->block_size + sizeof(WAV_Block));
        if (block->block_id[0] == 'f' && block->block_id[1] == 'm' &&
            block->block_id[2] == 't' && block->block_id[3] == ' ') {
            WAV_FormatBlock* fmt = reinterpret_cast<WAV_FormatBlock*>(block);
            audio_format = WAV_AudioFormat(fmt->audio_format);
            channel_cnt = fmt->channel_cnt;
            sample_rate = fmt->sample_rate;
            bytes_per_sec = fmt->bytes_per_sec;
            bytes_per_block = fmt->bytes_per_block;
            bits_per_sample = fmt->bits_per_sample;
        } else if (block->block_id[0] == 'd' && block->block_id[1] == 'a' &&
                   block->block_id[2] == 't' && block->block_id[3] == 'a') {
            WAV_DataBlock* datablock = reinterpret_cast<WAV_DataBlock*>(block);
            data_length = datablock->block_size;
            data = ns::alloc_n<byte>(data_length);
            ns::mem_copy(data, datablock->data, data_length);
        }
        ns::free_raw(block, sizeof(WAV_Block) + block->block_size);
    }
    ns::fs::close(&f);
    NS_ASSERT(data != nullptr);
}

WAV_File::~WAV_File() {
    ns::free_n(data, data_length);
}

}
