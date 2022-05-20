#include "compiler.h"

int Compiler_CreateListing(struct Compiler *compiler, struct Tree_Node *syntaxTree)
{
    Compiler_CreateListingPreamble(compiler);    
    Compiler_GrammarListing(compiler, syntaxTree);

    if (compiler->errorCode != COMPILER_ERR_NoError) {
        Man_PrintCompilerError(compiler);
    }
    return 0;
}

int Compiler_CreateListingPreamble(struct Compiler *compiler)
{
    if (compiler == NULL) {
        return 1;
    }

    fprintf(compiler->listingFile, "; %s\n\n"
                                   "global _start\n\n"
                                   "section .text\n\n"
                                   "_start:\n"
                                   "\t\tcall main\n"
                                   "\t\tmov rdi, rax\n"
                                   "\t\tmov rax, 0x3c\n" // 0x3c is system function for exit
                                   "\t\tsyscall\n\n",
                                   compiler->listingName);
    return 0;
}

int Compiler_GrammarListing(struct Compiler *compiler, struct Tree_Node *grammarNode)
{
    if (compiler == NULL || grammarNode == NULL) {
        return 1;
    }

    struct Tree_Node *currentNode = grammarNode;

    while (currentNode->token->type != TOKEN_TYPE_Endmarker) {
        if (currentNode->token->type != TOKEN_TYPE_Operator) {
            return 1;
        }

        switch (currentNode->token->operator) {
            case TOKEN_KW_Function: 
                compiler->localVarCount = 0;
                strcpy(compiler->currentScope, currentNode->left->token->identifier);
                Compiler_FunctionListing(compiler, currentNode->left);
                break;

            case TOKEN_KW_VarDecl: 
                strcpy(compiler->currentScope, "global");
                Compiler_VarDeclListing (compiler, currentNode->left);
                break;

            default: return 1;
        }

        currentNode = currentNode->right;
        continue;
    }
    
    return 0;
}

/* GLOBALS NOT IMPLEMENTED YET */
int Compiler_VarDeclListing(struct Compiler *compiler, struct Tree_Node *varDecLNode)
{
    if (compiler == NULL || varDecLNode == NULL) {
        return 1;
    }

    struct Tree_Node *currentNode = varDecLNode;
    while (currentNode->right != NULL) {
        compiler->localVarCount++;
        HashTable_Insert(compiler->identifiersList, varDecLNode->left->left->token->identifier,
                         compiler->currentScope,    compiler->localVarCount);
        Compiler_ExpressionListing(compiler, varDecLNode->left);
        fprintf(compiler->listingFile, "\t\tadd rsp, 8 * 1\n");
        currentNode = currentNode->right;
    }

    return 0;
}

int Compiler_IfStatementListing(struct Compiler *compiler, struct Tree_Node *ifNode)
{
    if (compiler == NULL || ifNode == NULL) {
        return 1;
    }
    
    int ifFalse = compiler->labelCount;
    compiler->labelCount += 1;

    Compiler_ExpressionListing(compiler, ifNode->left);
    fprintf(compiler->listingFile, "\t\tpop rax\n"
                                   "\t\tcmp rax, 0\n"
                                   "\t\tje .L%d\n", ifFalse);

    if (ifNode->right->token->type == TOKEN_TYPE_Operator &&
        ifNode->right->token->operator == TOKEN_KW_Else) {

        int skipElse = compiler->labelCount;
        compiler->labelCount += 1;

        Compiler_MultilineOperatorListing(compiler, ifNode->right->left);
        fprintf(compiler->listingFile, "\t\tjmp .L%d\n"
                                       ".L%d:\n", 
                                       skipElse, ifFalse);

        Compiler_MultilineOperatorListing(compiler, ifNode->right->right);
        fprintf(compiler->listingFile, ".L%d:\n", skipElse);
        return 0;
    }

    Compiler_MultilineOperatorListing(compiler, ifNode->right);
    fprintf(compiler->listingFile, ".L%d:\n", ifFalse);
    return 0;
}

/* NOT IMPLEMENTED YET */
int Compiler_ForStatementListing(struct Compiler *compiler, struct Tree_Node *forNode)
{
    if (compiler == NULL || forNode == NULL) {
        return 1;
    }

    return 0;
}

