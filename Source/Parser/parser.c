#include "parser.h"

void __Parser_RaiseError(struct Parser *parser, enum Parser_Error error)
{
    if (parser == NULL || error == PARSER_ERR_NoError) {
        return;
    }
    
    parser->errorCode = error;
    Man_PrintError(parser->currentToken, "Parser Error");
    return;
}

void Parser_Advance(struct Parser *parser)
{
    if (parser == NULL) {
        return;
    }

    parser->currentToken = Tokenizer_GetNextToken(parser->tok);
    if (parser->currentToken == NULL) {
        RAISE(PARSER_ERR_TokenizerError);
    }
    return;
}

void Parser_RequireOperator(struct Parser *parser, enum Token_Code operator)
{
    if (parser == NULL) {
        return;
    }

    if (parser->currentToken->operator != operator) {
        RAISE(PARSER_ERR_WrongOperator);
    }
    return;
}

void Parser_RequireType(struct Parser *parser, enum Token_Type type)
{
    if (parser == NULL) {
        return;
    }

    if (parser->currentToken->type != type) {
        RAISE(PARSER_ERR_WrongTokenType);
    }
    return;
}

struct Parser *Parser_New(const char *fileName)
{
    struct Parser *parser = calloc(1, sizeof (struct Parser));
    if (parser == NULL) {
        return parser;
    }
    parser->fileName = fileName;
    
    parser->tok = Tokenizer_New(fileName);
    parser->currentToken = Tokenizer_GetNextToken(parser->tok);

    if (parser->tok->errorCode != TOK_ERR_NoError || parser->currentToken == NULL) {
        parser->errorCode = PARSER_ERR_TokenizerError;
        return parser;
    }

    parser->errorCode = PARSER_ERR_NoError;
    return parser;
}

struct Tree_Node *Parser_GetGrammar(struct Parser *parser)
{
    struct Tree_Node *syntaxTree = NULL;

    if (parser->currentToken->type != TOKEN_TYPE_Operator) {
        RAISE(PARSER_ERR_WrongTokenType);
        return syntaxTree;
    }

    switch (parser->currentToken->operator) {
        case TOKEN_KW_Function:
            syntaxTree = Parser_GetFunction(parser);
            break;

        case TOKEN_KW_VarDecl:  
            syntaxTree = Parser_GetVarDecl(parser);
            Parser_RequireOperator(parser, TOKEN_OP_Semicolon);
            Parser_Advance(parser);
            break;

        default: 
            RAISE(PARSER_ERR_ImpossibleOperator);
    }

    struct Tree_Node *lastLine = syntaxTree;

    while (parser->currentToken->type != TOKEN_TYPE_Endmarker) {
        if (parser->currentToken->type != TOKEN_TYPE_Operator) {
            RAISE(PARSER_ERR_WrongTokenType);
        }

        switch (parser->currentToken->operator) {
            case TOKEN_KW_Function: 
                lastLine->right = Parser_GetFunction(parser); 
                break;

            case TOKEN_KW_VarDecl:  
                lastLine->right = Parser_GetVarDecl(parser); 
                Parser_RequireOperator(parser, TOKEN_OP_Semicolon); 
                Parser_Advance(parser); 
                break;

            default: 
                RAISE(PARSER_ERR_ImpossibleOperator);
        }

        lastLine = lastLine->right;
    }

    lastLine->right = Tree_NewNode(parser->currentToken);
    return syntaxTree;
}

struct Tree_Node *Parser_GetVarDecl(struct Parser *parser)
{
    Parser_RequireOperator(parser, TOKEN_KW_VarDecl);
    struct Tree_Node *declNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);

    Parser_RequireType(parser, TOKEN_TYPE_Identifier);
    struct Tree_Node *varNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);

    Parser_RequireOperator(parser, TOKEN_OP_Assignment);
    struct Tree_Node *assignmentNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    assignmentNode->left = varNode;
    
    assignmentNode->right = Parser_GetLogicalOr(parser);
    declNode->left = assignmentNode;

    struct Tree_Node *currentNode = declNode;
    while (parser->currentToken->type == TOKEN_TYPE_Operator && parser->currentToken->operator == TOKEN_OP_Comma) {
        Parser_Advance(parser); 
        Parser_RequireOperator(parser, TOKEN_KW_VarDecl);
        struct Tree_Node *nextDeclNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        
        Parser_RequireType(parser, TOKEN_TYPE_Identifier);
        struct Tree_Node *nextVarNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);

        Parser_RequireOperator(parser, TOKEN_OP_Assignment);
        struct Tree_Node *nextAssignmentNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        nextAssignmentNode->left = nextVarNode;

        nextAssignmentNode->right = Parser_GetLogicalOr(parser);
        nextDeclNode->left = nextAssignmentNode;

        declNode->right = nextDeclNode;
        currentNode = currentNode->right;
    }

    return declNode;
}

