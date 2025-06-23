#ifndef CLI_DEBUGER_H
#define CLI_DEBUGER_H

#include "Globals.h"

void CLIDebuger (){
    Serial.print (DefaultLocation);
    Serial.print (":" );
    Serial.println (DefaultMessage);
}

#endif