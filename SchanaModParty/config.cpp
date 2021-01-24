class CfgPatches
{
	class SchanaModParty
	{
		requiredAddons[] = { "SchanaModPartyDefine", "JM_CF_Scripts" };
		units[] = {};
		weapons[] = {};
	};
};

class CfgMods
{
	class SchanaModParty
	{
		name = "DBM Party";
		action = "https://deutschebohrmaschine.de";
		author = "Krypton91";
		type = "mod";
		inputs = "SchanaModParty/Data/Inputs.xml";
		dependencies[] =
		{
			"Game",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {
					"SchanaModParty/Game"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {
					"SchanaModParty/Mission"
				};
			};
		};
	};
};
