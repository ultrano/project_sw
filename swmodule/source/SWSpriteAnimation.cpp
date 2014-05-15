#include "SWSpriteAnimation.h"
#include "SWSpriteSequence.h"
#include "SWSprite.h"
#include "SWAssets.h"
#include "json.h"


SWHardRef<SWSpriteAnimation> SWSpriteAnimation::create( const tstring& json )
{
	Json::Reader reader;
	Json::Value root;

	if ( !reader.parse( json, root ) ) return NULL;

	Json::Value sprites = root.get( "sprites", Json::nullValue );
	if ( sprites == Json::nullValue ) return NULL;

	Json::Value sequences = root.get( "sequences", Json::nullValue );
	if ( sequences == Json::nullValue ) return NULL;

	ttable<tstring, SWHardRef<SWSprite> > spriteTable;
	for ( int i = 0 ; i < sprites.size() ; ++i )
	{
		Json::Value info = sprites.get( (Json::Value::UInt)i, Json::nullValue );
		if ( info == Json::nullValue ) continue;
		if ( info.type() != Json::objectValue ) continue;

		Json::Value name = info.get( "name", Json::nullValue );
		if ( name == Json::nullValue ) continue;
		if ( name.type() != Json::stringValue ) continue;

		Json::Value texFile = info.get( "texture", Json::nullValue );
		if ( texFile == Json::nullValue ) continue;
		if ( texFile.type() != Json::stringValue ) continue;
		
		Json::Value rect = info.get( "rect", Json::nullValue );
		if ( rect == Json::nullValue ) continue;
		if ( rect.type() != Json::arrayValue ) continue;
		if ( rect.size() != 4 ) continue;

		Json::Value offsetX = rect.get( (Json::Value::UInt)0, Json::nullValue );
		if ( offsetX == Json::nullValue ) continue;
		
		Json::Value offsetY = rect.get( (Json::Value::UInt)1, Json::nullValue );
		if ( offsetY == Json::nullValue ) continue;
		
		Json::Value width = rect.get( (Json::Value::UInt)2, Json::nullValue );
		if ( width == Json::nullValue ) continue;
		
		Json::Value height = rect.get( (Json::Value::UInt)3, Json::nullValue );
		if ( height == Json::nullValue ) continue;

		SWHardRef<SWTexture> texture = SWAssets.loadTexture( texFile.asString() );
		if ( texture.isValid() == false ) continue;

		SWHardRef<SWSprite> sprite = new SWSprite( texture, offsetX.asInt(), offsetY.asInt(), width.asInt(), height.asInt() );

		spriteTable.insert( std::make_pair( name.asString(), sprite ) );
	}

	SWHardRef<SWSpriteAnimation> animation = new SWSpriteAnimation();
	animation()->m_sequences.reserve( sequences.size() );
	for ( int i = 0 ; i < sequences.size() ; ++i )
	{
		Json::Value info = sequences.get( (Json::Value::UInt)i, Json::nullValue );
		if ( info == Json::nullValue ) continue;
		if ( info.type() != Json::objectValue ) continue;

		Json::Value name = info.get( "name", Json::nullValue );
		if ( name == Json::nullValue ) continue;
		if ( name.type() != Json::stringValue ) continue;

		Json::Value delay = info.get( "delay", Json::nullValue );
		if ( delay == Json::nullValue ) continue;
		if ( delay.type() != Json::realValue ) continue;

		Json::Value sprites = info.get( "sprites", Json::nullValue );
		if ( sprites == Json::nullValue ) continue;
		if ( sprites.type() != Json::arrayValue ) continue;

		SWHardRef<SWSpriteSequence> sequence = new SWSpriteSequence();
		sequence()->setName( name.asString() );
		sequence()->setDelayPerUnit( (float)delay.asDouble() );

		for ( int j = 0 ; j < sprites.size() ; ++j )
		{
			Json::Value spriteName = sprites.get( (Json::Value::UInt)j, Json::nullValue );
			if ( spriteName == Json::nullValue ) continue;
			if ( spriteName.type() != Json::stringValue ) continue;

			ttable<tstring, SWHardRef<SWSprite> >::iterator itor = spriteTable.find( spriteName.asString() );
			if ( itor == spriteTable.end() ) continue;

			sequence()->addSprite( itor->second() );
		}

		animation()->m_sequences.push_back( sequence() );
	}

	return animation;
}

SWSpriteAnimation::SWSpriteAnimation()
{

}

SWSpriteAnimation::~SWSpriteAnimation()
{
}

void SWSpriteAnimation::addSequence( SWSpriteSequence* sequence )
{
	for ( tuint i = 0 ; i < m_sequences.size() ; ++i )
	{
		if ( m_sequences[i]() == sequence ) return;
	}
	m_sequences.push_back( sequence );
}

SWSpriteSequence* SWSpriteAnimation::getSequenceByName( const tstring& name ) const
{
	for ( tuint i = 0 ; i < m_sequences.size() ; ++i )
	{
		SWSpriteSequence* seq = getSequenceAt( i );
		if ( seq == NULL ) return NULL;
		if ( seq->getName() == name ) return seq;
	}
	return NULL;
}

SWSpriteSequence* SWSpriteAnimation::getSequenceAt( tuint index ) const
{
	if ( index >= m_sequences.size() ) return NULL;
	SWHardRef<SWSpriteSequence> seq = m_sequences.at( index );
	return seq();
}

tuint SWSpriteAnimation::count() const
{
	return m_sequences.size();
}