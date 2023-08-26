#include "QlaunchPatchPage.hpp"
#include "../ViewFunctions.hpp"
#include "RemoteInstall/Worker.hpp"

class ThemeUpdateDownloader : public RemoteInstall::Worker::BaseWorker {
public:
	struct Result {
		std::string error;
		long httpCode;
		std::vector<u8> data;
	};

	ThemeUpdateDownloader(const std::string& url, Result& r) : BaseWorker({url}, true), OutResult(r) {
		appendUrlToError = false;
		SetLoadingLine("Ueberpruefe auf Patch Updates...");
	}

protected:
	void OnComplete() {
		const auto& str = Errors.str();
		if (str.length())
			OutResult.error = str;
		else
			OutResult.data = Results.at(0);
	}

	bool OnFinished(uintptr_t index, long httpCode) override {
		OutResult.httpCode = httpCode;
		return true;
	}

	Result& OutResult;
};

QlaunchPatchPage::QlaunchPatchPage() : IPage("Theme Patches") { }

void QlaunchPatchPage::Render(int X, int Y)
{
	Utils::ImGuiSetupPage(this, X, Y);

	ImGui::TextWrapped(
		"Seit Firmware-Version 9.0 muessen bestimmte Teile des Homemenu gepatcht werden, um Themes zu installieren.\n"
		"Wenn du diesen Bildschirm siehst, bedeutet das, dass die fuer deine Firmware benoetigten Patches nicht installiert sind."
	);	

	if (PatchMng::QlaunchBuildId() != "")
	{
		ImGui::Text("Dein Homemenu hat folgende Version (BuildID) :");
		ImGui::PushStyleColor(ImGuiCol_Text, Colors::Highlight);
		Utils::ImGuiCenterString(PatchMng::QlaunchBuildId());
		ImGui::PopStyleColor();
	}
	else 
	{
		ImGui::PushStyleColor(ImGuiCol_Text, Colors::Red);
		ImGui::Text("Fehler: Homemenu Version konnte nicht ermittelt werden");
		ImGui::PopStyleColor();
	}

	if (patchStatus == PatchMng::InstallResult::MissingIps) 
	{		
		ImGui::TextWrapped("Diese Version wird derzeit nicht unterstuetzt. Nach Veroeffentlichung einer neuen Firmware kann es einige Tage dauern, bis die Patches aktualisiert werden.");
		ImGui::TextWrapped(
			"Neue Patches werden jetzt automatisch aus dem Internet heruntergeladen, sobald du diese Anwendung startest. "
			"Wenn du moechtest, kannst du auch jetzt nach Updates suchen."
		);
		
		if (ImGui::Button("Auf Updates pruefen"))
			PushFunction([this]() { CheckForUpdates(); });

		if (updateMessageString != "")
		{
			ImGui::SameLine();

			if (updateMessageIsError)
				ImGui::PushStyleColor(ImGuiCol_Text, Colors::Red);
			else ImGui::PushStyleColor(ImGuiCol_Text, Colors::Highlight);
			
			ImGui::TextWrapped(updateMessageString.c_str());
			ImGui::PopStyleColor();
		}

		ImGui::TextWrapped(
			"Wenn du deine Konsole nicht mit dem Internet verbinden moechtest, kannst du die Patches manuell herunterladen, indem du den Anweisungen unter folgendem Link folgst"
		);
		
		ImGui::PushStyleColor(ImGuiCol_Text, Colors::Highlight);
		ImGui::Text("https://github.com/exelix11/theme-patches");
		ImGui::PopStyleColor();
	}
	else if (patchStatus == PatchMng::InstallResult::SDError)
	{
		ImGui::TextWrapped(
			"Beim Lesen/Schreiben von Dateien auf deine SD-Karte ist ein Fehler aufgetreten. Dies bedeutet in der Regel, dass deine SD-Karte beschaedigt ist.\n"
			"Bitte lass 'Fix Archive-Bit' durchlaufen. Wenn das immer noch nicht funktioniert, nutze eine der BAT-Dateien aus unserem Paket um die SD-Karte neu einzurichten"
		);
	}
	else if (patchStatus == PatchMng::InstallResult::UnsupportedCFW)
	{
		ImGui::TextWrapped(
			"Deine CFW scheint nicht unterstuetzt zu werden.\n"
			"Wenn deine CFW unterstuetzt wird und du trotzdem diese Meldung siehst, gibt es wahrscheinlich ein Problem mit deiner SD-Karte. Installiere deine CFW erneut."
		);
	}
	else if (patchStatus == PatchMng::InstallResult::Ok)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, Colors::Highlight);
		ImGui::Text("Erfolgreich aktualisiert, starte deine Konsole neu. !");
		ImGui::PopStyleColor();
	}

	Utils::ImGuiSetWindowScrollable();
	Utils::ImGuiCloseWin();
}

void QlaunchPatchPage::Update()
{
	if (Utils::PageLeaveFocusInput())
		Parent->PageLeaveFocus(this);
}

void QlaunchPatchPage::CheckForUpdates() {
	ThemeUpdateDownloader::Result res;
	PushPageBlocking(new ThemeUpdateDownloader("https://github.com/exelix11/theme-patches/tree/master/systemPatches" + PatchMng::QlaunchBuildId(), res));

	if (res.error != "")
	{
		updateMessageIsError = true;
		updateMessageString = res.error;
	}
	else if (res.httpCode == 404)
	{
		updateMessageIsError = false;
		updateMessageString = "Kein Update gefunden";
	}
	else if (res.httpCode != 200)
	{
		updateMessageIsError = true;
		updateMessageString = "HTTP Fehler: Code " + res.httpCode;
	}
	else
	{
		updateMessageIsError = false;
		fs::patches::WritePatchForBuild(PatchMng::QlaunchBuildId(), res.data);
		patchStatus = PatchMng::EnsureInstalled();
		updateMessageString = "Erfolgreich aktualisiert, starte deine Konsole neu !";
	}
}

bool QlaunchPatchPage::ShouldShow()
{
	patchStatus = PatchMng::EnsureInstalled();

	if (patchStatus == PatchMng::InstallResult::Ok)
		return false;

	if (patchStatus == PatchMng::InstallResult::MissingIps)
	{
		CheckForUpdates();
		// Has anything changed ? 
		if (patchStatus == PatchMng::InstallResult::Ok)
			return false;
	}
	
	return true;
}
