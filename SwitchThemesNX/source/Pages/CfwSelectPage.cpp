#include "CfwSelectPage.hpp"
#include "../ViewFunctions.hpp"
#include "../UI/UIManagement.hpp"

using namespace std;

CfwSelectPage::CfwSelectPage(const vector<string>& folders) : Folders(folders) {}

CfwSelectPage::~CfwSelectPage()
{

}

static const int BtnWidth = 500;
static const int XCursorBtn = SCR_W / 2 - BtnWidth / 2;

void CfwSelectPage::Render(int X, int Y)
{
	Utils::ImGuiSetupPageFullscreen("CfwSelectPage", 10, 10);
	ImGui::SetWindowFocus();

	if (Folders.size() == 0)
	{
		ImGui::PushFont(font30);
		Utils::ImGuiCenterString("Es wurde kein CFW Ordner gefunden.");
		ImGui::PopFont();
		ImGui::NewLine();
		ImGui::TextWrapped(
			"Stelle sicher das sich der \"atmosphere\" Ordner im root der SD-Karte befindet.\n\n"
			"Moeglicherweise wurde eine der BAT-Dateien oder das updater-Skript nicht richtig durchlaufen.\n"
			"In dem Fall solltest du unser Paket erneut auf deine SD-Karte mit einer der Moeglichkeiten installieren.");
		
	}	
	else {
		Utils::ImGuiCenterString("Mehrere CFW Ordner gefunden, welchen moechtest du benutzen ?");

		ImGui::PushFont(font30);
		ImGui::SetCursorPos({ (float)XCursorBtn, ImGui::GetCursorPosY() + 30 });

		int count = 0;
		for (const auto& e : Folders)
		{
			ImGui::SetCursorPosX((float)XCursorBtn);
			if (ImGui::Button(e.c_str(), { BtnWidth, 50 }))
			{
				fs::cfw::SetFolder(e);
				PopPage(this);
			}
			count++;
		}

		ImGui::PopFont();
	}

	ImGui::NewLine();
	Utils::ImGuiCenterString("Wenn deine CFW nicht unterstuetzt wird, eroeffne ein Issue auf Github.");
	if (Utils::ImGuiCenterButton("Die Anwendungen beenden"))
		App::Quit();

	Utils::ImGuiSetWindowScrollable();
	Utils::ImGuiCloseWin();
}

void CfwSelectPage::Update()
{
	if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_START])
		App::Quit();
}


