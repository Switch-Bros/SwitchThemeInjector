#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../SwitchThemesCommon/SwitchThemesCommon.hpp"
#include "../UI/UI.hpp"
#include "../fs.hpp"

#include "../Platform/Platform.hpp"
#include "../ViewFunctions.hpp"

class RebootPage : public IPage
{
	public:
		RebootPage()
		{
			Name = "Neustart";
		}
		
		void Render(int X, int Y) 
		{
			Utils::ImGuiSetupPage(this, X, Y);
			ImGui::PushFont(font30);
			ImGui::SetCursorPos({ 5, 10 });

			ImGui::TextUnformatted("Neustarten der Konsole wird deine Aenderungen uebernehmen.");
			ImGui::TextWrapped("Dies ist eine Verknuepfung zur System-Neustarttaste. Wenn deine CFW keinen Neustart zum Payload unterstuetzt, benoetigst du eine Moeglichkeit, einen Payload aus dem RCM-Modus zu injizieren.");
			if (ImGui::Button("Neustarten"))
			{
				PlatformReboot();
			}
			PAGE_RESET_FOCUS;
			
			ImGui::PopFont();
			Utils::ImGuiCloseWin();
		}
		
		void Update() override
		{
			if (Utils::PageLeaveFocusInput())
				Parent->PageLeaveFocus(this);
		}
};