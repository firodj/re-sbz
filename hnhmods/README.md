# HNH - Hana No Hime

Game Engine Breakdown:

# Video Encoders:

Install ffdshow_trytout 32-bit, andd ffmpeg to re-encode.

```
$ ffmpeg -i op.avi -c:v libxvid -q:v 3 -c:a pcm_s16le op_xvid.avi
$ ffmpeg -i ED.avi -c:v libxvid -q:v 3 -c:a pcm_s16le ED_xvid.avi
```


