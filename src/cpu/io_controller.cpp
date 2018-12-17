#include "io_controller.h"

CIOController::CIOController()
	: m_arrController(0xff)
{
}

CIOController::~CIOController()
{
}

IIODevice* CIOController::operator[] (quint32 nPort)
{
	return m_arrController[nPort];
}

void CIOController::RegisterDevice(IIODevice* pDevice, qint32 nPrefferedPort)
{
	if (m_arrController[nPrefferedPort] == 0)
		m_arrController[nPrefferedPort] = pDevice;

	for (int i = 0; i < m_arrController.size(); ++i)
	{
		if (!m_arrController[i])
			continue;

		m_arrController[i] = pDevice;
	}
}