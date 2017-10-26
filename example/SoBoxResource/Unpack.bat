
:: 首先尝试删除已有的用户资源文件夹
rd /q /s UserResource

goto __1
= 这里是注释
= 解压
= %1 是外界传递的第一个参数，其值是工作路径，也即SoBox.exe所在文件夹的路径。
= %1SoBoxResource\7z.exe 就是7z.exe的完整路径。
= -o%1 表示输出目录就是工作目录。
:__1
%1SoBoxResource\7z.exe x %1UserResource.7z -o%1 -aoa

