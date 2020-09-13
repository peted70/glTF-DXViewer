#pragma once
#ifdef REMOTE_RENDER

#include <Common\HolographicDeviceResources.h>

class RemoteRenderer
{
public:
	winrt::Windows::Graphics::Holographic::HolographicFrame Update(DX::StepTimer const& timer);
	void Render(winrt::Windows::Graphics::Holographic::HolographicFrame holographicFrame);
	
	future<void> ConnectAsync(const wchar_t* ipAddress);
	void Initialise();

	void RegisterForStatusUpdates(std::function<void(wchar_t*)> onStatusUpdate)
	{
		_statusUpdated = onStatusUpdate;
	}

private:
	// Asynchronously creates resources for new holographic cameras.
	void OnCameraAdded(
		const winrt::Windows::Graphics::Holographic::HolographicSpace& sender,
		const winrt::Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs& args);

	// Synchronously releases resources for holographic cameras that are no longer
	// attached to the system.
	void OnCameraRemoved(
		const winrt::Windows::Graphics::Holographic::HolographicSpace& sender,
		const winrt::Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs& args);

	winrt::Windows::Graphics::Holographic::HolographicSpace _holographicSpace = nullptr;
	std::shared_ptr<DXHelper::HolographicDeviceResources> _deviceResources = nullptr;

	winrt::Windows::Perception::Spatial::SpatialLocator _spatialLocator = nullptr;

	// A reference frame that is positioned in the world.
	winrt::Windows::Perception::Spatial::SpatialStationaryFrameOfReference _referenceFrame = nullptr;

	// Event registration tokens.
	winrt::event_token _cameraAddedToken;
	winrt::event_token _cameraRemovedToken;

	std::function<void(wchar_t*)> _statusUpdated = {};
	std::unique_ptr<SceneContext> _context;
};

#endif