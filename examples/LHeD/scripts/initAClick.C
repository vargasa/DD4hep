//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================

// ROOT include files
#include "TInterpreter.h"
#include "TSystem.h"
#include "RVersion.h"

// C/C++ include files
#include <iostream>
#include <string>

std::string make_str(const char* data)  {
  if ( !data )   {
    std::cout << "make_str:  '" << (data ? data : "Bad-Pointer") << "'" << std::endl;
    return std::string("");
  }
  return std::string(data);
}

/// Process a single command in the ROOT interpreter
int processCommand(const char* command, bool end_process)   {
  int status;
  // Disabling auto-parse is a hack required by a bug in ROOT
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
  gInterpreter->SetClassAutoparsing(false);
  status = gInterpreter->ProcessLine(command);
  gInterpreter->SetClassAutoparsing(true);
#else
  status = gInterpreter->ProcessLine(command);
#endif
  ::printf("+++ Status(%s) = %d\n",command,status);
  if ( end_process )  {
    gInterpreter->ProcessLine("gSystem->Exit(0)");
  }
  return status;
}

/// Process a ROOT AClick given a file
int processMacro(const char* macro, bool end_process)   {
  std::string cmd = ".X ";
  cmd += macro;
  cmd += ".C+()";
  return processCommand(cmd.c_str(), end_process);
}

/// Initialize the ROOT environment to compile and execute a ROOT ACLick
int initAClick(const char* command=0)  {
  std::string rootsys = make_str(gSystem->Getenv("ROOTSYS"));
  std::string geant4  = make_str(gSystem->Getenv("G4INSTALL"));
  std::string dd4hep  = make_str(gSystem->Getenv("DD4hepINSTALL"));
  std::string clhep   = make_str(gSystem->Getenv("CLHEP_ROOT_DIR"));
  std::string defs    = "";
  std::string libs    = " -L"+rootsys+"/lib";
  std::string inc     = " -I"+dd4hep+"/examples/LHeD/scripts -I"+dd4hep + " -I"+dd4hep+"/include"+clhep+"/include -I"+geant4+"/include";
  libs += " -L"+dd4hep+"/lib -lDDCore -lDDG4";
  if ( !geant4.empty() )  {
    inc  += " -I"+geant4+"/include/Geant4";
#ifdef __APPLE__
    libs += (" -L"+geant4+"/lib");
#else
    libs += (" -L"+geant4+"/lib -L"+geant4+"/lib64");
#endif
  }
  if ( !clhep.empty() )  {
    // A bit unclear how to deal with CLHEP libraries here, 
    // if CLHEP is not included in Geant4...
    inc += " -I"+clhep+"/include";
    std::string clhep_lib = make_str(gSystem->Getenv("CLHEP_LIBRARY_PATH"));
    if ( !clhep_lib.empty() ) libs += " -L"+clhep_lib+"/lib";
  }
  inc += " -Wno-shadow -g -O0" + defs;
#ifndef __APPLE__
  libs += " -lCore -lMathCore -pthread -lm -ldl -rdynamic";
#endif
  gSystem->AddIncludePath(inc.c_str());
  gSystem->AddLinkedLibs(libs.c_str());
  std::cout << "+++ Includes:   " << gSystem->GetIncludePath() << std::endl;
  std::cout << "+++ Linked libs:" << gSystem->GetLinkedLibs()  << std::endl;
  int ret = 0;  // gSystem->Load("libDDG4Plugins");
  if ( 0 == ret )   {
    if ( command )  {
      processCommand(command, true);
    }
  }
  return ret;
}
