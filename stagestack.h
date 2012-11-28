
#pragma once

#include "consts.h"
#include "stage.h"

class StageStack
{
  private:
    int StackIndex;
    Stage* Stack[MAX_STACK_SIZE];

  public:
    StageStack();
    int Push(Stage* newStage);
    Stage* Pop();
    Stage* Current();
    int GetStackIndex();
    Stage* Item(int index);
		Stage* Previous( Stage* checkStage );
		bool IsEmpty();

};