struct Tree_Node *Parser_GetFunction(struct Parser *parser)
{
    Parser_RequireOperator(parser, TOKEN_KW_Function);
    struct Tree_Node *functionNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);

    Parser_RequireType(parser, TOKEN_TYPE_Identifier);
    struct Tree_Node *functionName = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    functionNode->left = functionName;
    
    Parser_RequireOperator(parser, TOKEN_OP_Lround);
    Parser_Advance(parser);
    struct Tree_Node *functionParams = Parser_GetParametersList(parser);
    Parser_RequireOperator(parser, TOKEN_OP_Rround);
    Parser_Advance(parser);
    functionName->left = functionParams;

    if (parser->currentToken->type == TOKEN_TYPE_Operator && parser-> currentToken->operator == TOKEN_OP_Semicolon) {
        functionName->right = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        return functionNode;
    }

    Parser_RequireOperator(parser, TOKEN_OP_Lcurly);
    Parser_Advance(parser);
    struct Tree_Node *functionBody = Parser_GetMultilineOperator(parser);
    struct Tree_Node *lastLine = functionBody;

    while (!(parser->currentToken->type     == TOKEN_TYPE_Operator && parser->currentToken->operator == TOKEN_OP_Rcurly)) {
        lastLine->right = Parser_GetMultilineOperator(parser);
        lastLine = lastLine->right;
    }
    Parser_RequireOperator(parser, TOKEN_OP_Rcurly);
    Parser_Advance(parser);

    functionName->left  = functionParams;
    functionName->right = functionBody;
    return functionNode;
}

struct Tree_Node *Parser_GetParametersList(struct Parser *parser)
{
    if (parser->currentToken->type == TOKEN_TYPE_Operator &&
        parser->currentToken->operator == TOKEN_KW_Void) {
        
        struct Tree_Node *declNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        return declNode;
    }

    Parser_RequireOperator(parser, TOKEN_KW_VarDecl);
    struct Tree_Node *declNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);

    Parser_RequireType(parser, TOKEN_TYPE_Identifier);
    struct Tree_Node *varNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    declNode->left = varNode;

    struct Tree_Node *currentNode = declNode;
    while (parser->currentToken->type == TOKEN_TYPE_Operator && parser->currentToken->operator == TOKEN_OP_Comma) {
        Parser_Advance(parser);
        Parser_RequireOperator(parser, TOKEN_KW_VarDecl);
        struct Tree_Node *nextDeclNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);

        Parser_RequireType(parser, TOKEN_TYPE_Identifier);
        struct Tree_Node *nextVarNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        nextDeclNode->left = nextVarNode;

        currentNode->right = nextDeclNode;
        currentNode = currentNode->right;
    }

    return declNode;
}

struct Tree_Node *Parser_GetMultilineOperator(struct Parser *parser)
{
    struct Tree_Node *left = NULL;

    if ((parser->currentToken->type     == TOKEN_TYPE_Operator) && 
        (parser->currentToken->operator == TOKEN_KW_If   || parser->currentToken->operator == TOKEN_KW_While    ||
         parser->currentToken->operator == TOKEN_KW_For  || parser->currentToken->operator == TOKEN_KW_Function ||
         parser->currentToken->operator == TOKEN_KW_Void || parser->currentToken->operator == TOKEN_KW_VarDecl)) {

        switch (parser->currentToken->operator) {
            case TOKEN_KW_If: {
                left = Parser_GetIf(parser);
                break;
            }

            case TOKEN_KW_For: {
                left = Parser_GetFor(parser);
                break;
            }

            case TOKEN_KW_While: {
                left = Parser_GetWhile(parser);
                break;
            }

            case TOKEN_KW_VarDecl: {  
                left = Parser_GetVarDecl(parser);
                Parser_RequireOperator(parser, TOKEN_OP_Semicolon);
                struct Tree_Node *newNode = Tree_NewNode(parser->currentToken);
                Parser_Advance(parser);
                newNode->left = left;
                return newNode;
            }

            // Impossible, but still
            default: RAISE(PARSER_ERR_ImpossibleOperator);
        }

        struct Token *newSemicolon = calloc(1, sizeof (struct Token));
        if (newSemicolon == NULL) {
            RAISE(PARSER_ERR_UnsuccessfulAllocation);
        }
        newSemicolon->type = TOKEN_TYPE_Operator;
        newSemicolon->operator = TOKEN_OP_Semicolon;
        struct Tree_Node *newNode = Tree_NewNode(newSemicolon);
        newNode->left = left;
        return newNode;
    }

