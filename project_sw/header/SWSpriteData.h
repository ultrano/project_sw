//
//  SWSpriteData.h
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWSpriteData_h
#define protopang_SWSpriteData_h

#include "SWRefCounter.h"
#include "SWImageRegion.h"
#include <string>
#include <vector>
#include <map>

class SWSpriteData : public SWRefCountable
{
public:

    typedef std::vector<SWImageRegion>     Frames;
    typedef std::vector<int>               Sequence;
    typedef std::map<std::string,Sequence> SequenceTable;
    
private:

    Frames         m_frames;
    SequenceTable  m_sequences;
    
public:
    
    SWSpriteData();
    ~SWSpriteData();
    
    void          setFrames( const Frames& frames );
    const Frames& getFrames();
    
    unsigned int addRegion( const SWImageRegion& region );
    unsigned int addRegion( const int& img, int x, int y, int width, int height );
    
    
    bool getRegion( unsigned int index, SWImageRegion& region );
    bool setRegion( unsigned int index, const SWImageRegion& region );
    
    void      setSequence( const std::string& sequence, const Sequence &frames );
    Sequence* getSequence( const std::string& sequence );
    
    
};

#endif
