push-location
cd (Split-Path $PSCommandPath -Parent)

.\build.ps1
cd .\builds

dir -Filter *.pdf | % {
    Write-Verbose "Copying $_"
    copy-item $_.FullName "..\..\..\P�blico\Math\$($_.Name)"
}


pop-location
