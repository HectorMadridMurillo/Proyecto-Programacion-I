# Proyecto-Programacion-I
Sistema de Planilla Hospitalaria
Integrantes
Nombre completo	/ Cédula
Raymond Saldaña Caseres / 7-0330-0099
Britanny Mariel Cordero Arroyo / 2-0910-0341
Héctor Iván Madrid Murillo / 8-0161-0280

Cómo compilar
Requiere un compilador de C++11 o superior (g++, clang++ o MSVC). No usa librerías externas.

Linux / macOS
bash
g++ -std=c++11 -Wall -Wextra -o hospital \
    main.cpp Sistema.cpp Empleado.cpp Departamento.cpp \
    ComprobantePago.cpp Repositorio.cpp Consola.cpp
Windows (MinGW)
bash
g++ -std=c++11 -Wall -Wextra -o hospital.exe ^
    main.cpp Sistema.cpp Empleado.cpp Departamento.cpp ^
    ComprobantePago.cpp Repositorio.cpp Consola.cpp
Visual Studio
Crear un proyecto de aplicación de consola en C++.

Agregar todos los archivos .cpp y .h al proyecto.

Compilar con Ctrl + Shift + B.

Cómo usarlo
Ejecutar el programa:

bash
./hospital
Al iniciar se cargan automáticamente los archivos departamentos.txt, empleados.txt y planilla.txt si existen en la misma carpeta. Si no existen, se crean al guardar el primer cambio.

El menú principal muestra:

text
===== PLANILLA HOSPITALARIA =====
1. Registrar departamento
2. Registrar puesto
3. Registrar empleado
4. Ver departamentos
5. Ver empleados
6. Actualizar jornada
7. Calcular planilla
8. Ver comprobantes
9. Procesar pago
0. Salir
Orden recomendado
Crear departamentos (opción 1).

Agregar puestos a cada departamento (opción 2).

Registrar empleados indicando tipo (1 = Médico, 2 = Enfermero, 3 = Administrativo) y el ID de un departamento existente (opción 3).

Opcionalmente, actualizar la jornada de un empleado (opción 6).

Calcular la planilla indicando ID del empleado, periodo (por ejemplo 2026-11) y porcentaje de deducción entre 0 y 100 (opción 7).

Consultar comprobantes generados (opción 8).

Procesar el pago de un comprobante pendiente (opción 9).

Para salir, elegir la opción 0.
