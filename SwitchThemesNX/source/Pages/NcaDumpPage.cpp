#include "NcaDumpPage.hpp"
#include "../ViewFunctions.hpp"
#include "../fs.hpp"
#include "../SwitchTools/hactool.hpp"
#include <filesystem>
#include "../Platform/Platform.hpp"
#include "../SwitchThemesCommon/NXTheme.hpp"

using namespace std;

NcaDumpPage::NcaDumpPage()
{
	Name = "Entpacke Homemenu";
}

void NcaDumpPage::Render(int X, int Y)
{
	Utils::ImGuiSetupPage(this, X, Y);
	ImGui::PushFont(font30);

	ImGui::TextWrapped("Um .nxtheme-Dateien zu installieren, musst du zuerst das Homemenu entpacken.\n"
		"Dies ist jedes Mal erforderlich, wenn die Firmware geaendert wird, sowohl bei Upgrades als auch bei Downgrades.\n"
		"Wenn die extrahierte Version nicht mit deiner Firmware uebereinstimmt, wirst du aufgefordert, dies zu tun.\n\n"
		"In der Regel musst du es nicht manuell entpacken, aber falls du auf Probleme stoeßt, kannst du es hier versuchen.");

	if (ImGui::Button("Entpacke Homemenu"))
	{
		PushFunction([]() {
			if ((gamepad.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] && gamepad.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]))
			{
				DialogBlocking("Supergeheime Kombination eingegeben, nur die Homemenu-NCA wird gesichert (sie wird nicht extrahiert).");
				DisplayLoading("Entpacke NCA...");
				if (fs::theme::DumpHomeMenuNca())
					Dialog("Das Homemenu-NCA wurde extrahiert. Verwende jetzt den Injektor, um die Einrichtung abzuschließen.\nWenn du dies nicht absichtlich getan hast, ignoriere diese Nachricht.");
				return;
			}
			if (!YesNoPage::Ask(
				"Um benutzerdefinierte Themes zu installieren, musst du zuerst das Homemenu entpacken. Dieser Vorgang kann einige Minuten dauern. Lasse deine Konsole nicht in den Ruhezustand gehen und druecke nicht die Home-Taste.\n"
				"Moechtest du fortfahren ?")) return;
			fs::RemoveSystemDataDir();
			try
			{				
				hactool::ExtractHomeMenu();
				hactool::ExtractPlayerSelectMenu();
				hactool::ExtractUserPage();
				Dialog("Abgeschlossen, das Homemenu wurde extrahiert. Jetzt kannst du .nxtheme-Dateien installieren. !");
			}
			catch (std::runtime_error &err)
			{
				DialogBlocking("Fehler beim entpacken des Homemenu: " + string(err.what()));
			}
		});
	}
	PAGE_RESET_FOCUS;
	
	ImGui::PopFont();
	Utils::ImGuiCloseWin();
}

void NcaDumpPage::Update()
{	
	if (Utils::PageLeaveFocusInput()){
		Parent->PageLeaveFocus(this);
	}
}

void NcaDumpPage::CheckHomeMenuVer()
{
	if (!filesystem::exists(SD_PREFIX "/themes/systemData/ResidentMenu.szs"))
	{
		DialogBlocking("Um benutzerdefinierte Themes zu installieren, musst du zuerst das Homemenu entpacken. Dieser Vorgang kann einige Minuten dauern. Lasse deine Konsole nicht in den Ruhezustand gehen und druecke nicht die Home-Taste.\nDruecke A, um zu starten.");
		goto DUMP_HOMEMENU;
	}
	
	if (filesystem::exists(SD_PREFIX "/themes/systemData/ver.cfg"))
	{
		FILE *ver = fopen(SD_PREFIX "/themes/systemData/ver.cfg", "r");
		if (ver)
		{
			char str[50] = {0};
			fgets(str,49,ver);
			fclose(ver);
			string version(str);
			if (version != SystemVer) goto ASK_DUMP;
			else return;
		}
		else goto ASK_DUMP;
	}
	else if (HOSVer.major >= 7) goto ASK_DUMP;
	else WriteHomeNcaVersion();
	return;
	
ASK_DUMP:
	if (!YesNoPage::Ask("Die aktuelle FW-Version unterscheidet sich von der des entpackten Homemenus. Moechtest du das Homemenu erneut entpacken?\nWenn das entpackte Homemenu nicht mit dem installierten uebereinstimmt, koennen Themes abstuerzen."))
	{
		DialogBlocking("Diese Nachricht wird nicht erneut angezeigt. Im Falle von Abstuerzen kannst du das Homemenu manuell entpacken, indem du die Option 'Entpacke Homemenu'im Hauptmenue verwendest.");
		WriteHomeNcaVersion();
		return;
	}
	
DUMP_HOMEMENU:
	fs::RemoveSystemDataDir();
	try
	{
		hactool::ExtractHomeMenu();
	}
	catch (std::runtime_error &err)
	{
		DialogBlocking("Fehler beim entpacken des Homemenu: " + string(err.what()));
	}
}

void NcaDumpPage::WriteHomeNcaVersion()
{
	FILE* ver = fopen(fs::path::NcaVersionCfg.c_str(), "w");
	if (!ver)
		return;
	fprintf(ver, "%s", SystemVer.c_str());
	fclose(ver);
}



