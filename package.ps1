param
(
    $tag = ""
)

&.\clean

if($tag -eq "")
{
    Write-Host "`nERROR: no tag specified. Try: 'package v0.0.1'`n" -Fore Red
    exit -2
}

&.\run release --compile-only
if($LASTEXITCODE -ne 0)
{
    Write-Host "`nERROR: failed to package windows build.`n" -Fore Red
    exit -1
}

$dir=".\build\mapedit_$tag`_win64"

Write-Host "creating package folder under $dir...`n"

mkdir $dir

if(-Not(Test-Path $dir))
{
    Write-Host "`nnERROR: could not create package folder: '$dir'.`n"
    exit -2
}

mkdir -p "$dir\bin\release\"
mkdir -p "$dir\assets\"

cp ".\vendor\river2D\bin\release\river2Dsoftware.dll" "$dir\bin\release\river2Dsoftware.dll"
cp ".\bin\release\mapedit.exe"                        "$dir\bin\release\mapedit.exe"
cp ".\river2D.ini"                                    "$dir\river2D.ini"
cp ".\assets\*"                                       "$dir\assets\"
cp ".\assets\tiles\*"                                 "$dir\assets\tiles\"
cp ".\mapedit.lnk"                                    "$dir\mapedit.lnk"
