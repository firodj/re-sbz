typedef struct ID3DXFont *LPD3DXFONT;

struct ID3DXFont {
    ID3DXFontVtbl * lpVtbl;
}

struct D3DXFONT_DESCA
{
    INT Height;
    UINT Width;
    UINT Weight;
    UINT MipLevels;
    BOOL Italic;
    BYTE CharSet;
    BYTE OutputPrecision;
    BYTE Quality;
    BYTE PitchAndFamily;
    CHAR FaceName[32];

}

struct D3DXFONT_DESCW
{
    INT Height;
    UINT Width;
    UINT Weight;
    UINT MipLevels;
    BOOL Italic;
    BYTE CharSet;
    BYTE OutputPrecision;
    BYTE Quality;
    BYTE PitchAndFamily;
    WCHAR FaceName[32];
}

typedef struct ID3DXSprite *LPD3DXSPRITE;

struct ID3DXFontVtbl
{

    HRESULT (__stdcall * QueryInterface)(ID3DXFont * This, const IID * const iid, LPVOID *ppv) ;
    ULONG (__stdcall * AddRef)(ID3DXFont * This) ;
    ULONG (__stdcall * Release)(ID3DXFont * This) ;


    HRESULT (__stdcall * GetDevice)(ID3DXFont * This, LPDIRECT3DDEVICE9 *ppDevice) ;
    HRESULT (__stdcall * GetDescA)(ID3DXFont * This, D3DXFONT_DESCA *pDesc) ;
    HRESULT (__stdcall * GetDescW)(ID3DXFont * This, D3DXFONT_DESCW *pDesc) ;
    BOOL (__stdcall * GetTextMetricsA)(ID3DXFont * This, TEXTMETRICA *pTextMetrics) ;
    BOOL (__stdcall * GetTextMetricsW)(ID3DXFont * This, TEXTMETRICW *pTextMetrics) ;

    HDC (__stdcall * GetDC)(ID3DXFont * This) ;
    HRESULT (__stdcall * GetGlyphData)(ID3DXFont * This, UINT Glyph, LPDIRECT3DTEXTURE9 *ppTexture, RECT *pBlackBox, POINT *pCellInc) ;

    HRESULT (__stdcall * PreloadCharacters)(ID3DXFont * This, UINT First, UINT Last) ;
    HRESULT (__stdcall * PreloadGlyphs)(ID3DXFont * This, UINT First, UINT Last) ;
    HRESULT (__stdcall * PreloadTextA)(ID3DXFont * This, LPCSTR pString, INT Count) ;
    HRESULT (__stdcall * PreloadTextW)(ID3DXFont * This, LPCWSTR pString, INT Count) ;

    INT (__stdcall * DrawTextA)(ID3DXFont * This, LPD3DXSPRITE pSprite, LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color) ;
    INT (__stdcall * DrawTextW)(ID3DXFont * This, LPD3DXSPRITE pSprite, LPCWSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color) ;

    HRESULT (__stdcall * OnLostDevice)(ID3DXFont * This) ;
    HRESULT (__stdcall * OnResetDevice)(ID3DXFont * This) ;
};