int Compiler_WhileStatementListing(struct Compiler *compiler, struct Tree_Node *whileNode)
{
    if (compiler == NULL || whileNode == NULL) {
        return 1;
    }

    int whileLabel = compiler->labelCount;
    int endLabel   = compiler->labelCount + 1;
    compiler->labelCount += 2;

    fprintf(compiler->listingFile, ".L%d:\n", whileLabel);

    Compiler_ExpressionListing(compiler, whileNode->left);
    fprintf(compiler->listingFile, "\t\tpop rax\n"
                                   "\t\tcmp rax, 0\n"
                                   "\t\tje .L%d\n", endLabel);

    Compiler_MultilineOperatorListing(compiler, whileNode->right);
    fprintf(compiler->listingFile, "\t\tjmp .L%d\n"
                                   ".L%d:\n",
                                   whileLabel, endLabel);
    return 0;
}

int Compiler_MultilineOperatorListing(struct Compiler *compiler, struct Tree_Node *operatorNode)
{
    if (compiler == NULL || operatorNode == NULL) {
        return 1;
    }

    struct Tree_Node *currentNode = operatorNode;
    while (currentNode != NULL) {
        if ((currentNode->left->token->type == TOKEN_TYPE_Operator) &&
            (currentNode->left->token->operator == TOKEN_KW_If    ||
             currentNode->left->token->operator == TOKEN_KW_While ||
             currentNode->left->token->operator == TOKEN_KW_VarDecl)) {

            switch (currentNode->left->token->operator) {
                case TOKEN_KW_While:   Compiler_WhileStatementListing(compiler, currentNode->left); break;
                case TOKEN_KW_If:      Compiler_IfStatementListing   (compiler, currentNode->left); break;
                case TOKEN_KW_VarDecl: Compiler_VarDeclListing       (compiler, currentNode->left); break;
                default: return 1;
            }

            currentNode = currentNode->right;
            continue;
        }
         
        Compiler_ExpressionListing(compiler, currentNode->left);
        fprintf(compiler->listingFile, "\t\tadd rsp, 8 * 1\n");
        currentNode = currentNode->right;
        continue;   
    }

    return 0;
}

