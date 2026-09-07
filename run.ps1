param
(
    [Parameter(Position = 0)][string]$build,
    [Parameter(Position = 1)][string]$compile_only
)

if(-Not(Test-Path "./bin/" -PathType Container))
{
    mkdir "./bin/"
}

if($build -eq $null -or $build -eq "")
{
    $build = "release"
}

$args_always=@("-DBUILD_WINDOWS",
"src/win32_mapedit_entry.c",
"src/mapedit_input.c",
"src/mapedit_main.c",
"src/mapedit_menus.c",
"src/mapedit_saving.c",
"src/mapedit_tiles.c",
"vendor/river2D/vendor/imgsurf/src/imgsurf_main.c",
"vendor/river2D/vendor/imgsurf/src/imgsurf_format_bmp.c",
"vendor/river2D/vendor/imgsurf/src/imgsurf_format_qoi.c",
"vendor/river2D/vendor/imgsurf/src/imgsurf_format_png.c",
"vendor/river2D/vendor/imgsurf/vendor/datasurf/src/datasurf_formats.c",
"vendor/river2D/vendor/imgsurf/vendor/datasurf/src/datasurf_algo_deflate.c",
"-Iinclude/",
"-Ivendor/river2D/include/",
"-Ivendor/river2D/vendor/imgsurf/include/",
"-Ivendor/river2D/vendor/imgsurf/vendor/datasurf/include/",
"-Ivendor/river2D/vendor/imgsurf/vendor/datasurf/vendor/puddle/include/",
"-Lvendor/river2D/bin/$build",
"-lriver2Dcommon",
"-lgdi32",
"-luser32",
"-std=c99",
"-Wextra", "-Wall", "-Wpedantic", "-Wconversion", "-Wshadow", "-Wsign-compare",
"-Wtype-limits", "-Wunused",
"-Wno-unsafe-buffer-usage", "-Wno-declaration-after-statement", "-Wno-vla",
"-Wno-implicit-void-ptr-cast")

$args_release=@("-O2")

$args_debug=@("-DDEBUG", "-gcodeview", "-O0")
$args_debug_cl=@("/DDEBUG", "/Zi", "/Od")

$args_asan=$args_debug_cl+@("-oa.exe", "/clang:-std=c99", "/DASAN",
"/fsanitize=address", "/MD",
"/link", "/SUBSYSTEM:CONSOLE")

function compile_engine
{
    param($1)

    if(-Not(Test-Path "./vendor/river2D/run.ps1" -PathType Leaf))
    {
        Write-Host"`nERROR: can't find river2D's run script.`n" -Fore Red
    }

    pushd "./vendor/river2D/"
    &./run.ps1 $build
    if($LASTEXITCODE -ne 0)
    {
        exit 3;
    }
    popd
}

function compile
{
    param( [string[]]$1 )

    Write-Host "identifying a compiler..."

    if($build -eq "asan")
    {
        if(-Not(Get-Command clang -ErrorAction SilentlyContinue))
        {
            Write-Host "ERROR: clang-cl needed for address sanitization." -Fore Red
        }
        $script:compiler="clang-cl"
    }
    elseif(Get-Command clang -ErrorAction SilentlyContinue)
    {
        Write-Host "found clang."
        $script:compiler="clang"
    }
    elseif(Get-Command gcc -ErrorAction SilentlyContinue)
    {
        Write-Host "found gcc."
        $script:compiler="gcc"
    }
    else
    {
        Write-Host "ERROR: no suitable compiler found." -Fore Red
    }

    Write-Host ""
    Write-Host "compiling mapedit..." -Fore Cyan
    Write-Host ""

    if(-Not (Test-Path "./bin/$build/" -PathType Container))
    {
        mkdir "./bin/$build/"
    }

    Write-Host "compiling $build build with the following command:"
    Write-Host "$script:compiler $1"
    &$script:compiler @1
    if($LASTEXITCODE -ne 0)
    {
        Write-Host "`nERROR: $script:compiler failed to compile mapedit.`n" -Fore Red
        exit -1
    }
    Move-Item ./a.exe ./bin/$build/mapedit.exe -Force
    if($build -eq "release")
    {
        return;
    }
    Move-Item ./a.pdb ./bin/$build/mapedit.pdb -Force
}

if($build -eq "release")
{
    compile_engine $build
    compile ($args_always + $args_release)
}
elseif($build -eq "debug")
{
    compile_engine $build
    compile ($args_always + $args_debug)
}
elseif($build -eq "asan")
{
    compile_engine $build
    compile ($args_always + $args_asan)
}
else
{
    Write-Host "`nERROR: invalid make config: $build." -Fore Red
    exit 3;
}

Write-Host "`n"

if($compile_only -eq "--compile-only")
{
    exit 0
}

&./bin/$build/mapedit
