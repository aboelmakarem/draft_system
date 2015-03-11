//Ahmed Hussein
//ahussei4@jhu.edu

#ifndef PARSER_H_
#define PARSER_H_

#include "Scanner.h"
//#include "ParseTreeAction.h"
//#include "ParserObserver.h"
//#include "ParserError.h"
#include "list"
#include "TreeNode.h"
//#include "DeclarationsScope.h"

using namespace std;

namespace DraftCompiler
{
	//const int ErrorTokenSkipDistance = 8;
	class Parser
	{
	public:
		Parser();
		Parser(const Parser& oParser);
		virtual ~Parser();
		Parser& operator=(const Parser& oParser);
		virtual void Reset();
		void ClearTokens();
		void SetTokens(vector<Token*>* pvpoTokens);
		virtual bool Parse();
		//void AddObserver(ParserObserver* poObserver);
		//void RemoveObserver(ParserObserver* poObserver);
		//ParseTreeAction* GetLatestAction();
		//list<ParserError>* GetErrors();
		virtual void PrintSymbolTable(const bool& bPrintGraphicalTable = false);
		virtual void PrintAST(const bool& bPrintGraphicalTable = false);
		
	private:
	
	protected:
		virtual void Initialize();
		Token* GetToken(const int& iOffset = 0);
		void IncrementPosition(const unsigned int& iOffset = 1);
		void DecrementPosition(const unsigned int& iOffset = 1);
		void SetPosition(const unsigned int& iPosition);
		virtual void PrintCST(TreeNode<Token*>* poCSTree);
		//ParserError* GetLatestError();
		//void FlagError(const ParserErrorType& eType,const string& sErrorString);
		//void AddError(const ParserErrorType& eType,const string& sErrorString);
		//bool DropLastError();
		//bool SeekToNextStrongToken();
		
		virtual TreeNode<Token*>* ParseReal(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseScaleFactor(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseNumber(const unsigned int& iLevel);
 		virtual TreeNode<Token*>* ParseExpression(const unsigned int& iLevel);
 		virtual TreeNode<Token*>* ParseQualifiedIdentifier(const unsigned int& iLevel);
 		virtual TreeNode<Token*>* ParseExpressionList(const unsigned int& iLevel);
 		virtual TreeNode<Token*>* ParseSelector(const unsigned int& iLevel);
 		virtual TreeNode<Token*>* ParseDesignator(const unsigned int& iLevel);
 		virtual TreeNode<Token*>* ParseActualParameters(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseSimpleExpression(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseTerm(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseFactor(const unsigned int& iLevel);
		
		virtual TreeNode<Token*>* ParseAssign(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseCall(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseIf(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseWhile(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseWrite(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseStatement(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseStatementSequence(const unsigned int& iLevel);
		virtual bool ParseControlBody(const unsigned int& iLevel,TreeNode<Token*>*& poExpression,TreeNode<Token*>*& poOpenBrace,TreeNode<Token*>*& poStatementSequence,TreeNode<Token*>*& poCloseBrace);
		virtual bool ParseStatementBlock(const unsigned int& iLevel,TreeNode<Token*>*& poOpenBrace,TreeNode<Token*>*& poStatementSequence,TreeNode<Token*>*& poCloseBrace);
		
		virtual TreeNode<Token*>* ParseIdentifierList(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseArrayType(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseClassType(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseProcedureType(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseStructuredType(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseType(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseFieldListSequence(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseFormalParameter(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseFormalParameterList(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseFormalParameters(const unsigned int& iLevel);
		
		virtual TreeNode<Token*>* ParseConstDeclaration(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseTypeDeclaration(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseVarDeclaration(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseProcedureDeclaration(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseDeclaration(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseDeclarationSequence(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseProcedureBody(const unsigned int& iLevel);
		virtual TreeNode<Token*>* ParseProgram(const unsigned int& iLevel);
		
		//void NotifyObservers();
		//void AddAction(const ParseTreeActionType& eType,const string& sString);
		//void AddStartAction(const string& sString);
		//void AddMatchAction(Token* poToken);
		//void AddEndAction(const string& sString);
		//void AddRetractAction();
		//void AddRetractAndGoUpAction();*/
		vector<Token*> m_vpoTokens;
		unsigned int m_iTokensCount;
		unsigned int m_iPosition;
		//ParseTreeAction m_oLatestAction;
		//list<ParserObserver*> m_lpoObservers;
		//list<ParserError> m_loErrors;
		//unsigned int m_iLatestErrorTokenIndex;
		//bool m_bLastErrorLocked;
		//bool m_bParsingProcedureInstructions;
	};
}

#endif


