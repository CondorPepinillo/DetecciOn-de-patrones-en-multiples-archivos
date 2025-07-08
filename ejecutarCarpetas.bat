@echo off
setlocal enabledelayedexpansion

:: Configuración
set EXE=busquedaBoyer.exe
set CARPETA=./datasets/20Archivos
set N=20
set ARCHIVO_PATRONES=./patrones/100patrones.txt
set OUT=resultados_boyer.csv

:: Inicializa el CSV con encabezados
echo Archivo;Patron;Ocurrencias;Tiempo(s);N_Textos > %OUT%

:: Ejecuta el programa y guarda la salida en un archivo temporal
%EXE% %CARPETA% 2 %ARCHIVO_PATRONES% > temp_output.txt

:: Procesa la salida línea por línea
for /f "tokens=1,2,3,4" %%A in (temp_output.txt) do (
    echo %%A;%%B;%%C;%%D;%N% >> %OUT%
)

:: Limpieza
del temp_output.txt >nul 2>&1

echo Resultados guardados en %OUT%
pause