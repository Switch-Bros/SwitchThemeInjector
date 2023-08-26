#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <filesystem>
#include <variant>

#include "Version.hpp"
#include "UI/UIManagement.hpp"
#include "UI/UI.hpp"
#include "Platform/Platform.hpp"

#include "Pages/ThemePage.hpp"
#include "Pages/CfwSelectPage.hpp"
#include "Pages/UninstallPage.hpp"
#include "Pages/NcaDumpPage.hpp"
#include "Pages/TextPage.hpp"
#include "Pages/ExternalInstallPage.hpp"
#include "ViewFunctions.hpp"
#include "SwitchThemesCommon/SwitchThemesCommon.hpp"
#include "SwitchThemesCommon/NXTheme.hpp"
#include "Pages/RemoteInstallPage.hpp"
#include "Pages/SettingsPage.hpp"
#include "Pages/RebootPage.cpp"
#include "Pages/QlaunchPatchPage.hpp"

#include "SwitchTools/PatchMng.hpp"

//#define DEBUG

static inline void ImguiBindController()
{
	ImGuiIO& io = ImGui::GetIO();

	NAV_DOWN	= (KeyPressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN)	|| StickAsButton(1) > .3f	|| StickAsButton(3) > .3f );
	NAV_UP		= (KeyPressed(GLFW_GAMEPAD_BUTTON_DPAD_UP)		|| StickAsButton(1) < -.3f	|| StickAsButton(3) < -.3f);
	NAV_LEFT	= (KeyPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT)	|| StickAsButton(0) < -.3f	|| StickAsButton(2) < -.3f);
	NAV_RIGHT	= (KeyPressed(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT)	|| StickAsButton(0) > .3f	|| StickAsButton(2) > .3f );

	io.NavInputs[ImGuiNavInput_DpadDown]	= NAV_DOWN	? 1.0f : 0;
	io.NavInputs[ImGuiNavInput_DpadUp]		= NAV_UP	? 1.0f : 0;
	io.NavInputs[ImGuiNavInput_DpadLeft]	= NAV_LEFT	? 1.0f : 0;
	io.NavInputs[ImGuiNavInput_DpadRight]	= NAV_RIGHT	? 1.0f : 0;

	io.NavInputs[ImGuiNavInput_Activate] = KeyPressed(GLFW_GAMEPAD_BUTTON_A);
	io.NavInputs[ImGuiNavInput_Cancel] = KeyPressed(GLFW_GAMEPAD_BUTTON_B);

	io.NavActive = true;
	io.NavVisible = true;
}

static bool IsRendering = false;
static std::vector<IUIControlObj*> Pages;
static std::vector<IUIControlObj*> PopList;

static IUIControlObj *ViewObj = nullptr;

void PopPage(IUIControlObj* page)
{
	PopList.push_back(page);
}

static void _PopPage()
{
	auto ShouldRemove = [](IUIControlObj* obj) -> bool 
	{
		auto p = std::find(PopList.begin(), PopList.end(), obj);
		if (p == PopList.end()) return false;
		delete *p;
		return true;
	};

	Pages.erase(std::remove_if(Pages.begin(), Pages.end(), ShouldRemove));
	PopList.clear();

	ViewObj = Pages.size() ? Pages.back() : nullptr;
}

//All pages must be dynamically allocated
void PushPage(IUIControlObj* page)
{
	Pages.push_back(page);
	ViewObj = page;
}

std::vector<std::function<void()>> DeferredFunctions;
void PushFunction(const std::function<void()>& fun)
{
	DeferredFunctions.push_back(fun);
}

static void ExecuteDeferredFunctions() 
{
	auto vec = std::move(DeferredFunctions);
	DeferredFunctions = {};
	for (auto& fun : vec)
		fun();
}

void Dialog(const std::string &msg)
{
	PushPage(new DialogPage(msg));
}

static inline void DisplayLoading(LoadingOverlay &&o)
{
	GFX::StartFrame();
	o.Render(0, 0);
	GFX::EndFrame();
}

void DisplayLoading(const std::string& msg)
{
	DisplayLoading(LoadingOverlay(msg));
}

void DisplayLoading(std::initializer_list<std::string> lines)
{
	DisplayLoading(LoadingOverlay(lines));
}

#ifdef DEBUG
double previousTime = glfwGetTime();
int frameCount = 0;
int fpsValue = 0;

