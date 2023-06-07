
if (Test-Path ".\bin")
{
   # folder exists
}
else
{
    # folder doesn't exist
    mkdir ".\bin"
}

#$env:Path += "C:\qt\5.15.0\mingw81_64\bin\windeployqt.exe"

Write-Output "Source"
$src = (Get-ChildItem ".\build-*-Release\*.exe").FullName
Write-Output $src

Write-Output "Destination"
$dst = (Get-Item ".\bin").FullName
Write-Output $dst

Write-Output "Deploying..."
Copy-Item $src $dst
C:\Qt\5.15.0\mingw81_64\bin\windeployqt.exe $src --dir $dst --verbose 2
Write-Output "Done."