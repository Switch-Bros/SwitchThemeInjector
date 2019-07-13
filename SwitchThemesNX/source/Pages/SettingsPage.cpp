#include "SettingsPage.hpp"
#include "../ViewFunctions.hpp"
#include "../Platform/Platform.hpp"

using namespace std;

SettingsPage::SettingsPage() : 
lblGuide( "Theme shuffle is implemented as a custom sysmodule, get it from:\nhttps://git.io/fhtY8 \n"
"To install a theme in the shuffle list press R while pressing A or + in the theme install page"),
lblInstalled("Currently installed themes number: 0")
{
	Name = "Settings";
	//hasFocus = false;
	LoadShuffleState();
}

void SettingsPage::Render(int X, int Y)
{
	Utils::ImGuiSetupPage(this, X, Y);
	ImGui::PushFont(font25);

	ImGui::NewLine();
	ImGui::Checkbox("Animation support for nxthemes", &UseAnimations);
	PAGE_RESET_FOCUS
	ImGui::NewLine();

	ImGui::PushFont(font30);
	ImGui::Text("Theme shuffle settings (BETA)");
	ImGui::PopFont();

	ImGui::TextWrapped(lblGuide.c_str());
	ImGui::Text("Theme shuffle mode:");
	ImGui::SameLine();
	if (ImGui::RadioButton("Shuffle", &shuffleValue, -1))
		WriteShuffleFlag(shuffleValue);
	ImGui::SameLine();
	if (ImGui::RadioButton("Cycle", &shuffleValue, 0))
		WriteShuffleFlag(shuffleValue);

	IsLayoutBlockingLeft = GImGui->NavId == ImGui::GetCurrentWindow()->GetID("Cycle");

	ImGui::Text(lblInstalled.c_str());
	ImGui::PushStyleColor(ImGuiCol_Button, 0x6B70000ff);
	if (ImGui::Button("Remove all"))
	{
		ClearThemeShuffle();
		LoadShuffleState();
		Dialog("Theme shuffle deleted");
	}
	ImGui::PopStyleColor();

	ImGui::PopFont();
	Utils::ImGuiSetWindowScrollable();
	Utils::ImGuiCloseWin();
}

void SettingsPage::LoadShuffleState()
{
	lblInstalled = ("Currently installed themes number: " + to_string(GetShuffleCount()));
	FILE *index = fopen(SD_PREFIX "/themes/shuffle/index.db", "r");
	if (index)
	{
		int t = 0;
		fscanf(index,"%d",&t);
		if (t < 0)
			shuffleValue = -1;
		else
			shuffleValue = 0;
		fclose(index);
	}
}

void SettingsPage::WriteShuffleFlag(int i)
{
	FILE *index = fopen(SD_PREFIX "/themes/shuffle/index.db", "w");
	if (index)
	{
		fprintf(index,"%d",i);
		if (i < 0)
			shuffleValue = -1;
		else
			shuffleValue = 0;
		fclose(index);
	}
	else
		Dialog("Error: couldn't write to file. Maybe you don't have any theme in the shuffle.");
}

void SettingsPage::Update()
{	
	if (Utils::PageLeaveFocusInput(!IsLayoutBlockingLeft))
	{
		//hasFocus = false;
		Parent->PageLeaveFocus(this);
		return;
	}
}









