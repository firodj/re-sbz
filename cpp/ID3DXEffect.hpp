typedef LPCSTR D3DXHANDLE;

typedef D3DMATRIX D3DXMATRIX;

struct D3DXEFFECT_DESC
{
    LPCSTR Creator;
    UINT Parameters;
    UINT Techniques;
    UINT Functions;
}

struct D3DXPASS_DESC
{
    LPCSTR Name;
    UINT Annotations;

    const DWORD *pVertexShaderFunction;
    const DWORD *pPixelShaderFunction;
};

enum D3DXPARAMETER_CLASS
{
    D3DXPC_SCALAR,
    D3DXPC_VECTOR,
    D3DXPC_MATRIX_ROWS,
    D3DXPC_MATRIX_COLUMNS,
    D3DXPC_OBJECT,
    D3DXPC_STRUCT,


    D3DXPC_FORCE_DWORD = 0x7fffffff
};

enum D3DXPARAMETER_TYPE
{
    D3DXPT_VOID,
    D3DXPT_BOOL,
    D3DXPT_INT,
    D3DXPT_FLOAT,
    D3DXPT_STRING,
    D3DXPT_TEXTURE,
    D3DXPT_TEXTURE1D,
    D3DXPT_TEXTURE2D,
    D3DXPT_TEXTURE3D,
    D3DXPT_TEXTURECUBE,
    D3DXPT_SAMPLER,
    D3DXPT_SAMPLER1D,
    D3DXPT_SAMPLER2D,
    D3DXPT_SAMPLER3D,
    D3DXPT_SAMPLERCUBE,
    D3DXPT_PIXELSHADER,
    D3DXPT_VERTEXSHADER,
    D3DXPT_PIXELFRAGMENT,
    D3DXPT_VERTEXFRAGMENT,
    D3DXPT_UNSUPPORTED,


    D3DXPT_FORCE_DWORD = 0x7fffffff

};

struct D3DXPARAMETER_DESC
{
    LPCSTR Name;
    LPCSTR Semantic;
    D3DXPARAMETER_CLASS Class;
    D3DXPARAMETER_TYPE Type;
    UINT Rows;
    UINT Columns;
    UINT Elements;
    UINT Annotations;
    UINT StructMembers;
    DWORD Flags;
    UINT Bytes;
};

struct D3DXTECHNIQUE_DESC
{
    LPCSTR Name;
    UINT Passes;
    UINT Annotations;

};

struct D3DXFUNCTION_DESC
{
    LPCSTR Name;
    UINT Annotations;

};

struct ID3DXEffect {
    struct ID3DXEffectVtbl * lpVtbl;
}; 
typedef struct ID3DXEffect *LPD3DXEFFECT;

//typedef struct IDirect3DBaseTexture9 *LPDIRECT3DBASETEXTURE9;
//typedef struct IDirect3DPixelShader9 *LPDIRECT3DPIXELSHADER9;

typedef struct ID3DXEffectPool *LPD3DXEFFECTPOOL;
typedef struct ID3DXEffectStateManager *LPD3DXEFFECTSTATEMANAGER;


struct ID3DXEffectVtbl
{

    HRESULT (__stdcall * QueryInterface)(ID3DXEffect * This, const IID * const iid, LPVOID *ppv) ;
    ULONG (__stdcall * AddRef)(ID3DXEffect * This) ;
    ULONG (__stdcall * Release)(ID3DXEffect * This) ;

