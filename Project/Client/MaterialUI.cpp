#include "pch.h"
#include "MaterialUI.h"

MaterialUI::MaterialUI()
	: ResUI("Material##UI", RES_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::update()
{


	ResUI::update();
}

void MaterialUI::render_update()
{
	ResUI::render_update();


}
