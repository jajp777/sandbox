push-location
cd (Split-Path $PSCommandPath -Parent)

.\build.ps1
copy-item .\builds\handout.Log.pdf "..\..\P�blico\Math\Handout.log.pdf"

pop-location
