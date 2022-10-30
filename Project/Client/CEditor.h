#pragma once

class CGameObjectEx;

class CEditor
	: public CSingletone<CEditor>
{
private:
	vector<CGameObjectEx*>							m_vecEditorObj;
	array<CGameObjectEx*, (UINT)DEBUG_SHAPE::END>	m_DebugDrawObject;

public:
	void init();
	void progress();

private:
	void tick();
	void render();
private:
	void CreateDebugDrawObject();
	void DebugDraw(tDebugShapeInfo& _info);

public:
	CEditor();
	~CEditor();
};