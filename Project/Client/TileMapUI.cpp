#include "pch.h"
#include "TileMapUI.h"

#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>

#include <Engine\CGameObject.h>
#include <Engine\CTileMap.h>

#include <Engine\CTexture.h>
#include <Engine\CResMgr.h>

#include <Engine\CScript.h>
#include <Engine\CTileScript.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"

TileMapUI::TileMapUI()
	: UI("TileMap")
{
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::update()
{
	UI::update();
}

void TileMapUI::render_update()
{
	//UI::render();
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button("Tile");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	auto pTileMap = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"TileMap");

	for (UINT i{}; i < 32; ++i)
	{
		wstring str = L"Tile";
		str += std::to_wstring(i);
		ImTextureID myImage = CResMgr::GetInst()->FindRes<CTexture>(str)->GetSRV().Get();

		if (ImGui::ImageButton(myImage, ImVec2(50.f, 50.f)))
		{
			pTileMap->GetScript<CTileScript>(L"CTileScript")->SetTileInfo(i);
		}

		if ((i % 16 != 0))
		{
			ImGui::SameLine();
		}
	}

}




