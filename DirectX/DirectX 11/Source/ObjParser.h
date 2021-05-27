#pragma once

#include <filesystem>
#include <optional>

struct ObjParser final
{
	static bool Parse(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath);
};
