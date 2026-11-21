#include "pch.h"

#include <iostream>

#include <fstream>
#include <cinttypes>

#include <map>
#include <string_view>

#include "console.hpp"
#include "globals.hpp"
#include "util.hpp"

#include "sbz_header.hpp"
#include "sbz_intern.hpp"

#include "struct.hpp"

namespace sbz {

  void AssertStructSize() {
    static_assert(sizeof(CSBZGlobal) == 0x19CD4, "CSGZGlobal size mismatch");
    static_assert(sizeof(CSceneManager) == 0xC78C, "CSceneManager size mismatch");
    static_assert(sizeof(CSceneOp) == 0x2C, "CSceneOp size mismatch");
    static_assert(sizeof(CSceneStack) == 0x224, "CSceneStack size mismatch");
    static_assert(sizeof(CSceneEntry) == 0x34, "CSceneEntry size mismatch");
    static_assert(sizeof(CSceneCallbacks) == 0x24, "CSceneCallbacks size mismatch");
    static_assert(sizeof(CSceneRenderMgr) == 0xC668, "CSceneRenderMgr size mismatch");
    static_assert(sizeof(CSetting_448) == 0x448, "CSetting_448 size mismatch");
    static_assert(sizeof(CPackFormatSecret) == 0x28, "CPackFormatSecret size mismatch");
    static_assert(sizeof(CSpriteSheet) == 0x530, "CSpriteSheet size mismatch");
    static_assert(sizeof(CSBGameCore) == 0x5C0, "CSBGameCore size mismatch");
    static_assert(sizeof(CGameData) == 0xCD0C, "CGameData size mismatch");
    // TODO: CPackItem
    // TODO: CPack
  }

  const std::map<ESBZScene, std::string_view>& GetSceneMap()
  {
    static const std::map<ESBZScene, std::string_view> sceneMap = {
      { ESBZScene::POP_SCENE,          "POP_SCENE" },
      { ESBZScene::LOGO_SCENE,         "LOGO_SCENE" },
      { ESBZScene::TITLE_SCENE,        "TITLE_SCENE" },
      { ESBZScene::MAP_SCENE,          "MAP_SCENE" },
      { ESBZScene::ROOM_SCENE,         "ROOM_SCENE" },
      { ESBZScene::SHOP_SCENE,         "SHOP_SCENE" },
      { ESBZScene::NIGHTSHOP_SCENE,    "NIGHTSHOP_SCENE" },
      { ESBZScene::PRESENT_SCENE,      "PRESENT_SCENE" },
      { ESBZScene::COSTUME_SCENE,      "COSTUME_SCENE" },
      { ESBZScene::LOAD0_SCENE,        "LOAD0_SCENE" },
      { ESBZScene::LOAD2_SCENE,        "LOAD2_SCENE" },
      { ESBZScene::LOAD1_SCENE,        "LOAD1_SCENE" },
      { ESBZScene::OILCOATING_SCENE,   "OILCOATING_SCENE" },
      { ESBZScene::HCHECK_SCENE,       "HCHECK_SCENE" },
      { ESBZScene::H_SCENE,            "H_SCENE" },
      { ESBZScene::DATE_SCENE,         "DATE_SCENE" },
      { ESBZScene::PLAY_SCENE,         "PLAY_SCENE" },
      { ESBZScene::KISS_SCENE,         "KISS_SCENE" },
      { ESBZScene::RESULT_SCENE,       "RESULT_SCENE" },
      { ESBZScene::CONFIG_SCENE,       "CONFIG_SCENE" },
      { ESBZScene::LOADING_SCENE,      "LOADING_SCENE" },
      { ESBZScene::DIALOG_SCENE,       "DIALOG_SCENE" },
      { ESBZScene::EXTRA_SCENE,        "EXTRA_SCENE" },
      { ESBZScene::STATUS_SCENE,       "STATUS_SCENE" },
      { ESBZScene::EDIT_BODY_SCENE,    "EDIT_BODY_SCENE" },
      { ESBZScene::CHECK_SCENE,        "CHECK_SCENE" },
      { ESBZScene::ESCAPE_SCENE,       "ESCAPE_SCENE" },
      { ESBZScene::SPECIAL_SCENE,      "SPECIAL_SCENE" },
      { ESBZScene::SP_TITLE_SCENE,     "SP_TITLE_SCENE" },
      { ESBZScene::SP_CHA_SEL_SCENE,   "SP_CHA_SEL_SCENE" },
      { ESBZScene::SP_MAP_SEL_SCENE,   "SP_MAP_SEL_SCENE" },
      { ESBZScene::SP_EMERGENCY_SCENE, "SP_EMERGENCY_SCENE" },
      { ESBZScene::CUSTOM_SCENE,       "CUSTOM_SCENE" },
      { ESBZScene::COL_TEST_SCENE,     "COL_TEST_SCENE" },
      { ESBZScene::DATE_TEST_SCENE,    "DATE_TEST_SCENE" },
      { ESBZScene::TEST_PARENT_SCENE,  "TEST_PARENT_SCENE" },
      { ESBZScene::TEST_PLAY_SCENE,    "TEST_PLAY_SCENE" },
      { ESBZScene::TEST_TTY_SCENE,     "TEST_TTY_SCENE" },
      { ESBZScene::TOOL_SCENE,         "TOOL_SCENE" }
    };
    return sceneMap;
  }

  std::string_view SceneToString(ESBZScene scene)
  {
    const auto& map = GetSceneMap();
    const auto it = map.find(scene);
    return (it != map.end()) ? it->second : std::string_view{ "UNKNOWN_SCENE" };
  }

  /* Enumerate all entries with a callback/callable; return false to stop early */

  void EnumerateScenes(EnumerateSceneCb callback)
  {
    for (const auto& [scene, name] : GetSceneMap())
    {
      if (!std::invoke(std::forward<EnumerateSceneCb>(callback), scene, name))
      {
        break;
      }
    }
  }

}