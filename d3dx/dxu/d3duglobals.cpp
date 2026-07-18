#include "d3dx.hpp"

CD3duGlobals::CD3duGlobals()
{
  CD3duAlloc::CD3duAlloc();
  this->m_pHelInfo = 0;
  this->m_pDDrawDeviceList = 0;
  this->m_pD3duDeviceList = 0;
  this->m_pD3duContextList = 0;
  this->m_pCapsFile = 0;
  this->m_bInitialized = 0;
  // return this;
}

CD3duGlobals::~CD3duGlobals()
{
  CD3duContextList *m_pD3duContextList; // edi
  CHelInfo *m_pHelInfo; // edi
  CDDrawDeviceList *m_pDDrawDeviceList; // edi
  CD3duDeviceList *m_pD3duDeviceList; // edi
  CCapsFile *m_pCapsFile; // esi

#if SKIP_ME != 1
  m_pD3duContextList = this->m_pD3duContextList;
  if ( m_pD3duContextList )
  {
    CD3duContextList::~CD3duContextList(this->m_pD3duContextList);
    CD3duAlloc::operator delete(m_pD3duContextList);
  }
  m_pHelInfo = this->m_pHelInfo;
  if ( m_pHelInfo )
  {
    CHelInfo::~CHelInfo(this->m_pHelInfo);
    CD3duAlloc::operator delete(m_pHelInfo);
  }
  m_pDDrawDeviceList = this->m_pDDrawDeviceList;
  if ( m_pDDrawDeviceList )
  {
    CDDrawDeviceList::~CDDrawDeviceList(this->m_pDDrawDeviceList);
    CD3duAlloc::operator delete(m_pDDrawDeviceList);
  }
  m_pD3duDeviceList = this->m_pD3duDeviceList;
  if ( m_pD3duDeviceList )
  {
    CD3duDeviceList::~CD3duDeviceList(this->m_pD3duDeviceList);
    CD3duAlloc::operator delete(m_pD3duDeviceList);
  }
  m_pCapsFile = this->m_pCapsFile;
  if ( m_pCapsFile )
  {
    CCapsFile::~CCapsFile(m_pCapsFile);
    CD3duAlloc::operator delete(m_pCapsFile);
  }
#endif
}