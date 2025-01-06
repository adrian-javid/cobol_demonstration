000100 IDENTIFICATION DIVISION.
000200  PROGRAM-ID. App_mainLoop.
000300  AUTHOR. Me.
000400  DATE-WRITTEN. 01/06/2025.
000500  DATE-COMPILED. 01/06/2025.
000600
000700 ENVIRONMENT DIVISION.
000800  CONFIGURATION SECTION.
000900   SOURCE-COMPUTER. Linux.
001000   OBJECT-COMPUTER. Linux.
001100  INPUT-OUTPUT SECTION.
001200   FILE-CONTROL.
001300   I-O-CONTROL.
001400
001500 DATA DIVISION.
001600  FILE SECTION.
001700  WORKING-STORAGE SECTION.
001800
001900  LINKAGE SECTION.
002000  REPORT SECTION.
002100  SCREEN SECTION.
002200
002300 PROCEDURE DIVISION.
002400     CALL "App_processEvents".
002500     CALL "App_processKeyboardInput".
002600     CALL "App_updateCellGrid".
002700     CALL "App_drawCellGrid".
002800     CALL "App_renderCanvas".
