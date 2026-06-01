# Sexy Beach 3: Modding

Tools
-----

-   **For the pp files**
    -   [IlluPak](https://web.archive.org/web/20221128125105/http://illusionist.sosavalanche.net/)
    -   For the love of god can someone fix this because the illupak is no longer there!!!!!

-   **For the lst files**
    -   [Darkhound's lst tools](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1073245&postcount=1073245) (A set of tools to create files to be used with LST_INSERT logic implemented in SB3 Wizzard)
    -   [Nobody123's lst toolset](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1003574&postcount=110) (Teo tools to decode/encode lst files once they have been unpacked from a pp file with Illupak)

-   **Other modding tools**
    -   [xx toolset](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1004671&postcount=184) (Basic toolset similar to Ilupak that only extracts and repacks .xx files)
    -   [SB3Utility](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1100477&postcount=1700) (Rewrite of the RLUtility for SB3)
        -   [RLUtility](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1058556&postcount=876) (Post also includes a tutorial on how to work with xx files)
    -   [.xx file layout infomation](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=843606&postcount=570)
    -   [Additional .xx file format information](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1078000&postcount=1187)
    -   [Darkhound's Illusion Morpher v0.1](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1709879&postcount=198) (The program has been created to adjust mqo files in various ways. This includes adjusting/morphing one file to the shape of another, creation or recreation of symmetry in an object, closing of small gaps in models vertices without reducing vertex count, automatic refitting of clothes to the new shape, etc.)

-   **For save game files**
    -   [Darkhound's save game patcher v0.2](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1093836&postcount=1093836) (outdated and replace by SB3 Wizzard)

-   **To apply mods**
    -   [Darkhound's SB3 Wizzard v0.5c](https://web.archive.org/web/20221128125105/http://www.hongfire.com/forum/showpost.php?p=1153872&postcount=5) (A tool to apply wizzard ready mods to SB3 with just a mouse click)

Common codes
------------

-   There are three body types:
    -   small = 1 (occasionally "A")
    -   medium = 2 (occasionally "B")
    -   large = 3 (occasionally "C")
-   There are 5+3 characters:
    -   Manami = 1
    -   Maya (expansion) = 2
    -   Esk = 3
    -   Eo = 4
    -   Reiko = 5
    -   Maria = 6
    -   Bael = 11
    -   Fei = 12
-   There are 5+1 swimsuit meshes:
    -   Normal = 1
    -   High waistline = 2
    -   Tight around the breast = 3
    -   Liftable bra = 4
    -   Suspenders = 5
    -   Nude = 6

File overviews
--------------

-   sb3_0000 - contains textual information, including info on where certain data are located, and properties of various in-game objects/interactions.

-   sb3_0100 - In-game menus and icons
-   sb3_0110 - Accesories, skirts, pareos
-   sb3_0200 - Option and clothing menu buttons (previews shown when you dress the girl up)
-   sb3_0300 - menu icons, textures for skin/faces, mosaic for pubic hair.
-   sb3_0310 - Texture files for all the clothes and their respective tanning files
-   sb3_0400 - 3D information for locations
-   sb3_0500 - 3D information for girls' heads
-   sb3_0510 - girls 3D information during date scenes as well as body in general
-   sb3_0520 - girls 3D information during oil scenes
-   sb3_0530 - girls 3D information during H scenes
-   sb3_0540 - guy 3D information
-   sb3_0600 - BGM, ambient sound, sound effects, and volume adjustment voice
-   sb3_0700 - contains story script.

Additional info to be organized from sb3_0000.

sb3_0000
--------

The lst files stored in sb3_0000.pp tells the game where certain assets are stored and what properties to give them. Thus, by manipulating the .lst files, it is possible change things in the game (clothing, meshes, animations) without modifing the original files contain those assets. It is even possible to add new clothing instead of replacing existing ones.

In order to view or edit the lst files, you must decrypt it via the lst tool. After you are done editing, you must reencrypt it using the tool found the that same post again, before repacking the .pp file.

### lb??_00_00_00_00.lst

The lb files are all swimsuit information. Each lb file contain information on the same swimsuit, and most of the data are similar. The question mark corresponding to the character code. For example, lb03_00_00_00_00.lst is the swimsuit information for Esk.

Because of the size of the file, it's best to manage it inside a spreadsheet. Thus for the rest of this section we will use letters to denote the "columns" in the file, with A being the first column, and BI being the last column.

#### A~H

The first column is a label number. Modifyng this number doesn't seem to affect anything (does not have to go from 0~250, can skip around, multiple swimsuits can share the same number). Most likely serves as a comment system to help quickly locating a line. Note that this is different from the "line number", which is the actual ordering of lines in the file. The line label helps you identify the line number, but you can change the label to whatever and it will not affect the game at all.

1.  124 is Bael's Original swimsuit.
2.  141 is Maya's Default swimsuit.
3.  15 is Maya's Original swimsuit.
4.  62 is unnamed and unavaiilabe in regular gameplay. It is Bael's Reception Desk Uniform.

The M-key addon, "Mufufu Glasses", is counted as a costume. It's #241.

And by the way, the tga files of the format "sb0x_0y_*" and "sn0x_0y_*" are of the convention:

-   x=0 for general purpose (not specific to any girl)
-   x=1 for Manami
-   x=3 for Esk
-   x=4 for I-o
-   x=5 for Reiko
-   x=6 for Maria
-   y=1 for small breasts (Manami)
-   y=2 for medium breasts (Esk & I-o)
-   y=3 for large breasts (Reiko & Maria)

Of course if you are going modify the lst file anyways, then you don't have to worry about how to name your stuff.

The game keeps track of which swimsuit have been unlocked via the line number, not label number. Thus if you swap two lines, one is for a locked swimsuit and theother is an unlocked swimsuit, their lock status will be reversed even if you keep all the data the same.

The line number also affects the mesh used for the swimsuit:

-   1~80 standard mesh
-   81~120 = high waist mesh
-   121~200 = round breasts mesh (for most bikinis and some of the other swimsuits)
-   201~230 = liftable bra mesh (for bikinis whose bra can be lifted)
-   231~240 = suspender mesh
-   241~250 = nude mesh

The mesh types are adjustable via the file pointed to in ld01_01~ld01_11, lines 21~26.

The second column is the internal name of the swimsuit. Most of them correspond exactly to the Dressup menu. But not all (such as the school swimsuit with character's name on it). This is just a label for internal identification purposes, changing it doesn't affect anything.

#### I~Z - Interactions

Column in I~Z specify what interactions you may perform during oil and H mode. The numbers refers to the interactions listed in lf01_00_00_00_00.lst By changing the numbers, you change the interaction available. the oil and h mode share the numbers, but the number may mean different things in the different modes.

Upper body:

-   Columns I, J are the outer area of the breasts (Left, Right)
-   Columns K, L are the lower third area of the breasts (Left, Right)
-   Columns M, N are the upper third of the breasts (Left, Right)
-   Column O, P are the middle third area of the breasts (Left, Right)
-   Column Q is between the breasts

Lower body:

-   Column Y and Z are the left and right waists respectively.
-   Columns R, S are the left and right hips respectively
-   Columns T, W are from the front
-   Column U, X are from behind
-   Column V is from below

Thus it is possible to set up an interaction by mousing over the left breast that actually affects the right breast.

#### AB~AL - See-through when wet

Columns AB~AL controls see-through of swimsuit when wet. 0 means won't get wet at all. 1 means it'll get wet, but won't become transparent (for white clothing, it turns silverish grey). 2 turns transparent.

-   AB = left breast
-   AC = right breast
-   AD = stomach
-   AE = left half of back
-   AF = right half of back
-   AG = pubic hair area
-   AH = front hole
-   AI = left hip
-   AJ = right hip
-   AK = back hole, left half
-   AL = back hole, right half

#### AN ~ AX

Column AN control upper half swimsuit brightness. 1=normal, 2= glossy, 3 = a little dark, 4 = darkish glossy, 5 = slightly bright glossy. Column AT is for lower half swimsuit brightness.

#### AZ ~ BI

The column AZ controls the global glossing effect of the swimsuit. if it is set to 1, it will have glossing. if 2, then not.

The BA column is used to hide the top mesh of the swimsuit by setting it to 0, such as the Blacktopless swimsuit. For normal swimsuits, put in 1.

The BB column is used to hide the bottom mesh of the swimsuit by setting it to 0, such as the bathtowel. For normal swimsuits, put in 1.

The BC column controls whether to display the little knots found on some of the liftable bra bikinies. Setting it to 0 will hide those knots. Setting it to 1 for other meshes does nothing.

The BD column controls whether the swimsuit "constrains" the breasts. This is effect is subtly comparable in H mode by toggling swimsuit using the "9" key - you will notice the breasts change the angles a little. Swimsuits not constraining the breasts (ie pressing 9 has no effect) has this column set to 10, others are set to 0. Note: pressing "M" with the Mufufu glasses is *different*, and will show no difference whatsoever in the angle of the breasts.\
Column BE is the order of the item wthin the category. The order does not have to be consecutive, and multiple swimsuits can have the same order. However, if two swimsuits share the same order, either one could appear before the other (behavior is undefined), and their button graphic will be the same.

Column BF corresponds to the swimsuit category (0 = One-piece; 1 = Separate; 2 = Bikini; 3 = Special).

The category and order combned determines which swimsuits are the "default" swimsuits that you can only obtain by talking to the respective girls and cannot be obtained via regular dating with other girls. Additionally, the category and order combned determine's which button to use for selecting the swimsuit. In the dressup screen, the swimsuits will be sortedby order in each category. If multiple swimsuits in teh same category have teh same order, they will use the same button, and the actual order between them is undefined (not in line number or serial number order).

Column BG varies from 0~2. It happens to positively correlat with the line number. The normal and high-waist swimsuit meshes happen to all be 0, the tight-breasts and liftable bra meshes happen to all be 1, and the suspender mesh happen to all be 2. However it is unknown what this really do.

Column BH is the heart requirement (1~5).

Column BI goes from 1~15 (0 being used as a NULL value), and has a correlation with the heart requirement. It is NOT related to the order of swimsuit unlocking.

### lc00_00_00_00_00.lst

The lc files are for accessories. there are 11 tabs, corresponding in order (didn't check each one, but the few I sampled mtches) with lc01~lc11. lc00 itself has info about the 15 locations (some tabs have left/right).

lc file is relatively straight forward. Line number, internal name, pp file name for the asset, xx filename for the asset inside the pp file, then order.

For order, 1 is reserved for the "remove" box. Most accessory starts from 2 and go up one by one. But the types that have left/right, they start from 3 and jump up odd numbers (left use the odd numbers, right automatically uses odd+1). If the order is 255 then it is not available.

I'm notsure what teh o01_N_*_AC are for. AC standsfor accessory, the * part corresponds to the accessory tab, what I don't understand is why only face and head need the two columns, why two columns are needed when the values are identical, as well as why different head accessories have different "Atama0?" for the * part.

For skirt, I see no place for mesh information. It might be they all just use the same mesh and have very different alpha channels, or mesh info is stored elsewhere.

### ld00_00_00_00_00.lst

ld00_00_00_00_00.lst contains some misc image information.

Lines 1~8 are the basic skin texture, light head texture, and tanned head texture. For the two hidden characters 2 (Maya) and 11 (Bael?), only the tanned head texture assets actually exist (in sb3_0310.pp as opposed to sb3_1310.pp).

Line8 is the first time there is a hint that there may be a third hidden character. None of its three assets specified on that line can be found.

Lines 11, 12 and 15 are the game menu toolbar, menu buttons, and title screen.

Lines 21~28 lists the files used for pubic hair and its censoring. It works by constantly rotating the 4 listed files. This does not include the mosic censoring of the private parts. this is pubic hair only.

Line 31 specifies the files used for the player's hands.

Line 32 specifies the files for the various lotions. Used to show the oil as it is being shot at the Girl. Looks somewaht like a tiny comet.

Line 33 specifies the graphic for the crosshair of camera.

Line 34 specifies the looks of the oil when it lands on the Girls. Looks like a small blob.

Line 35 specifies the squirt/spray action of the Girls after they have orgasmed twice.

Line 36 specifies the files for the player to orgasm onto the Girls. Similar to the way Line 32 does the lotions.

Lines 41~62 covers in-game buttons.

Line 71 specifies the file name format for various background music. The "%20d" part is a variable, so when different situations need a different background music, a different number is plugged in for the variable to obtain the actual file name for the background music.

Line 72 specifies the file name format for the environtmental sounds of diffeerent dating locations.

Line 73 specifies the file name format for sound effects (when clicking on buttons, cancelng actions, etc etc). Of those, 29~35 are the "Sexy Beach 3" uttered by different girls at the Title screen. Note there are 7 voices.

Line 74 specifies the sample sound played in the system config screen (the speaker icon to the right of volumn adjustment). The last item, sm04_%02d_00_00_00.ogg, again has a variable from 1~7.

### ld01_??_00_00_00.lst

the ld01_01~ld01_11 files specify body part related information for each of the girls. Kao = face, Kami = hair, Bura = Bra (though it might be generically used to mean breasts in this context).

Lines 11 and 14 are for hair style 1 (mesh and texture) Lines 12 and 15 are for hair style 2 Lines 13 and 16 are for hair style 3 whose assets don't exist for any of the girls. If you plan on swapping hair between girls, make sure you swap the pair of lines or the game might crash.

The body parts mesh as well as hair texture are specified in these files, but not the skin/face textures. So only replacing the mesh and not the textures might result in weird stuff (giving Mamami the head of Esk will cause Manami's ears to go all black).

Lines 21~26 each specify a file containing the swimsuit & skirt mesh information, mosiac textures, how swimsuit texture applies to the mesh. The file also has information on breast shape (and thus differs between girls of different breast sizes). Each of the six lines corresponds to a different mesh type, in the order of: normal, high-waist, highlighted breasts, liftable bra, suspenders, and unknown.

Line 101 specifies the male member 3D file to be used for the girl. Thus it is possible to have an uncensored member for one girl and the default censored member for another girl.

Lines 151~171 specify the animation files for the various activities the girl does on a date, as well as model & texture for any extra props. By default the dates are:

-   Line 151 = standing
-   Line 152 = sitting
-   Line 153 = laying
-   Line 154 = swimming
-   Line 155 = book reading
-   Line 156 = taking shower
-   Line 157 = swim circle
-   Line 158 = mini-fireworks
-   Line 159 = using oil
-   Line 160 = jump rope
-   Line 161 = playing volleyball
-   Line 162 = eating popstickle
-   Line 163 = limbo
-   Line 164 = exercises
-   Line 165 = playing on the swing
-   Line 167 = balancing on a flating board
-   Line 168 = swimming 2
-   Line 169 = cooking stew
-   Line 170 = drinking sake

The dates in 165,167, 168, 169, and 170 happen to require a specific girl to see it for the first time for it to be "unlocked" for other girls. Specifically, the girl/date correspondence is (164 + girl_number). This might explain why we haven't found out anything for 166 and 171 (which corresponds to Maya and Bael). It is unknown if the unlocking has to do with the line number of the date, or if the date's own data specifies it. But considering the precedent with swimming suits and accessory, it seems likely to be governed by the line numbers.

### lf01_00_00_00_00.lst

lf01_00_00_00_00.lst contains informatin on the interactions you can perform with your hand. For the name, if you see A, B, or C in it, it refers to diferent body types. A = small breasts. B = normal breasts. C = large breasts.

In H scenes, certain interactions will be replaced by different interactions. This behavior is not controled by the lf01_00_00_00_00.lst file. Each line only controls a specific interaction, not the Oil/H bundle of two interactions.

#### The default interactions

Interactions all come in groups of less than 10, and each swimsuit by default (as specified in the lb files) only use one interaction from each group.

Upper body:

-   1 - squeeze left breast, body type A, over bra
-   2 - squeeze left breast, body type A, no bra
-   3 - squeeze left breast, body type A, completely nude
-   4~9 - same as 1~3 but for bodytype B and C
-   11~19 - mirror of 1~9 (right side)
-   21 - pull left bra
-   22 - pull left nipple (has swimsuit)
-   23 - pull left nipple (completely nude)
-   31~33 - mirror of 21~23
-   41 - put hand into left bra
-   42 - left nipple (has swimsuit)
-   43 - finger left nipple (completely nude)
-   51~53 - mirror of 41~43
-   61 - try to pull up bra, by selecting left breast
-   62 - push breasts together, by selecting left breast (has swimsuit)
-   63 - push breasts together, by selecting left breast (completely nude)
-   64 - pull up bra (and stay up), by selecting left breast
-   71~74 - mirror of 61~64
-   81~84 - same interaction as 61~64, by selecting the center of the chest (between breasts)

Lower body:

-   91 - squeeze left hip
-   101 - squeeze right hip
-   111 - pull panty from front
-   112 - finger crotch from front
-   113 - pull "open" panty from front
-   121~123 - mirror of 101, from below
-   131 - pull panty from back
-   132 - finger crotch from back
-   141 - put hand into panty from left
-   142 - finger crotch from left
-   151~152 - mirror of 141~142
-   161 - try to pull down panty, from left
-   162 - tickle from left
-   171~172 - mirror of 161~162

Suspender special:

All of the following interactions are "special" versions of the upperbody, for the suspender-type swimsuits.

-   191~193 - squeeze left breast, body type A~C
-   201~203 - mirror of 191~193
-   211 - put hand into left bra
-   212 - finger left nipple
-   221~222 - mirror of 211~212
-   231 - Try to pull apart bra, from left
-   232 - Push breasts together, from left
-   233 - Pull apart bra, from left
-   241~243 - Mirror of 231~233
-   251~253 - center version of 231~233

#### The columns

Note: Specifying invalid values (with respect to the specific animation) may cause a pulled swimsuit not to return to its natural position after the interaction, unless some other interaction touches the swimsuit.

The animation/object values are probably defined in individual 3D object data files.

-   Column A is the serial number, most likely just a label to enhance human readability and doesn't affect the game.
-   Column B is the name of the interaction.
-   Column C is the starting position for the right hand animation
-   Column D is the starting position for the left hand animation (left hand is usually hidden, see column Y).

-   Columns E and F affect the swimsuit after the interaction. Different animations (column V) will have different results for the same values in columns E and F.

Column E only have non-zero value for "squeeze" interactions, in which case the value would be 10. But editing the value produce no currently observable difference.

-   Column G specifies how many values from columns H~K to read from, thus the maximum is 4. However, it is possible to specify a higher value, and force the game to read from columns L and beyond. Note that values in columns L and beyond will then serve dual purpose, and in general it is best to keep column L as 0.

-   Columns H~K specify which sets of 3d objects will be animated. Only the number of values specified in column G will be checked, and the rest ignored. In general:
    -   0 = left breast
    -   1/2 = left bra outer/inner surface
    -   3~5 = mirror of 0~2
    -   6 = both breasts
    -   7/8 = entire front bra outer/inner surface
    -   9/10 = panty front, outer/inner surface
    -   11/12 = panty back, outer/inner surface
    -   13~16 = entire panty
    -   17 = suspender, leftside
    -   18 = suspender, rightside
    -   19 = suspender, bothsides

Note that each animation only support a small subset of 3d object sets. If you attempt a "lift bra by selecting center of chest" animation, for example, and attempt to animate the left bra, nothing will happen. Also, animation for certain valid sets may not be available for specific swimsuit meshes.

-   Columns L~R are non-zero for interactions that insert the hand under the swimwear. If column L is 1, then the rest of the columns are used. If column L is 0, the rest of the columns are ignored.

-   Column S specifies whether the interaction affect the "open/close" state of the swimsuit. Ie, whether the bra is lifted, or the panty shifted etc. If the value in this column is non-zero, then performing the interaction will toggle the state of the "group" (value of column T) between open and close.

By default, all "groups" are in their "close" state. Performing a state-changing interaction will change the state of the corresponding group to "open". While in the open state, interactions in the same group that do not change the state are disabled, leaving only other state-changing interactions. For example, in the default game, lifting up the bra will disable all other interactions on the bra, as they are all in group 1.

Additionally, the hint label for the interactions in their open state use the next label graphic. For example, if one interaction usually uses ema-st00_00_04_00_00.tga in the close state, then in the open state it will use ema-st00_00_05_00_00.tga

Note that just because the bra is in the open state doesn't mean the bra will be automatically lifted. Only interactions with the specific animations will do the actual lifting. The open/close states only govern what interactions are available.

-   Column T organizes the interactions into groups for the open/close states.

-   Column U specifies the "return" animation for swimsuit pieces from the open state to the closed state. It is only used if column S has a non-zero value. If the animation is unspecified, the hand will appear at the initial animation location but not move, and the corresponding swimsuit piece will just "jump" to the regular closed position.
    -   70 for liftable bra
    -   105 for panty
    -   80 for suspenders

-   Column V specifies the hand and swimsuit animation. For the "insert hand into swimsuit" sequences, the animation has more parameters and uses columns L~R for the additional parameters.
-   Column W specifies the girl's reaction during the interaction for oil scenes, including pose/animation and voice.
-   Column X specifies the girl's reaction during the interaction for H scenes, including pose/animation and voice.
-   Column Y specifies whether to display the left hand also. For animations that are intended as one-handed only, the left hand will show up at the location specified by column D, but will not otherwise move.
-   Column Z specifies which (if any) body part will follow the motions of the hand movement. Thus it's possible to be squeezing the left breast but have the right breast move around with your mouse-movement.
    -   0 - none
    -   1 - left breast
    -   2 - right breast
    -   3 - left hip
    -   4 - right hip

-   Column AA specifies which hint label to display when the mouse cursor hovers over the interaction location. The label graphics are stored in sb3_0200 with filenames ema-st00_00_??_00_00.tga, where ?? is (1 + the value in column AA)

### lh??_00_00_00_00.lst

These files affect what the girls say during oil mode. They most likely do other things too that have not yet been discovered...

-   lh01 for standing posture
-   lh02 for sitting posture
-   lh03 for laying down posture
-   lh04 for not-yet-available posture in the exapansion?

### lp0?_0?_0?_00_00.lst

the lp files specify file name formats for certan wave files.

# Sexy Beach 3: Save file structure

Overview
--------

-   save(1~3)000.sav - Status with each of the girls (hearts and the 6 counts for each girl in the status screen)
-   save(1~3)001.sav - In-game time, total play time, swimsuit & accesory & hairstyle lock status (locked, new, unlocked), CG status
-   save(1~3)0(1~7).sav - Data on what each of the 7 girls are currently wearing and their skin tanning status, also information about heart level and clear status of the girl (remark: you need to change the setting in this file, if you change only the file save(1~3)000.sav, it does not work)

--~~~~Insert non-formatted text here'''Bold text'''== save(1~3)000.sav ==

-   00000016~1C - Clear Status for each girl (00 = Show Nothing, 01 = Show Clear Status)
-   0000002A~30 - Heart Status for each girl (01 = 1 Heart, 02 = 2 Hearts, 03 = 3 Hearts, 04 = 4 Hearts, 05 = Big Heart)

save(1~3)001.sav
----------------

-   00000001 - Hours played
-   00000001 - Minutes played
-   00000009 - Seconds played
-   00000011 - In-game day
-   00000013 - In-game time (01 = morning, 02 = noon, 03 = sunset, 04 = night, 05 = latenight)

### CG lock status

00 = locked, 01 = unlocked

-   00000064~67 - Manami's CG
-   00000078~7B - Maya's CG
-   0000008C~8F - Esk's CG
-   000000A0~A3 - Eo's CG
-   000000B4~B7 - Reiko's CG
-   000000C8~CB - Maria's CG
-   0000012C~12F - Bael's CG
-   00000140 - Manami's CG - Ecchi Scene
-   00000141 - Esk's CG - Ecchi Scene
-   00000142 - Eo's CG - Ecchi Scene
-   00000143 - Reiko's CG - Ecchi Scene
-   00000144 - Maria's CG - Ecchi Scene
-   00000145 - Maya's CG - Ecchi Scene
-   00000146 - Bael's CG - Ecchi Scene
-   00000147 - Fei's CG - Ecchi Scene

### Video sequences

00 = new / show, 01 = already seen / don't show

-   00000022~26 - Beach: Morning/Noon/Sunset/Evening/Midnight
-   0000002C~30 - Pool: Morning/Noon/Sunset/Evening/Midnight
-   00000031~35 - Falls: Morning/Noon/Sunset/Evening/Midnight
-   00000036~3A - Hot Spring: Morning/Noon/Sunset/Evening/Midnight
-   0000003B~3F - Grotto: Morning/Noon/Sunset/Evening/Midnight

### Special scenes to be viewed with playback

00 = locked, 01 = unlocked

-   0000001B - On Swing (Grotto)
-   0000001C - Playing Tag (anywhere outside except Reef)
-   0000001D - Balancing Act (anywhere outside except Reef)
-   0000001E - Swim Fast (Reef)
-   0000001F - Cooking (Kitchen)
-   00000020 - Drink Sake (Hot Spring)
-   00000021 - Billiards (Living Room)

### Dressup room lock status

00 = locked, 01 = unlocked, 03 = unlocked and new

-   000001E1 - Swimsuit #001
-   000002DA - Swimsuit #250

-   000002E1 - Accessory head #001
-   00000361 - Accessory face #001
-   000003E1 - Accessory ear #001
-   00000461 - Accessory neck #001
-   000004E1 - Accessory back #001
-   00000561 - Accessory waist #001
-   000005E1 - Accessory paero #001
-   00000661 - Accessory wrist #001
-   000006E1 - Accessory finger #001
-   00000761 - Accessory ankle #001
-   000007E1 - Accessory shoes #001

-   00000860 - Manami's basic hairstyle
-   00000861 - Manami's advanced hairstyle
-   00000862 - Manami's basic hairstyle (plus pack: color adjustable)
-   00000863 - Manami's advanced hairstyle (plus pack: color adjustable)
-   0000086A - Maya's basic hairstyle
-   0000086B - Maya's advanced hairstyle
-   0000086C - Maya's basic hairstyle (plus pack: color adjustable)
-   0000086D - Maya's advanced hairstyle (plus pack: color adjustable)
-   00000874 - Esk's basic hairstyle
-   00000875 - Esk's advanced hairstyle
-   00000876 - Esk's basic hairstyle (plus pack: color adjustable)
-   00000877 - Esk's advanced hairstyle (plus pack: color adjustable)
-   0000087E - Eo's basic hairstyle
-   0000087F - Eo's advanced hairstyle
-   0000087F - Eo's basic hairstyle (plus pack: color adjustable)
-   00000880 - Eo's advanced hairstyle (plus pack: color adjustable)
-   00000888 - Reiko's basic hairstyle
-   00000889 - Reiko's advanced hairstyle
-   0000088A - Reiko's basic hairstyle (plus pack: color adjustable)
-   0000088B - Reiko's advanced hairstyle (plus pack: color adjustable)
-   00000892 - Maria's basic hairstyle
-   00000893 - Maria's advanced hairstyle
-   00000894 - Maria's basic hairstyle (plus pack: color adjustable)
-   00000895 - Maria's advanced hairstyle (plus pack: color adjustable)
-   0000089C - Bael's basic hairstyle
-   0000089D - Bael's advanced hairstyle
-   0000089E - Bael's basic hairstyle (plus pack: color adjustable)
-   0000089F - Bael's advanced hairstyle (plus pack: color adjustable)
-   8a6-8ae - Sunscreen oil

save(1~3)0?0.sav
----------------

### filename and corresponding girl

The ? stands for a number between 1 and 5 (sb3 without plus pack) and a number between 1 and 7 (sb3 with plus pack installed). The number indicates a girl:

-   1 -> Manami
-   2 -> Esk
-   3 -> Eo
-   4 -> Reiko
-   5 -> Maria
-   6 -> Maya (SB3 plus pack only)
-   7 -> Bael (SB3 plus pack only)

### girls heart levels

-   00000000 Heart level of the girl (valid entries are 1,2,3,4 or 5)
-   00000001 Seems to be linked to the dialogs already spoken with the girl (not sure on that one)
-   00000002 clear status of the girl (00 -> not cleared, 01 -> cleared)

---

Archived: https://web.archive.org/web/20221128125105/http://wiki.anime-sharing.com/hgames/index.php?title=Sexy_Beach_3/Modding

Tools: https://euangoddard.github.io/clipboard2markdown/
