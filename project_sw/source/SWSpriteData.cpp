//
//  SWSpriteData.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWSpriteData.h"
#include "SWImageRegion.h"

SWSpriteData::SWSpriteData()
{
    
}

SWSpriteData::~SWSpriteData()
{
    
}


void      SWSpriteData::setFrames( const Frames& frames )
{
    m_frames = frames;
}

const SWSpriteData::Frames&   SWSpriteData::getFrames()
{
    return m_frames;
}

unsigned int SWSpriteData::addRegion( const SWImageRegion& region )
{
    m_frames.push_back( region );
    return m_frames.size() - 1;
}

unsigned int SWSpriteData::addRegion( const int& img, int x, int y, int width, int height )
{
    return addRegion( SWImageRegion( img, x, y, width, height ) );
}

bool SWSpriteData::getRegion( unsigned int index, SWImageRegion& region )
{
    if ( index >= m_frames.size() ) return false;
    region = m_frames.at( index );
    return true;
}

bool SWSpriteData::setRegion( unsigned int index, const SWImageRegion& region )
{
    if ( index >= m_frames.size() ) return false;
    m_frames.at( index ) = region;
    return true;
}

void SWSpriteData::setSequence( const std::string& sequence, const Sequence &frames )
{
    m_sequences[sequence] = frames;
}

SWSpriteData::Sequence* SWSpriteData::getSequence( const std::string& sequence )
{
    SequenceTable::iterator itor = m_sequences.find( sequence );
    if ( itor != m_sequences.end() )
    {
        return &(itor->second);
    }
    return NULL;
}