module;

#include <string>
#include <filesystem>
#include <cstdlib>


export module one_drive;

export std::string one_drive_path(std::string end_path)
{
	std::filesystem::path one_drive = std::getenv("ONEDRIVE");
	std::string name = one_drive.concat(end_path).string();
	return name;
}

