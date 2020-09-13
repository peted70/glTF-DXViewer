#pragma once

#include <iostream>
#include "RemoteRenderer.h"

class Container
{
public:
	static Container& Instance()
	{
		static Container instance;
		return instance;
	}

	template<class T>
	T Create()
	{
		return injector.create<T>();
	}

	std::shared_ptr<DirectXPageViewModelData> ResolveDirectXPageViewModelData()
	{
		if (_dxPageVMData == nullptr)
			_dxPageVMData = make_shared<DirectXPageViewModelData>();
		return _dxPageVMData;
	}

	shared_ptr<RemoteRenderer> ResolveRemoteRenderer()
	{
		if (_remoteRenderer == nullptr)
			_remoteRenderer = make_shared<RemoteRenderer>();
		return _remoteRenderer;
	}

private:
	Container() 
	{
	}
	std::shared_ptr<DirectXPageViewModelData> _dxPageVMData;
};


