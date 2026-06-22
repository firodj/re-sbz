# Illusion

## Sexy Beach Zero (October 29, 2010)

* Sexy Beach (December 2002)
* Sexy Beach 2 (July 11, 2003)
  * Chiku Chiku Beach (Expansion, December 9, 2003)
* Sexy Beach 3 (September 29, 2006)
  * Sexy Beach 3 Plus (Expansion, December 15, 2006)
* Sexy Beach Zero (October 29, 2010)
  + vorbis 1.3.1, ogg 1.2.0 (March 26, 2010) or vorbis 1.3.2, ogg 1.2.1 (November 3, 2010)
* Jinkō Gakuen 2 (C++ YAYOI Engine --last--)("Artificial Academy 2", June 13, 2014)
  * Jinkō Gakuen 2: Append Set (Add-on, August 29, 2014)
  * Jinkō Gakuen 2: Append Set 2 (Add-on, October 31, 2014)
* Sexy Beach Premium Resort (Unity Engine) (September 11, 2015)

## Tools

Tools: https://euangoddard.github.io/clipboard2markdown/

## Scene Render Order

pfnDo_10
pfnDo_0C
field_210
field_214
pfnDo_14
field_218
pfnDo_1C
pfnDo_18
field_21C

## Reading File

1. LoadXX_70DDC0
   - CPack::GetSubFileFromPP_70C9C0
     - CPack::GetSubFile_70C770
   - LoadOldPPFile_7196F0
2. ScnObjLoadDataFromPP_70E050
   - CPack::GetSubFileFromPP_70C9C0
   - LoadOldPPFile_7196F0
3. CPack::LoadHeader_70C5A0
   - CPack::GetSubFileFromPP_70C9C0
     - CPack::GetSubFile_70C770
5. LoadSubFile_695220
   - GetFileContent_719230
   - CPack::GetSubFileFromPP_70C9C0
   - LoadOldPPFile_7196F0
