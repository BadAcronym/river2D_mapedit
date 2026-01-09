Invoke-Expression ".\run debug -compile_only"
if(-Not(Test-Path ".\bin\debug\mapedit.exe"))
{
    Write-Host "no executable to debug."
    return;
}

&raddbg