    left = Parser_GetExpression(parser);
    Parser_RequireOperator(parser, TOKEN_OP_Semicolon);
    struct Tree_Node *newNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    newNode->left = left;
    return newNode;
}

struct Tree_Node *Parser_GetIf(struct Parser *parser)
{
    struct Tree_Node *ifNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    
    Parser_RequireOperator(parser, TOKEN_OP_Lround);
    Parser_Advance(parser);
    struct Tree_Node *ifCondition = Parser_GetExpression(parser);
    Parser_RequireOperator(parser, TOKEN_OP_Rround);
    Parser_Advance(parser);

    Parser_RequireOperator(parser, TOKEN_OP_Lcurly);
    Parser_Advance(parser);
    struct Tree_Node *ifBody = Parser_GetMultilineOperator(parser);
    struct Tree_Node *lastLine = ifBody;

    while (!(parser->currentToken->type     == TOKEN_TYPE_Operator && parser->currentToken->operator == TOKEN_OP_Rcurly)) {
        lastLine->right = Parser_GetMultilineOperator(parser);
        lastLine = lastLine->right;
    }
    Parser_RequireOperator(parser, TOKEN_OP_Rcurly);
    Parser_Advance(parser);

    if (parser->currentToken->type     == TOKEN_TYPE_Operator &&
        parser->currentToken->operator == TOKEN_KW_Else) {
        
        struct Tree_Node *elseNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        ifNode->left  = ifCondition;
        ifNode->right = elseNode;

        if (parser->currentToken->type     == TOKEN_TYPE_Operator &&
            parser->currentToken->operator == TOKEN_KW_If) {

            elseNode->left  = ifBody;
            elseNode->right = Parser_GetIf(parser);
            return ifNode;
        }

        Parser_RequireOperator(parser, TOKEN_OP_Lcurly);
        Parser_Advance(parser);
        struct Tree_Node *elseBody = Parser_GetMultilineOperator(parser);
        struct Tree_Node *lastLine = elseBody;

        while (!(parser->currentToken->type     == TOKEN_TYPE_Operator && parser->currentToken->operator == TOKEN_OP_Rcurly)) {
            lastLine->right = Parser_GetMultilineOperator(parser);
            lastLine = lastLine->right;
        }
        Parser_RequireOperator(parser, TOKEN_OP_Rcurly);
        Parser_Advance(parser);

        elseNode->left  = ifBody;
        elseNode->right = elseBody;
        return ifNode; 
    }

    ifNode->left  = ifCondition;
    ifNode->right = ifBody;
    return ifNode;
}

/* NOT IMPLEMENTED YET */
struct Tree_Node *Parser_GetFor(struct Parser *parser)
{
    struct Tree_Node *forNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    
    Parser_RequireOperator(parser, TOKEN_OP_Lround);
    Parser_Advance(parser);

    struct Tree_Node *forPre = Parser_GetExpression(parser);
    Parser_RequireOperator(parser, TOKEN_OP_Semicolon);
    struct Tree_Node *preSeparator = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    preSeparator->left = forPre;
    
    struct Tree_Node *forIn = Parser_GetExpression(parser);
    Parser_RequireOperator(parser, TOKEN_OP_Semicolon);
    struct Tree_Node *inSeparator = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    preSeparator->right = inSeparator;
    inSeparator->left = forIn;

    struct Tree_Node *forAfter = Parser_GetExpression(parser);
    inSeparator->right = forAfter;

