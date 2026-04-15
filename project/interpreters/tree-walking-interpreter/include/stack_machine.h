#pragma once


#include "control_flow_graph.h"
#include "json.h"

class StackMachine
{
public:
   StackMachine( const ControlFlowGraph& cfg );
   Json exec();
private:
  ControlFlowGraph mCfg;
};
