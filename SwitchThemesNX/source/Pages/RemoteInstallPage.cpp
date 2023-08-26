#include "RemoteInstallPage.hpp"
#include "../ViewFunctions.hpp"
#include <numeric>
#include "RemoteInstall/API.hpp"
#include "../SwitchThemesCommon/Layouts/json.hpp"

using namespace std;

static bool ComboBoxApiProviderGetter(void*, int index, const char** str)
{
	if (index < 0 || (size_t)index >= RemoteInstall::API::ProviderCount())
		return false;
	
	*str = RemoteInstall::API::GetProvider(index).Name.c_str();
	return true;
}

RemoteInstallPage::~RemoteInstallPage()
{
	if (!UseLowMemory)
		RemoteInstall::Finalize();
}

RemoteInstallPage::RemoteInstallPage() : 
BtnStart("Starte Remote Inst.###InstallBtn")
{
	Name = "Themes holen";
	if (!UseLowMemory)
	{
		SetRemoteInstallCode("");
		RemoteInstall::Initialize();
	}
}

void RemoteInstallPage::Render(int X, int Y)
{
	AllowLeft = true;

	Utils::ImGuiSetupPage(this, X, Y, DefaultWinFlags & ~ImGuiWindowFlags_NoScrollbar);
	
	if (!RemoteInstallFile)
	{
		ImGui::PushFont(font40);
		ImGui::Text("Download vom Internet");
		ImGui::PopFont();

		if (UseLowMemory)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::TextWrapped("Die Funktion ist im Applet-Modus nicht verfuegbar, starte mit Title-Override.");
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::TextWrapped("Waehle einen Anbieter aus der Liste, um Themes zu Downloaden.\nDu kannst benutzerdefinierte Anbieter hinzufuegen, wie im Wiki auf GitHub erklaert wird.");
			ImGui::PushItemWidth(500);
			if (ImGui::Combo("###ProviderSelection", &ProviderIndex, ComboBoxApiProviderGetter, nullptr, RemoteInstall::API::ProviderCount()))
				SelectedProviderStatic = RemoteInstall::API::GetProvider(ProviderIndex).Static;
			
			PAGE_RESET_FOCUS;
			if (ImGui::IsItemFocused())
				ImGui::SetScrollY(0);

			if (!SelectedProviderStatic) {
				ImGui::SameLine();
				if (ImGui::Button("Zufaelliges Theme"))
					StartRemoteInstallFixed(RemoteInstall::FixedTypes::Random);
				CurItemBlockLeft();
				ImGui::SameLine();
				if (ImGui::Button("Neues Theme"))
					StartRemoteInstallFixed(RemoteInstall::FixedTypes::Recent);
				CurItemBlockLeft();
			}

			ImGui::TextWrapped("Oder Theme ueber die ID suchen");
			ImGui::PushStyleColor(ImGuiCol_Button, 0xDFDFDFDF);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0xEFEFEFEF);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xFFFFFFFF);
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
			if (ImGui::Button(RemoteInstallBtnText.c_str(), { 300, 0 }))
				SetRemoteInstallCode(PlatformTextInput(RemoteInstallCode.c_str()));
			ImGui::PopStyleColor(4);
			ImGui::SameLine(0, 20);
			if (ImGui::Button("Suchen###SearchBtn", { 150, 0 }) && RemoteInstallCode != "")
				StartRemoteInstallByCode();
			CurItemBlockLeft();
			ImGui::TextWrapped("IDs sind keine Namen, Themes nach Namen suchen wird nicht funktionieren. Oeffne den Theme-Anbieter im Browser, waehle ein Theme aus und es sollte dessen eindeutige ID anzeigen.");
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushFont(font40);
		ImGui::Text("Remote-Installation ueber den Theme Injector");
		ImGui::PopFont();
	}

	if (RemoteInstallFile)
	{
		if (RemoteInstallFile->Render() == ThemeEntry::UserAction::Enter || AutoInstall)
			PushFunction([this]() {
				RemoteInstallFile->Install(!AutoInstall);
				RemoteInstallFile = nullptr;

				if (AutoInstall)
					PlatformReboot();
			});
		if (ImGui::IsWindowFocused())
			Utils::ImGuiSelectItem();

		ImGui::TextWrapped("Drueck A zum installieren, B zum Abbrechen");
	}
	else 
	{
		ImGui::TextWrapped("Du kannst ein Theme direkt vom PC mit dem Theme-Injector installieren. Gehe zum Tab 'NXTheme Builder' und klicke auf 'Remote Installation...'");
		if (ImGui::Button(BtnStart.c_str()))
		{
			if (!server.IsHosting())
				StartServer();
			else
				StopServer();
		}
		if (UseLowMemory) PAGE_RESET_FOCUS;
		ImGui::TextWrapped("Halte den Fokus im Menue auf dieser Seite, sonst werden Anfragen nicht ausgefuehrt.");
		ImGui::Checkbox("Automatisch installieren und Neustarten", &AutoInstall);
	}
	Utils::ImGuiSetWindowScrollable();

	Utils::ImGuiCloseWin();
}

