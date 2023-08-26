#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "MyTypes.h"
#include <unordered_map>
#include "SarcLib/Sarc.hpp"

struct ThemeFileManifest
{
	int Version;
	std::string Author;
	std::string ThemeName;
	std::string LayoutInfo;
	std::string Target;
};

struct SystemVersion { 
	u32 major, minor, micro;

	constexpr auto operator<=>(const SystemVersion& other) const
	{
		auto m = major <=> other.major;
		if (m == std::strong_ordering::equal)
			m = minor <=> other.minor;
		if (m == std::strong_ordering::equal)
			m = micro <=> other.micro;
		return m;
	}
};

extern SystemVersion HOSVer;

extern std::unordered_map<std::string,std::string> ThemeTargetToName;
extern std::unordered_map<std::string,std::string> ThemeTargetToFileName;

const std::unordered_map<std::string,std::string> ThemeTargetToName6X
{
	{"home","Homemenu"},
	{"lock","Sperrbildschirm"},
	{"user","Benutzer Seite"},
	{"apps","Alle Software Menue"},
	{"set","Einstellungen Seite"},
	{"news","Nachrichten Applet" },
	{"psl","Profilauswahl" },
};

const std::unordered_map<std::string,std::string> ThemeTargetToFileName6X
{
	{"home","ResidentMenu.szs"},
	{"lock","Entrance.szs"},
	{"user","MyPage.szs"},
	{"apps","Flaunch.szs"},
	{"set","Set.szs"},
	{"news","Notification.szs"},
	{"psl","Psl.szs" },
};

const std::unordered_map<std::string,std::string> ThemeTargetToName5X
{
	{"home","Homemenu"},
	{"lock","Sperrbildschirm"},
	{"user","Benutzer Seite"},
	{"apps","Alle Software"},
	{"set","Alle Einstellungen"},
	{"news","Alle Nachrichten"},
	{"psl","Profilauswahl" },
};

ThemeFileManifest ParseNXThemeFile(SARC::SarcData &SData);