param
(
    $tag = ""
)

.\clean

if($LASTEXITCODE -ne 0)
{
    Write-Host "\033[31m\nERROR: failed to package linux build.\n\033[0m"
    exit -1
}

if($tag -eq "")
{
    Write-Host "\033[31m\nERROR: no tag specified. Try: 'package v0.0.1'\n\033[0m"
    exit -2
}

.\run release -compile_only

$dir=".\build\mapedit_$tag`_win64"

Write-Host "creating package folder under $dir...`n"

mkdir $dir

if(-Not(Test-Path $dir))
{
    Write-Host "\033[31m\nERROR: could not create package folder.\n\033[0m"
    exit -2
}

mkdir -p "$dir\vendor\river2D\bin\release\"
mkdir -p "$dir\assets\tiles\"
mkdir -p "$dir\bin\release\"

cp ".\vendor\river2D\bin\release\river2Dsoftware.dll" "$dir\vendor\river2D\bin\release\river2Dsoftware.dll"
cp ".\bin\release\mapedit.exe"                        "$dir\bin\release\mapedit.exe"
cp ".\river2D.ini"                                    "$dir\river2D.ini"
cp ".\assets\*"                                       "$dir\assets\"
cp ".\mapedit.lnk"                                    "$dir\mapedit.lnk"
