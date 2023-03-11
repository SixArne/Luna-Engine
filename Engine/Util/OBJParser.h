#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <functional>

namespace Util
{
	struct Vertex
	{
		float x{};
		float y{};
		float z{};
	};

	struct Normal
	{
		float x{};
		float y{};
		float z{};
	};
	
	struct Face
	{
		int v1{};
		int v2{};
		int v3{};
	};

	struct OBJData
	{
		int vertexCount{};
		std::vector<Vertex> vertices{};

		int normalCount{};
		std::vector<Normal> normals{};

		int faceCount{};
		std::vector<Face> faces{};
	};

	class OBJParser
	{
	public:
		OBJParser(const std::string& filenameWithoutExtension);

		void ReadTextOBJFile();
		void ReadBinaryOBJFile();
		void WriteToBinary();
		void WriteToText();

	private:
		void ParseVerticesLine(std::string& line, size_t charPositionToPrune);
		void ParseNormalsLine(std::string& line, size_t charPositionToPrune);
		void ParseFacesLine(std::string& line, size_t charPositionToPrune);
		void ParseCommentsLine(const std::string& line);

		void SplitString(std::vector<std::string>& output, std::string& input, const std::string& delimiter);

		template<typename T>
		std::vector<T> ConvertStringVector(const std::vector<std::string>& stringVector, std::function<T(const std::string&)> converter, size_t vectorOffset = 0);

		const std::string m_Filename{};
		OBJData m_OBJData{};
	};
}

template<typename T>
std::vector<T> Util::OBJParser::ConvertStringVector(const std::vector<std::string>& stringVector, std::function<T(const std::string&)> converter, size_t vectorOffset)
{
	std::vector<T> output{};
	std::for_each(stringVector.begin() + vectorOffset, stringVector.end(), [&output, converter](const std::string& val) {
		T valueToAdd = converter(val);
		output.emplace_back(valueToAdd);
	});
	
	return output;
}

#endif
