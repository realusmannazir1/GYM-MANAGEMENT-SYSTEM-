$ErrorActionPreference = "Stop"
$exe = "$env:BUILD_EXE"
if (-not (Test-Path $exe)) { Write-Output "EXE NOT FOUND: $exe"; exit 1 }
$cert = Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Select-Object -First 1
if ($cert) {
    $r = Set-AuthenticodeSignature -FilePath $exe -Certificate $cert -HashAlgorithm SHA256
    Write-Output ("Status=" + $r.Status + " Signer=" + $r.SignerCertificate.Subject)
} else {
    Write-Output "NO CODESIGNING CERT FOUND"
}
