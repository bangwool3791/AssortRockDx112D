#include "pch.h"
#include "Animator2DUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CMeshRender.h>
#include <Engine\CAnimator2D.h>

#include "CEditor.h"

#include "CImGuiMgr.h"
#include "ListUI.h"

bool SelectionRect(ImVec2* start_pos, ImVec2* end_pos, ImGuiMouseButton mouse_button = ImGuiMouseButton_Left)
{
    //IM_ASSERT(start_pos != NULL);
    //IM_ASSERT(end_pos != NULL);
    if (ImGui::IsMouseClicked(mouse_button))
    {
        *start_pos = ImGui::GetMousePos();
    }
    if (ImGui::IsMouseDown(mouse_button))
    {
        *end_pos = ImGui::GetMousePos();
        ImDrawList* draw_list = ImGui::GetForegroundDrawList(); //ImGui::GetWindowDrawList();
        draw_list->AddRect(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 255)));   // Border
        draw_list->AddRectFilled(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 50)));    // Background
    }
    return ImGui::IsMouseReleased(mouse_button);
}

Animator2DUI::Animator2DUI()
    : ComponentUI("Animator2D", COMPONENT_TYPE::ANIMATOR2D)
    , m_strPause("Stop")
    , m_tInfo{}
    , m_iIndex{}
{
}

Animator2DUI::~Animator2DUI()
{
}
#include <string.h>
#include "TreeUI.h"

void Animator2DUI::begin()
{
    const map<wstring, Ptr<CRes>>& mapResource = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);

    for (auto iter{ mapResource.begin() }; iter != mapResource.end(); ++iter)
    {
        string str = string(iter->first.begin(), iter->first.end());
        m_items.push_back(str);
    }
    m_pAnimator = (CAnimator2D*)CEditor::GetInst()->GetArrComponent(COMPONENT_TYPE::ANIMATOR2D);
}

void Animator2DUI::update()
{
	ComponentUI::update();
}

