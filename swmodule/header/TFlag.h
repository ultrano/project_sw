#ifndef TFlag_h__
#define TFlag_h__

#include "SWPrimaryType.h"

template<typename T>
class TFlag : public SWMemory
{
public:
	
	T flags;

	TFlag():flags(0){};
	inline void	set(tuint index,bool check);
	inline bool get(tuint index) const;
	inline void	clear( bool check = false );
	inline bool	operator ==( const TFlag<T> flag ) { return flag.flags == flags; };
	inline bool	operator !=( const TFlag<T> flag ) { return flag.flags != flags; };
};

typedef	TFlag<tuint8>	TFlag8;
typedef	TFlag<tuint16>	TFlag16;
typedef	TFlag<tuint32>	TFlag32;
typedef	TFlag<tuint64>	TFlag64;

template<typename T>
inline void	TFlag<T>::set(tuint index,bool check)
{
	if (check)
	{
		flags |= 1<<index;
	}else{
		flags &= ~(1<<index);
	}
}

template<typename T>
inline bool	TFlag<T>::get(tuint index)  const
{
	return (( flags & ( 1 << index ) ) != 0);
}

template<typename T>
inline void	TFlag<T>::clear( bool check )
{
	flags = check? -1 : 0;
}

#endif // TFlag_h__