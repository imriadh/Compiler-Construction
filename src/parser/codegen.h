#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h"
#include <fstream>
#include <vector>
#include <string>

class CodeGenerator {
private:
    std::ofstream asmFile;
    std::vector<std::string> rawAssembly;

    void generateExpression(ASTNode* node, std::string& targetReg) {
        if (!node) return;
        if (NumberNode* num = dynamic_cast<NumberNode*>(node)) {
            rawAssembly.push_back("\tMOV " + targetReg + ", " + std::to_string((int)num->value));
        } else if (IdentifierNode* id = dynamic_cast<IdentifierNode*>(node)) {
            rawAssembly.push_back("\tMOV " + targetReg + ", [" + id->name + "]");
        } else if (BinaryOpNode* binOp = dynamic_cast<BinaryOpNode*>(node)) {
            std::string leftReg = "AX"; std::string rightReg = "BX";
            generateExpression(binOp->left, leftReg);
            rawAssembly.push_back("\tPUSH " + leftReg);
            generateExpression(binOp->right, rightReg);
            rawAssembly.push_back("\tPOP " + leftReg);
            if (binOp->op == '+') rawAssembly.push_back("\tADD " + leftReg + ", " + rightReg);
            else if (binOp->op == '-') rawAssembly.push_back("\tSUB " + leftReg + ", " + rightReg);
            targetReg = leftReg;
        }
    }

public:
    CodeGenerator(std::string filename) { asmFile.open(filename); }

    void generate(ProgramNode* program) {
        rawAssembly.push_back(".MODEL SMALL");
        rawAssembly.push_back(".STACK 100H");
        rawAssembly.push_back(".DATA");
        for (auto stmt : program->statements) {
            if (DeclarationNode* decl = dynamic_cast<DeclarationNode*>(stmt)) {
                rawAssembly.push_back(decl->varName + " DW 0"); 
            }
        }
        rawAssembly.push_back(".CODE");
        rawAssembly.push_back("MAIN PROC");
        rawAssembly.push_back("\tMOV AX, @DATA");
        rawAssembly.push_back("\tMOV DS, AX");

        for (auto stmt : program->statements) {
            if (AssignmentNode* assign = dynamic_cast<AssignmentNode*>(stmt)) {
                std::string targetReg = "AX";
                generateExpression(assign->expression, targetReg);
                rawAssembly.push_back("\tMOV [" + assign->varName + "], " + targetReg);
            }
        }
        rawAssembly.push_back("\tMOV AH, 4CH");
        rawAssembly.push_back("\tINT 21H");
        rawAssembly.push_back("MAIN ENDP");
        rawAssembly.push_back("END MAIN");

        // Peephole Optimization
        std::vector<std::string> optimized;
        for (size_t i = 0; i < rawAssembly.size(); i++) {
            std::string curr = rawAssembly[i];
            std::string next = (i + 1 < rawAssembly.size()) ? rawAssembly[i+1] : "";
            if (curr.find("MOV AX, AX") != std::string::npos) continue;
            if (curr.find("PUSH AX") != std::string::npos && next.find("POP AX") != std::string::npos) { i++; continue; }
            optimized.push_back(curr);
        }
        for (const auto& line : optimized) asmFile << line << "\n";
        asmFile.close();
    }
};
#endif