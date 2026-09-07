if(Test-Path "./vendor/river2D/clean.ps1")
{
    pushd "./vendor/river2D/"
    &./clean.ps1
    popd
}

Write-Host "cleaning up mapedit builds..." -Fore Yellow

if(Test-Path "./bin")
{
    rm "./bin/" -Recurse -Force
}

foreach($file in (gci *.o))
{
    if(Test-Path $file)
    {
        Remove-Item $file
    }
}

foreach($file in (gci *.obj))
{
    if(Test-Path $file)
    {
        Remove-Item $file
    }
}

foreach($file in (gci *.exe))
{
    if(Test-Path $file)
    {
        Remove-Item $file
    }
}

foreach($file in (gci *.lib))
{
    if(Test-Path $file)
    {
        Remove-Item $file
    }
}

foreach($file in (gci *.pdb))
{
    if(Test-Path $file)
    {
        Remove-Item $file
    }
}

Write-Host "cleaned mapedit!`n" -Fore Green
