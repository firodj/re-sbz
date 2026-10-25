#include "dx7.hpp"
#include "console.hpp"

// === GLOBAL FUNCTIONS ===

const char * (__cdecl*DSErrToString_423CE7)(HRESULT hr) = reinterpret_cast<const char *(__cdecl*)(HRESULT hr)>(0x423CE7);
const char* (__cdecl* DDErrToString_41898E)(HRESULT hr) = reinterpret_cast<const char* (__cdecl*)(HRESULT hr)>(0x41898E);

// === FUNCTION HOOKS ===

const char *__cdecl hook_DSErrToString_423CE7(HRESULT hr)
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

const char *__cdecl hook_DDErrToString_41898E(int hr)
{
  const char *result; // eax

  if ( !hr )
    return 0;

  DebugLog("[HNH:SUB] DDErrToString_41898E\n");

  if ( hr > 0x887601A4 )
  {
    switch ( hr )
    {
    case DDERR_SURFACEBUSY:
      result = "DirectDraw: サーフェスがもうひとつのスレッドによってロックされている為、アクセスが拒否された";
      break;
    case DDERR_CANTLOCKSURFACE:
      result = "DirectDraw: DCIサポートなしでプライマリサーフェスのロックをしようとした為、このサーフェスへのアクセスが拒否された";
      break;
    case DDERR_SURFACEISOBSCURED:
      result = "DirectDraw: サーフェスへのアクセスが、サーフェスが隠蔽されている為、拒否された";
      break;
    case DDERR_SURFACELOST:
      result = "DirectDraw: サーフェスメモリの不足の為、サーフェスへのアクセスが拒否された。このサーフェスを表わすDirectDrawSurfaceオブジェクトには、IDirectDrawSurface3::R"
        "estoreメソッドが呼び出されなければならない";
      break;
    case DDERR_SURFACENOTATTACHED:
      result = "DirectDraw: 要求されたサーフェスはアタッチ出来ない";
      break;
    case DDERR_TOOBIGHEIGHT:
      result = "DirectDraw: DirectDrawによって要求された高さが大きすぎる";
      break;
    case DDERR_TOOBIGSIZE:
      result = "DirectDraw: DirectDrawによって要求された大きさが大きすぎる。しかし、個別の高さと幅は問題がない";
      break;
    case DDERR_TOOBIGWIDTH:
      result = "DirectDraw: DirectDrawによって要求された幅が大きすぎる";
      break;
    case DDERR_UNSUPPORTEDFORMAT:
      result = "DirectDraw: 要求されたFourCCフォーマットはDirectDrawによってサポートされていない";
      break;
    case DDERR_UNSUPPORTEDMASK:
      result = "DirectDraw: 要求されたピクセルフォーマットでのビットマスクは、DirectDrawによってサポートされていない";
      break;
    case DDERR_VERTICALBLANKINPROGRESS:
      result = "DirectDraw: 垂直帰線期間である";
      break;
    case DDERR_WASSTILLDRAWING:
      result = "DirectDraw: このサーフェスから、または、サーフェスへの前回のブロック転送操作が完了していない";
      break;
    case DDERR_XALIGN:
      result = "DirectDraw: 与えられた矩形を必要とされる範囲に水平１列に並べる事が出来なかった";
      break;
    case DDERR_INVALIDDIRECTDRAWGUID:
      result = "DirectDraw: DierctDrawCreate関数に渡されるグローバルユニーク識別子(GUID)は、有効なDirectDrawドライバ識別子ではない";
      break;
    case DDERR_DIRECTDRAWALREADYCREATED:
      result = "DirectDraw: DirectDrawオブジェクトは既に作成されている";
      break;
    case DDERR_NODIRECTDRAWHW:
      result = "DirectDraw: ハードウェア専用DirectDrawオブジェクトを作成出来ない。ドライバはハードウェアをサポートしない";
      break;
    case DDERR_PRIMARYSURFACEALREADYEXISTS:
      result = "DirectDraw: この処理は既にプライマリサーフェスを作成している";
      break;
    case DDERR_NOEMULATION:
      result = "DirectDraw: ソフトウェアエミュレーションが利用出来ない";
      break;
    case DDERR_REGIONTOOSMALL:
      result = "DirectDraw: IDirectDrawClipper::GetClipListメソッドに渡された領域が小さすぎる";
      break;
    case DDERR_CLIPPERISUSINGHWND:
      result = "DirectDraw: ウィンドウハンドルを既にモニタしているDirectDrawClipperオブジェクトにクリッピングリストをセットしようとした";
      break;
    case DDERR_NOCLIPPERATTACHED:
      result = "DirectDraw: このサーフェースオブジェクトには、DirectDrawClipperオブジェクトをアタッチ出来ない";
      break;
    case DDERR_NOHWND:
      result = "DirectDraw: クリッピング通知がウィンドウハンドルを必要とする、あるいは、ウィンドウハンドルが協調レベルウィンドウハンドルとして、あらかじめセットされていない";
      break;
    case DDERR_HWNDSUBCLASSED:
      result = "DirectDraw: DirectDraw協調レベルウィンドウハンドルがサブクラス化されている為、DirectDrawの状態のリストアが阻止された";
      break;
    case DDERR_HWNDALREADYSET:
      result = "DirectDraw: DirectDraw協調レベルウィンドウハンドルは既に設定されている。ウィンドウハンドルはプロセスがサーフェス、あるいは、生成したパレットを保持している間はリセット出来ない";
      break;
    case DDERR_NOPALETTEATTACHED:
      result = "DirectDraw: パレットオブジェクトがこのサーフェースにアタッチされていない";
      break;
    case DDERR_NOPALETTEHW:
      result = "DirectDraw: １６色、あるいは、２５６色のパレットに対するハードウェアサポートがない";
      break;
    case DDERR_BLTFASTCANTCLIP:
      result = "DirectDraw: DirectDrawClipperオブジェクトがIDirectDrawSurface3::BltFaseメソッドの呼び出しに渡される転送元サーフェースにアタッチされている";
      break;
    case DDERR_NOBLTHW:
      result = "DirectDraw: ブロック転送ハードウェアが存在しない";
      break;
    case DDERR_NODDROPSHW:
      result = "DirectDraw: DirectDrawラスタ処理(ROP)ハードウェアが利用出来ない";
      break;
    case DDERR_OVERLAYNOTVISIBLE:
      result = "DirectDraw: IDirectDrawSurface3::GetOverlayPositionメソッドが、不可視オーバーレイ上で呼び出された";
      break;
    case DDERR_NOOVERLAYDEST:
      result = "DirectDraw: 転送先確立の為のIDirectDrawSurface3::UpdateOverlayメソッド呼び出しをしていないオーバーレイに対し、IDirectDrawSurface3::GetO"
        "verlayPositionメソッドが呼び出された";
      break;
    case DDERR_INVALIDPOSITION:
      result = "DirectDraw: 転送先上のオーバーレイの位置が不正である";
      break;
    case DDERR_NOTAOVERLAYSURFACE:
      result = "DirectDraw: 非オーバーレイサーフェースに対し、オーバーレイコンポーネントが呼び出された";
      break;
    case DDERR_EXCLUSIVEMODEALREADYSET:
      result = "DirectDraw: 既に排他的モードがセットされているにも関わらず、協調レベルをセットしようとした";
      break;
    case DDERR_NOTFLIPPABLE:
      result = "DirectDraw:\tフリッピング出来ないサーフェースをフリッピングしようとした";
      break;
    case DDERR_CANTDUPLICATE:
      result = "DirectDraw: プライマリサーフェース、３Ｄサーフェース、暗黙的に作られるサーフェースは複製出来ない";
      break;
    case DDERR_NOTLOCKED:
      result = "DirectDraw:\tロックされていないサーフェイスのロック解除が行われた";
      break;
    case DDERR_CANTCREATEDC:
      result = "DirectDraw: Windowsはそれ以上のいかなるデバイスコンテキスト(DC)を作る事が出来ない";
      break;
    case DDERR_NODC:
      result = "DirectDraw: DCは、これまでにこのサーフェース用に作られていなかった";
      break;
    case DDERR_WRONGMODE:
      result = "DirectDraw: 異なったモードで作られた為、このサーフェスはリストアする事が出来ない";
      break;
    case DDERR_IMPLICITLYCREATED:
      result = "DirectDraw: 暗黙的に作られたサーフェースをリストアしようとした";
      break;
    case DDERR_NOTPALETTIZED:
      result = "DirectDraw: 使用しているサーフェースはパレットベースのサーフェースではない";
      break;
    case DDERR_UNSUPPORTEDMODE:
      result = "DirectDraw: ディスプレイは現在サポートされていないモードである";
      break;
    case DDERR_NOMIPMAPHW:
      result = "DirectDraw: ミップマップテクスチャマッピングのハードウェアが存在していないか、あるいは、利用出来ない為、処理を実行出来ない";
      break;
    case DDERR_INVALIDSURFACETYPE:
      result = "DirectDraw: サーフェースが間違ったタイプであった為、要求された処理を実行出来なかった";
      break;
    case DDERR_DCALREADYCREATED:
      result = "DirectDraw: デバイスコンテキストは既にこのサーフェースに対し返されている。１サーフェスにつき、ひとつのDCしか取得する事は出来ない";
      break;
    case DDERR_CANTPAGELOCK:
      result = "DirectDraw: サーフェスのページロックが失敗した。ページロックはディスプレイメモリサーフェス、あるいは、エミュレートされたプライマリサーフェスの上では動作しない";
      break;
    case DDERR_CANTPAGEUNLOCK:
      result = "DirectDraw: サーフェスのページロック解除が失敗した。ページロック解除はディスプレイメモリサーフェス、あるいは、エミュレートされたプライマリサーフェスの上では動作しない";
      break;
    case DDERR_NOTPAGELOCKED:
      result = "DirectDraw: 未処理のページロックがないサーフェースのページロック解除が行われた";
      break;
    default:
      return "DirectDraw: 不明なエラー";
    }
  }
  else
  {
    if ( hr == DDERR_SURFACEALREADYDEPENDENT )
      return "DirectDraw: 既に他のサーフェスに依存しているサーフェスを依存させようとした";
    if ( hr > 0x887600DC )
    {
      switch ( hr )
      {
      case DDERR_NODIRECTDRAWSUPPORT:
        result = "DirectDraw: 現在のディスプレイドライバはDirectDrawをサポートしていない";
        break;
      case DDERR_NOEXCLUSIVEMODE:
        result = "DirectDraw: アプリケーションに対し排他的モードが要求されたが、アプリケーションは排他的モードを持っていない";
        break;
      case DDERR_NOFLIPHW:
        result = "DirectDraw: 可視サーフェースはフリッピング出来ない";
        break;
      case DDERR_NOGDI:
        result = "DirectDraw: GDIが存在しない";
        break;
      case DDERR_NOMIRRORHW:
        result = "DirectDraw: ミラーリングハードウェアが存在しないか、あるいは、利用出来ない為、処理が実行出来ない";
        break;
      case DDERR_NOTFOUND:
        result = "DirectDraw: 要求された項目は見つからなかった";
        break;
      case DDERR_NOOVERLAYHW:
        result = "DirectDraw: オーバーレイハードウェアが存在していないか、あるいは、利用出来ない為、処理が実行出来ない";
        break;
      case DDERR_NORASTEROPHW:
        result = "DirectDraw: 適切なラスタ処理ハードウェアが存在しないか、あるいは、利用出来ない為、処理が実行出来ない";
        break;
      case DDERR_NOROTATIONHW:
        result = "DirectDraw: 回転ハードウェアが存在していないか、あるいは、利用出来ない為、処理が実行出来ない";
        break;
      case DDERR_NOSTRETCHHW:
        result = "DirectDraw: 拡大ハードウェアが存在していないか、あるいは、利用出来ない為、処理が実行出来ない";
        break;
      case DDERR_NOT4BITCOLOR:
        result = "DirectDraw: DirectDrawSurfaceオブジェクトは、要求された４ビット色パレットを使っていない";
        break;
      case DDERR_NOT4BITCOLORINDEX:
        result = "DirectDraw:\tDirectDrawSurfaceオブジェクトは、要求された４ビットのカラーインデックスパレットを使っていない";
        break;
      case DDERR_NOT8BITCOLOR:
        result = "DirectDraw: DirectDrawSurfaceオブジェクトは、要求された８ビット色パレットを使っていない";
        break;
      case DDERR_NOTEXTUREHW:
        result = "DirectDraw: テクスチャマッピングハードウェアが存在していないか、あるいは、利用出来ない為、処理が実行出来ない";
        break;
      case DDERR_NOVSYNCHW:
        result = "DirectDraw: 垂直帰線同期処理に対するハードウェアサポートがない為、処理を実行出来ない";
        break;
      case DDERR_NOZBUFFERHW:
        result = "DirectDraw: Ｚバッファに対するハードウェアサポートがない為、ディスプレイメモリでのＺバッファ作成、あるいは、Ｚバッファを利用したブロック転送が実行出来ない";
        break;
      case DDERR_NOZOVERLAYHW:
        result = "DirectDraw: オーバーレイサーフェスは、ハードウェアがオーバーレイＺオーダをサポートしない為、Ｚオーダが無視された";
        break;
      case DDERR_OUTOFCAPS:
        result = "DirectDraw: 要求された処理に必要なハードウェアが既に割り当てられている";
        break;
      case DDERR_OUTOFVIDEOMEMORY:
        result = "DirectDraw: DirectDrawが処理を行うのに充分なディスプレイメモリがない";
        break;
      case DDERR_OVERLAYCANTCLIP:
        result = "DirectDraw: ハードウェアはクリッピングオーバーレイをサポートしない";
        break;
      case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
        result = "DirectDraw: オーバーレイ上にひとつ以上の有効なカラーキーを持とうとした";
        break;
      case DDERR_PALETTEBUSY:
        result = "DirectDraw: パレットがもうひとつのスレッドによってロックされている為、アクセスが拒否された";
        break;
      case DDERR_COLORKEYNOTSET:
        result = "DirectDraw: 転送元カラーキーが指定されていない";
        break;
      case DDERR_SURFACEALREADYATTACHED:
        result = "DirectDraw: 既にアタッチしているもうひとつのサーフェスにサーフェスをアタッチしようとした";
        break;
      default:
        return "DirectDraw: 不明なエラー";
      }
    }
    else
    {
      if ( hr == DDERR_NOCOLORKEYHW )
        return "DirectDraw: 転送先カラーキーに対するハードウェアサポートがない為、処理が実行出来ない";
      if ( hr > 0x88760064 )
      {
        switch ( hr )
        {
        case DDERR_INVALIDCLIPLIST:
          result = "DirectDraw: DirectDrawは与えられたクリッピングリストをサポートしない";
          break;
        case DDERR_INVALIDMODE:
          result = "DirectDraw: DirectDrawは要求されたモードをサポートしない";
          break;
        case DDERR_INVALIDOBJECT:
          result = "DirectDraw: DirectDrawは無効なDirectDrawオブジェクトのポインタを受け取った";
          break;
        case DDERR_INVALIDPIXELFORMAT:
          result = "DirectDraw: 指定されたピクセルフォーマットは無効である";
          break;
        case DDERR_INVALIDRECT:
          result = "DirectDraw: 与えられた矩形が無効である";
          break;
        case DDERR_LOCKEDSURFACES:
          result = "DirectDraw: ひとつ以上のサーフェスがロックされており、要求された処理に失敗した";
          break;
        case DDERR_NO3D:
          result = "DirectDraw: ３Ｄハードウェアあるいはエミュレーションが存在しない";
          break;
        case DDERR_NOALPHAHW:
          result = "DirectDraw: アルファアクセラレーションハードウェアが存在しないか、あるいは、利用出来なかった為、要求された処理に失敗した";
          break;
        case DDERR_NOCLIPLIST:
          result = "DirectDraw: クリッピングリストが利用出来ない";
          break;
        case DDERR_NOCOLORCONVHW:
          result = "DirectDraw: 色変換ハードウェアが存在しない、あるいは、利用できない為、処理が実行出来ない";
          break;
        case DDERR_NOCOOPERATIVELEVELSET:
          result = "DirectDraw: IDirectDraw2::SetCooperativeLevelメソッドを呼び出す事なく作成関数が呼び出された";
          break;
        case DDERR_NOCOLORKEY:
          result = "DirectDraw: サーフェースは現在カラーキーを持っていない";
          break;
        default:
          return "DirectDraw: 不明なエラー";
        }
      }
      else
      {
        if ( hr == DDERR_INVALIDCAPS )
          return "DirectDraw: コールバック関数に渡されたひとつ以上の能力ビットが不正である";
        if ( hr > 0x8876000A )
        {
          switch ( hr )
          {
          case DDERR_CANNOTDETACHSURFACE:
            result = "DirectDraw: 要求されたサーフェースにはサーフェースをデタッチ出来ない";
            break;
          case DDERR_CURRENTLYNOTAVAIL:
            result = "DirectDraw: 現在サポートされていない";
            break;
          case DDERR_EXCEPTION:
            result = "DirectDraw: 要求された処理を行う間に例外が発生した";
            break;
          case DDERR_HEIGHTALIGN:
            result = "DirectDraw: 与えられた矩形の高さは必要とされる整数の倍数ではない";
            break;
          case DDERR_INCOMPATIBLEPRIMARY:
            result = "DirectDraw: プライマリサーフェース作成要求が既存のプライマリサーフェスと一致していない";
            break;
          default:
            return "DirectDraw: 不明なエラー";
          }
        }
        else
        {
          if ( hr == 0x8876000A )
            return "DirectDraw: 要求されたサーフェースにはサーフェースをアタッチ出来ない";
          if ( hr > 0x8007000E )
          {
            if ( hr == DDERR_INVALIDPARAMS )
              return "DirectDraw: メソッドに渡されたひとつ以上のパラメータが正しくない";
            if ( hr == DDERR_ALREADYINITIALIZED )
              return "DirectDraw: オブジェクトは既に初期化されている";
          }
          else
          {
            switch ( hr )
            {
            case DDERR_OUTOFMEMORY:
              return "DirectDraw: DirectDrawが処理を行うのに充分なメモリがない";
            case DDERR_UNSUPPORTED:
              return "DirectDraw: サポートされていない処理であった";
            case DDERR_GENERIC:
              return "DirectDraw: 未定義のエラー状態である";
            case DDERR_NOTINITIALIZED:
              return "DirectDraw: オブジェクトが初期化される前に、CoCreateInstanceで作成されたDirectDrawオブジェクトのインターフェイスメソッドを呼び出そうとした";
            }
          }
          return "DirectDraw: 不明なエラー";
        }
      }
    }
  }
  return result;
}

void register_DSErrToString_423CE7() {
  hnh_Hook(0x423CE7, hook_DSErrToString_423CE7, "hook_DSErrToString_423CE7");
  hnh_Hook(0x41898E, hook_DDErrToString_41898E, "DDErrToString_41898E");
}