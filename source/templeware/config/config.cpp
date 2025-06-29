#include "config.h"

// CBA to make proper atm, it's 03:42 right now.
// For now just stores config values don't mind it too much
//
// (FYI THIS IS A HORRID SOLUTION BUT FUNCTIONS) 

namespace Config {
	bool esp = false;
	bool glow = false;
	bool showHealth = false;
	bool teamCheck = false;
	bool espFill = false;
	bool showNameTags = false;
	bool Radar = false;		
	bool Night = false;
	bool GrenadePrediction = false;

	bool enemyChamsInvisible = false;
	bool enemyChams = false;
	bool teamChams = false;
	bool teamChamsInvisible = false;
	int chamsMaterial = 0;

	bool Triggerbot = false;         
    int Triggerbotkey = VK_MENU;        
    int Triggerdelay = 50;               

	ImVec4 colVisualChams = ImVec4(1, 0, 0, 1);
	ImVec4 colVisualChamsIgnoreZ = ImVec4(1, 0, 0, 1);
	ImVec4 teamcolVisualChamsIgnoreZ = ImVec4(1, 0, 0, 1);
	ImVec4 teamcolVisualChams = ImVec4(1, 0, 0, 1);

	float espThickness = 1.0f;
	float espFillOpacity = 0.5f;
	ImVec4 espColor = ImVec4(1, 0, 0, 1);

	bool fovEnabled = false;
	float fov = 90.0f;
	
	bool team_check = false;
	ImVec4 NightColor = ImVec4(0.1, 0.1, 0.1, 1);
}
