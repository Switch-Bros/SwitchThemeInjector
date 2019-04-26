#include "Patches.hpp"
#include <algorithm>

using namespace std;

namespace NewFirmFixes 
{
	const static string DogeLayoutFix = "{\"PatchName\":\"DogeLayout 8.x fix\",\"AuthorName\":\"autoDiff\",\"Files\":[{\"FileName\":\"blyt/HudTime.bflyt\",\"Patches\":[{\"PaneName\":\"N_AMPM\",\"Position\":{\"X\":30,\"Y\":-1,\"Z\":0},\"Scale\":{\"X\":0.9,\"Y\":0.9}}]},{\"FileName\":\"blyt/RdtBtnFullLauncher.bflyt\",\"Patches\":[{\"PaneName\":\"N_Tip\",\"Scale\":{\"X\":1.1,\"Y\":1.1}},{\"PaneName\":\"B_Hit\",\"Size\":{\"X\":80,\"Y\":80}}]},{\"FileName\":\"blyt/Cursor3.bflyt\",\"Patches\":[{\"PaneName\":\"P_Main\",\"UsdPatches\":[{\"PropName\":\"S_BorderSize\",\"PropValues\":[\"7\"],\"type\":2}]},{\"PaneName\":\"P_Grow\",\"UsdPatches\":[{\"PropName\":\"S_BorderSize\",\"PropValues\":[\"7\"],\"type\":2}]}]},{\"FileName\":\"blyt/RdtBtnMyPage.bflyt\",\"Patches\":[{\"PaneName\":\"N_Tip\",\"Position\":{\"X\":125,\"Y\":0,\"Z\":0}},{\"PaneName\":\"B_Hit\",\"Scale\":{\"X\":1.428571,\"Y\":1.428571},\"Size\":{\"X\":40,\"Y\":40}}]},{\"FileName\":\"blyt/RdtBtnIconGame.bflyt\",\"Patches\":[{\"PaneName\":\"RootPane\",\"Scale\":{\"X\":0.5,\"Y\":0.5}},{\"PaneName\":\"P_InnerCursor\",\"Visible\":false},{\"PaneName\":\"N_BtnFocusKey\",\"Size\":{\"X\":259,\"Y\":259}},{\"PaneName\":\"N_Tip\",\"Scale\":{\"X\":1.1,\"Y\":1.1}},{\"PaneName\":\"B_Hit\",\"Scale\":{\"X\":2,\"Y\":2},\"Size\":{\"X\":132,\"Y\":132}}]},{\"FileName\":\"blyt/RdtBase.bflyt\",\"Patches\":[{\"PaneName\":\"N_ScrollArea\",\"Position\":{\"X\":0,\"Y\":-218,\"Z\":0},\"Scale\":{\"X\":1,\"Y\":0.5},\"Size\":{\"X\":1300,\"Y\":322}},{\"PaneName\":\"N_ScrollWindow\",\"Position\":{\"X\":0,\"Y\":-218,\"Z\":0},\"Size\":{\"X\":100000,\"Y\":322}},{\"PaneName\":\"T_Blank\",\"Position\":{\"X\":0,\"Y\":197,\"Z\":0}},{\"PaneName\":\"N_GameRoot\",\"Position\":{\"X\":-530,\"Y\":-218,\"Z\":0},\"Scale\":{\"X\":0.00001,\"Y\":1}},{\"PaneName\":\"N_Game\",\"Position\":{\"X\":0,\"Y\":0,\"Z\":0},\"Scale\":{\"X\":100000,\"Y\":1}},{\"PaneName\":\"N_Icon_01\",\"Position\":{\"X\":135,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_02\",\"Position\":{\"X\":270,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_03\",\"Position\":{\"X\":405,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_04\",\"Position\":{\"X\":540,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_05\",\"Position\":{\"X\":675,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_06\",\"Position\":{\"X\":810,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_07\",\"Position\":{\"X\":945,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_08\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_09\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_10\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_11\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_12\",\"Position\":{\"X\":1080,\"Y\":0,\"Z\":0},\"Scale\":{\"X\":1,\"Y\":1}},{\"PaneName\":\"L_BtnFlc\",\"Scale\":{\"X\":0.5,\"Y\":0.5}}]},{\"FileName\":\"blyt/Hud.bflyt\",\"Patches\":[{\"PaneName\":\"N_Time\",\"Size\":{\"X\":12,\"Y\":30}},{\"PaneName\":\"L_Time\",\"Position\":{\"X\":-18,\"Y\":0,\"Z\":0}}]}]}";
	const static string DiamondFix = "{\"PatchName\":\"Diamond 8 fix\",\"AuthorName\":\"akai\",\"Files\":[{\"FileName\":\"blyt/RdtBtnFullLauncher.bflyt\",\"Patches\":[{\"PaneName\":\"N_Root\",\"Rotation\":{\"X\":0,\"Y\":0,\"Z\":45}}]},{\"FileName\":\"blyt/RdtBtnIconGame.bflyt\",\"Patches\":[{\"PaneName\":\"RootPane\",\"Scale\":{\"X\":0.37,\"Y\":0.37}},{\"PaneName\":\"B_Hit\",\"Scale\":{\"X\":3,\"Y\":3},\"Size\":{\"X\":97.68,\"Y\":97.68}}]},{\"FileName\":\"blyt/RdtBase.bflyt\",\"Patches\":[{\"PaneName\":\"N_ScrollArea\",\"Position\":{\"X\":-30,\"Y\":-200,\"Z\":0},\"Size\":{\"X\":1300,\"Y\":322}},{\"PaneName\":\"N_ScrollWindow\",\"Position\":{\"X\":-30,\"Y\":-200,\"Z\":0},\"Size\":{\"X\":1080,\"Y\":322}},{\"PaneName\":\"N_GameRoot\",\"Position\":{\"X\":200,\"Y\":-195,\"Z\":0},\"Scale\":{\"X\":0.00001,\"Y\":1}},{\"PaneName\":\"N_Game\",\"Position\":{\"X\":0,\"Y\":0,\"Z\":0},\"Scale\":{\"X\":100000,\"Y\":1}},{\"PaneName\":\"N_Icon_01\",\"Position\":{\"X\":140,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_02\",\"Position\":{\"X\":280,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_03\",\"Position\":{\"X\":68,\"Y\":73,\"Z\":0}},{\"PaneName\":\"N_Icon_04\",\"Position\":{\"X\":208,\"Y\":73,\"Z\":0}},{\"PaneName\":\"N_Icon_05\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_06\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_07\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_08\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_09\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_10\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_11\",\"Position\":{\"X\":1,\"Y\":99999,\"Z\":0}},{\"PaneName\":\"N_Icon_12\",\"Position\":{\"X\":348,\"Y\":73,\"Z\":0}},{\"PaneName\":\"L_BtnFlc\",\"Scale\":{\"X\":0.37,\"Y\":0.37}},{\"PaneName\":\"L_BtnAccount_00\",\"Position\":{\"X\":-247,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_01\",\"Position\":{\"X\":-175,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_02\",\"Position\":{\"X\":-103,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_03\",\"Position\":{\"X\":-31,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_04\",\"Position\":{\"X\":41,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_05\",\"Position\":{\"X\":113,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_06\",\"Position\":{\"X\":185,\"Y\":0,\"Z\":0}}]},{\"FileName\":\"blyt/Hud.bflyt\",\"Patches\":[{\"PaneName\":\"N_Time\",\"Position\":{\"X\":10,\"Y\":0,\"Z\":0}}]}]}";
	const static string CompactFix = "{\"PatchName\":\"Compact 8 fix\",\"AuthorName\":\"akai\",\"Files\":[{\"FileName\":\"blyt/RdtBtnFullLauncher.bflyt\",\"Patches\":[{\"PaneName\":\"N_Root\",\"Rotation\":{\"X\":0,\"Y\":0,\"Z\":45}}]},{\"FileName\":\"blyt/RdtBtnIconGame.bflyt\",\"Patches\":[{\"PaneName\":\"RootPane\",\"Scale\":{\"X\":0.37,\"Y\":0.37}},{\"PaneName\":\"B_Hit\",\"Scale\":{\"X\":3,\"Y\":3},\"Size\":{\"X\":97.68,\"Y\":97.68}}]},{\"FileName\":\"blyt/RdtBase.bflyt\",\"Patches\":[{\"PaneName\":\"N_ScrollArea\",\"Position\":{\"X\":0,\"Y\":-280,\"Z\":0},\"Scale\":{\"X\":1,\"Y\":0.5},\"Size\":{\"X\":1300,\"Y\":322}},{\"PaneName\":\"N_ScrollWindow\",\"Position\":{\"X\":0,\"Y\":-280,\"Z\":0},\"Scale\":{\"X\":1,\"Y\":0.5},\"Size\":{\"X\":1080,\"Y\":322}},{\"PaneName\":\"N_GameRoot\",\"Position\":{\"X\":-50,\"Y\":-230,\"Z\":0},\"Scale\":{\"X\":0.000001,\"Y\":1}},{\"PaneName\":\"N_Game\",\"Position\":{\"X\":0,\"Y\":0,\"Z\":0},\"Scale\":{\"X\":1000000,\"Y\":1}},{\"PaneName\":\"N_Icon_01\",\"Position\":{\"X\":100,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_02\",\"Position\":{\"X\":200,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_03\",\"Position\":{\"X\":300,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_04\",\"Position\":{\"X\":400,\"Y\":0,\"Z\":0}},{\"PaneName\":\"N_Icon_05\",\"Position\":{\"X\":500,\"Y\":0,\"Z\":0},\"Visible\":true},{\"PaneName\":\"N_Icon_06\",\"Position\":{\"X\":1,\"Y\":-9999999,\"Z\":0}},{\"PaneName\":\"N_Icon_07\",\"Position\":{\"X\":1,\"Y\":-9999999,\"Z\":0}},{\"PaneName\":\"N_Icon_08\",\"Position\":{\"X\":1,\"Y\":-9999999,\"Z\":0}},{\"PaneName\":\"N_Icon_09\",\"Position\":{\"X\":1,\"Y\":-9999999,\"Z\":0}},{\"PaneName\":\"N_Icon_10\",\"Position\":{\"X\":1,\"Y\":-9999999,\"Z\":0}},{\"PaneName\":\"N_Icon_11\",\"Position\":{\"X\":1,\"Y\":-9999999,\"Z\":0}},{\"PaneName\":\"N_Icon_12\",\"Position\":{\"X\":600,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnFlc\",\"Scale\":{\"X\":0.37,\"Y\":0.37}},{\"PaneName\":\"L_BtnAccount_00\",\"Position\":{\"X\":-247,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_01\",\"Position\":{\"X\":-175,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_02\",\"Position\":{\"X\":-103,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_03\",\"Position\":{\"X\":-31,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_04\",\"Position\":{\"X\":41,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_05\",\"Position\":{\"X\":113,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_BtnAccount_06\",\"Position\":{\"X\":185,\"Y\":0,\"Z\":0}},{\"PaneName\":\"L_Hud\",\"Position\":{\"X\":-290,\"Y\":-325,\"Z\":0}}]},{\"FileName\":\"blyt/Hud.bflyt\",\"Patches\":[{\"PaneName\":\"N_Time\",\"Position\":{\"X\":10,\"Y\":0,\"Z\":0}}]}]}";

	//https://stackoverflow.com/questions/3152241/case-insensitive-stdstring-find
	static bool findStringIC(const std::string& strHaystack, const std::string& strNeedle)
	{
		auto it = std::search(
			strHaystack.begin(), strHaystack.end(),
			strNeedle.begin(), strNeedle.end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);
		return (it != strHaystack.end());
	}

	vector<LayoutFilePatch> GetFix(const string &LayoutName)
	{
		if (findStringIC(LayoutName, "dogelayout") || findStringIC(LayoutName, "clearlayout"))
			return Patches::LoadLayout(DogeLayoutFix).Files;
		else if (findStringIC(LayoutName, "diamond"))
			return Patches::LoadLayout(DiamondFix).Files;
		else if (findStringIC(LayoutName, "small compact"))
			return Patches::LoadLayout(CompactFix).Files;
		return {};
	}
}