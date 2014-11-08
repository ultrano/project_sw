#ifndef SWDictionary_h__
#define SWDictionary_h__

#include "SWObject.h"

class SWDictionary: public SWObject
{
	SW_RTTI( SWDictionary, SWObject );
public:
	typedef ttable< SWObject::Ref, SWObject::Ref > Value;
	typedef Value::iterator iterator;
	typedef Value::const_iterator const_iterator;

	SWDictionary();
	~SWDictionary();

	SWObject* findObject( const SWObject* key ) const;
	SWObject* findObject( const tstring& key ) const;

	void insert( const SWObject* key, const SWObject* obj );
	void insert( const tstring& key, const SWObject* obj );

	void remove( const SWObject* key );
	void remove( const tstring& key );

	iterator find( const SWObject* key );
	iterator find( const tstring& key );
	const_iterator find( const SWObject* key ) const;
	const_iterator find( const tstring& key ) const;

	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

private:

	Value m_dic;

};

#endif // SWDictionary_h__