    Parser_RequireOperator(parser, TOKEN_OP_Rround);
    Parser_Advance(parser);

    Parser_RequireOperator(parser, TOKEN_OP_Lcurly);
    Parser_Advance(parser);
    struct Tree_Node *forBody = Parser_GetMultilineOperator(parser);
    struct Tree_Node *lastLine = forBody;

    while (!(parser->currentToken->type     == TOKEN_TYPE_Operator && parser->currentToken->operator == TOKEN_OP_Rcurly)) {
        lastLine->right = Parser_GetMultilineOperator(parser);
        lastLine = lastLine->right;
    }
    Parser_RequireOperator(parser, TOKEN_OP_Rcurly);
    Parser_Advance(parser);

    forNode->left  = preSeparator;
    forNode->right = forBody;
    return forNode;
}

struct Tree_Node *Parser_GetWhile(struct Parser *parser)
{
    struct Tree_Node *whileNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    
    Parser_RequireOperator(parser, TOKEN_OP_Lround);
    Parser_Advance(parser);
    struct Tree_Node *whileCondition = Parser_GetExpression(parser);
    Parser_RequireOperator(parser, TOKEN_OP_Rround);
    Parser_Advance(parser);

    Parser_RequireOperator(parser, TOKEN_OP_Lcurly);
    Parser_Advance(parser);
    struct Tree_Node *whileBody = Parser_GetMultilineOperator(parser);
    struct Tree_Node *lastLine = whileBody;
    
    while (!(parser->currentToken->type     == TOKEN_TYPE_Operator && parser->currentToken->operator == TOKEN_OP_Rcurly)) {
        lastLine->right = Parser_GetMultilineOperator(parser);
        lastLine = lastLine->right;
    }
    Parser_RequireOperator(parser, TOKEN_OP_Rcurly);
    Parser_Advance(parser);

    whileNode->left  = whileCondition;
    whileNode->right = whileBody;
    return whileNode;
}

struct Tree_Node *Parser_GetExpression(struct Parser *parser)
{
    if (parser->currentToken->type     == TOKEN_TYPE_Operator &&
        parser->currentToken->operator == TOKEN_KW_Break) {
        
        struct Tree_Node *newBreak = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        return newBreak;
    }

    if (parser->currentToken->type     == TOKEN_TYPE_Operator &&
        parser->currentToken->operator == TOKEN_KW_Continue) {
        
        struct Tree_Node *newContinue = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        return newContinue;
    }

    if (parser->currentToken->type     == TOKEN_TYPE_Operator &&
        parser->currentToken->operator == TOKEN_KW_Return) {
        
        struct Tree_Node *newReturn= Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        newReturn->left = Parser_GetComma(parser);
        return newReturn;
    }

    // weakest rule
    return Parser_GetComma(parser);
}

#define NEW_EXPRESSION_RULE(rule, dependence, precedence)                   \
                                                                            \
struct Tree_Node *Parser_Get##rule(struct Parser *parser)                   \
{                                                                           \
    struct Tree_Node *newNode = Parser_Get##dependence(parser);             \
                                                                            \
    while (IsPrecedence##precedence(parser->currentToken->operator)) {      \
        struct Tree_Node *operator = Tree_NewNode(parser->currentToken);    \
        Parser_Advance(parser);                                             \
                                                                            \
        struct Tree_Node *secondOperand = Parser_Get##dependence(parser);   \
        operator->left  = newNode;                                          \
        operator->right = secondOperand;                                    \
        newNode = operator;                                                 \
    }                                                                       \
                                                                            \
    return newNode;                                                         \
}

NEW_EXPRESSION_RULE(Comma,      Assignment, 15)
NEW_EXPRESSION_RULE(Assignment, LogicalOr,  14)
NEW_EXPRESSION_RULE(LogicalOr,  LogicalAnd, 13)
NEW_EXPRESSION_RULE(LogicalAnd, BitOr,      12)
NEW_EXPRESSION_RULE(BitOr,      BitXor,     11)
NEW_EXPRESSION_RULE(BitXor,     BitAnd,     10)
NEW_EXPRESSION_RULE(BitAnd,     Equality,    9)
NEW_EXPRESSION_RULE(Equality,   Comparison,  8)
NEW_EXPRESSION_RULE(Comparison, BitShift,    7)
NEW_EXPRESSION_RULE(BitShift,   AddSub,      6)
NEW_EXPRESSION_RULE(AddSub,     MulDivMod,   5)
NEW_EXPRESSION_RULE(MulDivMod,  UnarySign,   4)