int Compiler_ExpressionListing(struct Compiler *compiler, struct Tree_Node *expressionNode)
{
    if (compiler == NULL || expressionNode == NULL) {
        return 1;
    }

    Compiler_ExpressionListing(compiler, expressionNode->left);
    Compiler_ExpressionListing(compiler, expressionNode->right);

    if (expressionNode->token->type == TOKEN_TYPE_Operator) {
        switch (expressionNode->token->operator) {
            case TOKEN_OP_Comma:
                // excluding top stack element as the value of comma
                // operator is the value of the rightest operand
                fprintf(compiler->listingFile, "\t\tadd rsp, 8 * 1\n");
                break;

            case TOKEN_OP_Bitor:
                fprintf(compiler->listingFile, "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tmov rbx, rax\n"
                                               "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tor rax, rbx\n"
                                               "\t\timul rax, 1000\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Bitxor:
                fprintf(compiler->listingFile, "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tmov rbx, rax\n"
                                               "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\txor rax, rbx\n"
                                               "\t\timul rax, 1000\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Bitand:
                fprintf(compiler->listingFile, "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tmov rbx, rax\n"
                                               "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tand rax, rbx\n"
                                               "\t\timul rax, 1000\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Bitshr:
                fprintf(compiler->listingFile, "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tmov rcx, rax\n"
                                               "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tsar rax, cl\n"
                                               "\t\timul rax, 1000\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Bitshl:
                fprintf(compiler->listingFile, "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tmov rcx, rax\n"
                                               "\t\tpop rax\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tsal rax, cl\n"
                                               "\t\timul rax, 1000\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Add:
                if (expressionNode->left == NULL) {
                    fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                                   "\t\tmov rax, 0\n"
                                                   "\t\tadd rax, rbx\n"
                                                   "\t\tpush rax\n");
                    break;
                }
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tadd rax, rbx\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Sub:
                if (expressionNode->left == NULL) {
                    fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                                   "\t\tmov rax, 0\n"
                                                   "\t\tsub rax, rbx\n"
                                                   "\t\tpush rax\n");
                    break;
                }
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tsub rax, rbx\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Mul:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\timul rax, rbx\n"
                                               "\t\tmov rcx, 1000\n"
                                               "\t\tidiv rcx\n"
                                               "\t\tpush rax\n");
                break;
                
            case TOKEN_OP_Div:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\timul rax, 1000\n"
                                               "\t\tidiv rbx\n"
                                               "\t\tpush rax\n");
                break;

            case TOKEN_OP_Mod:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tmov rcx, rax\n"
                                               "\t\timul rax, 1000\n"
                                               "\t\tidiv rbx\n"
                                               "\t\timul rax, rbx\n"
                                               "\t\tmov rdx, 1000\n"
                                               "\t\tidiv rdx\n"
                                               "\t\tsub rcx, rax\n"
                                               "\t\tpush rcx\n"); 
                break;

            case TOKEN_OP_Lor:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, 0\n"
                                               "\t\tjne .L%d\n"
                                               "\t\tcmp rbx, 0\n"
                                               "\t\tje .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 1000\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 0\n"
                                               ".L%d:\n",
                                               compiler->labelCount,     compiler->labelCount + 1, compiler->labelCount,
                                               compiler->labelCount + 2, compiler->labelCount + 1, compiler->labelCount + 2);
                compiler->labelCount += 3;
                break;

            case TOKEN_OP_Land:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, 0\n"
                                               "\t\tje .L%d\n"
                                               "\t\tcmp rbx, 0\n"
                                               "\t\tje .L%d\n"
                                               "\t\tpush qword 1000\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 0\n"
                                               ".L%d:\n",
                                               compiler->labelCount, compiler->labelCount, compiler->labelCount + 1,
                                               compiler->labelCount, compiler->labelCount + 1);
                compiler->labelCount += 2;
                break;

            case TOKEN_OP_Equals:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, rbx\n"
                                               "\t\tje .L%d\n"
                                               "\t\tpush qword 0\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 1000\n"
                                               ".L%d:\n",
                                               compiler->labelCount, compiler->labelCount + 1,
                                               compiler->labelCount, compiler->labelCount + 1);
                compiler->labelCount += 2;
                break;

            case TOKEN_OP_Nequals:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, rbx\n"
                                               "\t\tjne .L%d\n"
                                               "\t\tpush qword 0\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 1000\n"
                                               ".L%d:\n",
                                               compiler->labelCount, compiler->labelCount + 1,
                                               compiler->labelCount, compiler->labelCount + 1);
                compiler->labelCount += 2;
                break;

            case TOKEN_OP_Less:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, rbx\n"
                                               "\t\tjl .L%d\n"
                                               "\t\tpush qword 0\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 1000\n"
                                               ".L%d:\n",
                                               compiler->labelCount, compiler->labelCount + 1,
                                               compiler->labelCount, compiler->labelCount + 1);
                compiler->labelCount += 2;
                break;

            case TOKEN_OP_Greater:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, rbx\n"
                                               "\t\tjg .L%d\n"
                                               "\t\tpush qword 0\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 1000\n"
                                               ".L%d:\n",
                                               compiler->labelCount, compiler->labelCount + 1,
                                               compiler->labelCount, compiler->labelCount + 1);
                compiler->labelCount += 2;
                break;

            case TOKEN_OP_Lesseq:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, rbx\n"
                                               "\t\tjle .L%d\n"
                                               "\t\tpush qword 0\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 1000\n"
                                               ".L%d:\n",
                                               compiler->labelCount, compiler->labelCount + 1,
                                               compiler->labelCount, compiler->labelCount + 1);
                compiler->labelCount += 2;
                break;

            case TOKEN_OP_Greatereq:
                fprintf(compiler->listingFile, "\t\tpop rbx\n"
                                               "\t\tpop rax\n"
                                               "\t\tcmp rax, rbx\n"
                                               "\t\tjge .L%d\n"
                                               "\t\tpush qword 0\n"
                                               "\t\tjmp .L%d\n"
                                               ".L%d:\n"
                                               "\t\tpush qword 1000\n"
                                               ".L%d:\n",
                                               compiler->labelCount, compiler->labelCount + 1,
                                               compiler->labelCount, compiler->labelCount + 1);
                compiler->labelCount += 2;
                break;

            case TOKEN_KW_Return:
                fprintf(compiler->listingFile, "\t\tpop rax\n"
                                               "\t\tret\n");
                break;

            case TOKEN_OP_Assignment:
                fprintf(compiler->listingFile, "\t\tpop rax\n"
                                               "\t\tpop rbx\n"
                                               "\t\tmov [rbp - 8 * %d], rax\n"
                                               "\t\tpush rax\n",
                                               compiler->identifiersList->data[HashTable_Find(compiler->identifiersList, 
                                                                                              expressionNode->left->token->identifier,
                                                                                              compiler->currentScope)].value);
                break;

            case TOKEN_OP_FunctionCall:
                Compiler_FunctionArgsListing(compiler, expressionNode->right);
                fprintf(compiler->listingFile, "\t\tcall %s\n", expressionNode->left->token->identifier);
                fprintf(compiler->listingFile, "\t\tadd rsp, 8 * %d\n", Compiler_CountLocalVariables(compiler, expressionNode->right));
                fprintf(compiler->listingFile, "\t\tpush rax\n");
                break;

            default:
                compiler->errorCode = COMPILER_ERR_ImpossibleOperator;
                break;
        }   
    }
    
    if (expressionNode->token->type == TOKEN_TYPE_Number) {
        fprintf(compiler->listingFile, "\t\tpush qword %" PRId64 "\n", expressionNode->token->number);
    }
    
    if (expressionNode->token->type == TOKEN_TYPE_Identifier) {
        fprintf(compiler->listingFile, "\t\tpush qword [rbp - 8 * %d]\n", 
                                       compiler->identifiersList->data[HashTable_Find(compiler->identifiersList, 
                                                                                      expressionNode->token->identifier,
                                                                                      compiler->currentScope)].value);
    }
    
    if (expressionNode->token->type == TOKEN_TYPE_String) {
        /* NOT IMPLEMENTED YET */
    }
    return 0;
}

