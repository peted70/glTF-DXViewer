#include "pch.h"
#include "RootPageViewModel.h"
#include "DelegateCommand.h"
#include "SceneManager.h"
#include "Utility.h"
#include <experimental/resumable>
#include <pplawait.h>
#include "LoadingWrapper.h"

using namespace ViewModels;
using namespace Windows::Storage::Pickers;
using namespace std;

RootPageViewModel::RootPageViewModel()
{
	LoadFileCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteLoadCommand), nullptr);
	CreateCubeCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteCreateCubeCommand), nullptr);
	CreateSphereCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteCreateSphereCommand), nullptr);
	CreateCylinderCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteCreateCylinderCommand), nullptr);
	CreateConeCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteCreateConeCommand), nullptr);
	CreateTorusCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteCreateTorusCommand), nullptr);
}

future<shared_ptr<GraphNode>> RootPageViewModel::LoadFileAsync()
{
	auto fop = ref new FileOpenPicker();
	fop->FileTypeFilter->Append(".glb");
	fop->FileTypeFilter->Append(".gltf");

	auto storageFile = co_await fop->PickSingleFileAsync();
	if (storageFile == nullptr)
		co_return nullptr;

	// RAII-style for ensuring that the progress gets cleared robustly
	auto loader = make_unique<LoadingWrapper>([this]() { Loading = true; }, [this]() { Loading = false; });

	Utility::Out(L"filename = %s", storageFile->Path->Data());
	Filename = storageFile->Path;

	auto ret = co_await ModelFactory::Instance().CreateFromFileAsync(storageFile);
	co_return ret;
}

future<shared_ptr<GraphNode>> RootPageViewModel::CreateCubeAsync()
{
	// RAII-style for ensuring that the progress gets cleared robustly
	auto loader = make_unique<LoadingWrapper>([this]() { Loading = true; }, [this]() { Loading = false; });

	auto sf = co_await Windows::ApplicationModel::Package::Current->InstalledLocation->GetFolderAsync("Assets\\3D Primitives");
	auto file = co_await sf->GetFileAsync("cube.glb");

	// open th efile in assets 'cube.glb'
	auto ret = co_await ModelFactory::Instance().CreateFromFileAsync(file);
	co_return ret;
}

future<void> RootPageViewModel::Load()
{
	Utility::Out(L"At Start of Load");
	auto node = co_await LoadFileAsync();
	if (node == nullptr)
		co_return;

	Utility::Out(L"Loaded");

	// Add the GraphNode to the scene	
	auto current = SceneManager::Instance().Current();
	SceneManager::Instance().AddNode(node);
}

void RootPageViewModel::ExecuteLoadCommand(Object^ param)
{
	Load();
}

future<void> RootPageViewModel::CreateCube()
{
	Utility::Out(L"At Start of CreateCube");
	auto node = co_await CreateCubeAsync();
	if (node == nullptr)
		co_return;

	Utility::Out(L"Cube Created");

	// Add the GraphNode to the scene	
	auto current = SceneManager::Instance().Current();
	SceneManager::Instance().AddNode(node);
}

void RootPageViewModel::ExecuteCreateCubeCommand(Object^ param)
{
	CreateCube();
}

void RootPageViewModel::ExecuteCreateSphereCommand(Object^ param)
{
	CreateSphere();
}

void RootPageViewModel::ExecuteCreateCylinderCommand(Object^ param)
{
	CreateCylinder();
}

void RootPageViewModel::ExecuteCreateConeCommand(Object^ param)
{
	CreateCone();
}

void RootPageViewModel::ExecuteCreateTorusCommand(Object^ param)
{
	CreateTorus();
}

future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateSphereAsync()
{
	// RAII-style for ensuring that the progress gets cleared robustly
	auto loader = make_unique<LoadingWrapper>([this]() { Loading = true; }, [this]() { Loading = false; });

	auto sf = co_await Windows::ApplicationModel::Package::Current->InstalledLocation->GetFolderAsync("Assets\\3D Primitives");
	auto file = co_await sf->GetFileAsync("sphere.glb");

	// open th efile in assets 'cube.glb'
	auto ret = co_await ModelFactory::Instance().CreateFromFileAsync(file);
	co_return ret;
}