void Animator2DUI::render_update()
{
    ComponentUI::render_update();

    static ImGuiComboFlags flags = 0;

    static int item_current_idx = 0; // Here we store our selection data as an index.
    const char* combo_preview_value = m_items[item_current_idx].c_str();  // Pass in the preview value visible before opening the combo (it could be anything)

    if (ImGui::BeginCombo("Texture", combo_preview_value, flags))
    {
        for (int n = 0; n < m_items.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(m_items[n].c_str(), is_selected))
                item_current_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##ModelTree"))
        {
            TreeNode* pNode = (TreeNode*)payload->Data;
            CComponent* pCompoent = (CComponent*)pNode->GetData();
            if (dynamic_cast<CAnimator2D*>(pCompoent))
            {
                m_pAnimator = dynamic_cast<CAnimator2D*>(pCompoent);
            }
        }
        ImGui::EndDragDropTarget();
    }

    wstring wstr = wstring(m_items[item_current_idx].begin(), m_items[item_current_idx].end());
    ImTextureID myImage = CResMgr::GetInst()->FindRes<CTexture>(wstr)->GetSRV().Get();

    if (ImGui::ImageButton(myImage, ImVec2(500.f, 500.f)))
    {
        ImVec2 screen_pos = ImGui::GetCursorScreenPos();
        cout << "ImGui Mouse [x] [y] " << screen_pos.x << " " << screen_pos.y << endl;
    }

    ImGui::Text("Index  "); ImGui::SameLine(); ImGui::InputInt("##Index", &m_iIndex);

    if (ImGui::Button("Load", ImVec2(200, 100)))
    {
        if (nullptr == m_pAnimator)
        {
            //메시지 박스 처리
        }
        m_vecAniInfo.clear();

        m_vecAniInfo = m_pAnimator->GetFames();

        if (m_iIndex < 0)
            m_iIndex = 0;

        if (m_iIndex >= m_vecAniInfo.size())
            m_iIndex = m_vecAniInfo.size() - 1;
    }

    if (m_iIndex < 0)
        m_iIndex = 0;

    if (m_iIndex != 0 && m_iIndex >= m_vecAniInfo.size())
        m_iIndex = m_vecAniInfo.size() - 1;

    if (!m_vecAniInfo.empty())
    {
        ImGui::Text("vLeftTop  "); ImGui::SameLine(); ImGui::InputFloat2("##vLeftTop", m_vecAniInfo[m_iIndex].vLeftTop);
        ImGui::Text("vOffset   "); ImGui::SameLine(); ImGui::InputFloat2("##vOffset", m_vecAniInfo[m_iIndex].vOffset);
        ImGui::Text("fDuration "); ImGui::SameLine(); ImGui::InputFloat("##fDuration", (float*)&(m_vecAniInfo[m_iIndex].fDuration));
        ImGui::Text("vSlice    "); ImGui::SameLine(); ImGui::InputFloat2("##vSlice", m_vecAniInfo[m_iIndex].vSlice);
        ImGui::Text("vFullSize "); ImGui::SameLine(); ImGui::InputFloat2("##vFullSize", m_vecAniInfo[m_iIndex].vFullSize);
    }
    //if (m_pAnimator)
    //{
    //    tInfo = m_pAnimator->GetAniFrameCopy();

    //    static ImVec2 v1{ 0,0 };
    //    static ImVec2 v2{ 0.5f,0.5f };

    //    v1.x = tInfo.vLeftTop.x;
    //    v1.y = tInfo.vLeftTop.y;

    //    v2.x = tInfo.vSlice.x + v1.x;
    //    v2.y = tInfo.vSlice.y + v1.y;

    //    ImGui::Image(myImage, ImVec2(500.f, 500.f), v1, v2);

    //    if (ImGui::Button(m_strPause.c_str(), ImVec2(100.f, 50.f)))
    //    {
    //        if ("Stop" == m_strPause)
    //        {
    //            m_strPause = "Play";
    //            m_pAnimator->Deactivate();
    //        }
    //        else if ("Play" == m_strPause)
    //        {
    //            m_strPause = "Stop";
    //            m_pAnimator->Activate();
    //        }
    //    }

    //    if ("Play" == m_strPause)
    //    {
    //        static bool bFlag = false;

    //        const vector<tAnim2DFrm>& Frames = m_pAnimator->GetFames();

    //        ImGui::NewLine();

    //        ImGui::Text("Index  "); ImGui::SameLine(); ImGui::InputInt("##Index", &m_iIndex);

    //        if (m_iIndex >= Frames.size())
    //        {
    //            return;
    //        }


    //        if (!bFlag)
    //        {
    //            m_tInfo = Frames[m_iIndex];
    //            bFlag = true;
    //        }
    //    }
    //}

    //ImGui::NewLine();

    //ImGui::Text("vLeftTop  "); ImGui::SameLine(); ImGui::InputFloat2("##vLeftTop1", m_tInfo.vLeftTop, "%.1f");
    //ImGui::NewLine();
    //ImGui::Text("vOffset   "); ImGui::SameLine(); ImGui::InputFloat2("##vOffset1", m_tInfo.vOffset);
    //ImGui::NewLine();
    //ImGui::Text("fDuration "); ImGui::SameLine(); ImGui::InputFloat("##fDuration1", (float*)&(m_tInfo.fDuration));
    //ImGui::NewLine();
    //ImGui::Text("vSlice    "); ImGui::SameLine(); ImGui::InputFloat2("##vSlice1", m_tInfo.vSlice);
    //ImGui::NewLine();
    //ImGui::Text("vFullSize "); ImGui::SameLine(); ImGui::InputFloat2("##vFullSize1", m_tInfo.vFullSize);

    //if (m_pAnimator)
    //{
    //    m_pAnimator->SetLeftTop(m_tInfo.vLeftTop, m_iIndex);
    //    m_pAnimator->SetSlice(m_tInfo.vSlice, m_iIndex);
    //    m_pAnimator->SetOffset(m_tInfo.vOffset, m_iIndex);
    //    m_pAnimator->SetFullSize(m_tInfo.vFullSize, m_iIndex);
    //    m_pAnimator->SetDuration(m_tInfo.fDuration, m_iIndex);
    //}
    SelectionRect(&m_vMouseStart, &m_vMouseEnd);
}

