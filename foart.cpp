#include "foart.h"

void file_t::read(ByteReader* reader)
{
	hdr_t newHdr;
	newHdr.read(reader);
	hdr = newHdr;

	for (size_t i = 0; i < hdr.dirs; i++)
	{
		data_t currData;
		currData.hdr_ptr = &hdr;
		currData.read(reader);
		data.push_back(currData);
	}
}

void hdr_t::read(ByteReader* reader)
{
	check_num1 = reader->u8();
	frames_count = reader->u16();
	anim_ticks = reader->u16();
	dirs = reader->u8();
}

void data_t::read(ByteReader* reader)
{
	offs_x = reader->i16();
	offs_y = reader->i16();
	for (size_t i = 0; i < hdr_ptr->frames_count; i++)
	{
		frame_t currFrame;
		currFrame.read(reader);
		frames.push_back(currFrame);
	}
}

void frame_t::read(ByteReader* reader)
{
	is_shared = reader->u8() > 0;
	if(!is_shared)
	{
		width = reader->u16();
		height = reader->u16();
		next_x = reader->i16();
		next_y = reader->i16();
		for (size_t i = 0, len = width * height; i < len; i++)
		{
			ucolor currPixel{ reader->u8(), reader->u8(), reader->u8(), reader->u8() };
			pixels.push_back(currPixel);
		}
	}
	else
	{
		shared_indx = reader->u16();
	}
}
