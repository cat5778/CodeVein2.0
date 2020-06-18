#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);


public:
	_ulong	Get_TriCnt(void) const { return m_dwTriCnt; }

public:
	void	SendToClient(void* pVertex);
	void	ReceiveToEngine(void* pVertex);

public:
	virtual HRESULT			Ready_Buffer(void);
	void					Render_Buffer(void);

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	_ulong							m_dwVtxSize;
	_ulong							m_dwVtxCnt;
	_ulong							m_dwVtxFVF;
	_ulong							m_dwTriCnt;

	_ulong							m_dwIdxSize;
	D3DFORMAT						m_IdxFmt;
	
public:
	virtual void Free(void);

};


END
#endif // VIBuffer_h__
