..\tool\zma bgm_driver_for_z88dk.asm BGMDRV.BIN
python makeup_bgmdriver_parameter.py
zcc +msx -create-app -subtype=msxdos main.c bgmdriver.c -bn BGMTEST.COM
