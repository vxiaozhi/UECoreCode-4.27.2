if not "%UE_SDKS_ROOT%"=="" (for /F "tokens=*" %%A in (%UE_SDKS_ROOT%\HostWin64\Android\OutputEnvVars.txt) do set %%A)