$ENGINE_FOLDER='C:\Program Files\Epic Games\UE_4.24'
$PROJECT_NAME='EnglishVR'

########
$ErrorActionPreference = "Stop"
Write-Output "Running on $env:computername..."

$env:enginepath="$ENGINE_FOLDER"
$env:projectname="$PROJECT_NAME"
$env:projectpath=$pwd.Path
$env:projectfilepath="$env:projectpath\$project_name.uproject"
$env:buildconfig='Development'
$env:artifactsfolderlocation=$pwd.Path
$env:artifactsfoldername='__artifacts__'
$env:artifactspath="$env:artifactsfolderlocation\$env:artifactsfoldername"
$env:artifactszip=$pwd.Path + '\output.zip'


& "$env:enginepath\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="$env:projectfilepath" -game -rocket -progress

Write-Output "### Compilation..."
& "$env:enginepath\Engine\Binaries\DotNET\UnrealBuildTool.exe" $env:projectname $env:buildconfig Win64 -project="$env:projectfilepath" -rocket -editorrecompile -progress -noubtmakefiles -NoHotReloadFromIDE -2017

Write-Output "### Erasing previous build folder..."
Remove-Item $env:artifactspath -Recurse -ErrorAction Ignore

Write-Output "### Creating build folder..."
New-Item -Path $env:artifactsfolderlocation -Name $env:artifactsfoldername -ItemType "directory"

Write-Output "### Build and Cook..."
& "$env:enginepath\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="$env:projectfilepath" -noP4 -platform=Win64 -clientconfig="$env:buildconfig" -cook -allmaps -build -stage -pak -archive -archivedirectory="$env:artifactspath"

Write-Output "### Archiving the artifacts..."
& Compress-Archive -Path $env:artifactspath -DestinationPath $env:artifactszip

Write-Output "### $env:artifactszip created"

if(!$?) { Exit $LASTEXITCODE }
