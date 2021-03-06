#pragma region License
/*
 * This file is part of the Boomerang Decompiler.
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 */
#pragma endregion License
#include "RTLInstDict.h"

#include "boomerang/ssl/RTL.h"
#include "boomerang/ssl/exp/Binary.h"
#include "boomerang/ssl/exp/Const.h"
#include "boomerang/ssl/exp/Location.h"
#include "boomerang/ssl/exp/Terminal.h"
#include "boomerang/ssl/parser/SSL2ParserDriver.h"
#include "boomerang/ssl/statements/Assign.h"
#include "boomerang/ssl/type/FloatType.h"
#include "boomerang/ssl/type/IntegerType.h"
#include "boomerang/util/log/Log.h"


RTLInstDict::RTLInstDict(bool verboseOutput)
    : m_verboseOutput(verboseOutput)
    , m_endianness(Endian::Little)
{
}


RTLInstDict::~RTLInstDict()
{
}


int RTLInstDict::insert(const QString &name, std::list<QString> &params, const RTL &rtl)
{
    QString opcode = name.toUpper();

    opcode.remove(".");

    if (m_instructions.find(opcode) == m_instructions.end()) {
        m_instructions.emplace(opcode, TableEntry(params, rtl));
    }
    else {
        return m_instructions[opcode].appendRTL(params, rtl);
    }

    return 0;
}


bool RTLInstDict::readSSLFile(const QString &sslFileName)
{
    LOG_MSG("Loading machine specifications from '%1'...", sslFileName);
    // emptying the rtl dictionary
    m_instructions.clear();

    // Clear all state
    reset();

    SSL2ParserDriver drv(this);

    if (drv.parse(sslFileName.toStdString()) != 0) {
        return false;
    }

    if (m_verboseOutput) {
        OStream q_cout(stdout);
        q_cout << "\n=======Expanded RTL template dictionary=======\n";
        print(q_cout);
        q_cout << "\n==============================================\n\n";
    }

    return true;
}


void RTLInstDict::print(OStream &os /*= std::cout*/)
{
    for (auto &elem : m_instructions) {
        // print the instruction name
        os << (elem).first << "  ";

        // print the parameters
        const std::list<QString> &params((elem).second.m_params);
        int i = params.size();

        for (auto s = params.begin(); s != params.end(); ++s, i--) {
            os << *s << (i != 1 ? "," : "");
        }

        os << "\n";

        // print the RTL
        RTL &rtlist = (elem).second.m_rtl;
        rtlist.print(os);
        os << "\n";
    }
}


std::pair<QString, DWord> RTLInstDict::getSignature(const QString &instructionName,
                                                    bool *found) const
{
    // Take the argument, convert it to upper case and remove any .'s
    const QString sanitizedName = QString(instructionName).remove(".").toUpper();

    // Look up the dictionary
    const auto it = m_instructions.find(sanitizedName);
    if (it != m_instructions.end()) {
        if (found) {
            *found = true;
        }
        return { sanitizedName, (it->second).m_params.size() };
    }
    else if (found) {
        *found = false;
    }

    return { sanitizedName, (DWord)-1 };
}


std::unique_ptr<RTL> RTLInstDict::instantiateRTL(const QString &name, Address natPC,
                                                 const std::vector<SharedExp> &args)
{
    // TODO try to retrieve fast instruction mappings
    // before trying the verbose instructions
    auto dict_entry = m_instructions.find(name);
    if (dict_entry == m_instructions.end()) {
        return nullptr; // instruction not found
    }

    TableEntry &entry(dict_entry->second);
    std::unique_ptr<RTL> rtl = instantiateRTL(entry.m_rtl, natPC, entry.m_params, args);
    if (rtl) {
        return rtl;
    }
    else {
        LOG_ERROR("Cannot instantiate instruction '%1' at address %2: "
                  "Instruction has %3 parameters, but got %4 arguments",
                  name, natPC, entry.m_params.size(), args.size());
        return nullptr;
    }
}


std::unique_ptr<RTL> RTLInstDict::instantiateRTL(RTL &existingRTL, Address natPC,
                                                 std::list<QString> &params,
                                                 const std::vector<SharedExp> &args)
{
    if (params.size() != args.size()) {
        return nullptr;
    }

    // Get a deep copy of the template RTL
    std::unique_ptr<RTL> newList(new RTL(existingRTL));
    newList->setAddress(natPC);

    // Iterate through each Statement of the new list of stmts
    for (Statement *ss : *newList) {
        // Search for the formals and replace them with the actual arguments
        auto arg = args.begin();

        for (QString paramName : params) {
            /* Simple parameter - just construct the formal to search for */
            Location param(opParam, Const::get(paramName), nullptr);
            ss->searchAndReplace(param, *arg);
            ++arg;
        }
        assert(arg == args.end());
        ss->fixSuccessor();

        if (m_verboseOutput) {
            OStream q_cout(stdout);
            q_cout << "            " << ss << "\n";
        }
    }

    // Perform simplifications, e.g. *1 in Pentium addressing modes
    for (Statement *s : *newList) {
        s->simplify();
    }

    return newList;
}


void RTLInstDict::reset()
{
    m_regDB.clear();

    m_definedParams.clear();
    m_flagFuncs.clear();
    m_instructions.clear();
}


RegDB *RTLInstDict::getRegDB()
{
    return &m_regDB;
}


const RegDB *RTLInstDict::getRegDB() const
{
    return &m_regDB;
}
