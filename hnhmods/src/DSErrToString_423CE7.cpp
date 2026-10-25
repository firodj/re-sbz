#include "dx7.hpp"
#include "console.hpp"

const char * (__cdecl*DSErrToString_423CE7)(DWORD hr) = reinterpret_cast<const char *(__cdecl*)(DWORD hr)>(0x423CE7);

const char *__cdecl hook_DSErrToString_423CE7(DWORD hr)
{
    DebugLog("[HNH:SUB] DSErrToString_423CE7\n");

    if ( hr > 0x88780046 )
    {
        if ( hr > 0x88780096 )
        {
            switch ( hr )
            {
            case 0x887800A0:
                return "DirectSound: この値は過去のものであり使用されない";
            case 0x887800AA:
                return "DirectSound: 他のメソッドが呼び出される前にInitializeメソッドが呼び出されなかったかあるいは呼び出しが成功しなかった";
            case 0u:
                return 0;
            }
        }
        else
        {
            switch ( hr )
            {
            case 0x88780096:
                return "DirectSound: バッファメモリが失われたためリストアしなければならない";
            case 0x88780064:
                return "DirectSound: 指定されたWAVEはサポートされない";
            case 0x88780078:
                return "DirectSound: 使用するサウンドドライバが利用できない";
            case 0x88780082:
                return "DirectSound: オブジェクトはすでに初期化されている";
            }
        }
    }
    else
    {
        if ( hr == 0x88780046 )
            return "DirectSound: 呼び出しは関数が成功するために必要とする優先レベルを持っていない";
        if ( hr > 0x80070057 )
        {
            switch ( hr )
            {
            case 0x8878000A:
                return "DirectSound: 要求は優先レベルのようなリソースが他の呼び出しによってすでに使用中であるため失敗した";
            case 0x8878001E:
                return "DirectSound: 呼び出しによって要求する制御（ボリューム、パンなど）は利用可能でない";
            case 0x88780032:
                return "DirectSound: この関数はこのオブジェクトの現在の状態において有効ではない";
            }
        }
        else
        {
            if ( hr == 0x80070057 )
                return "DirectSound: 無効なパラメータが関数に渡された";
            if ( hr > 0x80040110 )
            {
                if ( hr == 0x8007000E )
                    return "DirectSound: DirectSoundサブシステムは呼び出しの要求を完了するために十分なメモリを割り当てることができなかった";
            }
            else
            {
                switch ( hr )
                {
                case 0x80040110:
                    return "DirectSound: オブジェクトはCOM集成化をサポートしない";
                case 0x80004001:
                    return "DirectSound: 呼び出した関数はここではサポートされない";
                case 0x80004005:
                    return "DirectSound: 未確定なエラーがDirectSoundサブシステムの中で起こった";
                }
            }
        }
    }
    return "DirectSound: 不明なエラー";
}

void register_DSErrToString_423CE7() {
  hnh_Hook(0x423CE7, hook_DSErrToString_423CE7, "hook_DSErrToString_423CE7");
}