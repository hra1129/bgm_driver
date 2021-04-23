..\tool\zma sample.asm SAMPLE.BIN
..\tool\zma bgm_driver_for_z88dk.asm BGMDRV.BIN
zcc +msx -create-app -subtype=msxdos main.c bgmdriver.c -bn BGMTEST.COM