#undef NEW_EXPRESSION_RULE

struct Tree_Node *Parser_GetUnarySign(struct Parser *parser)
{
    struct Tree_Node *newNode = NULL;
    if (parser->currentToken->operator == TOKEN_OP_Add ||
        parser->currentToken->operator == TOKEN_OP_Sub) {
        
        newNode = Tree_NewNode(parser->currentToken);
        Parser_Advance(parser);
        newNode->right = Parser_GetUnarySign(parser);
    }
    else {
        newNode = Parser_GetParenthesis(parser);
    }
    return newNode;
}

struct Tree_Node *Parser_GetParenthesis(struct Parser *parser)
{
    if (parser->currentToken->operator == TOKEN_OP_Lround) {
        Parser_Advance(parser);
        struct Tree_Node *newNode = Parser_GetExpression(parser);
        Parser_RequireOperator(parser, TOKEN_OP_Rround);
        Parser_Advance(parser);
        return newNode;
    }
    
    if (parser->currentToken->type == TOKEN_TYPE_Identifier) {
        return Parser_GetIdentifier(parser);
    }

    if (parser->currentToken->type == TOKEN_TYPE_String) {
        return Parser_GetString(parser);
    }
    
    if (parser->currentToken->type == TOKEN_TYPE_Number) {
        return Parser_GetNumber(parser);
    }

    return NULL;
}

struct Tree_Node *Parser_GetNumber(struct Parser *parser)
{
    Parser_RequireType(parser, TOKEN_TYPE_Number);
    struct Tree_Node *newNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    return newNode;
}

struct Tree_Node *Parser_GetString(struct Parser *parser)
{
    Parser_RequireType(parser, TOKEN_TYPE_String);
    struct Tree_Node *newNode = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    return newNode;
}

struct Tree_Node *Parser_GetIdentifier(struct Parser *parser)
{
    Parser_RequireType(parser, TOKEN_TYPE_Identifier);
    struct Tree_Node *newIdentifier = Tree_NewNode(parser->currentToken);
    Parser_Advance(parser);
    
    if (parser->currentToken->type     == TOKEN_TYPE_Operator &&
        parser->currentToken->operator == TOKEN_OP_Lround) {

        Parser_Advance(parser);
        struct Token *newFunctionCall = calloc(1, sizeof (struct Token));
        if (newFunctionCall == NULL) {
            RAISE(PARSER_ERR_UnsuccessfulAllocation);
        }
        newFunctionCall->type = TOKEN_TYPE_Operator;
        newFunctionCall->operator = TOKEN_OP_FunctionCall;
        struct Tree_Node *newNode = Tree_NewNode(newFunctionCall);
        struct Tree_Node *newArgList = Parser_GetComma(parser);
        Parser_RequireOperator(parser, TOKEN_OP_Rround);
        Parser_Advance(parser);
        newNode->left  = newIdentifier;
        newNode->right = newArgList;
        return newNode;
    }

    if (parser->currentToken->type     == TOKEN_TYPE_Operator &&
        parser->currentToken->operator == TOKEN_OP_Lsquare) {

        Parser_Advance(parser);
        struct Token *newIndexating = calloc(1, sizeof (struct Token));
        if (newIndexating == NULL) {
            RAISE(PARSER_ERR_UnsuccessfulAllocation);
        }
        newIndexating->type = TOKEN_TYPE_Operator;
        newIndexating->operator = TOKEN_OP_Indexate;
        struct Tree_Node *newNode = Tree_NewNode(newIndexating);
        struct Tree_Node *newIndex = Parser_GetExpression(parser);
        Parser_RequireOperator(parser, TOKEN_OP_Rsquare);
        Parser_Advance(parser);
        newNode->left  = newIdentifier;
        newNode->right = newIndex;
        return newNode;
    }

    return newIdentifier;
}

struct Parser *Parser_Delete(struct Parser *parser)
{
    parser->tok = Tokenizer_Delete(parser->tok);
    parser->errorCode  = PARSER_ERR_NoError;
    parser->fileName     = NULL;
    parser->currentToken = NULL;
    free(parser);
    return NULL;
}
