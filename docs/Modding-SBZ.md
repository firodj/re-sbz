Introduction to modding
-----------------------

Like many other games from Illusion, Sexy Beach ZERO can be modified by mods. Mods can change many attributes of the game, like e.g. the hair color of the models, the bodies, clothes, etc. There is huge modding community in the internet which has created a lot of cool and funny mods which can be applied to the game. So e.g. if you do not like the blue hair color of some models in the game, there is a [mod](https://web.archive.org/web/20180118110516/http://www.hongfire.com/forum/showthread.php/243186-Mod-Release-Thread-%28Illusion%29-Sexy-Beach-Zero/page3?p=2609938#post2609938) to change them to black hair.

Modding basically works by replacing existing multimedia objects of the games with new ones. This is done on the file level. e.g. there is a file that contains a bitmap with the blue hair, which can be exchanges with a bitmap of black hair.

Technically all multimedia objects in this game are packed into proprietary archives, so in order to replace them you need to first unpack all files from the right archive, replace the right file, and then repack them again into the archive. You find a detailed description of the archives below the the chapter "pp files"

This can be done either manually by using a program called [ppextractor](https://web.archive.org/web/20180118110516/http://www.hongfire.com/forum/showthread.php/243630-Illusion-Subtitle-Overlay-PPExtractor-and-friends), or more conveniently with the Illusion Wizzard (see below), which will automate all file operations for the use. However not all mods are prepared for the wizard, so for them you need to operate on the file level.

How to use Illusion Wizzard
---------------------------

[How to use Illusion Wizzard](https://web.archive.org/web/20180118110516/https://wiki.anime-sharing.com/hgames/index.php?title=Illusion_Games_How_to_use_Illusion_Wizzard "Illusion Games How to use Illusion Wizzard")

PP files
--------

-   The .sdt files with the story is in sb00_00, which also contains lst files as well as other important things.
-   sb01_00 contains .wav files i assume bgm and sfx.
    -   All background music except the dance minigame music is in OGG format, you can encode your MP3s to OGG and replace them to listen to them ingame.
-   sb01_50 contains what appears to be animation files.
-   sb02_00 to sb02_04 Contain ... the Voice files for each respective character.
-   sb02_98 and sb02_99 also contain voice files.
-   sb03_00.pp contains the models.
-   sb03_01 contains the environment ... the Maps also the Props in the environment. Also contains items like the beachball etc..
-   sb05_00 contains what seems to be animation + Liquid effects such as the lotion and eeehhh cum lol.
-   Things like accessories and what not are contained in sb06_00 and sb07_00, sb08_00 and sb09_00 (for expansion volumes 1 thru 3). You could potentially create an sbXX_XXXXX.pp file with swimsuit textures and the game will load it as long as you point to it from the sb00_00.pp\sw*.lst swimsuit definitions.
-   sb07_00 is important to UI translation since everything here is the image files for the UI as well as icons and whatnot.
-   There is also a sb99_99 which contains 2 files... centerXX.xx and cm.tga I have no idea what this is for.