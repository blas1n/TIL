#include "ObjParser.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
	struct Vertex final
	{
		float x, y, z;
	};

	struct Face final
	{
		int v0, v1, v2;
		int t0, t1, t2;
		int n0, n1, n2;
	};

	struct Data final
	{
		std::vector<Vertex> vertices;
		std::vector<Vertex> texes;
		std::vector<Vertex> normals;
		std::vector<Face> faces;
	};

	bool ImportModel(const std::filesystem::path& path, Data& data)
	{
		std::fstream fin{ path };
		if (fin.fail()) return false;

		unsigned int vertexCount = 0, normalCount = 0, textureCount = 0, faceCount = 0;
		std::string input{};

		while (!fin.eof())
		{
			std::getline(fin, input);

			if (input[0] == 'v')
			{
				if (input[1] == ' ') ++vertexCount;
				else if (input[1] == 'n') ++normalCount;
				else if (input[1] == 't') ++textureCount;
			}
			else if (input[0] == 'f')
			{
				if (input[1] == ' ') ++faceCount;
			}
		}

		data.vertices.resize(vertexCount);
		data.normals.resize(normalCount);
		data.texes.resize(textureCount);
		data.faces.resize(faceCount);

		unsigned int vertexIndex = 0, texIndex = 0, normalIndex = 0, faceIndex = 0;

		fin.seekg(std::ios_base::beg);

		while (!fin.eof())
		{
			std::getline(fin, input);

			if (input[0] == 'v')
			{
				std::istringstream sin{ input.substr(2) };

				if (input[1] == ' ')
				{
					sin >> data.vertices[vertexIndex].x >> data.vertices[vertexIndex].y >> data.vertices[vertexIndex].z;
					data.vertices[vertexIndex].z = -data.vertices[vertexIndex].z;
					++vertexIndex;
				}
				else if (input[1] == 'n')
				{
					sin >> data.normals[normalIndex].x >> data.normals[normalIndex].y >> data.normals[normalIndex].z;
					data.normals[normalIndex].z = -data.normals[normalIndex].z;
					++normalIndex;
				}
				else if (input[1] == 't')
				{
					sin >> data.texes[texIndex].x >> data.texes[texIndex].y;
					data.texes[texIndex].y = 1.0f - data.texes[texIndex].y;
					++texIndex;
				}
			}
			else if (input[0] == 'f')
			{
				if (input[1] == ' ')
				{
					std::istringstream sin{ input.substr(2) };
					char temp = 0;

					sin >> data.faces[faceIndex].v2 >> temp >> data.faces[faceIndex].t2 >> temp >> data.faces[faceIndex].n2
						>> data.faces[faceIndex].v1 >> temp >> data.faces[faceIndex].t1 >> temp >> data.faces[faceIndex].n1
						>> data.faces[faceIndex].v0 >> temp >> data.faces[faceIndex].t0 >> temp >> data.faces[faceIndex].n0;

					++faceIndex;
				}
			}
		}

		fin.close();
		return true;
	}

	bool ExportModel(const std::filesystem::path& path, const Data& data)
	{
		constexpr static auto WriteData = [](std::ofstream& fout, const Data& data,
			unsigned int vIndex, unsigned int nIndex, unsigned int tIndex)
		{
			fout << data.vertices[vIndex].x << ' ' << data.vertices[vIndex].y << ' ' << data.vertices[vIndex].z << ' '
				<< data.texes[tIndex].x << ' ' << data.texes[tIndex].y << ' '
				<< data.normals[nIndex].x << ' ' << data.normals[nIndex].y << ' ' << data.normals[nIndex].z << std::endl;
		};

		std::ofstream fout{ path };
		if (fout.fail()) return false;

		const int faceCount = data.faces.size();

		fout << "Vertex Count: " << (faceCount * 3) << std::endl << std::endl;
		fout << "Data:" << std::endl << std::endl;

		for (int i = 0; i < faceCount; ++i)
		{
			unsigned int vIndex = data.faces[i].v0 - 1;
			unsigned int nIndex = data.faces[i].n0 - 1;
			unsigned int tIndex = data.faces[i].t0 - 1;
			WriteData(fout, data, vIndex, nIndex, tIndex);

			vIndex = data.faces[i].v1 - 1;
			nIndex = data.faces[i].n1 - 1;
			tIndex = data.faces[i].t1 - 1;
			WriteData(fout, data, vIndex, nIndex, tIndex);

			vIndex = data.faces[i].v2 - 1;
			nIndex = data.faces[i].n2 - 1;
			tIndex = data.faces[i].t2 - 1;
			WriteData(fout, data, vIndex, nIndex, tIndex);
		}

		fout.close();
		return true;
	}
}

bool ObjParser::Parse(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath)
{
	Data data{};
	if (!ImportModel(inputPath, data)) return false;
	if (!ExportModel(outputPath, data)) return false;
	return true;
}
