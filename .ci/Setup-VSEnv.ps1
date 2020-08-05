param (
	[string]$architecture="amd64",
	[string]$toolset=""
)

. $PSScriptRoot/Invoke-BatchFile.ps1

# Find latest Visual Studio installation
$vs_path = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -nologo -utf8 -format value -property installationPath

# Don't send telemetry data
# https://developercommunity.visualstudio.com/solutions/699819/view.html
[Environment]::SetEnvironmentVariable("VSCMD_SKIP_SENDTELEMETRY", "1") 
# Debug setup script if necessary
#[Environment]::SetEnvironmentVariable("VSCMD_DEBUG", "3")

$host_architecture = "x86"
if ($env:PROCESSOR_ARCHITECTURE -eq "AMD64") {
	$host_architecture = "amd64"
}
if (![string]::IsNullOrEmpty($toolset)) {
	$toolset = "-vcvars_ver=$toolset"
}

# Setup compilation environment
# https://devblogs.microsoft.com/cppblog/finding-the-visual-c-compiler-tools-in-visual-studio-2017/
#Invoke-BatchFile "$vs_path\Common7\Tools\vsdevcmd.bat" "-help"
Invoke-BatchFile "$vs_path\Common7\Tools\vsdevcmd.bat" "-no_logo -arch=$architecture -host_arch=$host_architecture $toolset"
