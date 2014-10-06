#ifndef TFlag_h__
#define TFlag_h__

#include "SWPrimaryType.h"

template<typename T>
class TFlag : public SWMemory
{
public:
	TFlag():m_flags(0){};
	inline void	set(tuint index,bool check);
	inline bool get(tuint index);
	inline void	clear( bool check = false );
	inline bool	operator ==( const TFlag<T> flag ) { return flag.m_flags == m_flags; };
	inline bool	operator !=( const TFlag<T> flag ) { return flag.m_flags != m_flags; };
private:
	T m_flags;
};

typedef	TFlag<tuint8>	TFlag8;
typedef	TFlag<tuint16>	TFlag16;
typedef	TFlag<tuint32>	TFlag32;

template<typename T>
inline void	TFlag<T>::set(tuint index,bool check)
{
	if (check)
	{
		m_flags |= 1<<index;
	}else{
		m_flags &= ~(1<<index);
	}
}

template<typename T>
inline bool	TFlag<T>::get(tuint index)
{
	return (( m_flags & ( 1 << index ) ) != 0);
}

template<typename T>
inline void	TFlag<T>::clear( bool check )
{
	m_flags = check? -1 : 0;
}

#endif // TFlag_h__