#pragma once

#include "Common/ViewModelBase.h"
#include <future>
#include "ModelFactory.h"

namespace ViewModels
{
	using namespace Common;
	using namespace Windows::UI::Xaml::Input;
	using namespace Platform;

	[Windows::UI::Xaml::Data::Bindable]
	public ref class RootPageViewModel sealed : public ViewModelBase
	{
	public:
		RootPageViewModel();

		property bool Loading { bool get(); void set(bool val); }

		property ICommand^ LoadFileCommand;
		property ICommand^ CreateCubeCommand;
		property ICommand^ CreateSphereCommand;
		property ICommand^ CreateCylinderCommand;
		property ICommand^ CreateConeCommand;
		property ICommand^ CreateTorusCommand;

		property String^ Filename { String^ get(); void set(String^ val); }

	private:
		void ExecuteLoadCommand(Object^ param);
		void ExecuteCreateCubeCommand(Object^ param);
		void ExecuteCreateSphereCommand(Object^ param);
		void ExecuteCreateCylinderCommand(Object^ param);
		void ExecuteCreateConeCommand(Object^ param);
		void ExecuteCreateTorusCommand(Object^ param);

		std::future<std::shared_ptr<GraphNode>> RootPageViewModel::LoadFileAsync();
		std::future<void> RootPageViewModel::Load();

		std::future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateCubeAsync();
		std::future<void> RootPageViewModel::CreateCube();

		std::future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateSphereAsync();
		std::future<void> RootPageViewModel::CreateSphere();

		std::future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateCylinderAsync();
		std::future<void> RootPageViewModel::CreateCylinder();

		std::future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateConeAsync();
		std::future<void> RootPageViewModel::CreateCone();

		std::future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateTorusAsync();
		std::future<void> RootPageViewModel::CreateTorus();

		bool _loading = false;
		String^ _filename;
	};
}
