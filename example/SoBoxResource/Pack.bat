
:: 首先尝试删除已有的压缩包。
del UserResource.7z

goto __1
= 这里是注释
= 生成新的压缩包。
= %1 是外界传递的第一个参数，其值是工作路径，也即SoBox.exe所在文件夹的路径。
= %1SoBoxResource\7z.exe 就是7z.exe的完整路径。
= %1UserResource 就是UserResource目录的完整路径。
:__1
%1SoBoxResource\7z.exe a -t7z %1UserResource.7z %1UserResource -r -mx=9 -m0=LZMA2 -mmt=on

