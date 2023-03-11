#include "pch.h"
#include "OBJParser.h"
#include <fstream>

Util::OBJParser::OBJParser(const std::string& filenameWithoutExtension)
	:m_Filename{ filenameWithoutExtension }
{
}

void Util::OBJParser::ReadTextOBJFile()
{
	std::ifstream input{};
	std::string filename{ m_Filename + ".obj" };

	if (input.open(filename.c_str()); input.is_open())
	{
		L_DEBUG("Reading Text OBJ file and saving contents . . .");

		std::string line{};
		while (std::getline(input, line))
		{
			// if found vertex line

			if (line.find("#") != std::string::npos)
			{
				ParseCommentsLine(line);
			}
			else if (auto vFound = line.find("v"); vFound != std::string::npos)
			{
				ParseVerticesLine(line, vFound);
			}
			else if (auto fFound = line.find("f"); fFound != std::string::npos)
			{
				ParseFacesLine(line, fFound);
			}
			else if (auto nFound = line.find("vn"); nFound != std::string::npos)
			{
				ParseNormalsLine(line, nFound);
			}
		}

		input.close();
	}
}

void Util::OBJParser::ReadBinaryOBJFile()
{
	std::ifstream input{};
	std::string filename{ m_Filename + ".bobj" };

	if (input.open(filename.c_str(), std::ios::binary); input.is_open())
	{
		L_DEBUG("Reading Binary OBJ file and saving contents . . .");

		// Read in vertexCount
		int vertexCount{};
		input.read(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));
		m_OBJData.vertexCount = vertexCount;
		L_ERROR("read from BOBJ. Vertex count: {}", vertexCount);

		// Read in vertices
		for (int i{}; i < vertexCount; ++i)
		{
			Vertex vertex{};
			input.read(reinterpret_cast<char*>(&vertex), sizeof(vertex));
			m_OBJData.vertices.emplace_back(vertex);
		}

		// Read in normalCount
		int normalCount{};
		input.read(reinterpret_cast<char*>(&normalCount), sizeof(normalCount));
		m_OBJData.normalCount = normalCount;
		L_ERROR("read from BOBJ. Normal count: {}", normalCount);

		// Read in normals
		for (int i{}; i < normalCount; ++i)
		{
			Normal normal{};
			input.read(reinterpret_cast<char*>(&normal), sizeof(normal));
			m_OBJData.normals.emplace_back(normal);
		}

		// Read in faceCount
		int faceCount{};
		input.read(reinterpret_cast<char*>(&faceCount), sizeof(faceCount));
		m_OBJData.faceCount = faceCount;
		L_ERROR("read from BOBJ. Face count: {}", faceCount);

		// Read in faces
		for (int i{}; i < faceCount; ++i)
		{
			Face face{};
			input.read(reinterpret_cast<char*>(&face), sizeof(face));
			m_OBJData.faces.emplace_back(face);
		}
	}
}

void Util::OBJParser::WriteToBinary()
{
	L_DEBUG("Writing OBJ data to binary file . . .");

	std::ofstream output{};
	std::string filename{ m_Filename + ".bobj" };

	if (output.open(filename.c_str(), std::ios::binary); output.is_open())
	{
		// Write vertices
		output.write(reinterpret_cast<const char*>(&m_OBJData.vertexCount), sizeof(m_OBJData.vertexCount));
		for (const auto val : m_OBJData.vertices)
		{
			output.write(reinterpret_cast<const char*>(&val), sizeof(val));
		}
		L_ERROR("written into BOBJ. Vertex count: {}", m_OBJData.vertexCount);


		// Write normals
		output.write(reinterpret_cast<const char*>(&m_OBJData.normalCount), sizeof(m_OBJData.normalCount));
		for (const auto val : m_OBJData.normals)
		{
			output.write(reinterpret_cast<const char*>(&val), sizeof(val));
		}
		L_ERROR("written into BOBJ. Normal count: {}", m_OBJData.normalCount);

		// Write faces
		output.write(reinterpret_cast<const char*>(&m_OBJData.faceCount), sizeof(m_OBJData.faceCount));
		for (const auto val : m_OBJData.faces)
		{
			output.write(reinterpret_cast<const char*>(&val), sizeof(val));
		}
		L_ERROR("written into BOBJ. Face count: {}", m_OBJData.faceCount);

		output.close();
	}

	L_DEBUG("Writing binary OBJ file done. Output: {}", filename);

	// Reset obj data
	m_OBJData = {};
}

void Util::OBJParser::WriteToText()
{
	 
}

void Util::OBJParser::ParseVerticesLine(std::string& line, size_t)
{
	// Split line into array of strings
	std::vector<std::string> output{};
	SplitString(output, line, " ");

	// Transform array of strings into array of floats
	std::vector<float> destination(output.size() - 1);
	std::transform(output.begin() + 1, output.end(), destination.begin(), [](const std::string& val) {
		return std::stof(val);
	});

	// Save vertex
	Vertex vertex{ destination[0], destination[1], destination[2] };
	m_OBJData.vertices.emplace_back(vertex);

	// Increase vertex count
	m_OBJData.vertexCount++;
}

void Util::OBJParser::ParseNormalsLine(std::string& line, size_t)
{
	std::vector<std::string> output{};
	SplitString(output, line, " ");

	// Transform array of strings into array of floats
	std::vector<float> destination(output.size() - 1);
	std::transform(output.begin() + 1, output.end(), destination.begin(), [](const std::string& val) {
		return std::stof(val);
	});

	// Save normal
	Normal normal{ destination[0], destination[1], destination[2] };
	m_OBJData.normals.emplace_back(normal);

	m_OBJData.normalCount++;
}

void Util::OBJParser::ParseFacesLine(std::string& line, size_t)
{
	std::vector<std::string> output{};
	SplitString(output, line, " ");

	std::vector<int> destination(output.size() - 1);
	std::transform(output.begin() + 1, output.end(), destination.begin(), [](const std::string& val) {
		return std::stoi(val);
	});

	Face face{ destination[0], destination[1], destination[2] };
	m_OBJData.faces.emplace_back(face);

	m_OBJData.faceCount++;
}

void Util::OBJParser::ParseCommentsLine(const std::string& line)
{
	L_WARN("Comments found {}", line);
}

void Util::OBJParser::SplitString(std::vector<std::string>& output, std::string& input, const std::string& delimiter)
{
	size_t pos{};
	std::string token{};
	while ((pos = input.find(delimiter)) != std::string::npos)
	{
		token = input.substr(0, pos);
		input.erase(0, pos + delimiter.length());

		output.emplace_back(token);
	}

	if (not token.empty())
	{
		output.emplace_back(input);
	}
}
