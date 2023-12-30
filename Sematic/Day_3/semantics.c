/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

static int search = 0;

Object* lookupObject(char *name) {
  // TODO
  Object* object = NULL;
  static Scope* current = NULL;

  if(search == 0){
    current = symtab->currentScope;
  }
  while(current != NULL) {
    object = findObject(current->objList, name);

    current = current->outer;

    search = 1;

    if(object != NULL){
      return object;
    }
  
  }
  object = findObject(symtab->globalObjectList, name);

  search = 0;

  return object;
}

void checkFreshIdent(char *name) {
  // TODO
  Object *object = findObject(symtab->currentScope->objList, name);
  if(object != NULL){
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  }

}

Object* checkDeclaredIdent(char* name) {
  // TODO
  search = 0;

  Object *object = findObject(symtab->currentScope->objList, name);
  if(object != NULL){
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  }
}

Object* checkDeclaredConstant(char* name) {
  // TODO
  search = 0;
  Object *object = lookupObject(name);
  search = 0;

  if(object == NULL){
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);

  }
  return object;

}

Object* checkDeclaredType(char* name) {
  // TODO
  search = 0;
  Object *object = NULL;
  
  do{
    object = lookupObject(name);
    if(object != NULL &&object->kind == OBJ_TYPE){
      break;
    }
  } while(object != NULL);

  search = 0;

  if(object == NULL){
    error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  return object;
}

Object* checkDeclaredVariable(char* name) {
  // TODO
  Object *object = NULL;

  search = 0;
  do{
    object = lookupObject(name);
    if(object != NULL && object->kind == OBJ_VARIABLE){
      break;
    } 

  }while(object != NULL);
  search = 0;
  if(object == NULL){
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  return object;
}

// Object* checkDeclaredFunction(char* name) {
//   // TODO
// }

// Object* checkDeclaredProcedure(char* name) {
//   // TODO
// }

// Object* checkDeclaredLValueIdent(char* name) {
//   // TODO
// }

Object* checkDeclaredFunction(char* name) {
    Object *obj = NULL;

    // Start search
    search = 0;

    do {
        obj = lookupObject(name);
        if (obj != NULL && obj->kind == OBJ_FUNCTION)
            break;
    } while (obj != NULL);

    // End search
    search = 0;

    if (obj == NULL)
            error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);

    return obj;
}

Object* checkDeclaredProcedure(char* name) {
    Object *obj = NULL;

    // Start search
    search = 0;

    do {
        obj = lookupObject(name);
        if (obj != NULL && obj->kind == OBJ_PROCEDURE)
            break;
    } while (obj != NULL);

    // End search
    search = 0;

    if (obj == NULL)
            error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);

    return obj;
}

Object* checkDeclaredLValueIdent(char* name) {
    Object *obj = NULL;

    // Start search
    search = 0;

    do {
        obj = lookupObject(name);
        if (obj != NULL && (obj->kind == OBJ_FUNCTION || obj->kind == OBJ_PARAMETER || obj->kind == OBJ_VARIABLE))
            break;
    } while (obj != NULL);

    // End search
    search = 0;

    if (obj == NULL)
            error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);

    return obj;
}