    HRESULT (__stdcall * GetDesc)(ID3DXEffect * This, D3DXEFFECT_DESC* pDesc) ;
    HRESULT (__stdcall * GetParameterDesc)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXPARAMETER_DESC* pDesc) ;
    HRESULT (__stdcall * GetTechniqueDesc)(ID3DXEffect * This, D3DXHANDLE hTechnique, D3DXTECHNIQUE_DESC* pDesc) ;
    HRESULT (__stdcall * GetPassDesc)(ID3DXEffect * This, D3DXHANDLE hPass, D3DXPASS_DESC* pDesc) ;
    HRESULT (__stdcall * GetFunctionDesc)(ID3DXEffect * This, D3DXHANDLE hShader, D3DXFUNCTION_DESC* pDesc) ;

    D3DXHANDLE (__stdcall * GetParameter)(ID3DXEffect * This, D3DXHANDLE hParameter, UINT Index) ;
    D3DXHANDLE (__stdcall * GetParameterByName)(ID3DXEffect * This, D3DXHANDLE hParameter, LPCSTR pName) ;
    D3DXHANDLE (__stdcall * GetParameterBySemantic)(ID3DXEffect * This, D3DXHANDLE hParameter, LPCSTR pSemantic) ;
    D3DXHANDLE (__stdcall * GetParameterElement)(ID3DXEffect * This, D3DXHANDLE hParameter, UINT Index) ;
    D3DXHANDLE (__stdcall * GetTechnique)(ID3DXEffect * This, UINT Index) ;
    D3DXHANDLE (__stdcall * GetTechniqueByName)(ID3DXEffect * This, LPCSTR pName) ;
    D3DXHANDLE (__stdcall * GetPass)(ID3DXEffect * This, D3DXHANDLE hTechnique, UINT Index) ;
    D3DXHANDLE (__stdcall * GetPassByName)(ID3DXEffect * This, D3DXHANDLE hTechnique, LPCSTR pName) ;
    D3DXHANDLE (__stdcall * GetFunction)(ID3DXEffect * This, UINT Index) ;
    D3DXHANDLE (__stdcall * GetFunctionByName)(ID3DXEffect * This, LPCSTR pName) ;
    D3DXHANDLE (__stdcall * GetAnnotation)(ID3DXEffect * This, D3DXHANDLE hObject, UINT Index) ;
    D3DXHANDLE (__stdcall * GetAnnotationByName)(ID3DXEffect * This, D3DXHANDLE hObject, LPCSTR pName) ;

    HRESULT (__stdcall * SetValue)(ID3DXEffect * This, D3DXHANDLE hParameter, LPCVOID pData, UINT Bytes) ;
    HRESULT (__stdcall * GetValue)(ID3DXEffect * This, D3DXHANDLE hParameter, LPVOID pData, UINT Bytes) ;
    HRESULT (__stdcall * SetBool)(ID3DXEffect * This, D3DXHANDLE hParameter, BOOL b) ;
    HRESULT (__stdcall * GetBool)(ID3DXEffect * This, D3DXHANDLE hParameter, BOOL* pb) ;
    HRESULT (__stdcall * SetBoolArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const BOOL* pb, UINT Count) ;
    HRESULT (__stdcall * GetBoolArray)(ID3DXEffect * This, D3DXHANDLE hParameter, BOOL* pb, UINT Count) ;
    HRESULT (__stdcall * SetInt)(ID3DXEffect * This, D3DXHANDLE hParameter, INT n) ;
    HRESULT (__stdcall * GetInt)(ID3DXEffect * This, D3DXHANDLE hParameter, INT* pn) ;
    HRESULT (__stdcall * SetIntArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const INT* pn, UINT Count) ;
    HRESULT (__stdcall * GetIntArray)(ID3DXEffect * This, D3DXHANDLE hParameter, INT* pn, UINT Count) ;
    HRESULT (__stdcall * SetFloat)(ID3DXEffect * This, D3DXHANDLE hParameter, FLOAT f) ;
    HRESULT (__stdcall * GetFloat)(ID3DXEffect * This, D3DXHANDLE hParameter, FLOAT* pf) ;
    HRESULT (__stdcall * SetFloatArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const FLOAT* pf, UINT Count) ;
    HRESULT (__stdcall * GetFloatArray)(ID3DXEffect * This, D3DXHANDLE hParameter, FLOAT* pf, UINT Count) ;
    HRESULT (__stdcall * SetVector)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXVECTOR4* pVector) ;
    HRESULT (__stdcall * GetVector)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXVECTOR4* pVector) ;
    HRESULT (__stdcall * SetVectorArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXVECTOR4* pVector, UINT Count) ;
    HRESULT (__stdcall * GetVectorArray)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXVECTOR4* pVector, UINT Count) ;
    HRESULT (__stdcall * SetMatrix)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXMATRIX* pMatrix) ;
    HRESULT (__stdcall * GetMatrix)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXMATRIX* pMatrix) ;
    HRESULT (__stdcall * SetMatrixArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXMATRIX* pMatrix, UINT Count) ;
    HRESULT (__stdcall * GetMatrixArray)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXMATRIX* pMatrix, UINT Count) ;
    HRESULT (__stdcall * SetMatrixPointerArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXMATRIX** ppMatrix, UINT Count) ;
    HRESULT (__stdcall * GetMatrixPointerArray)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXMATRIX** ppMatrix, UINT Count) ;
    HRESULT (__stdcall * SetMatrixTranspose)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXMATRIX* pMatrix) ;
    HRESULT (__stdcall * GetMatrixTranspose)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXMATRIX* pMatrix) ;
    HRESULT (__stdcall * SetMatrixTransposeArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXMATRIX* pMatrix, UINT Count) ;
    HRESULT (__stdcall * GetMatrixTransposeArray)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXMATRIX* pMatrix, UINT Count) ;
    HRESULT (__stdcall * SetMatrixTransposePointerArray)(ID3DXEffect * This, D3DXHANDLE hParameter, const D3DXMATRIX** ppMatrix, UINT Count) ;
    HRESULT (__stdcall * GetMatrixTransposePointerArray)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXMATRIX** ppMatrix, UINT Count) ;
    HRESULT (__stdcall * SetString)(ID3DXEffect * This, D3DXHANDLE hParameter, LPCSTR pString) ;
    HRESULT (__stdcall * GetString)(ID3DXEffect * This, D3DXHANDLE hParameter, LPCSTR* ppString) ;
    HRESULT (__stdcall * SetTexture)(ID3DXEffect * This, D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture) ;
    HRESULT (__stdcall * GetTexture)(ID3DXEffect * This, D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 *ppTexture) ;
    HRESULT (__stdcall * GetPixelShader)(ID3DXEffect * This, D3DXHANDLE hParameter, LPDIRECT3DPIXELSHADER9 *ppPShader) ;
    HRESULT (__stdcall * GetVertexShader)(ID3DXEffect * This, D3DXHANDLE hParameter, LPDIRECT3DVERTEXSHADER9 *ppVShader) ;

	HRESULT (__stdcall * SetArrayRange)(ID3DXEffect * This, D3DXHANDLE hParameter, UINT uStart, UINT uEnd) ;

    HRESULT (__stdcall * GetPool)(ID3DXEffect * This, LPD3DXEFFECTPOOL* ppPool) ;

    HRESULT (__stdcall * SetTechnique)(ID3DXEffect * This, D3DXHANDLE hTechnique) ;
    D3DXHANDLE (__stdcall * GetCurrentTechnique)(ID3DXEffect * This) ;
    HRESULT (__stdcall * ValidateTechnique)(ID3DXEffect * This, D3DXHANDLE hTechnique) ;
    HRESULT (__stdcall * FindNextValidTechnique)(ID3DXEffect * This, D3DXHANDLE hTechnique, D3DXHANDLE *pTechnique) ;
    BOOL (__stdcall * IsParameterUsed)(ID3DXEffect * This, D3DXHANDLE hParameter, D3DXHANDLE hTechnique) ;

    HRESULT (__stdcall * Begin)(ID3DXEffect * This, UINT *pPasses, DWORD Flags) ;
    HRESULT (__stdcall * BeginPass)(ID3DXEffect * This, UINT Pass) ;
    HRESULT (__stdcall * CommitChanges)(ID3DXEffect * This) ;
    HRESULT (__stdcall * EndPass)(ID3DXEffect * This) ;
    HRESULT (__stdcall * End)(ID3DXEffect * This) ;

    HRESULT (__stdcall * GetDevice)(ID3DXEffect * This, LPDIRECT3DDEVICE9* ppDevice) ;
    HRESULT (__stdcall * OnLostDevice)(ID3DXEffect * This) ;
    HRESULT (__stdcall * OnResetDevice)(ID3DXEffect * This) ;

    HRESULT (__stdcall * SetStateManager)(ID3DXEffect * This, LPD3DXEFFECTSTATEMANAGER pManager) ;
    HRESULT (__stdcall * GetStateManager)(ID3DXEffect * This, LPD3DXEFFECTSTATEMANAGER *ppManager) ;

    HRESULT (__stdcall * BeginParameterBlock)(ID3DXEffect * This) ;
    D3DXHANDLE (__stdcall * EndParameterBlock)(ID3DXEffect * This) ;
    HRESULT (__stdcall * ApplyParameterBlock)(ID3DXEffect * This, D3DXHANDLE hParameterBlock) ;
    HRESULT (__stdcall * DeleteParameterBlock)(ID3DXEffect * This, D3DXHANDLE hParameterBlock) ;

    HRESULT (__stdcall * CloneEffect)(ID3DXEffect * This, LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT* ppEffect) ;
    HRESULT (__stdcall * SetRawValue)(ID3DXEffect * This, D3DXHANDLE hParameter, LPCVOID pData, UINT ByteOffset, UINT Bytes) ;
};
