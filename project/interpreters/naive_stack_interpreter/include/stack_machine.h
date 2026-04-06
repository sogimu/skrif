#pragma once


#include "control_flow_graph.h"
#include "json.h"

class StackMachine
{
public:
   StackMachine( const ControlFlowGraph& cfg );
   Json exec();
private:
   // std::list< std::shared_ptr< EnvScope > > clousures1;
  std::map< std::string, std::shared_ptr< EnvScope > > clousures1;
  ControlFlowGraph mCfg;
};
