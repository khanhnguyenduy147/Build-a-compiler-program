/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 2.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    //printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void) {
  while(lookAhead->tokenType == TK_IDENT){
    compileConstDecl();
  }
}

void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
  // TODO
  while(lookAhead->tokenType == TK_IDENT){
    compileTypeDecl();
  }
}

void compileTypeDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

void compileVarDecls(void) {
  while(lookAhead->tokenType == TK_IDENT){
    compileVarDecl();
  }
}
void compileIndex1(void){
  eat(TK_IDENT);
  compileIndex2();
}
void compileIndex2(void){
  if(lookAhead->tokenType == SB_COMMA){
    eat(SB_COMMA);
    eat(TK_IDENT);
    compileIndex2();
  }
}
void compileVarDecl(void) {
  // TODO
  compileIndex1();
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  // TODO
  if(lookAhead->tokenType == KW_FUNCTION){
    compileFuncDecl();
    compileSubDecls();
  }else if(lookAhead->tokenType == KW_PROCEDURE){
    compileProcDecl();
    compileSubDecls();
  }
  assert("Subtoutines parsed ....");
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);

  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  // TODO
  if(lookAhead->tokenType == TK_NUMBER){
    eat(TK_NUMBER);
  } else if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
  } else if(lookAhead->tokenType== TK_CHAR){
    eat(TK_CHAR);
  } else{
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileConstant(void) {
  // TODO
  if(lookAhead->tokenType == SB_PLUS){
    eat(SB_PLUS);
    compileConstant2();
  } else if(lookAhead->tokenType == SB_MINUS){
    eat(SB_MINUS);
    compileConstant2();
  } else if(lookAhead->tokenType == TK_CHAR){
    eat(TK_CHAR);
  } else {
    compileConstant2();
  }
}

void compileConstant2(void) {
  // TODO
  if(lookAhead->tokenType == TK_NUMBER){
    eat(TK_NUMBER);
  } else if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
  } else {
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileType(void) {
  // TODO
  if(lookAhead->tokenType == KW_INTEGER){
    eat(KW_INTEGER);
  } else if(lookAhead->tokenType == KW_CHAR){
    eat(KW_CHAR);
  } else if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
  } else if(lookAhead->tokenType == KW_ARRAY){
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
  } else {
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileBasicType(void) {
  // TODO
  if(lookAhead->tokenType == KW_INTEGER){
    eat(KW_INTEGER);

  } else if(lookAhead->tokenType == TK_CHAR){
    eat(TK_CHAR);
  } else {
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileParams(void) {
  // TODO
  if(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
}

void compileParams2(void) {
  // TODO
  if(lookAhead->tokenType == SB_SEMICOLON){
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
  }
}

void compileParam(void) {
  // TODO
  if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  } else if(lookAhead->tokenType == KW_VAR){
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  } else {
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileStatements(void) {
  // TODO
  compileStatement();
  compileStatements2();
}

void compileStatements2(void) {
  // TODO
  if(lookAhead->tokenType == SB_SEMICOLON){
    eat(SB_SEMICOLON);
    compileStatement();
    compileStatements2();
  } else if(lookAhead->tokenType == KW_END){

  } else {
    //error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    eat(SB_SEMICOLON);
  }
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case SB_LPAR:
    compileAssignSt();
    break;
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
void compileVariable(void){
  eat(TK_IDENT);
  if(lookAhead->tokenType == SB_LSEL){
    compileIndexes();
  }
}
void compileVariable2(void) {
  if(lookAhead->tokenType == SB_COMMA){
    eat(SB_COMMA);
    compileVariable();
    compileVariable2();
  }
}
void compileVariables(void) {
  compileVariable();
  compileVariable2();
}
void compileExpressionplus2(void){
  if(lookAhead->tokenType == SB_COMMA){
    eat(SB_COMMA);
    compileExpression();
    compileExpressionplus2();
  }
}
void compileExpressionsplus(void){
  compileExpression();
  compileExpressionplus2();
}
void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO
  // eat(TK_IDENT);
  // if(lookAhead->tokenType == SB_LSEL){
  //   compileIndexes();
  // }
  if(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    eat(TK_IDENT);
    while(lookAhead->tokenType == SB_COMMA){
      eat(SB_COMMA);
      eat(TK_IDENT);
    }
    eat(SB_RPAR);
  } else {
    eat(TK_IDENT);
    if(lookAhead->tokenType == SB_LSEL){
      compileIndexes();
    }
  }

  eat(SB_ASSIGN);

  if(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    compileExpression();
    while(lookAhead->tokenType == SB_COMMA){
      eat(SB_COMMA);
      compileExpression();
    }
    eat(SB_RPAR);
  } else {
    compileExpression();
  }
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
    eat(SB_ASSIGN);
    compileExpression();
    eat(KW_TO);
    compileExpression();
     eat(KW_DO);
  } else {
    eat(SB_LPAR);
    compileAssignSt();
    eat(SB_SEMICOLON);
    compileCondition();
    eat(SB_SEMICOLON);
    compileAssignSt();
    eat(SB_RPAR);
  }
  
  compileStatement();
  assert("For statement parsed ....");
}

void compileArguments(void) {
  // TODO
  if(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
  }
}

void compileArguments2(void) {
  // TODO
  if(lookAhead->tokenType == SB_COMMA){
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
  }
}

void compileCondition(void) {
  // TODO
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  // TODO
  if(lookAhead->tokenType == SB_EQ){
    eat(SB_EQ);
    compileExpression();
  } else if(lookAhead->tokenType == SB_NEQ){
    eat(SB_NEQ);
    compileExpression();
  } else if(lookAhead->tokenType == SB_LE){
    eat(SB_LE);
    compileExpression();
  } else if(lookAhead->tokenType == SB_LT){
    eat(SB_LT);
    compileExpression();
  } else if(lookAhead->tokenType == SB_GE){
    eat(SB_GE);
    compileExpression();
  } else if(SB_GT){
    eat(SB_GT);
    compileExpression();
  } else {
    error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  if(lookAhead->tokenType == SB_PLUS){
    eat(SB_PLUS);
    compileExpression2();
  } else if(lookAhead->tokenType == SB_MINUS){
    eat(SB_MINUS);
    compileExpression2();
  } else {
    compileExpression2();
  }
  assert("Expression parsed");
}

void compileExpression2(void) {
  // TODO
  compileTerm();
  compileExpression3();
}


void compileExpression3(void) {
  // TODO
  if(lookAhead->tokenType == SB_PLUS){
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
  } else if(lookAhead->tokenType == SB_MINUS){
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
  }
}

void compileTerm(void) {
  // TODO
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  // TODO
  if(lookAhead->tokenType == SB_TIMES){
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
  } else if(lookAhead->tokenType == SB_SLASH){
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
  }
}

void compileFactor(void) {
  // TODO
  if(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
  } else if(lookAhead->tokenType == TK_NUMBER){
    eat(TK_NUMBER);
  } else if(lookAhead->tokenType == TK_CHAR){
    compileUnsignedConstant();
  } else if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
    if(lookAhead->tokenType == SB_LSEL){
      compileIndexes();
    } else if(lookAhead->tokenType == SB_LPAR){
      compileArguments();
    }
  } else {
    error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileIndexes(void) {
  // TODO
  if(lookAhead->tokenType == SB_LSEL){
    eat(SB_LSEL);
    compileExpression();
    while(lookAhead->tokenType == SB_COMMA){
      eat(SB_COMMA);
      compileExpression();
    }
    eat(SB_RSEL);
    compileIndexes();
  }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}
