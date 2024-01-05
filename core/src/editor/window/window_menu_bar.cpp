
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/window.h"
#include "editor/global.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{

	void Window::DrawMenuBar(Context *context)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
				}
				if (ImGui::MenuItem("Save As.."))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z"))
				{
				}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
				{
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X"))
				{
				}
				if (ImGui::MenuItem("Copy", "CTRL+C"))
				{
				}
				if (ImGui::MenuItem("Paste", "CTRL+V"))
				{
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

}