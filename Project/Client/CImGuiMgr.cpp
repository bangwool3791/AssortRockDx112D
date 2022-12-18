#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine\CDevice.h>

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "UI.h"
#include "ParamUI.h"

CImGuiMgr::CImGuiMgr()
{
}

CImGuiMgr::~CImGuiMgr()
{
    Safe_Del_Map(m_mapUI);

    clear();
}


void CImGuiMgr::init(HWND _hWnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    // UI »ý¼º
    CreateUI();
}

void CImGuiMgr::progress()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ParamUI::ParamCount = 0;

    {
        bool bTrue = true;
        ImGui::ShowDemoWindow(&bTrue);

        
        map<string, UI*>::iterator iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->update();
        }       

        iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->render();
        }
    }


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImGuiMgr::clear()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


#include "InspectorUI.h"
#include "ListUI.h"
#include "TileMapUI.h"
#include "ProgressUI.h"
#include "OutlinerUI.h"
#include "ContentUI.h"
#include "DummyUI.h"
#include "ModelUI.h"
#include "ModelComUI.h"
#include "ComInspector.h"
#include "MenuUI.h"

#include "CEditor.h"

void CImGuiMgr::CreateUI()
{
    UI* pUI = new InspectorUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new TileMapUI;
    pUI->begin();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));
    ((TileMapUI*)pUI)->Initialize(CEditor::GetInst()->FindByName(L"MapTool"));

    pUI = new OutlinerUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ContentUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new DummyUI;
    pUI->begin();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ModelUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ListUI;    
    pUI->SetModal(true);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ModelComUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ComInspector;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new MenuUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    ProgressUI* progressui = new ProgressUI;
    progressui->SetModal(true);
    progressui->Close();
    m_mapUI.insert(make_pair(progressui->GetName(), progressui));
}

UI* CImGuiMgr::FindUI(const string& _name)
{
    map<string, UI*>::iterator iter =  m_mapUI.find(_name);
    if (iter == m_mapUI.end())
    {
        return nullptr;
    }

    return iter->second;
    
}