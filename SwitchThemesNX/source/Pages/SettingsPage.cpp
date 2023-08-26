#include "SettingsPage.hpp"
#include "../ViewFunctions.hpp"
#include "../Platform/Platform.hpp"

using namespace std;

namespace Settings {
	bool UseIcons = true;
	bool UseCommon = true;
};

SettingsPage::SettingsPage() 
{
	Name = "Einstellungen";
}

void SettingsPage::Render(int X, int Y)
{
	Utils::ImGuiSetupWin(Name.c_str(), X, Y, DefaultWinFlags);
	ImGui::SetWindowSize(ImVec2(SCR_W - (float)X - 30, SCR_H - (float)Y - 70));
	ImGui::PushFont(font25);

	ImGui::PushFont(font30);
	ImGui::TextUnformatted("NXTheme Einstellungen");
	ImGui::PopFont();
	ImGui::TextWrapped("Die Einstellungen gelten nur für die Installation von NXThemes und werden nicht gespeichert. Du musst sie jedes Mal zuruecksetzen, wenn du die App startest.");
	ImGui::Checkbox("Benutzerdefinierte Symbole aktivieren", &Settings::UseIcons);
	PAGE_RESET_FOCUS;
	ImGui::Checkbox("Zusaetzliche Layouts aktivieren (z. B. common.szs)", &Settings::UseCommon);
	ImGui::NewLine();

	ImGui::PopFont();
	Utils::ImGuiCloseWin();
}

void SettingsPage::Update()
{	
	if (Utils::PageLeaveFocusInput(true))
	{
		Parent->PageLeaveFocus(this);
		return;
	}
}









