#pragma once

#include "Interface.h"

class CConsole {
public:
	void Render() {
		if (Pointers::showConsole) {
			ImGui::Begin("Console");

			ImGui::End();
		}
	}
};