future<void> RootPageViewModel::CreateSphere()
{
	Utility::Out(L"At Start of CreateSphere");
	auto node = co_await CreateSphereAsync();
	if (node == nullptr)
		co_return;

	Utility::Out(L"Sphere Created");

	// Add the GraphNode to the scene	
	auto current = SceneManager::Instance().Current();
	SceneManager::Instance().AddNode(node);
}

future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateCylinderAsync()
{
	// RAII-style for ensuring that the progress gets cleared robustly
	auto loader = make_unique<LoadingWrapper>([this]() { Loading = true; }, [this]() { Loading = false; });

	auto sf = co_await Windows::ApplicationModel::Package::Current->InstalledLocation->GetFolderAsync("Assets\\3D Primitives");
	auto file = co_await sf->GetFileAsync("cylinder.glb");

	// open th efile in assets 'cube.glb'
	auto ret = co_await ModelFactory::Instance().CreateFromFileAsync(file);
	co_return ret;
}

future<void> RootPageViewModel::CreateCylinder()
{
	Utility::Out(L"At Start of CreateCylinder");
	auto node = co_await CreateCylinderAsync();
	if (node == nullptr)
		co_return;

	Utility::Out(L"Cylinder Created");

	// Add the GraphNode to the scene	
	auto current = SceneManager::Instance().Current();
	SceneManager::Instance().AddNode(node);
}

future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateConeAsync()
{
	// RAII-style for ensuring that the progress gets cleared robustly
	auto loader = make_unique<LoadingWrapper>([this]() { Loading = true; }, [this]() { Loading = false; });

	auto sf = co_await Windows::ApplicationModel::Package::Current->InstalledLocation->GetFolderAsync("Assets\\3D Primitives");
	auto file = co_await sf->GetFileAsync("cone.glb");

	// open th efile in assets 'cube.glb'
	auto ret = co_await ModelFactory::Instance().CreateFromFileAsync(file);
	co_return ret;
}

future<void> RootPageViewModel::CreateCone()
{
	Utility::Out(L"At Start of CreateCone");
	auto node = co_await CreateConeAsync();
	if (node == nullptr)
		co_return;

	Utility::Out(L"Cone Created");

	// Add the GraphNode to the scene	
	auto current = SceneManager::Instance().Current();
	SceneManager::Instance().AddNode(node);
}

future<std::shared_ptr<GraphNode>> RootPageViewModel::CreateTorusAsync()
{
	// RAII-style for ensuring that the progress gets cleared robustly
	auto loader = make_unique<LoadingWrapper>([this]() { Loading = true; }, [this]() { Loading = false; });

	auto sf = co_await Windows::ApplicationModel::Package::Current->InstalledLocation->GetFolderAsync("Assets\\3D Primitives");
	auto file = co_await sf->GetFileAsync("torus.glb");

	// open th efile in assets 'cube.glb'
	auto ret = co_await ModelFactory::Instance().CreateFromFileAsync(file);
	co_return ret;
}

future<void> RootPageViewModel::CreateTorus()
{
	Utility::Out(L"At Start of CreateTorus");
	auto node = co_await CreateTorusAsync();
	if (node == nullptr)
		co_return;

	Utility::Out(L"Torus Created");

	// Add the GraphNode to the scene	
	auto current = SceneManager::Instance().Current();
	SceneManager::Instance().AddNode(node);
}

bool RootPageViewModel::Loading::get()
{
	return _loading;
}

void RootPageViewModel::Loading::set(bool val)
{
	if (_loading == val)
		return;
	_loading = val;
	OnPropertyChanged(getCallerName(__FUNCTION__));
}

String^ RootPageViewModel::Filename::get()
{
	return _filename;
}

void RootPageViewModel::Filename::set(String^ val)
{
	if (_filename == val)
		return;
	_filename = val;
	OnPropertyChanged(getCallerName(__FUNCTION__));
}
