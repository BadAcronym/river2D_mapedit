param
(
    [Parameter(Position = 0)][string]$build,
    [Parameter(Position = 1)][string]$compile_only
)

if(-Not(Test-Path ".\obj\"))
{
    &mkdir .\obj\
}

if(-Not(Test-Path ".\build\"))
{
    &mkdir .\build\
}

if(-Not(Test-Path ".\bin\"))
{
    &mkdir .\bin\
}

if($build -eq $null -or $build -eq "")
{
    $build = "release"
}

if($build -eq "asan" -or $build -eq "debug" -or $build -eq "release")
{

    if(Test-Path "./vendor/river2D/run.ps1")
    {
        pushd "./vendor/river2D/"
        &./run.ps1 $build --compile-only
        if($LASTEXITCODE -ne 0)
        {
            popd
            exit -3;
        }
        popd
    }
    else
    {
        Write-Host "ERROR: can't find river2D's run script." -ForegroundColor Red
    }

    Write-Host "`ncompiling mapedit...`n" -Fore Cyan

    premake5 gmake
    pushd "./build/"
    make config=$build`_windows
    popd
}
else
{
    Write-Host "ERROR: invalid make config: '$build'." -ForegroundColor Red
    exit -2;
}

if($LASTEXITCODE -ne 0)
{
    Write-Host "`nERROR: failed to compile mapedit.`n" -ForegroundColor Red
    exit -1;
}

Write-Host "`n"

if($compile_only -eq "--compile-only")
{
    exit 0;
}

if(0 -eq $LASTEXITCODE)
{
    $target = ".\bin\$build\mapedit.exe"
    Write-Host "`nrunning $target..."
    Invoke-Expression $target
}