static void calcFPS() 
{
	double currentTime = glfwGetTime();
	frameCount++;

	if (currentTime - previousTime >= 1.0)
	{
		fpsValue = frameCount;

		frameCount = 0;
		previousTime = currentTime;
	}
	ImGui::Text("FPS %d", fpsValue);
	for (int i = 0; i < 6; i++)
	{
		ImGui::Text("pad[%d] = %d %f %f ", i, (int)(StickAsButton(i) != 0), gamepad.axes[i], OldGamepad.axes[i]);
	}
}
#endif

static inline void MainLoop(bool BreakOnPop = false)
{
	while (App::MainLoop() && ViewObj)
	{
		PlatformGetInputs();
		ImguiBindController();
		PlatformImguiBinds();

		//A control may push a page either in the render or the update function.
		IUIControlObj* CurObj = ViewObj;

		IsRendering = true;
		GFX::StartFrame();
		CurObj->Render(0,0);
#ifdef DEBUG
		calcFPS();
#endif
		GFX::EndFrame();
		IsRendering = false;

		if (CurObj == ViewObj)
			CurObj->Update();
		
		ExecuteDeferredFunctions();
		if (PopList.size())
		{
			_PopPage();
			if (BreakOnPop)
				break;
		}

		PlatformSleep(1 / 30.0f * 1000);
	}
}


void PushPageBlocking(IUIControlObj* page)
{
	if (IsRendering)
	{
		throw std::runtime_error("Attempted to push a blocking page while rendering");
	}

	PushPage(page);
	MainLoop(true);
}

void DialogBlocking(const std::string& msg)
{
	PushPageBlocking(new DialogPage(msg));
}

class QuitPage : public IPage
{
	public:
		QuitPage()
		{
			Name = "Beenden";
		}	
		
		void Render(int X, int Y) override {}
		
		void Update() override
		{
			App::Quit();
		}
};

void ShowFirstTimeHelp(bool WelcomeScr)
{	
	if (WelcomeScr)
		DialogBlocking("Willkommen beim NXThemes Installer " + Version::Name + "!\n\nDiese Seiten enthalten einige wichtige Informationen. Es wird empfohlen, sie sorgfaeltig zu lesen.\nDies wird nur einmal angezeigt, du kannst es erneut im Bereich 'Credits' nachlesen.");

	DialogBlocking(
		"Benutzerdefinierte Themes koennen deine Konsole NICHT 'bricken', da sie nur auf der SD-Karte installiert werden. \n"
		"Wenn nach der Installation eines Themes deine Konsole nicht mehr startet, loesche manuell die Ordner '0100000000001000' und '0100000000001013' im Verzeichnis 'SD-Karte/atmosphere/contents'.\n\n"
		"Wenn du die Firmware deiner Konsole aenderst (upgrade oder downgrade), musst du zuerst das Theme deinstallieren, da die auf der SD-Karte installierten Dateien von der Firmware abhaengig sind.\n"
		"Wenn die von dir installierte Firmware Themes unterstuetzt, kannst du sie nach dem Update wieder installieren.\n\n"
		"Lockscreen-Themes nach FW 9.0 werden nicht von allen CFWs unterstuetzt, da einige keine Unterstuetzung zum Patchen von Titeln über IPS bieten.."
	);

	DialogBlocking(
		"Du kannst einige Themes im /r/NXThemes-Subreddit finden und im Qcean-Discord-Server (Einladung: CUnHJgb), wo du auch um Unterstuetzung bitten kannst. \n\n"
		"Um eigene Themes zu erstellen, lade die Windows-App herunter unter: https://github.com/exelix11/SwitchThemeInjector\n"
		"oder benutze den Online-Editor unter: https://themezer.net/tools/theme-creator\n"
		"\n"
		"Das ist alles, viel Spaß mit benutzerdefinierten Themes! :)"
	);
}

// Note that CfwFolder is set after the constructor of any page pushed before CheckCFWDir is called, CfwFolder shouldn't be used until the theme is actually being installed
static void SetCfwFolder()
{
	auto f = fs::cfw::SearchFolders();
	if (f.size() != 1)
		PushPageBlocking(new CfwSelectPage(f));
	else
		fs::cfw::SetFolder(f[0]);
}

