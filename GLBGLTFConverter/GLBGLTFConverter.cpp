// GLBGLTFConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../SharedGLTFParser/gltfparser.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include <locale>
#include <codecvt>

using namespace std;
using namespace Microsoft::glTF;
using namespace std::filesystem;

class GLTFStreamReader : public IStreamReader
{
public:
	GLTFStreamReader(shared_ptr<istream> wrapped, const string& baseUri) :
		m_stream(wrapped)
	{
		path pth(baseUri);
		auto buri = pth.remove_filename().c_str();
		auto base = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(buri);
		auto sep = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(path::preferred_separator);
		_baseUri = base + sep;
	}

	shared_ptr<istream> GetInputStream(const string& uri) const override
	{
		auto path = _baseUri + uri;
		auto file = make_shared<ifstream>(path);
		if (file->fail())
		{
			throw std::exception("Unable to open file");
		}
		return file;
	}

private:
	shared_ptr<istream> m_stream;
	string _baseUri;
};

int main(int argc, char **argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string fileStr;
	if (argc > 1)
	{
		fileStr = argv[1];
	}
	else
	{
		std::cout << "Error: you must supply an input file";
		return -1;
	}

	//std::ifstream infile(fileStr, std::ios::binary);
	auto infile = make_shared<ifstream>(fileStr, ios::binary);

	if (infile->fail())
	{
		std::cout << "Error: can't read the file";
		return false;
	}

	auto gltfStreamReader = make_shared<GLTFStreamReader>(infile, fileStr);
	ParseFile(infile, "", *(gltfStreamReader.get()), [](const BufferData& data)
	{

	},
	[](const MaterialData& data)
	{

	},
	[](const TextureData& data)
	{

	},
	[](const TransformData& data)
	{

	},
	[](const SceneNodeData& data)
	{

	});

    return 0;
}