void RemoteInstallPage::StartRemoteInstallByCode()
{
	PushFunction([this]() {
		try {
			DisplayLoading("Lade...");
			RemoteInstall::Begin(SelectedProvider(), RemoteInstallCode);
		}
		catch (nlohmann::json::type_error& ex)	{
			DialogBlocking("Es gab einen Fehler beim Verarbeiten der Antwort vom Server. Dies bedeutet oft, dass der angeforderte Code nicht gefunden werden konnte. Stelle sicher, dass der Code gueltig ist.\n\nFehlermeldung:\n"s + ex.what());
		}
		catch (std::exception& ex) {
			DialogBlocking("Es gab einen Fehler bei der Verarbeitung der Anfrage. Stelle sicher, dass der Code gueltig ist und dass du mit dem Internet verbunden bist.\n\nFehlermeldung:\n"s + ex.what());
		}
	});
}

void RemoteInstallPage::StartRemoteInstallFixed(RemoteInstall::FixedTypes type)
{
	PushFunction([this, type]() {
		try {
			DisplayLoading("Lade...");
			RemoteInstall::BeginType(SelectedProvider(), type);
		}
		catch (std::exception& ex) {
			DialogBlocking("Es gab einen Fehler bei der Verarbeitung der Anfrage. Stelle sicher, dass du mit dem Internet verbunden bist, und versuche es in Kuerze erneut. Falls es immer noch nicht funktioniert, ist es moeglich, dass der ausgewaehlte Anbieter diese Option nicht unterstuetzt.\n\nFehlermeldung:\n"s + ex.what());
		}
	});
}

void RemoteInstallPage::CurItemBlockLeft() 
{
	AllowLeft &= !ImGui::IsItemFocused();
}

void RemoteInstallPage::Update()
{
	if (RemoteInstallFile && KeyPressed(GLFW_GAMEPAD_BUTTON_B))
	{
		RemoteInstallFile = nullptr;
		return;
	}

	if (Utils::PageLeaveFocusInput(AllowLeft)) {
		Parent->PageLeaveFocus(this);
		return;
	}

	if (RemoteInstallFile) return;

	UpdateServer();
}

void RemoteInstallPage::SetRemoteInstallCode(const char* input)
{
	RemoteInstallCode = std::string(input);
	if (RemoteInstallCode == "")
		RemoteInstallBtnText = "ID eingeben###themeIDinput";
	else
		RemoteInstallBtnText = RemoteInstallCode + "###themeIDinput";
}

void RemoteInstallPage::StartServer()
{
	try 
	{
		server.StartHosting();
		BtnStart = ("IP: " + server.GetHostname() + " - Druecken zum Abbrechen###InstallBtn");
	}
	catch (std::exception& ex)
	{
		Dialog(ex.what());
	}
}

void RemoteInstallPage::StopServer()
{
	server.StopHosting();
	BtnStart = "Beginne Remote Installation###InstallBtn";
}

void RemoteInstallPage::DialogError(const std::string &msg)
{
	Dialog("Ein Fehler ist aufgetreten, versuch es erneut.\n" + msg);
}

const RemoteInstall::Provider& RemoteInstallPage::SelectedProvider()
{
	return RemoteInstall::API::GetProvider(ProviderIndex);
}

void RemoteInstallPage::UpdateServer()
{	
	try 
	{
		server.HostUpdate();
		if (server.HasFinished())
		{
			RemoteInstallFile = ThemeEntry::FromSZS(server.Buffer());
			server.Clear();
			StopServer();
		}
	}
	catch (std::exception& ex)
	{
		Dialog(ex.what());
		StopServer();
	}
}