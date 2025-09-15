class CfgPatches
{
	class ZenLazyAction
	{
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
};

class CfgMods
{
	class ZenLazyAction
	{
		dir = "ZenLazyAction";
		name = "";
		author = "Zenarchist";
		credits = "";
		version = "1.0";
		type = "mod";
		inputs="ZenLazyAction/data/inputs.xml";
		dependencies[] = 
		{ 
			"Game", 
			"World", 
			"Mission" 
		};
		class defs
		{
			class gameScriptModule
			{
				files[] = 
				{ 
					"ZenLazyAction/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				files[] = 
				{ 
					"ZenLazyAction/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				files[] = 
				{ 
					"ZenLazyAction/scripts/5_mission"
				};
			};
		};
	};
};