set SHADER="shader"
set DX11="../../runtime/shaders/dx11"

rem SkyRenderer
shaderc --type v --platform windows -p vs_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/vs_PBR_skybox.sc -o vs_PBR_skybox.bin
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR_skybox.sc -o fs_PBR_skybox.bin

rem SceneRenderer
shaderc --type v --platform windows -p vs_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/vs_PBR.sc -o vs_PBR.bin
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_0.bin
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_1.bin --define POINT_LIGHT_LENGTH=32
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_2.bin --define SPOT_LIGHT_LENGTH=48
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_3.bin --define POINT_LIGHT_LENGTH=32;SPOT_LIGHT_LENGTH=48
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_4.bin --define DIRECTIONAL_LIGHT_LENGTH=32
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_5.bin --define POINT_LIGHT_LENGTH=32;DIRECTIONAL_LIGHT_LENGTH=32
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_6.bin --define SPOT_LIGHT_LENGTH=48;DIRECTIONAL_LIGHT_LENGTH=32
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR.sc -o fs_PBR_7.bin --define POINT_LIGHT_LENGTH=32;SPOT_LIGHT_LENGTH=48;DIRECTIONAL_LIGHT_LENGTH=32

rem Post
shaderc --type v --platform windows -p vs_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/vs_fullscreen.sc -o vs_fullscreen.bin
shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADER%/varying.def.sc -f %SHADER%/fs_PBR_postProcessing.sc -o fs_PBR_postProcessing.bin

pause