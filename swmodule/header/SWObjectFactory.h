//
//  SWObjectFactory.h
//
//  Created by SangWoon-Han on 14. 9. 14..
//  Copyright (c) 2014. SangWoon-Han . All rights reserved.
//

#ifndef SWObjectFactory_h__
#define SWObjectFactory_h__

#include "SWRefCounter.h"
class SWObject;

class SWAbstractObjectFactory : public SWRefCountable
{
public: virtual SWHardRef<SWObject> newInstance() = 0;
};

//! it is just a symbol to be a constructor for the factory;
static struct factory_constructor{} factory_arg;

template<typename T>
class SWTemplateObjectFactory : public SWAbstractObjectFactory
{
public: SWHardRef<SWObject> newInstance() { return new T( factory_arg ); }
};

#endif //! SWObjectFactory_h__