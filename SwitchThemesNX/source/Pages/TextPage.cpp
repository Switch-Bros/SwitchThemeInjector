#include "TextPage.hpp"
#include "../Version.hpp"
#include "../ViewFunctions.hpp"
#include "../UI/imgui/imgui_internal.h"

using namespace std;

TextPage::TextPage(const std::string& title, const std::string& text) :
	Text(text)
{
	Name = title;
	c_str = Text.c_str();
}

TextPage::TextPage(const char* title, const char* text) 
{
	Name = title;
	c_str = text;
}

void TextPage::Render(int X, int Y)
{
	Utils::ImGuiSetupPage(this, X, Y);
	ImGui::PushFont(font25);
	ImGui::TextWrapped(c_str);
	ImGui::PopFont();
	Utils::ImGuiSetWindowScrollable();
	Utils::ImGuiCloseWin();
}

void TextPage::Update()
{
	Parent->PageLeaveFocus(this);
}

CreditsPage::CreditsPage() :
	creditsText("NXThemes Installer von exelix - " + Version::Name + " - Hauptversion" + SwitchThemesCommon::CoreVer +
		'\n' + Version::Commit +
		"\nSource: github.com/exelix11/SwitchThemeInjector"+
		"\nSpenden: ko-fi.com/exelix11\n\n")
{
	Name = "Danksagung";
}

extern void ShowFirstTimeHelp(bool WelcomeScr); //from main.cpp
void CreditsPage::Render(int X, int Y)
{
	Utils::ImGuiSetupPage(this, X, Y);
	ImGui::SetCursorPosY(20);
	ImGui::PushFont(font30);
	ImGui::TextWrapped(creditsText.c_str());
	ImGui::PopFont();

	ImGui::PushFont(font25);
	ImGui::TextWrapped(
		"Dank an:\n"
		"Syroot fuer die BinaryData Bibliothek\n"
		"AboodXD fuer die Bntx Editor und sarc Bibliothek\n"
		"shchmue fuer Lockpick\n"
		"SciresM fuer hactool\n"
		"Jeden von atmosphere und libnx\n"
		"switch-stuff auf github fuer den Font Konverter\n"
		"Fincs fuer die hybrid_app Vorlage\n"
		"Jeden von DearImgui github Repo"
	);

	if (ImGui::Button("Erste Startinformation anzeigen"))
		PushFunction([]() {ShowFirstTimeHelp(false); });
	PAGE_RESET_FOCUS;

	ImGui::PopFont();
	Utils::ImGuiSetWindowScrollable();
	Utils::ImGuiCloseWin();
}

void CreditsPage::Update()
{
	if (Utils::PageLeaveFocusInput())
		Parent->PageLeaveFocus(this);
}

