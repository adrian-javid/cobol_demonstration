000100
000200 IDENTIFICATION DIVISION.
000300  PROGRAM-ID. App_mainLoop.
000400  AUTHOR. Me.
000500  DATE-WRITTEN. 01/06/2025
000600  DATE-COMPILED. 01/06/2025
000700*
000800 ENVIRONMENT DIVISION.
000900  CONFIGURATION SECTION.
001000   SOURCE-COMPUTER. Linux.
001100   OBJECT-COMPUTER. Linux.
001200  INPUT-OUTPUT SECTION.
001300   FILE-CONTROL.
001400   I-O-CONTROL.
001500*
001600 DATA DIVISION.
001700  FILE SECTION.
001800  WORKING-STORAGE SECTION.
001900*
002000  LINKAGE SECTION.
002100  REPORT SECTION.
002200  SCREEN SECTION.
002300*
002400 PROCEDURE DIVISION.
002500     CALL "App_processEvents".
002600     CALL "App_processKeyboardInput".
002700     CALL "App_updateCellGrid".
002800     CALL "App_drawCellGrid".
002900     CALL "App_renderCanvas".
