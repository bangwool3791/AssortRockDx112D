#include "pch.h"
#include "Animator2DUI.h"

#include <Engine\CCamera.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CResMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CMeshRender.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CTransform.h>

#include "CEditor.h"

#include "CImGuiMgr.h"
#include "ListUI.h"

#include "CGameObjectEx.h"


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

/*
* Editor
* Rect Mesh Link
* * UI 클릭 -> 
* * Start Pos, End Pos
* Animator Link
* * Pixel Get
*/
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
#include <stack>

void Animator2DUI::begin()
{
    const map<wstring, Ptr<CRes>>& mapResource = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);

    for (auto iter{ mapResource.begin() }; iter != mapResource.end(); ++iter)
    {
        string str = string(iter->first.begin(), iter->first.end());
        m_items.push_back(str);
    }
    m_pAnimator = (CAnimator2D*)CEditor::GetInst()->GetArrComponent(COMPONENT_TYPE::ANIMATOR2D);
    m_pTexture = m_pAnimator->GetTexture();
    m_pixel_width = m_pTexture->GetWidth();
    m_pixel_height = m_pTexture->GetHeight();
    //const wstring& wstrName = m_pAnimator->GetTexture()->GetName();
    //const wstring& wstrName = L"C:\\Development\\MyEngine\\DirectX11\\OutputFile\\bin\\content\\texture\\Character.png";
    //Load_Image(m_vec_pixel, string(wstrName.begin(), wstrName.end()), m_pixel_width, m_pixel_height);
    m_MouseObject = CEditor::GetInst()->FindByName(L"MouseObject");
    m_CameraObject = CEditor::GetInst()->FindByName(L"Editor Camera");
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
            m_iIndex = (int)(m_vecAniInfo.size() - 1);
    }

    if (m_iIndex < 0)
        m_iIndex = 0;

    if (m_iIndex != 0 && m_iIndex >= m_vecAniInfo.size())
        m_iIndex = (int)(m_vecAniInfo.size() - 1);

    if (!m_vecAniInfo.empty())
    {
        ImGui::Text("vLeftTop  "); ImGui::SameLine(); ImGui::InputFloat2("##vLeftTop", m_vecAniInfo[m_iIndex].vLeftTop);
        ImGui::Text("vOffset   "); ImGui::SameLine(); ImGui::InputFloat2("##vOffset", m_vecAniInfo[m_iIndex].vOffset);
        ImGui::Text("fDuration "); ImGui::SameLine(); ImGui::InputFloat("##fDuration", (float*)&(m_vecAniInfo[m_iIndex].fDuration));
        ImGui::Text("vSlice    "); ImGui::SameLine(); ImGui::InputFloat2("##vSlice", m_vecAniInfo[m_iIndex].vSlice);
        ImGui::Text("vFullSize "); ImGui::SameLine(); ImGui::InputFloat2("##vFullSize", m_vecAniInfo[m_iIndex].vFullSize);
    }

    if (KEY_PRESSED(KEY::LBTN))
    {
        Vec3 vCamPos = m_CameraObject->Camera()->Transform()->GetRelativePos();
        float fScale = m_CameraObject->Camera()->GetOrthographicScale();
        m_pixel = m_pTexture->GetPixel();
        Vec3 vPos = m_MouseObject->Transform()->GetRelativePos();
        //vPos *= fScale;
        cout << "[X] : " << vPos.x << " [Y] : " << vPos.y << endl;
        vPos.x += m_pixel_width * 0.5f;
        vPos.y = -1.f * vPos.y + m_pixel_height * 0.5f;
        //vPos *= fScale;
        tRGBA tInfo = GetRGBA(vPos.x, vPos.y);
        cout << "X : " << vPos.x << " Y : " << vPos.y << endl;
        cout << "R : " << tInfo.R << " G : " << tInfo.G << " B : " << tInfo.B << " A : " << tInfo.A << endl;

        int dx[] = { -1, 0, 1, 0 };
        int dy[] = { 0, 1, 0, -1 };
        int group_id; // 단지의 번호로 첫번째 단지부터 1로 시작

        bool** visited = (bool** )malloc(sizeof(bool*) * m_pixel_width);
        for (int i{}; i < m_pixel_width; ++i)
        {
            *(visited + i) = (bool*)malloc(sizeof(bool) * m_pixel_height);
            memset(*(visited + i), 0, sizeof(bool) * m_pixel_height);
        }

        std::stack<std::pair<int, int>> s; // 이용할 스택, (x,y) -> (행, 열)
        int x = vPos.x;
        int y = vPos.y;

        int resultX = m_pixel_width;
        int resultY = m_pixel_height;
        s.push(make_pair(x, y)); // pair를 만들어서 stack에 넣습니다.

        // 처음 x,y를 방문 했기때문에
        visited[x][y] = true;

        while (!s.empty()) {

            // 스택의 top 원소 꺼내기
            x = s.top().first;
            y = s.top().second;
            s.pop();

            // 해당 위치의 주변을 확인
            for (int i = 0; i < 4; i++) 
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                // 지도를 벗어나지 않고
                if (0 <= nx && nx < m_pixel_width && 0 <= ny && ny < m_pixel_height) 
                {
                    // 집이면서 방문하지 않았다면 -> 방문
                    if (visited[nx][ny] == false) 
                    {
                        visited[nx][ny] = true;

                        //cout << "dfs 과정 좌표 [x] " << nx << " " << "[y] " << ny << endl;

                        if (nx < resultX && ny <= resultY)
                        {
                            int index = 4 * (nx + ny * m_pixel_width);
                            if (false == (m_pixel[index] == 255.f &&
                                m_pixel[index + 1] == 255.f &&
                                m_pixel[index + 2] == 255.f && m_pixel[index + 3] == 0))
                            {
                               // cout << "dfs 과정 좌표 [x] " << nx << " " << "[y] " << ny << endl;
                                resultX = nx;
                                resultY = ny;

                            }
                        }
                         s.push(make_pair(x, y)); // push하는 경우이기 때문에 현재 위치도 넣어준다.
                        s.push(make_pair(nx, ny)); // 스택에 현재 nx,ny도 푸시
                    }
                }
            }
        }
        cout << "dsf 결과 좌표 [x] " << resultX << " " << "[y]" << resultY << endl;
    }
}

void dfs_stack(int x, int y) {


}
extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include <Engine\stb_image.h>
}


bool Animator2DUI::Load_Image(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y)
{
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    if (data != nullptr)
    {
        image = std::vector<unsigned char>(data, data + x * y * 4);
    }
    stbi_image_free(data);
    return (data != nullptr);
}


tRGBA Animator2DUI::GetRGBA(int _x, int _y)
{

    tRGBA tInfo;

    static const size_t RGBA = 4;

    //_y = m_pixel_height - _y;
    size_t index = RGBA * (_y * m_pixel_width + _x);

    tInfo.R = m_pixel[index + 0];
    tInfo.G = m_pixel[index + 1];
    tInfo.B = m_pixel[index + 2];
    tInfo.A = m_pixel[index + 3];
    
    return tInfo;
}