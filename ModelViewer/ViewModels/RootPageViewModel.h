#pragma once

#include "Common/ViewModelBase.h"
#include <future>
#include "ModelFactory.h"

namespace ViewModels
{
	using namespace Common;
	using namespace std;
	using namespace Windows::UI::Xaml::Input;
	using namespace Platform;

	[Windows::UI::Xaml::Data::Bindable]
	public ref class RootPageViewModel sealed : public ViewModelBase
	{
	public:
		RootPageViewModel();

		property bool Loading { bool get(); void set(bool val); }
		property ICommand^ LoadFileCommand;
		property ICommand^ LoadDicomCommand;
		property String^ Filename { String^ get(); void set(String^ val); }

	private:
		void ExecuteLoadCommand(Object^ param);
		void ExecuteLoadDicomCommand(Object^ param);
		
		future<shared_ptr<GraphNode>> RootPageViewModel::LoadFileAsync();
		future<shared_ptr<GraphNode>> RootPageViewModel::LoadDicomFileAsync();

		future<void> RootPageViewModel::Load();
		future<void> RootPageViewModel::LoadDicom();

		bool _loading = false;
		String^ _filename;
	};
}
