#include "pch.h"
#include "RootPageViewModel.h"
#include "DelegateCommand.h"
#include "SceneManager.h"
#include "Utility.h"
#include <experimental/resumable>
#include <pplawait.h>

using namespace ViewModels;
using namespace Windows::Storage::Pickers;

RootPageViewModel::RootPageViewModel()
{
	LoadFileCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteLoadCommand), nullptr);
	LoadDicomCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RootPageViewModel::ExecuteLoadDicomCommand), nullptr);
}

class LoadingWrapper
{
public:
	LoadingWrapper(function<void()> ctor, function<void()> dtor) :
		_dtor(dtor)
	{
		Schedule(ctor);
	}

	future<void> Schedule(function<void()> fn)
	{
		auto disp = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher;
		co_await disp->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
			ref new Windows::UI::Core::DispatchedHandler([fn]() { fn(); }));
	}

	~LoadingWrapper()
	{
		Schedule(_dtor);
	}

private:
	function<void()> _dtor;
};

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

future<shared_ptr<GraphNode>> RootPageViewModel::LoadDicomFileAsync()
{
	auto fop = ref new FileOpenPicker();
	fop->ViewMode = PickerViewMode::Thumbnail;
	fop->SuggestedStartLocation = Pickers::PickerLocationId::PicturesLibrary;
	fop->FileTypeFilter->Append(".png");
	fop->FileTypeFilter->Append(".jpg");
	fop->FileTypeFilter->Append(".jpeg");

	auto storageFiles = co_await fop->PickMultipleFilesAsync();
	if (storageFiles == nullptr)
		co_return nullptr;

	// RAII-style for ensuring that the progress gets cleared robustly
	auto loader = make_unique<LoadingWrapper>([this]() { Loading = true; }, [this]() { Loading = false; });

	auto ret = co_await ModelFactory::Instance().CreateVolumeFromImagesAsync(storageFiles);
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

future<void> RootPageViewModel::LoadDicom()
{
	Utility::Out(L"At Start of Load");
	auto node = co_await LoadDicomFileAsync();
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

void RootPageViewModel::ExecuteLoadDicomCommand(Object^ param)
{
	LoadDicom();
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
