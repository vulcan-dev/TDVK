#pragma once

#include "Interface.h"

class CMenu {
public:
	void Render() {
		if (Pointers::showMenu) {
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("Developer")) {
					ImGui::MenuItem("Debug Pointers", nullptr, &this->m_ShowDebugTab);
					ImGui::MenuItem("Debug Game", nullptr, &this->m_ShowGameTab);
					ImGui::MenuItem("Show Console", nullptr, &Pointers::showConsole);
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			this->RenderDebugPtrTab();
			this->RenderDebugGameTab();
		}
	}

private:
	void RenderDebugPtrTab() {
		if (this->m_ShowDebugTab) {
			if (ImGui::Begin("Pointer Debug")) {
				ImGui::Text("Pointer Debug");
				ImGui::End();
			}
		}
	}

	void RenderDebugGameTab() {
		if (this->m_ShowGameTab) {
			if (ImGui::Begin("Game Debug")) {
				ImGui::Text("Game Debug");
				ImGui::End();
			}
		}
	}

private:
	bool m_ShowDebugTab = false;
	bool m_ShowGameTab = false;
	bool m_ShowConsole = false;
};