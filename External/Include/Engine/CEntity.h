#pragma once

class CEntity
{
private:
	static	UINT g_ID;

private:
	wstring		m_strName;
	const UINT	m_ID;

public:
	UINT GetID() { return m_ID; }
	const wstring& GetName() { return m_strName; }

	void SetName(const wstring& _str) { m_strName = _str; }

public:
	CEntity();
	CEntity(const CEntity& _other);
	virtual ~CEntity();
};

