
list(APPEND boomerang-db-sources
    db/BasicBlock.cpp
    db/BasicBlock.h
    db/BinaryImage.cpp
    db/BinaryImage.h
    db/CFG.cpp
    db/CFG.h
    db/DataFlow.cpp
    db/DataFlow.h
    db/Global.h
    db/Global.cpp
    db/IBinaryImage.h
    db/IBinarySection.h
    db/IBinarySymbols.h
    db/InsNameElem.cpp
    db/InsNameElem.h
    db/Managed.cpp
    db/Managed.h
    db/Module.cpp
    db/Module.h
    db/Prog.cpp
    db/Prog.h
    db/Register.cpp
    db/Register.h
    db/RTL.cpp
    db/RTL.h
    db/SectionInfo.cpp
    db/SectionInfo.h
    db/Signature.cpp
    db/Signature.h
    db/SymTab.cpp
    db/SymTab.h
    db/Table.cpp
    db/Table.h
    db/Visitor.cpp
    db/Visitor.h

    # exp handling
    db/exp/Exp.cpp
    db/exp/RefExp.h
    db/exp/FlagDef.h
    db/exp/TypeVal.cpp
    db/exp/Unary.cpp
    db/exp/Terminal.h
    db/exp/Location.h
    db/exp/Binary.h
    db/exp/ExpHelp.cpp
    db/exp/Unary.h
    db/exp/Const.cpp
    db/exp/Exp.h
    db/exp/Ternary.h
    db/exp/Ternary.cpp
    db/exp/TypeVal.h
    db/exp/RefExp.cpp
    db/exp/TypedExp.cpp
    db/exp/FlagDef.cpp
    db/exp/Binary.cpp
    db/exp/ExpHelp.h
    db/exp/Terminal.cpp
    db/exp/Location.cpp
    db/exp/TypedExp.h
    db/exp/Operator.cpp
    db/exp/Operator.h
    db/exp/Const.h

    db/proc/LibProc.cpp
    db/proc/LibProc.h
    db/proc/Proc.cpp
    db/proc/Proc.h
    db/proc/UserProc.cpp
    db/proc/UserProc.h

    db/ssl/RTLInstDict.cpp
    db/ssl/RTLInstDict.h
    db/ssl/sslparser.cpp
    db/ssl/sslparser.h
    db/ssl/sslparser_support.cpp
    db/ssl/sslscanner.cpp
    db/ssl/sslscanner.h

    # statement handling
    db/statements/Assign.cpp
    db/statements/Assign.h
    db/statements/Assignment.cpp
    db/statements/Assignment.h
    db/statements/BoolAssign.cpp
    db/statements/BoolAssign.h
    db/statements/BranchStatement.cpp
    db/statements/BranchStatement.h
    db/statements/CallStatement.cpp
    db/statements/CallStatement.h
    db/statements/CaseStatement.cpp
    db/statements/CaseStatement.h
    db/statements/GotoStatement.cpp
    db/statements/GotoStatement.h
    db/statements/ImplicitAssign.cpp
    db/statements/ImplicitAssign.h
    db/statements/ImpRefStatement.cpp
    db/statements/ImpRefStatement.h
    db/statements/JunctionStatement.cpp
    db/statements/JunctionStatement.h
    db/statements/PhiAssign.cpp
    db/statements/PhiAssign.h
    db/statements/ReturnStatement.cpp
    db/statements/ReturnStatement.h
    db/statements/Statement.cpp
    db/statements/Statement.h
    db/statements/StatementHelper.cpp
    db/statements/StatementHelper.h
    db/statements/TypingStatement.cpp
    db/statements/TypingStatement.h
)

set(boomerang-sources "${boomerang-sources};${boomerang-db-sources}" PARENT_SCOPE)