int Compiler_FunctionArgsListing(struct Compiler *compiler, struct Tree_Node *functionNode)
{
    if (compiler == NULL || functionNode == NULL) {
        return 1;
    }

    if (functionNode->token->type == TOKEN_TYPE_Operator &&
        functionNode->token->operator == TOKEN_OP_Comma) {
        
        Compiler_ExpressionListing(compiler, functionNode->right);
        Compiler_FunctionArgsListing(compiler, functionNode->left);
    }
    else {
        Compiler_ExpressionListing(compiler, functionNode);
    }

    return 0;
}

int Compiler_FunctionListing(struct Compiler *compiler, struct Tree_Node *functionNode)
{
    if (compiler == NULL || functionNode == NULL) {
        return 1;
    }

    fprintf(compiler->listingFile, "%s:\n", functionNode->token->identifier);
    Compiler_FunctionPreambleListing(compiler, functionNode);
    
    Compiler_FunctionParamsListing(compiler, functionNode->left);
    Compiler_MultilineOperatorListing(compiler, functionNode->right);

    Compiler_FunctionPostambleListing(compiler, functionNode);
    return 0;
}

int Compiler_FunctionParamsListing(struct Compiler *compiler, struct Tree_Node *functionNode)
{
    if (compiler == NULL || functionNode == NULL) {
        return 1;
    }
    
    
    return 0;    
}

int Compiler_FunctionPreambleListing(struct Compiler *compiler, struct Tree_Node *functionNode)
{
    if (compiler == NULL || functionNode == NULL) {
        return 1;
    }

    int localVars = Compiler_CountLocalVariables(compiler, functionNode);
    fprintf(compiler->listingFile, "\t\t; preamble\n"
                                   "\t\tpush rbp\n"
                                   "\t\tmov rbp, rsp\n"
                                   "\t\tsub rsp, 8 * %d\n\n",
                                   localVars);
    return 0;
}

int Compiler_FunctionPostambleListing(struct Compiler *compiler, struct Tree_Node *functionNode)
{
    if (compiler == NULL) {
        return 1;
    }
    
    int localVars = Compiler_CountLocalVariables(compiler, functionNode);

    fprintf(compiler->listingFile, "\n\t\t; postamble\n");
    fprintf(compiler->listingFile, "\t\tadd rsp, 8 * %d\n"
                                   "\t\tpop rbp\n"
                                   "\t\tret\n\n",
                                   localVars);
    return 0;
}
