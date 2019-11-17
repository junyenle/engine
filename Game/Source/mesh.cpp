#include "stdafx.h"
#include "mesh.h"
#include "game.h"
#include "Shader.h"
#include "stringUtil.h"
#include "texture.h"
#include "vertexBuffer.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"
#include <fstream>
#include <sstream>

Mesh::Mesh(Game* pGame, const VertexBuffer* vertexBuffer, const Shader* shader)
{
	mGame = pGame;
	mVertexBuffer = vertexBuffer;
	mShader = shader;
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++)
	{
		mTextures[i] = 0;
	}
}

Mesh::~Mesh()
{
	delete mVertexBuffer;
}

bool Mesh::Load(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpmesh" ||
		ver != 2)
	{
		return false;
	}

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		return false;
	}

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		if (i < Graphics::TEXTURE_SLOT_TOTAL)
		{
			std::wstring textureName;
			StringUtil::String2WString(textureName, textures[i].GetString());
			mTextures[i] = mGame->LoadTexture(textureName);
		}
	}

	// Read the vertex format
	const rapidjson::Value& vertFormat = doc["vertexformat"];
	if (!vertFormat.IsArray() || vertFormat.Size() < 1)
	{
		return false;
	}

	uint32_t vertSize = 0;
	uint32_t vertNumValues = 0;
	std::string inputLayoutName;

	for (rapidjson::SizeType i = 0; i < vertFormat.Size(); i++)
	{
		if (!vertFormat[i].IsObject())
		{
			return false;
		}

		inputLayoutName += vertFormat[i]["name"].GetString();

		std::string vertType = vertFormat[i]["type"].GetString();
		uint32_t elementSize = 0;
		if (vertType == "float")
		{
			elementSize = 4;
		}
		else
		{
			elementSize = 1;
		}

		vertNumValues += vertFormat[i]["count"].GetUint();
		vertSize += elementSize * vertFormat[i]["count"].GetUint();
	}

	if (vertNumValues < 3)
	{
		return false;
	}

	// Get the shader type
	std::wstring shaderName;
	StringUtil::String2WString(shaderName, doc["shader"]["name"].GetString());
	mShader = mGame->GetShader(shaderName);

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		return false;
	}

	union VertPacked
	{
		float f;
		uint8_t b[4];
	};

	isSkinned = (L"Skinned" == shaderName);
	this->isSkinned = isSkinned;
	uint32_t numVerts = vertsJson.Size();
	std::vector<VertPacked> vertices;
	vertices.reserve(numVerts * vertSize);
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != vertNumValues)
		{
			return false;
		}

		// Now stuff all the vertices into the vertex buffer
		if (isSkinned)
		{   //special case for skinned
			VertPacked temp;
			// Position/Normal
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				temp.f = vert[j].GetDouble();
				vertices.emplace_back(temp);
			}

			// Uints for bones and weights
			for (rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				temp.b[0] = vert[j].GetUint();
				temp.b[1] = vert[j + 1].GetUint();
				temp.b[2] = vert[j + 2].GetUint();
				temp.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(temp);
			}

			// Last two texture coordinates
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				temp.f = vert[j].GetDouble();
				vertices.emplace_back(temp);
			}
		}
		else
		{
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				VertPacked temp;
				temp.f = vert[j].GetDouble();
				vertices.emplace_back(temp);
			}
		}
	}

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		return false;
	}

	std::vector<uint16_t> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			return false;
		}

		indices.emplace_back(static_cast<uint16_t>(ind[0].GetUint()));
		indices.emplace_back(static_cast<uint16_t>(ind[1].GetUint()));
		indices.emplace_back(static_cast<uint16_t>(ind[2].GetUint()));
	}

	// Now create a vertex array
	mVertexBuffer = new VertexBuffer(mGame->GetGraphics(),
		vertices.data(), numVerts, vertSize,
		indices.data(), (uint32_t)indices.size(), sizeof(uint16_t)
		);
	return true;
}

Mesh* Mesh::StaticLoad(const WCHAR* fileName, Game* pGame)
{
	Mesh *pMesh = new Mesh(pGame, nullptr, nullptr);
	if (false == pMesh->Load(fileName))
	{
		delete pMesh;
		return nullptr;
	}
	return pMesh;
}

void Mesh::Draw() const
{
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++)
	{
		const Texture* texture = mTextures[i];
		if (texture)
		{
			texture->SetActive(i);
		}
		else
		{
			mGame->GetGraphics()->SetActiveTexture(i, nullptr);
		}
	}
	mShader->SetActive();
	mVertexBuffer->SetActive();
	mVertexBuffer->Draw();
}

void Mesh::SetTexture(int slot, const Texture* texture)
{
	mTextures[slot] = texture;
}