static std::vector<std::string> GetArgsInstallList(int argc, char** argv)
{
	if (argc <= 1) return {};

	std::vector<std::string> Args(argc - 1);
	for (size_t i = 0; i < Args.size(); i++)
		Args[i] = std::string(argv[i + 1]);

	//Appstore-style args
	if (StrStartsWith(Args[0], "installtheme="))
	{
		std::string key = "installtheme=";
		std::string pathss;
		std::vector<std::string> paths;
		for (auto argvs : Args)
		{
			auto pos = argvs.find(key);
			size_t index;
			while (true)
			{
				index = argvs.find("(_)");
				if (index == std::string::npos) break;
				argvs.replace(index, 3, " ");
			}
			if (pos != std::string::npos)
				pathss = argvs.substr(pos + 13);

			if (!pathss.empty())
			{
				std::string path;
				std::stringstream stream(pathss);
				while (getline(stream, path, ',')) {
					if (fs::Exists(path) && std::filesystem::is_regular_file(path))
						paths.push_back(path);
				}
			}
		}
		return paths;
	}
	else //File args from nx-hbloader
	{
		std::vector<std::string> res;

		for (auto& s : Args)
			if (fs::Exists(s) && std::filesystem::is_regular_file(s))
				res.push_back(move(s));

		return res;
	}
}	

std::string SystemVer = "";
static void SetupSysVer()
{
#if __SWITCH__
	setsysInitialize();
	SetSysFirmwareVersion firmware;
	auto res = setsysGetFirmwareVersion(&firmware);
	if (R_FAILED(res))
	{
		setsysExit();
		DialogBlocking("Konnte die Systemversion nicht abrufen. Ergebnis=" + std::to_string(res));
		return;
	}
	HOSVer = { firmware.major,firmware.minor,firmware.micro };
	setsysExit();
#else 
	HOSVer = { 11,0,0 };
#endif
	if (HOSVer.major <= 5)
	{
		ThemeTargetToName = ThemeTargetToName5X;
		ThemeTargetToFileName = ThemeTargetToFileName6X; //The file names are the same
	}
	else //6.X
	{
		ThemeTargetToName = ThemeTargetToName6X;
		ThemeTargetToFileName = ThemeTargetToFileName6X;
	}
	SystemVer = std::to_string(HOSVer.major) + "." + std::to_string(HOSVer.minor) + "." + std::to_string(HOSVer.micro);
}

int main(int argc, char **argv)
{
	PlatformInit();

	if (!GFX::Init())
	{
		PlatformExit();
		return -1;
	}
	PlatformAfterInit();

	SetupSysVer();
	DisplayLoading("Lade Systeminformationen...");

	bool ThemesFolderExists = fs::EnsureThemesFolderExists();
	NcaDumpPage::CheckHomeMenuVer();
	SetCfwFolder();

	PatchMng::Init();

	if (envHasArgv() && argc > 1)
	{
		PatchMng::EnsureInstalled(); // no checks here :(
		auto paths = GetArgsInstallList(argc,argv);
		if (paths.size() != 0)
		{
			PushPage(new ExternalInstallPage(paths));
			MainLoop();
		}
	}	
	else
	{
		TabRenderer *t = new TabRenderer();
		PushPage(t);
		
		if (!ThemesFolderExists)
			ShowFirstTimeHelp(true);

		QlaunchPatchPage* patchPage = new QlaunchPatchPage();

		// Internally calls patchmng
		if (patchPage->ShouldShow())
			t->AddPage(patchPage);

		ThemesPage *p = new ThemesPage();
		t->AddPage(p);
		RemoteInstallPage* rmi = new RemoteInstallPage();
		t->AddPage(rmi);
		UninstallPage *up = new UninstallPage();
		t->AddPage(up);
		NcaDumpPage *dp = new NcaDumpPage();
		t->AddPage(dp);
		SettingsPage *sf = new SettingsPage();
		t->AddPage(sf);
		CreditsPage *credits = new CreditsPage();
		t->AddPage(credits);
		RebootPage *reboot = new RebootPage();
		t->AddPage(reboot);
		QuitPage *q = new QuitPage();
		t->AddPage(q);

		MainLoop();
		
		delete patchPage;
		delete p;
		delete up;
		delete dp;
		delete rmi;
		delete sf;
		delete credits;
		delete q;
	}

	while (Pages.size() != 0)
	{
		delete Pages.back();
		Pages.pop_back();
	}

	GFX::Exit();
	Image::Internal::AssertOnLeaks();
	PlatformExit();
	
    return 0;
}