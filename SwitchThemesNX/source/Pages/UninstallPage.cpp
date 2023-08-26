#include "UninstallPage.hpp"
#include "../ViewFunctions.hpp"
#include "../SwitchTools/PatchMng.hpp"

using namespace std;

UninstallPage::UninstallPage()
{
	Name = "Themes loeschen";
}

void UninstallPage::Render(int X, int Y)
{
	Utils::ImGuiSetupPage(this, X, Y);
	ImGui::PushFont(font30);

	ImGui::TextWrapped("Verwenden, um derzeit installierte Themes zu loeschen.\nWenn du Probleme hast, kannst du versuchen, den gesamten LayeredFS-Ordner und die Code-Patches zu entfernen.");

	ImGui::PushStyleColor(ImGuiCol_Button, u32(0x6B70000ff));
	
	auto i = Utils::ImGuiCenterButtons({ "Aktuelles Theme loeschen", "Alle Themes loeschen" } , &firstBtn);
	if (i != -1)
	{
		PushFunction([i]() {
			if (!YesNoPage::Ask("Bist du dir sicher ?")) return;
			if (i == 1)
			{
				DisplayLoading("Loesche LayeredFS Ordner...");
				fs::theme::UninstallTheme(true);
				PatchMng::RemoveAll();
				Dialog(
					"Abgeschlossen, alles im Zusammenhang mit Themes wurde entfernt. Starte deine Konsole neu, um die Aenderungen zu uebernehmen.\n"
					"Da dadurch auch die Homemenu-Patches entfernt wurden, solltest du diese App neu starten, bevor du ein neues Theme installierst."
				);
			}
			else
			{
				DisplayLoading("Lade...");
				fs::theme::UninstallTheme(false);
				Dialog("Abgeschlossen, alle installierten Themes wurden entfernt. Starte deine Konsole neu, um die Aenderungen zu uebernehmen.");
			}
		});
	}
	
	PAGE_RESET_FOCUS_FOR(firstBtn);
	ImGui::PopStyleColor();

	ImGui::PopFont();
	Utils::ImGuiSetWindowScrollable();
	Utils::ImGuiCloseWin();
}

void UninstallPage::Update()
{	
	if (Utils::PageLeaveFocusInput() && ImGui::GetFocusID() == firstBtn){
		Parent->PageLeaveFocus(this);
	}
}




