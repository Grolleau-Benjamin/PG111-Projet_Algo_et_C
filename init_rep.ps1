# Create directories
New-Item -ItemType Directory -Force -Path "bin", "lib"
New-Item -ItemType Directory -Force -Path "algo/binio/obj", "algo/binio/bin"
New-Item -ItemType Directory -Force -Path "algo/huffman/obj", "algo/huffman/bin"
New-Item -ItemType Directory -Force -Path "algo/mtf/obj", "algo/mtf/bin"


$pwd = (Get-Location).Path


$currentLD_LIBRARY_PATH = [System.Environment]::GetEnvironmentVariable("LD_LIBRARY_PATH", [System.EnvironmentVariableTarget]::User)
$newLD_LIBRARY_PATH = "$currentLD_LIBRARY_PATH;$pwd\lib"
[System.Environment]::SetEnvironmentVariable("LD_LIBRARY_PATH", $newLD_LIBRARY_PATH, [System.EnvironmentVariableTarget]::User)

Write-Output "LD_LIBRARY_PATH has been set to $newLD_LIBRARY_PATH"
