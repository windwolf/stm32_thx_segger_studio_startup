/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : STM32H7xx_Target.js
Purpose : STM32H7xx target script
*/

function Reset() {
  TargetInterface.resetAndStop();
}

function EnableTrace(traceInterfaceType) {
  // TODO: Enable trace
}

