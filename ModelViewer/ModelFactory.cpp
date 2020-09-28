#include "pch.h"
#include "ModelFactory.h"
#include "EventShim.h"
#include "SceneManager.h"

using namespace std;

ModelFactory::ModelFactory()
{
}

void ModelFactory::CreateBuffer(GLTF_BufferData^ data)
{
	auto node = dynamic_cast<MeshNode *>(_currentNode);
	if (node)
	{
		node->CreateBuffer(data);
	}
}
void ModelFactory::CreateTexture(GLTF_TextureData^ data)
{
	auto node = dynamic_cast<MeshNode *>(_currentNode);
	if (node)
	{
		node->CreateTexture(data);
	}
}
void ModelFactory::CreateMaterial(GLTF_MaterialData^ data)
{
	auto node = dynamic_cast<MeshNode *>(_currentNode);
	if (node)
	{
		node->CreateMaterial(data);
	}
}
void ModelFactory::CreateTransform(GLTF_TransformData^ data)
{
	auto node = dynamic_cast<GraphContainerNode *>(_currentNode);
	if (node)
	{
		node->CreateTransform(data);
	}
}

GraphNode * ModelFactory::InitialiseMesh(GLTF_SceneNodeData^ data)
{
	auto mesh = new MeshNode(data->NodeIndex);
	_currentNode = mesh;

	auto devResources = SceneManager::Instance().DevResources();
	mesh->Initialise(devResources);

	return mesh;
}

void ModelFactory::CreateSceneNode(GLTF_SceneNodeData^ data)
{
	GraphNode *parent = nullptr;
	if (_root && data->ParentIndex != -1)
	{
		parent = _root->FindChildByIndex(data->ParentIndex);
	}

	if (data->IsMesh)
	{
		_currentNode = InitialiseMesh(data);
	}
	else
	{
		_currentNode = new GraphContainerNode(data->NodeIndex);
		auto devResources = SceneManager::Instance().DevResources();
		_currentNode->Initialise(devResources);
	}
	_currentNode->SetName(data->Name->Data());

	if (_root == nullptr)
	{
		_root = _currentNode;
	}
	else if (parent && _currentNode)
	{
		shared_ptr<GraphNode> sp;
		sp.reset(_currentNode);
		parent->AddChild(sp);
	}
}

future<shared_ptr<GraphNode>> ModelFactory::CreateFromFileAsync(StorageFile^ file)
{
	_parser = ref new GLTF_Parser();
	_root = _currentNode = nullptr;

	function<void(GLTF_SceneNodeData^)> snmmemfun = bind(&ModelFactory::CreateSceneNode, &(ModelFactory::Instance()), placeholders::_1);
	function<void(GLTF_BufferData^)> memfun = bind(&ModelFactory::CreateBuffer, &(ModelFactory::Instance()), placeholders::_1);
	function<void(GLTF_TextureData^)> tmemfun = bind(&ModelFactory::CreateTexture, &(ModelFactory::Instance()), placeholders::_1);
	function<void(GLTF_MaterialData^)> mmemfun = bind(&ModelFactory::CreateMaterial, &(ModelFactory::Instance()), placeholders::_1);
	function<void(GLTF_TransformData^)> tmmemfun = bind(&ModelFactory::CreateTransform, &(ModelFactory::Instance()), placeholders::_1);

	auto es = ref new EventShim(memfun, mmemfun, tmemfun, tmmemfun, snmmemfun);

	_parser->OnBufferEvent += ref new BufferEventHandler(es, &EventShim::OnBuffer);
	_parser->OnTextureEvent += ref new TextureEventHandler(es, &EventShim::OnTexture);
	_parser->OnMaterialEvent += ref new MaterialEventHandler(es, &EventShim::OnMaterial);
	_parser->OnTransformEvent += ref new TransformEventHandler(es, &EventShim::OnTransform);
	_parser->OnSceneNodeEvent += ref new SceneNodeEventHandler(es, &EventShim::OnSceneNode);

	co_await async([this, file]() { _parser->ParseFile(file); });

	_root->AfterLoad();

	// call afterLoad on all children...
	_root->ForAllChildrenRecursive([](GraphNode& node) 
	{
		node.AfterLoad();
	});

	shared_ptr<GraphNode> sp;
	sp.reset(_root);
	co_return sp;
}

future<shared_ptr<GraphNode>> ModelFactory::CreateCubeAsync()
{
	auto mesh = new MeshNode(-1);

	auto devResources = SceneManager::Instance().DevResources();
	mesh->Initialise(devResources);

	mesh->SetName(L"Cube");

	static const XMFLOAT3 vertices[] =
	{
		XMFLOAT3(-0.5f, 0.5f, -0.5f),
		XMFLOAT3(0.5f, 0.5f, -0.5f),
		XMFLOAT3(-0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f, -0.5f, -0.5f),

		XMFLOAT3(-0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
	};

	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6,
	};

	BufferDescriptor bdverts(devResources, wstring(L"POSITION"), 0, (void *)(vertices), sizeof(XMFLOAT3)*8, 0, 8);
	mesh->CreateBuffer(bdverts);
	BufferDescriptor bdindices(devResources, wstring(L"INDICES"), 0, (void*)(indices), sizeof(int)*12, 0, 12);
	mesh->CreateBuffer(bdindices);

	MaterialDescriptor md(L"Cube Material");

	mesh->CreateMaterial(md);

	mesh->AfterLoad();

	shared_ptr<GraphNode> sp;
	sp.reset(mesh);
	co_return sp;
}
