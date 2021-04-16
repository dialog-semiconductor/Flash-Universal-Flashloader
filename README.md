# Flash-Universal-Flashloader

A flash loader (sometimes called flash driver or flash algorithm) is a debugger plug-in which helps the debugger download (program) code/data into a particular flash memory device from a particular MCU. Technically you have one plug-in binary per toolchain-MCU-flash combo.

This project is an Adesto universal flash loader. The Adesto-specific piece of the project (under the adesto folder) is generic and works on all Adesto memory products as the differences in terms of erase/program/verify(read) operations are minimal. The differences are in general:

•	Address size (3 bytes vs. 4 bytes)
•	Read dummy cycles (in fact if using SPI read all products use 8 dummy cycles for the “fast read” command, opcode 0Bh - so there is no difference)
•	The unprotect step (exists only on the EcoXiP/ATXPxxx family).

The universal Adesto layer reads JEDEC ID to detect the target Adesto device and based on that determines the above parameters.

What varies from one flash loader to the other is the following item:

•	Tool chain specifics: project files, linker files (under the each mcy in  the cu folder); flash loader API (unde the toolchains folder)
•	Host MCU specifics: initialization sequence, SDK driver API, xSPI host controller architecture (under the each mc
