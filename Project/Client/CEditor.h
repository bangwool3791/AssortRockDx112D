#pragma once

class CGameObjectEx;
class MemPool;

class CEditor
	: public CSingleton<CEditor>
{
private:
	vector<CGameObjectEx*>							m_vecEditorObj;
	list<tDebugShapeInfo>	m_DebugDrawList;
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
	virtual ~CEditor();
};