//Ahmed Hussein
//ahussei4@jhu.edu

#include "Parser.h"
#include "stdio.h"

namespace DraftCompiler
{
	Parser::Parser()
	{
		Initialize();
	}
	Parser::Parser(const Parser& oParser)
	{
		*this = oParser;
	}
	Parser::~Parser()
	{
		Reset();
	}
	Parser& Parser::operator=(const Parser& oParser)
	{
		Reset();
		unsigned int i = 0;
		unsigned int iSize = (unsigned int)oParser.m_vpoTokens.size();
		m_vpoTokens.resize(iSize);
		for(i = 0 ; i < iSize ; i++)
		{
			m_vpoTokens[i] = new Token(*(oParser.m_vpoTokens[i]));
		}
		m_iTokensCount = iSize;
		//m_oLatestAction = oParser.m_oLatestAction;
		//m_lpoObservers = oParser.m_lpoObservers;
		//m_loErrors = oParser.m_loErrors;
		//m_iLatestErrorTokenIndex = oParser.m_iLatestErrorTokenIndex;
		//m_bLastErrorLocked = oParser.m_bLastErrorLocked;
		//m_bParsingProcedureInstructions = oParser.m_bParsingProcedureInstructions;
		return *this;
	}
	void Parser::Reset()
	{
		ClearTokens();
		//m_oLatestAction.Reset();
		//m_lpoObservers.clear();
		//m_loErrors.clear();
		//m_iLatestErrorTokenIndex = 0;
		//m_bLastErrorLocked = false;
	}
	void Parser::ClearTokens()
	{
		unsigned int i = 0;
		unsigned int iSize = (unsigned int)m_vpoTokens.size();
		for(i = 0 ; i < iSize ; i++)
		{
			if(m_vpoTokens[i] != NULL)
			{
				delete m_vpoTokens[i];
			}
		}
		m_vpoTokens.clear();
		m_iTokensCount = 0;
		m_iPosition = 0;
	}
	void Parser::SetTokens(vector<Token*>* pvpoTokens)
	{
		Reset();
		//m_oLatestAction.Reset();
		unsigned int i = 0;
		unsigned int iSize = (unsigned int)pvpoTokens->size();
		m_vpoTokens.resize(iSize);
		for(i = 0 ; i < iSize ; i++)
		{
			m_vpoTokens[i] = pvpoTokens->at(i);
		}
		m_iTokensCount = iSize;
	}
	bool Parser::Parse()
	{
		// make sure that the scanner is set to scan from the beginning of the source code
		m_iPosition = 0;
		//return ParseProgram();
		TreeNode<Token*>* poCSTree = ParseProgram(0);
		if(poCSTree == NULL)
		{
			return false;
		}
		PrintCST(poCSTree);
		poCSTree->DeleteChildren();
		delete poCSTree;
		return true;
	}
	/*void Parser::AddObserver(ParserObserver* poObserver)
	{
		m_lpoObservers.push_back(poObserver);
	}
	void Parser::RemoveObserver(ParserObserver* poObserver)
	{
		// remove any observers on the list having the same pointer
		list<ParserObserver*>::iterator liObservers = m_lpoObservers.begin();
		while(liObservers != m_lpoObservers.end())
		{
			if((*liObservers) == poObserver)
			{
				liObservers = m_lpoObservers.erase(liObservers);
			}
			else
			{
				liObservers++;
			}
		}
	}
	ParseTreeAction* Parser::GetLatestAction()
	{
		return &m_oLatestAction;
	}*/
	void Parser::Initialize()
	{
		m_vpoTokens.clear();
		m_iTokensCount = 0;
		m_iPosition = 0;
		//m_oLatestAction.Reset();
		//m_lpoObservers.clear();
		//m_loErrors.clear();
		//m_iLatestErrorTokenIndex = 0;
		//m_bLastErrorLocked = false;
		//m_bParsingProcedureInstructions = false;
	}
	/*void Parser::NotifyObservers()
	{
		list<ParserObserver*>::iterator liObservers;
		for(liObservers = m_lpoObservers.begin() ; liObservers != m_lpoObservers.end() ; liObservers++)
		{
			(*liObservers)->Update();
		}
	}
	void Parser::AddStartAction(const string& sString)
	{
		m_oLatestAction.SetType(StartNonTerminalAction);
		m_oLatestAction.SetString(sString);
		NotifyObservers();
	}
	void Parser::AddMatchAction(Token* poToken)
	{
		m_oLatestAction.SetType(MatchTerminalAction);
		m_oLatestAction.SetString(poToken->GeneratePrintString());
		m_oLatestAction.SetToken(poToken);
		NotifyObservers();
		IncrementPosition();
	}
	void Parser::AddEndAction(const string& sString)
	{
		m_oLatestAction.SetType(EndNonTerminalAction);
		m_oLatestAction.SetString(sString);
		NotifyObservers();
	}
	void Parser::AddRetractAction()
	{
		m_oLatestAction.SetType(RetractAction);
		m_oLatestAction.SetString("");
		NotifyObservers();
		DecrementPosition();
	}
	void Parser::AddRetractAndGoUpAction()
	{
		m_oLatestAction.SetType(RetractAndGoUpAction);
		m_oLatestAction.SetString("");
		NotifyObservers();
	}*/
	Token* Parser::GetToken(const int& iOffset)
	{
		if(iOffset == 0)
		{
			// looking for the current token
			return m_vpoTokens[m_iPosition];
		}
		if(iOffset > 0)
		{
			// looking for a token ahead
			if((m_iPosition + iOffset) >= m_iTokensCount)
			{
				return m_vpoTokens[m_iTokensCount - 1];
			}
			else
			{
				return m_vpoTokens[m_iPosition + iOffset];
			}
		}
		if(iOffset < 0)
		{
			// looking for a past token
			if((m_iPosition - iOffset) < 0)
			{
				return m_vpoTokens[0];
			}
			else
			{
				return m_vpoTokens[m_iPosition - iOffset];
			}
		}
	}
	void Parser::IncrementPosition(const unsigned int& iOffset)
	{
		if(m_iPosition < (m_iTokensCount - iOffset - 1))
		{
			m_iPosition = m_iPosition + iOffset;
		}
		else
		{
			m_iPosition = m_iTokensCount - 1;
		}
	}
	void Parser::DecrementPosition(const unsigned int& iOffset)
	{
		if(m_iPosition > iOffset)
		{
			m_iPosition = m_iPosition - iOffset;
		}
		else
		{
			m_iPosition = 0;
		}
	}
	void Parser::SetPosition(const unsigned int& iPosition)
	{
		if(iPosition < 0)
		{
			m_iPosition = 0;
		}
		else if(m_iPosition > (m_iTokensCount - 1))
		{
			m_iPosition = m_iTokensCount - 1;
		}
		else
		{
			m_iPosition = iPosition;
		}
	}
	/*ParserError* Parser::GetLatestError()
	{
		if(m_loErrors.empty())
		{
			return NULL;
		}
		return &(m_loErrors.back());
	}
	void Parser::FlagError(const ParserErrorType& eType,const string& sErrorString)
	{
// 		if((eType != STGenerationError) && (eType != ASTGenerationError))
// 		{
// 			if(m_iLatestErrorTokenIndex != 0)
// 			{
// 				if((m_iPosition - m_iLatestErrorTokenIndex) < ErrorTokenSkipDistance)
// 				{
// 					m_bLastErrorLocked = true;
// 					return;
// 				}
// 			}
// 		}
// 		else
// 		{
//			m_bLastErrorLocked = true;
// 		}
		AddError(eType,sErrorString);
	}
	void Parser::AddError(const ParserErrorType& eType,const string& sErrorString)
	{
		ParserError oError(eType,sErrorString);
		oError.SetPosition(GetPreviousToken()->GetEndPosition());
// 		if((eType != STGenerationError) && (eType != ASTGenerationError))
// 		{
// 			m_iLatestErrorTokenIndex = m_iPosition;
// 			m_bLastErrorLocked = false;
// 		}
		m_loErrors.push_back(oError);
	}
	bool Parser::DropLastError()
	{
		// make sure that we are not removing a different error
// 		if(m_bLastErrorLocked)
// 		{
// 			return false;
// 		}
		if(!m_loErrors.empty())
		{
			m_loErrors.pop_back();
// 			if(!m_loErrors.empty())
// 			{
// 				m_iLatestErrorTokenIndex = m_loErrors.back().GetPosition();
// 			}
// 			else
// 			{
// 				m_iLatestErrorTokenIndex = 0;
// 			}
		}
		return true;
	}
	list<ParserError>* Parser::GetErrors()
	{
		return &m_loErrors;
	}
	bool Parser::SeekToNextStrongToken()
	{
		TokenKind eKind = NullTokenKind;
		while(m_iPosition < (m_iTokensCount - 1))
		{
			eKind = GetCurrentToken()->GetKind();
			if((eKind == ConstToken) || (eKind == TypeToken) || (eKind == VarToken) || (eKind == ProcedureToken) || 
			(eKind == IfToken) || (eKind == RepeatToken) || (eKind == WhileToken) || (eKind == WriteToken)
			|| (eKind == ReadToken) || (eKind == ProcedureToken))
			{
				return true;
			}
			else
			{
				IncrementPosition();
			}
		}
		return false;
	}*/
	TreeNode<Token*>* Parser::ParseReal(const unsigned int& iLevel)
	{
		// real = integer "." integer [ScaleFactor].
		Token* poLeadingIntegerToken = GetToken();
		Token* poPeriodToken = GetToken(1);
		Token* poTrailingIntegerToken = GetToken(2);
		if(poLeadingIntegerToken->GetKind() != IntegerToken)	return NULL;
		if(poPeriodToken->GetKind() != PeriodToken)				return NULL;
		if(poTrailingIntegerToken->GetKind() != IntegerToken)	return NULL;
		// now we have a real number, increment the position
		IncrementPosition(3);
		// build the tree
		TreeNode<Token*>* poRealNode = new TreeNode<Token*>;
		poRealNode->SetLevel(iLevel);
		poRealNode->SetLabel("Real");
		
		TreeNode<Token*>* poLeadingIntegerNode = new TreeNode<Token*>;
		poLeadingIntegerNode->SetLevel(iLevel + 1);
		poLeadingIntegerNode->SetData(poLeadingIntegerToken);
		poLeadingIntegerNode->SetLabel(poLeadingIntegerToken->GeneratePrintString());
		poLeadingIntegerNode->SetLeaf(true);
		poRealNode->AddChild(poLeadingIntegerNode);
		poLeadingIntegerNode->SetParent(poRealNode);
		
		TreeNode<Token*>* poPeriodNode = new TreeNode<Token*>;
		poPeriodNode->SetLevel(iLevel + 1);
		poPeriodNode->SetData(poPeriodToken);
		poPeriodNode->SetLabel(poPeriodToken->GeneratePrintString());
		poPeriodNode->SetLeaf(true);
		poRealNode->AddChild(poPeriodNode);
		poPeriodNode->SetParent(poRealNode);
		
		TreeNode<Token*>* poTrailingIntegerNode = new TreeNode<Token*>;
		poTrailingIntegerNode->SetLevel(iLevel + 1);
		poTrailingIntegerNode->SetData(poTrailingIntegerToken);
		poTrailingIntegerNode->SetLabel(poTrailingIntegerToken->GeneratePrintString());
		poTrailingIntegerNode->SetLeaf(true);
		poRealNode->AddChild(poTrailingIntegerNode);
		poTrailingIntegerNode->SetParent(poRealNode);
		
		// optionally, there is a scale factor, try to parse it
		TreeNode<Token*>* poScaleFactor = ParseScaleFactor(iLevel + 1);
		if(poScaleFactor != NULL)
		{
			// add it to the real node
			poRealNode->AddChild(poScaleFactor);
			poScaleFactor->SetParent(poRealNode);
		}
		// return it
		return poRealNode;
	}
	TreeNode<Token*>* Parser::ParseScaleFactor(const unsigned int& iLevel)
	{
		// ScaleFactor = "e" ("+" | "-") integer.
		Token* poExponentToken = GetToken();
		Token* poSignToken = GetToken(1);
		Token* poIntegerToken = GetToken(2);
		if(poExponentToken->GetKind() != ExponentEToken)	return NULL;
		if((poSignToken->GetKind() != PlusToken) && (poSignToken->GetKind() != MinusToken))			return NULL;
		if(poIntegerToken->GetKind() != IntegerToken)		return NULL;
		
		// now we have a full scale factor, increment the position first
		IncrementPosition(3);
		// build the tree
		TreeNode<Token*>* poScaleFactorNode = new TreeNode<Token*>;
		poScaleFactorNode->SetLevel(iLevel);
		poScaleFactorNode->SetLabel("ScaleFactor");
		
		TreeNode<Token*>* poExponentNode = new TreeNode<Token*>;
		poExponentNode->SetLevel(iLevel + 1);
		poExponentNode->SetData(poExponentToken);
		poExponentNode->SetLabel(poExponentToken->GeneratePrintString());
		poExponentNode->SetLeaf(true);
		poScaleFactorNode->AddChild(poExponentNode);
		poExponentNode->SetParent(poScaleFactorNode);
		
		TreeNode<Token*>* poSignNode = new TreeNode<Token*>;
		poSignNode->SetLevel(iLevel + 1);
		poSignNode->SetData(poSignToken);
		poSignNode->SetLabel(poSignToken->GeneratePrintString());
		poSignNode->SetLeaf(true);
		poScaleFactorNode->AddChild(poSignNode);
		poSignNode->SetParent(poScaleFactorNode);
		
		TreeNode<Token*>* poIntegerNode = new TreeNode<Token*>;
		poIntegerNode->SetLevel(iLevel + 1);
		poIntegerNode->SetData(poIntegerToken);
		poIntegerNode->SetLabel(poIntegerToken->GeneratePrintString());
		poIntegerNode->SetLeaf(true);
		poScaleFactorNode->AddChild(poIntegerNode);
		poIntegerNode->SetParent(poScaleFactorNode);
		
		// return it
		return poScaleFactorNode;	
	}
	TreeNode<Token*>* Parser::ParseNumber(const unsigned int& iLevel)
	{
		// number = integer | real.
		Token* poToken = GetToken();
		if(poToken->GetKind() != IntegerToken)			return NULL;
		poToken = GetToken(1);
		TreeNode<Token*>* poSubNode = NULL;
		if(poToken->GetKind() == PeriodToken)
		{
			poSubNode = ParseReal(iLevel + 1);
			if(poSubNode == NULL)							return NULL;
		}
		else
		{	
			poToken = GetToken();
			IncrementPosition();
			poSubNode = new TreeNode<Token*>;
			poSubNode->SetLevel(iLevel + 1);
			poSubNode->SetData(poToken);
			poSubNode->SetLabel(poToken->GeneratePrintString());
			poSubNode->SetLeaf(true);
		}
		TreeNode<Token*>* poNumberNode = new TreeNode<Token*>;
		poNumberNode->SetLevel(iLevel);
		poNumberNode->SetLabel("Number");
		poNumberNode->AddChild(poSubNode);
		poSubNode->SetParent(poNumberNode);	
		return poNumberNode;
	}
	TreeNode<Token*>* Parser::ParseFactor(const unsigned int& iLevel)
	{
		// factor = number | "null" | "true" | "false" | designator [ActualParameters] | "(" expression ")" | "not" factor.
		unsigned int iOriginalPosition = m_iPosition;
		Token* poToken = GetToken();
		TokenKind eKind = poToken->GetKind();
		TreeNode<Token*>* poFactorNode = NULL;
		TreeNode<Token*>* poSubNode = NULL;
		// check for the easy cases first
		if((eKind == NullToken) || (eKind == TrueToken) || (eKind == FalseToken))
		{
			IncrementPosition();
			poSubNode = new TreeNode<Token*>;
			poSubNode->SetLevel(iLevel + 1);
			poSubNode->SetData(poToken);
			poSubNode->SetLabel(poToken->GeneratePrintString());
			poSubNode->SetLeaf(true);
			
			poFactorNode = new TreeNode<Token*>;
			poFactorNode->SetLevel(iLevel);
			poFactorNode->SetLabel("Factor");
			poFactorNode->AddChild(poSubNode);
			poSubNode->SetParent(poFactorNode);
		}
		else if(eKind == NotToken)
		{
			IncrementPosition();
			// try to parse a factor
			poSubNode = ParseFactor(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			// factor parsed and its tree node created, create a not node and
			// add both to the factor node
			TreeNode<Token*>* poNotNode = new TreeNode<Token*>;
			poNotNode->SetLevel(iLevel + 1);
			poNotNode->SetData(poToken);
			poNotNode->SetLabel(poToken->GeneratePrintString());
			poNotNode->SetLeaf(true);
			
			poFactorNode = new TreeNode<Token*>;
			poFactorNode->SetLevel(iLevel);
			poFactorNode->SetLabel("Factor");
			poFactorNode->AddChild(poNotNode);
			poFactorNode->AddChild(poSubNode);
			poNotNode->SetParent(poFactorNode);
			poSubNode->SetParent(poFactorNode);
		}
		else if(eKind == RoundBracketOpenToken)
		{
			IncrementPosition();
			// try to parse an expression
			poSubNode = ParseExpression(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			// expression parsed and its tree node created, 
			// make sure that we have the closing bracket
			Token* poClosingBracketToken = GetToken();
			if(poToken->GetKind() != RoundBracketCloseToken)
			{
				// failed to find a closing bracket, delete the expression node
				m_iPosition = iOriginalPosition;
				poSubNode->DeleteChildren();
				delete poSubNode;
				return NULL;
			}
			IncrementPosition();
			// now we have everything we need, create the subtree
			TreeNode<Token*>* poOpenBracketNode = new TreeNode<Token*>;
			poOpenBracketNode->SetLevel(iLevel + 1);
			poOpenBracketNode->SetData(poToken);
			poOpenBracketNode->SetLabel(poToken->GeneratePrintString());
			poOpenBracketNode->SetLeaf(true);
			
			TreeNode<Token*>* poCloseBracketNode = new TreeNode<Token*>;
			poCloseBracketNode->SetLevel(iLevel + 1);
			poCloseBracketNode->SetData(poClosingBracketToken);
			poCloseBracketNode->SetLabel(poClosingBracketToken->GeneratePrintString());
			poCloseBracketNode->SetLeaf(true);
			
			poFactorNode = new TreeNode<Token*>;
			poFactorNode->SetLevel(iLevel);
			poFactorNode->SetLabel("Factor");
			poFactorNode->AddChild(poOpenBracketNode);
			poFactorNode->AddChild(poSubNode);
			poFactorNode->AddChild(poCloseBracketNode);
			poOpenBracketNode->SetParent(poFactorNode);
			poSubNode->SetParent(poFactorNode);
			poCloseBracketNode->SetParent(poFactorNode);
		}
		else if(eKind == IdentifierToken)
		{
			// a designator begins with an identifier, check to see whether we have
			// this as the first token, if so, try to parse it
			poSubNode = ParseDesignator(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			// now that we have a designator and a properly incremented position, 
			// try to parse the optional actual parameters
			TreeNode<Token*>* poActualParametersNode = ParseActualParameters(iLevel + 1);
			
			poFactorNode = new TreeNode<Token*>;
			poFactorNode->SetLevel(iLevel);
			poFactorNode->SetLabel("Factor");
			poFactorNode->AddChild(poSubNode);
			poSubNode->SetParent(poFactorNode);
			if(poActualParametersNode != NULL)
			{
				poFactorNode->AddChild(poActualParametersNode);
				poActualParametersNode->SetParent(poFactorNode);
			}
		}
		else
		{
			poSubNode = ParseNumber(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			
			poFactorNode = new TreeNode<Token*>;
			poFactorNode->SetLevel(iLevel);
			poFactorNode->SetLabel("Factor");
			poFactorNode->AddChild(poSubNode);
			poSubNode->SetParent(poFactorNode);
		}
		
		return poFactorNode;
	}
	TreeNode<Token*>* Parser::ParseTerm(const unsigned int& iLevel)
	{
		// term = factor {MulOperator factor}.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poFactor = ParseFactor(iLevel + 1);
		if(poFactor == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		
		list<TreeNode<Token*>*> lpoFactors;
		lpoFactors.push_back(poFactor);
		list<TreeNode<Token*>*> lpoMulOps;
		Token* poMulOpToken = NULL;
		TreeNode<Token*>* poMulOpNode = NULL;
		TokenKind eKind = NullTokenKind;
		while(true)
		{
			poMulOpToken = GetToken();
			eKind = poMulOpToken->GetKind();
			if((eKind != MultiplyToken) && (eKind != DivideToken) && (eKind != IntegerDivideToken) && (eKind != ModulusDivideToken) && (eKind != AndToken))
			{
				break;
			}
			IncrementPosition();
			poFactor = ParseFactor(iLevel + 1);
			if(poFactor == NULL)
			{
				// reset position
				m_iPosition = iOriginalPosition;
				// delete all created nodes
				while(!lpoFactors.empty())
				{
					poFactor = lpoFactors.front();
					if(poFactor != NULL)	
					{
						poFactor->DeleteChildren();
						delete poFactor;
					}
					lpoFactors.pop_front();
				}
				while(!lpoMulOps.empty())
				{
					poMulOpNode = lpoMulOps.front();
					if(poMulOpNode != NULL)
					{
						poMulOpNode->DeleteChildren();
						delete poMulOpNode;
					}
					lpoMulOps.pop_front();
				}
				return NULL;
			}
			// found a multiplicative operator followed by a factor, push them into the lists
			poMulOpNode = new TreeNode<Token*>;
			poMulOpNode->SetLevel(iLevel + 1);
			poMulOpNode->SetData(poMulOpToken);
			poMulOpNode->SetLabel(poMulOpToken->GeneratePrintString());
			poMulOpNode->SetLeaf(true);
			lpoMulOps.push_back(poMulOpNode);
			lpoFactors.push_back(poFactor);
		}
		// now we have a list of factor nodes and a list of mulop  nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poTermNode = new TreeNode<Token*>;
		poTermNode->SetLevel(iLevel);
		poTermNode->SetLabel("Term");
		// add the first expression to the parent node
		poFactor = lpoFactors.front();
		lpoFactors.pop_front();
		poTermNode->AddChild(poFactor);
		poFactor->SetParent(poTermNode);
		// add the rest of the factors and the mulops to the parent node
		while(!lpoFactors.empty())
		{
			poMulOpNode = lpoMulOps.front();
			lpoMulOps.pop_front();
			poTermNode->AddChild(poMulOpNode);
			poMulOpNode->SetParent(poTermNode);
			
			poFactor = lpoFactors.front();
			lpoFactors.pop_front();
			poTermNode->AddChild(poFactor);
			poFactor->SetParent(poTermNode);
		}
		return poTermNode;
	}
	TreeNode<Token*>* Parser::ParseSimpleExpression(const unsigned int& iLevel)
	{
		// SimpleExpression = ["+" | "-"] term {AddOperator term}.
		unsigned int iOriginalPosition = m_iPosition;
		// look for a leading sign
		bool bLeadingSignFound = false;
		Token* poLeadingSignToken = GetToken();
		if((poLeadingSignToken->GetKind() == PlusToken) || (poLeadingSignToken->GetKind() == MinusToken))
		{
			bLeadingSignFound = true;
			IncrementPosition();
		}
		TreeNode<Token*>* poTerm = ParseTerm(iLevel + 1);
		if(poTerm == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		
		list<TreeNode<Token*>*> lpoTerms;
		lpoTerms.push_back(poTerm);
		list<TreeNode<Token*>*> lpoAddOps;
		Token* poAddOpToken = NULL;
		TreeNode<Token*>* poAddOpNode = NULL;
		TokenKind eKind = NullTokenKind;
		while(true)
		{
			poAddOpToken = GetToken();
			eKind = poAddOpToken->GetKind();
			if((eKind != PlusToken) && (eKind != MinusToken) && (eKind != OrToken))
			{
				break;
			}
			IncrementPosition();
			poTerm = ParseTerm(iLevel + 1);
			if(poTerm == NULL)
			{
				// reset position
				m_iPosition = iOriginalPosition;
				// delete all created nodes
				while(!lpoTerms.empty())
				{
					poTerm = lpoTerms.front();
					if(poTerm != NULL)
					{
						poTerm->DeleteChildren();
						delete poTerm;
					}
					lpoTerms.pop_front();
				}
				while(!lpoAddOps.empty())
				{
					poAddOpNode = lpoAddOps.front();
					if(poAddOpNode != NULL)
					{
						poAddOpNode->DeleteChildren();
						delete poAddOpNode;
					}
					lpoAddOps.pop_front();
				}
				return NULL;
			}
			// found an additive operator followed by a term, push them into the lists
			poAddOpNode = new TreeNode<Token*>;
			poAddOpNode->SetLevel(iLevel + 1);
			poAddOpNode->SetData(poAddOpToken);
			poAddOpNode->SetLabel(poAddOpToken->GeneratePrintString());
			poAddOpNode->SetLeaf(true);
			lpoAddOps.push_back(poAddOpNode);
			lpoTerms.push_back(poTerm);
		}
		// now we have a list of term nodes and a list of addop  nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poSimpleExpressionNode = new TreeNode<Token*>;
		poSimpleExpressionNode->SetLevel(iLevel);
		poSimpleExpressionNode->SetLabel("SimpleExpression");
		// add the leading sign if found
		if(bLeadingSignFound)
		{
			TreeNode<Token*>* poSignNode = new TreeNode<Token*>;
			poSignNode->SetLevel(iLevel + 1);
			poSignNode->SetData(poLeadingSignToken);
			poSignNode->SetLabel(poLeadingSignToken->GeneratePrintString());
			poSignNode->SetLeaf(true);
			poSimpleExpressionNode->AddChild(poSignNode);
			poSignNode->SetParent(poSimpleExpressionNode);
		}
		// add the first expression to the parent node
		poTerm = lpoTerms.front();
		lpoTerms.pop_front();
		poSimpleExpressionNode->AddChild(poTerm);
		poTerm->SetParent(poSimpleExpressionNode);
		// add the rest of the expressions and the addops to the parent node
		while(!lpoTerms.empty())
		{
			poAddOpNode = lpoAddOps.front();
			lpoAddOps.pop_front();
			poSimpleExpressionNode->AddChild(poAddOpNode);
			poAddOpNode->SetParent(poSimpleExpressionNode);
			
			poTerm = lpoTerms.front();
			lpoTerms.pop_front();
			poSimpleExpressionNode->AddChild(poTerm);
			poTerm->SetParent(poSimpleExpressionNode);
		}
		return poSimpleExpressionNode;
	}
	TreeNode<Token*>* Parser::ParseExpression(const unsigned int& iLevel)
	{
		// expression = SimpleExpression [relation SimpleExpression].
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poFirstExpression = ParseSimpleExpression(iLevel + 1);
		if(poFirstExpression == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		TreeNode<Token*>* poExpressionNode = new TreeNode<Token*>;
		poExpressionNode->SetLevel(iLevel);
		poExpressionNode->SetLabel("Expression");
		poExpressionNode->AddChild(poFirstExpression);
		poFirstExpression->SetParent(poExpressionNode);
		
		// look for the optional second expression
		Token* poRelOpToken = GetToken();
		TokenKind eKind = poRelOpToken->GetKind();
		if((eKind == EqualToken) || (eKind == NotEqualToken) || (eKind == LessThanToken) || (eKind == GreaterThanToken) || (eKind == LessThanOrEqualToken) || (eKind == GreaterThanOrEqualToken))
		{
			// look for the second expression
			IncrementPosition();
			TreeNode<Token*>* poSecondExpression = ParseSimpleExpression(iLevel + 1);
			if(poSecondExpression == NULL)
			{
				m_iPosition = iOriginalPosition;
				poExpressionNode->DeleteChildren();
				delete poExpressionNode;
				return NULL;
			}
		
			TreeNode<Token*>* poRelOpNode = new TreeNode<Token*>;
			poRelOpNode->SetLevel(iLevel + 1);
			poRelOpNode->SetData(poRelOpToken);
			poRelOpNode->SetLabel(poRelOpToken->GeneratePrintString());
			poRelOpNode->SetLeaf(true);
			poExpressionNode->AddChild(poRelOpNode);
			poRelOpNode->SetParent(poExpressionNode);
			
			poExpressionNode->AddChild(poSecondExpression);
			poSecondExpression->SetParent(poExpressionNode);
		}
		
		return poExpressionNode;
	}
	TreeNode<Token*>* Parser::ParseQualifiedIdentifier(const unsigned int& iLevel)
	{
		// qualident = [ident "."] ident.
		Token* poFirstIdentifier = GetToken();
		if(poFirstIdentifier->GetKind() != IdentifierToken)		return NULL;
		Token* poPeriodToken = GetToken(1);
		TreeNode<Token*>* poQualifiedIdentifierNode = NULL;
		if(poPeriodToken->GetKind() == PeriodToken)
		{
			Token* poSecondIdentifier = GetToken(2);
			if(poSecondIdentifier->GetKind() != IdentifierToken)	return NULL;
			IncrementPosition(3);
			
			TreeNode<Token*>* poFirstIdentifierNode = new TreeNode<Token*>;
			poFirstIdentifierNode->SetLevel(iLevel + 1);
			poFirstIdentifierNode->SetData(poFirstIdentifier);
			poFirstIdentifierNode->SetLabel(poFirstIdentifier->GeneratePrintString());
			poFirstIdentifierNode->SetLeaf(true);
			
			TreeNode<Token*>* poPeriodNode = new TreeNode<Token*>;
			poPeriodNode->SetLevel(iLevel + 1);
			poPeriodNode->SetData(poPeriodToken);
			poPeriodNode->SetLabel(poPeriodToken->GeneratePrintString());
			poPeriodNode->SetLeaf(true);	
			
			TreeNode<Token*>* poSecondIdentifierNode = new TreeNode<Token*>;
			poSecondIdentifierNode->SetLevel(iLevel + 1);
			poSecondIdentifierNode->SetData(poSecondIdentifier);
			poSecondIdentifierNode->SetLabel(poSecondIdentifier->GeneratePrintString());
			poSecondIdentifierNode->SetLeaf(true);
			
			poQualifiedIdentifierNode = new TreeNode<Token*>;
			poQualifiedIdentifierNode->SetLevel(iLevel);
			poQualifiedIdentifierNode->SetLabel("QualifiedIdentifier");
			poQualifiedIdentifierNode->AddChild(poFirstIdentifierNode);
			poQualifiedIdentifierNode->AddChild(poPeriodNode);
			poQualifiedIdentifierNode->AddChild(poSecondIdentifierNode);
			poFirstIdentifierNode->SetParent(poQualifiedIdentifierNode);
			poPeriodNode->SetParent(poQualifiedIdentifierNode);
			poSecondIdentifierNode->SetParent(poQualifiedIdentifierNode);
		}
		else
		{
			IncrementPosition();
			TreeNode<Token*>* poSubNode = new TreeNode<Token*>;
			poSubNode->SetLevel(iLevel + 1);
			poSubNode->SetData(poFirstIdentifier);
			poSubNode->SetLabel(poFirstIdentifier->GeneratePrintString());
			poSubNode->SetLeaf(true);	
			
			poQualifiedIdentifierNode = new TreeNode<Token*>;
			poQualifiedIdentifierNode->SetLevel(iLevel);
			poQualifiedIdentifierNode->SetLabel("QualifiedIdentifier");
			poQualifiedIdentifierNode->AddChild(poSubNode);
			poSubNode->SetParent(poQualifiedIdentifierNode);
		}
		
		return poQualifiedIdentifierNode;
	}
	TreeNode<Token*>* Parser::ParseExpressionList(const unsigned int& iLevel)
	{
		// ExpList = expression {"," expression}.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poExpressionNode = ParseExpression(iLevel + 1);
		if(poExpressionNode == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		list<TreeNode<Token*>*> lpoExpressions;
		lpoExpressions.push_back(poExpressionNode);
		list<TreeNode<Token*>*> lpoCommaNodes;
		Token* poCommaToken = NULL;
		TreeNode<Token*>* poCommaNode = NULL;
		while(true)
		{
			poCommaToken = GetToken();
			if(poCommaToken->GetKind() != CommaToken)		break;
			IncrementPosition();
			poExpressionNode = ParseExpression(iLevel + 1);
			if(poExpressionNode == NULL)
			{
				// reset position
				m_iPosition = iOriginalPosition;
				// delete all created nodes
				while(!lpoExpressions.empty())
				{
					poExpressionNode = lpoExpressions.front();
					if(poExpressionNode != NULL)
					{
						poExpressionNode->DeleteChildren();
						delete poExpressionNode;
					}
					lpoExpressions.pop_front();
				}
				while(!lpoCommaNodes.empty())
				{
					poCommaNode = lpoCommaNodes.front();
					if(poCommaNode != NULL)
					{
						poCommaNode->DeleteChildren();
						delete poCommaNode;
					}
					lpoCommaNodes.pop_front();
				}
				return NULL;
			}
			// found a comma followed by an expression, push them into the list
			poCommaNode = new TreeNode<Token*>;
			poCommaNode->SetLevel(iLevel + 1);
			poCommaNode->SetData(poCommaToken);
			poCommaNode->SetLabel(poCommaToken->GeneratePrintString());
			poCommaNode->SetLeaf(true);
			lpoCommaNodes.push_back(poCommaNode);
			lpoExpressions.push_back(poExpressionNode);
		}
		// now we have a list of expression nodes and a list of comma nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poExpressionListNode = new TreeNode<Token*>;
		poExpressionListNode->SetLevel(iLevel);
		poExpressionListNode->SetLabel("ExpressionList");
		// add the first expression to the parent node
		poExpressionNode = lpoExpressions.front();
		lpoExpressions.pop_front();
		poExpressionListNode->AddChild(poExpressionNode);
		poExpressionNode->SetParent(poExpressionListNode);
		// add the rest of the expressions and the commas to the parent node
		while(!lpoExpressions.empty())
		{
			poCommaNode = lpoCommaNodes.front();
			lpoCommaNodes.pop_front();
			poExpressionListNode->AddChild(poCommaNode);
			poCommaNode->SetParent(poExpressionListNode);
			
			poExpressionNode = lpoExpressions.front();
			lpoExpressions.pop_front();
			poExpressionListNode->AddChild(poExpressionNode);
			poExpressionNode->SetParent(poExpressionListNode);
		}
		return poExpressionListNode;
	}
	TreeNode<Token*>* Parser::ParseSelector(const unsigned int& iLevel)
	{
		// selector = "." ident | "[" ExpList "]" | "^" | "(" qualident ")".
		unsigned int iOriginalPosition = m_iPosition;
		Token* poFirstToken = GetToken();
		TokenKind eKind = poFirstToken->GetKind();
		if(eKind == PeriodToken)
		{	
			Token* poSecondToken = GetToken(1);
			if(poSecondToken->GetKind() != IdentifierToken)
			{
				return NULL;
			}
			IncrementPosition(2);
			
			TreeNode<Token*>* poPeriodNode = new TreeNode<Token*>;
			poPeriodNode->SetLevel(iLevel + 1);
			poPeriodNode->SetData(poFirstToken);
			poPeriodNode->SetLabel(poFirstToken->GeneratePrintString());
			poPeriodNode->SetLeaf(true);
			
			TreeNode<Token*>* poIdentifierNode = new TreeNode<Token*>;
			poIdentifierNode->SetLevel(iLevel + 1);
			poIdentifierNode->SetData(poSecondToken);
			poIdentifierNode->SetLabel(poSecondToken->GeneratePrintString());
			poIdentifierNode->SetLeaf(true);
			
			TreeNode<Token*>* poSelectorNode = new TreeNode<Token*>;
			poSelectorNode->SetLevel(iLevel);
			poSelectorNode->SetLabel("Selector");
			poSelectorNode->AddChild(poPeriodNode);
			poSelectorNode->AddChild(poIdentifierNode);
			poPeriodNode->SetParent(poSelectorNode);
			poIdentifierNode->SetParent(poSelectorNode);
			
			return poSelectorNode;
		}
		else if(eKind == SquareBracketOpenToken)
		{
			IncrementPosition();
			TreeNode<Token*>* poSubNode = ParseExpressionList(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			Token* poLastToken = GetToken();
			if(poLastToken->GetKind() != SquareBracketCloseToken)
			{
				poSubNode->DeleteChildren();
				delete poSubNode;
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			IncrementPosition();
			
			TreeNode<Token*>* poOpenBracketNode = new TreeNode<Token*>;
			poOpenBracketNode->SetLevel(iLevel + 1);
			poOpenBracketNode->SetData(poFirstToken);
			poOpenBracketNode->SetLabel(poFirstToken->GeneratePrintString());
			poOpenBracketNode->SetLeaf(true);
			
			TreeNode<Token*>* poCloseBracketNode = new TreeNode<Token*>;
			poCloseBracketNode->SetLevel(iLevel + 1);
			poCloseBracketNode->SetData(poLastToken);
			poCloseBracketNode->SetLabel(poLastToken->GeneratePrintString());
			poCloseBracketNode->SetLeaf(true);
			
			TreeNode<Token*>* poSelectorNode = new TreeNode<Token*>;
			poSelectorNode->SetLevel(iLevel);
			poSelectorNode->SetLabel("Selector");
			poSelectorNode->AddChild(poOpenBracketNode);
			poSelectorNode->AddChild(poSubNode);
			poSelectorNode->AddChild(poCloseBracketNode);
			
			poOpenBracketNode->SetParent(poSelectorNode);
			poSubNode->SetParent(poSelectorNode);
			poCloseBracketNode->SetParent(poSelectorNode);
			
			return poSelectorNode;
		}
		else if(eKind == CaretToken)
		{
			IncrementPosition();
			TreeNode<Token*>* poCaretNode = new TreeNode<Token*>;
			poCaretNode->SetLevel(iLevel + 1);
			poCaretNode->SetData(poFirstToken);
			poCaretNode->SetLabel(poFirstToken->GeneratePrintString());
			poCaretNode->SetLeaf(true);
			
			TreeNode<Token*>* poSelectorNode = new TreeNode<Token*>;
			poSelectorNode->SetLevel(iLevel);
			poSelectorNode->SetLabel("Selector");
			poSelectorNode->AddChild(poCaretNode);
			poCaretNode->SetParent(poSelectorNode);
			
			return poSelectorNode;
		}
		else if(eKind == RoundBracketOpenToken)
		{
			IncrementPosition();
			TreeNode<Token*>* poSubNode = ParseQualifiedIdentifier(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			Token* poLastToken = GetToken();
			if(poLastToken->GetKind() != RoundBracketCloseToken)
			{
				poSubNode->DeleteChildren();
				delete poSubNode;
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			IncrementPosition();
			
			TreeNode<Token*>* poOpenBracketNode = new TreeNode<Token*>;
			poOpenBracketNode->SetLevel(iLevel + 1);
			poOpenBracketNode->SetData(poFirstToken);
			poOpenBracketNode->SetLabel(poFirstToken->GeneratePrintString());
			poOpenBracketNode->SetLeaf(true);
			
			TreeNode<Token*>* poCloseBracketNode = new TreeNode<Token*>;
			poCloseBracketNode->SetLevel(iLevel + 1);
			poCloseBracketNode->SetData(poLastToken);
			poCloseBracketNode->SetLabel(poLastToken->GeneratePrintString());
			poCloseBracketNode->SetLeaf(true);
			
			TreeNode<Token*>* poSelectorNode = new TreeNode<Token*>;
			poSelectorNode->SetLevel(iLevel);
			poSelectorNode->SetLabel("Selector");
			poSelectorNode->AddChild(poOpenBracketNode);
			poSelectorNode->AddChild(poSubNode);
			poSelectorNode->AddChild(poCloseBracketNode);
			
			poOpenBracketNode->SetParent(poSelectorNode);
			poSubNode->SetParent(poSelectorNode);
			poCloseBracketNode->SetParent(poSelectorNode);
			
			return poSelectorNode;
		}
		
		return NULL;
	}
	TreeNode<Token*>* Parser::ParseDesignator(const unsigned int& iLevel)
	{
		// designator = qualident {selector}.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poIdentifierNode = ParseQualifiedIdentifier(iLevel + 1);
		if(poIdentifierNode == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		
		list<TreeNode<Token*>*> lpoSelectors;
		TreeNode<Token*>* poSelectorNode = NULL;
		while(true)
		{
			poSelectorNode = ParseSelector(iLevel + 1);
			if(poSelectorNode == NULL)					break;
			lpoSelectors.push_back(poSelectorNode);
		}
		// now we have a list of selector nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poDesignatorNode = new TreeNode<Token*>;
		poDesignatorNode->SetLevel(iLevel);
		poDesignatorNode->SetLabel("Designator");
		// add the identifier to the parent node
		poDesignatorNode->AddChild(poIdentifierNode);
		poIdentifierNode->SetParent(poDesignatorNode);
		// add the selectors to the parent node
		while(!lpoSelectors.empty())
		{
			poSelectorNode = lpoSelectors.front();
			lpoSelectors.pop_front();
			poDesignatorNode->AddChild(poSelectorNode);
			poSelectorNode->SetParent(poDesignatorNode);
		}
		return poDesignatorNode;
	}
	TreeNode<Token*>* Parser::ParseActualParameters(const unsigned int& iLevel)
	{
		// ActualParameters = "(" [ExpList] ")" .
		unsigned int iOriginalPosition = m_iPosition;
		Token* poFirstToken = GetToken();
		if(poFirstToken->GetKind() != RoundBracketOpenToken)	return NULL;
		IncrementPosition();
		TreeNode<Token*>* poSubNode = ParseExpressionList(iLevel + 1);
		Token* poLastToken = GetToken();
		if(poLastToken->GetKind() != RoundBracketCloseToken)
		{
			m_iPosition = iOriginalPosition;
			if(poSubNode != NULL)
			{
				poSubNode->DeleteChildren();
				delete poSubNode;
			}
			return NULL;
		}
		IncrementPosition();
		
		TreeNode<Token*>* poOpenBracketNode = new TreeNode<Token*>;
		poOpenBracketNode->SetLevel(iLevel + 1);
		poOpenBracketNode->SetData(poFirstToken);
		poOpenBracketNode->SetLabel(poFirstToken->GeneratePrintString());
		poOpenBracketNode->SetLeaf(true);
		
		TreeNode<Token*>* poCloseBracketNode = new TreeNode<Token*>;
		poCloseBracketNode->SetLevel(iLevel + 1);
		poCloseBracketNode->SetData(poLastToken);
		poCloseBracketNode->SetLabel(poLastToken->GeneratePrintString());
		poCloseBracketNode->SetLeaf(true);
		
		TreeNode<Token*>* poActualParametersNode = new TreeNode<Token*>;
		poActualParametersNode->SetLevel(iLevel);
		poActualParametersNode->SetLabel("ActualParameters");
		poActualParametersNode->AddChild(poOpenBracketNode);
		if(poSubNode != NULL)
		{
			poActualParametersNode->AddChild(poSubNode);
		}
		poActualParametersNode->AddChild(poCloseBracketNode);
		
		poOpenBracketNode->SetParent(poActualParametersNode);
		if(poSubNode != NULL)
		{
			poSubNode->SetParent(poActualParametersNode);
		}
		poCloseBracketNode->SetParent(poActualParametersNode);
		
		return poActualParametersNode;
	}
	TreeNode<Token*>* Parser::ParseAssign(const unsigned int& iLevel)
	{
		// assignment = designator "=" expression.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poDesignator = ParseDesignator(iLevel + 1);
		if(poDesignator == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		Token* poAssignToken = GetToken();
		if(poAssignToken->GetKind() != AssignToken)
		{
			m_iPosition = iOriginalPosition;
			poDesignator->DeleteChildren();
			delete poDesignator;
			return NULL;
		}
		IncrementPosition();
		TreeNode<Token*>* poExpression = ParseExpression(iLevel + 1);
		if(poExpression == NULL)
		{
			m_iPosition = iOriginalPosition;
			poDesignator->DeleteChildren();
			delete poDesignator;
			return NULL;
		}
		
		TreeNode<Token*>* poAssignNode = new TreeNode<Token*>;
		poAssignNode->SetLevel(iLevel + 1);
		poAssignNode->SetData(poAssignToken);
		poAssignNode->SetLabel(poAssignToken->GeneratePrintString());
		poAssignNode->SetLeaf(true);
		
		TreeNode<Token*>* poAssignmentNode = new TreeNode<Token*>;
		poAssignmentNode->SetLevel(iLevel);
		poAssignmentNode->SetLabel("Assignment");
		poAssignmentNode->AddChild(poDesignator);
		poAssignmentNode->AddChild(poAssignNode);
		poAssignmentNode->AddChild(poExpression);
		poDesignator->SetParent(poAssignmentNode);
		poAssignNode->SetParent(poAssignmentNode);
		poExpression->SetParent(poAssignmentNode);
		return poAssignmentNode;
	}
	TreeNode<Token*>* Parser::ParseCall(const unsigned int& iLevel)
	{
		// ProcedureCall = designator [ActualParameters].
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poDesignatorNode = ParseDesignator(iLevel + 1);
		if(poDesignatorNode == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		// now that we have a designator and a properly incremented position, 
		// try to parse the optional actual parameters
		TreeNode<Token*>* poActualParametersNode = ParseActualParameters(iLevel + 1);
		
		TreeNode<Token*>* poCallNode = new TreeNode<Token*>;
		poCallNode->SetLevel(iLevel);
		poCallNode->SetLabel("Call");
		poCallNode->AddChild(poDesignatorNode);
		poDesignatorNode->SetParent(poCallNode);
		if(poActualParametersNode != NULL)
		{
			poCallNode->AddChild(poActualParametersNode);
			poActualParametersNode->SetParent(poCallNode);
		}
		return poCallNode;
	}
	TreeNode<Token*>* Parser::ParseIf(const unsigned int& iLevel)
	{
		// IfStatement = "if" expression "{" StatementSequence "}" {"elseif" expression "{" StatementSequence "}"} ["else" "{" StatementSequence "}"].
		unsigned int iOriginalPosition = m_iPosition;
		Token* poIfToken = GetToken();
		if(poIfToken->GetKind() != IfToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		TreeNode<Token*>* poExpression = NULL;
		TreeNode<Token*>* poStatementSequence = NULL;
		TreeNode<Token*>* poOpenBracesNode = NULL;
		TreeNode<Token*>* poCloseBracesNode = NULL;
		if(!ParseControlBody(iLevel + 1,poExpression,poOpenBracesNode,poStatementSequence,poCloseBracesNode))
		{
			// failed to find a control body, however, all the newly created
			// objects have been deleted, restore the position and return NULL
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		// now we a basic if statement, create the tree
		TreeNode<Token*>* poIfNode = new TreeNode<Token*>;
		poIfNode->SetLevel(iLevel + 1);
		poIfNode->SetData(poIfToken);
		poIfNode->SetLabel(poIfToken->GeneratePrintString());
		poIfNode->SetLeaf(true);
		TreeNode<Token*>* poIfStatementNode = new TreeNode<Token*>;
		poIfStatementNode->SetLevel(iLevel);
		poIfStatementNode->SetLabel("IfStatement");
		poIfStatementNode->AddChild(poIfNode);
		poIfStatementNode->AddChild(poExpression);
		poIfStatementNode->AddChild(poOpenBracesNode);
		if(poStatementSequence != NULL)
		{
			poIfStatementNode->AddChild(poStatementSequence);
		}
		poIfStatementNode->AddChild(poCloseBracesNode);
		poIfNode->SetParent(poIfStatementNode);
		poExpression->SetParent(poIfStatementNode);
		poOpenBracesNode->SetParent(poIfStatementNode);
		if(poStatementSequence != NULL)
		{
			poStatementSequence->SetParent(poIfStatementNode);
		}
		poCloseBracesNode->SetParent(poIfStatementNode);
		// now look for any number of elseif statements
		while(true)
		{
			poIfToken = GetToken();
			if(poIfToken->GetKind() != ElseIfToken)		break;
			IncrementPosition();
			if(!ParseControlBody(iLevel + 1,poExpression,poOpenBracesNode,poStatementSequence,poCloseBracesNode))
			{
				// elseif control body failure after finding elseif, abort
				m_iPosition = iOriginalPosition;
				poIfStatementNode->DeleteChildren();
				delete poIfStatementNode;
				return NULL;
			}
		
			poIfNode = new TreeNode<Token*>;
			poIfNode->SetLevel(iLevel + 1);
			poIfNode->SetData(poIfToken);
			poIfNode->SetLabel(poIfToken->GeneratePrintString());
			poIfNode->SetLeaf(true);
			
			poIfStatementNode->AddChild(poIfNode);
			poIfStatementNode->AddChild(poExpression);
			poIfStatementNode->AddChild(poOpenBracesNode);
			if(poStatementSequence != NULL)
			{
				poIfStatementNode->AddChild(poStatementSequence);
			}
			poIfStatementNode->AddChild(poCloseBracesNode);
			
			poIfNode->SetParent(poIfStatementNode);
			poExpression->SetParent(poIfStatementNode);
			poOpenBracesNode->SetParent(poIfStatementNode);
			if(poStatementSequence != NULL)
			{
				poStatementSequence->SetParent(poIfStatementNode);
			}
			poCloseBracesNode->SetParent(poIfStatementNode);
		}
		
		// finally, look for an optional else statement
		poIfToken = GetToken();
		if(poIfToken->GetKind() == ElseToken)
		{
			IncrementPosition();
			if(!ParseStatementBlock(iLevel + 1,poOpenBracesNode,poStatementSequence,poCloseBracesNode))
			{
				// else control body failure after finding elseif, abort
				m_iPosition = iOriginalPosition;
				poIfStatementNode->DeleteChildren();
				delete poIfStatementNode;
				return NULL;
			}
			poIfNode = new TreeNode<Token*>;
			poIfNode->SetLevel(iLevel + 1);
			poIfNode->SetData(poIfToken);
			poIfNode->SetLabel(poIfToken->GeneratePrintString());
			poIfNode->SetLeaf(true);
			
			poIfStatementNode->AddChild(poIfNode);
			poIfStatementNode->AddChild(poOpenBracesNode);
			if(poStatementSequence != NULL)
			{
				poIfStatementNode->AddChild(poStatementSequence);
			}
			poIfStatementNode->AddChild(poCloseBracesNode);
			
			poIfNode->SetParent(poIfStatementNode);
			poOpenBracesNode->SetParent(poIfStatementNode);
			if(poStatementSequence != NULL)
			{
				poStatementSequence->SetParent(poIfStatementNode);
			}
			poCloseBracesNode->SetParent(poIfStatementNode);
		}
		return poIfStatementNode;
	}
	TreeNode<Token*>* Parser::ParseWhile(const unsigned int& iLevel)
	{
		// WhileStatement = "while" expression "{" StatementSequence "}".
		unsigned int iOriginalPosition = m_iPosition;
		Token* poWhileToken = GetToken();
		if(poWhileToken->GetKind() != WhileToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		TreeNode<Token*>* poExpression = NULL;
		TreeNode<Token*>* poStatementSequence = NULL;
		TreeNode<Token*>* poOpenBracesNode = NULL;
		TreeNode<Token*>* poCloseBracesNode = NULL;
		if(!ParseControlBody(iLevel + 1,poExpression,poOpenBracesNode,poStatementSequence,poCloseBracesNode))
		{
			// failed to find a control body, however, all the newly created
			// objects have been deleted, restore the position and return NULL
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		// now we have everything, create the tree
		TreeNode<Token*>* poWhileNode = new TreeNode<Token*>;
		poWhileNode->SetLevel(iLevel + 1);
		poWhileNode->SetData(poWhileToken);
		poWhileNode->SetLabel(poWhileToken->GeneratePrintString());
		poWhileNode->SetLeaf(true);
		
		TreeNode<Token*>* poWhileStatementNode = new TreeNode<Token*>;
		poWhileStatementNode->SetLevel(iLevel);
		poWhileStatementNode->SetLabel("WhileStatement");
		poWhileStatementNode->AddChild(poWhileNode);
		poWhileStatementNode->AddChild(poExpression);
		poWhileStatementNode->AddChild(poOpenBracesNode);
		if(poStatementSequence != NULL)
		{
			poWhileStatementNode->AddChild(poStatementSequence);
		}
		poWhileStatementNode->AddChild(poCloseBracesNode);
		poWhileNode->SetParent(poWhileStatementNode);
		poExpression->SetParent(poWhileStatementNode);
		poOpenBracesNode->SetParent(poWhileStatementNode);
		if(poStatementSequence != NULL)
		{
			poStatementSequence->SetParent(poStatementSequence);
		}
		poCloseBracesNode->SetParent(poWhileStatementNode);
		return poWhileStatementNode;
	}
	TreeNode<Token*>* Parser::ParseWrite(const unsigned int& iLevel)
	{
		// Write = "print" expression .
		unsigned int iOriginalPosition = m_iPosition;
		Token* poPrintToken = GetToken();
		if(poPrintToken->GetKind() != PrintToken)			return NULL;
		IncrementPosition();
		TreeNode<Token*>* poExpression = ParseExpression(iLevel + 1);
		if(poExpression == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		
		TreeNode<Token*>* poWriteNode = new TreeNode<Token*>;
		poWriteNode->SetLevel(iLevel);
		poWriteNode->SetLabel("Write");
		
		TreeNode<Token*>* poPrintNode = new TreeNode<Token*>;
		poPrintNode->SetLevel(iLevel + 1);
		poPrintNode->SetData(poPrintToken);
		poPrintNode->SetLabel(poPrintToken->GeneratePrintString());
		poPrintNode->SetLeaf(true);
			
		poWriteNode->AddChild(poPrintNode);
		poWriteNode->AddChild(poExpression);
		poPrintNode->SetParent(poWriteNode);
		poExpression->SetParent(poWriteNode);
		
		return poWriteNode;
	}
	TreeNode<Token*>* Parser::ParseStatement(const unsigned int& iLevel)
	{
		// statement = [assignment | ProcedureCall | IfStatement | WhileStatement | Write]. ";"
		unsigned int iOriginalPosition = m_iPosition;
		Token* poFirstToken = GetToken();
		TokenKind eKind = poFirstToken->GetKind();
		TreeNode<Token*>* poSubNode = NULL;
		
		if(eKind == IfToken)
		{
			poSubNode = ParseIf(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		else if(eKind == WhileToken)
		{
			poSubNode = ParseWhile(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		else if(eKind == PrintToken)
		{
			poSubNode = ParseWrite(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		else
		{
			// the statement can be a procedure call or an assignment
			// both of them begin by designators, but the procedure call
			// does not have an equal sign "=" in the statement, hence, 
			// try to parse an assignment statement, and if this doesn't
			// work, try to parse a call statement
			poSubNode = ParseAssign(iLevel + 1);
			if(poSubNode == NULL)
			{
				poSubNode = ParseCall(iLevel + 1);
				if(poSubNode == NULL)
				{
					// nothing works, return NULL
					m_iPosition = iOriginalPosition;
					return NULL;
				}
			}
		}
		// now we have a statement, make sure that it is followed by a semicolon
		Token* poSemiColonToken = GetToken();
		if(poSemiColonToken->GetKind() != SemiColonToken)
		{
			m_iPosition = iOriginalPosition;
			poSubNode->DeleteChildren();
			delete poSubNode;
			return NULL;
		}
		// all is ok, create the tree
		IncrementPosition();
		TreeNode<Token*>* poSemiColonNode = new TreeNode<Token*>;
		poSemiColonNode->SetLevel(iLevel + 1);
		poSemiColonNode->SetData(poSemiColonToken);
		poSemiColonNode->SetLabel(poSemiColonToken->GeneratePrintString());
		poSemiColonNode->SetLeaf(true);
			
		TreeNode<Token*>* poStatementNode = new TreeNode<Token*>;
		poStatementNode->SetLevel(iLevel);
		poStatementNode->SetLabel("Statement");
		poStatementNode->AddChild(poSubNode);
		poStatementNode->AddChild(poSemiColonNode);
		poSubNode->SetParent(poStatementNode);
		poSemiColonNode->SetParent(poStatementNode);
		return poStatementNode;
	}
	TreeNode<Token*>* Parser::ParseStatementSequence(const unsigned int& iLevel)
	{
		// StatementSequence = statement { statement }.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poStatement = ParseStatement(iLevel + 1);
		if(poStatement == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		list<TreeNode<Token*>*> lpoStatements;
		lpoStatements.push_back(poStatement);

		while(true)
		{
			poStatement = ParseStatement(iLevel + 1);
			if(poStatement == NULL)
			{
				// either there are no statements to parse or there are ungrammatical statements.
				// in both cases, break the loop and return what we have so far
				break;
			}
			lpoStatements.push_back(poStatement);
		}
		// now we have a list of expression nodes and a list of comma nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poStatementSequenceNode = new TreeNode<Token*>;
		poStatementSequenceNode->SetLevel(iLevel);
		poStatementSequenceNode->SetLabel("StatementSequence");
		// add the first statement to the parent node
		poStatement = lpoStatements.front();
		lpoStatements.pop_front();
		poStatementSequenceNode->AddChild(poStatement);
		poStatement->SetParent(poStatementSequenceNode);
		// add the rest of the expressions and the commas to the parent node
		while(!lpoStatements.empty())
		{			
			poStatement = lpoStatements.front();
			lpoStatements.pop_front();
			poStatementSequenceNode->AddChild(poStatement);
			poStatement->SetParent(poStatementSequenceNode);
		}
		return poStatementSequenceNode;
	}
	bool Parser::ParseControlBody(const unsigned int& iLevel,TreeNode<Token*>*& poExpression,TreeNode<Token*>*& poOpenBrace,TreeNode<Token*>*& poStatementSequence,TreeNode<Token*>*& poCloseBrace)
	{
		// a control body is an expression, followed by a statement block
		unsigned int iOriginalPosition = m_iPosition;
		poExpression = ParseExpression(iLevel);
		if(poExpression == NULL)
		{
			m_iPosition = iOriginalPosition;
			return false;
		}
		if(!ParseStatementBlock(iLevel,poOpenBrace,poStatementSequence,poCloseBrace))
		{
			m_iPosition = iOriginalPosition;
			poExpression->DeleteChildren();
			delete poExpression;
			return false;
		}
		
		return true;
	}
	bool Parser::ParseStatementBlock(const unsigned int& iLevel,TreeNode<Token*>*& poOpenBrace,TreeNode<Token*>*& poStatementSequence,TreeNode<Token*>*& poCloseBrace)
	{
		// a statement block is an open curly brace, followed by a statement 
		// sequence and finally, a close curly brace
		unsigned int iOriginalPosition = m_iPosition;
		Token* poOpenBracesToken = GetToken();
		if(poOpenBracesToken->GetKind() != CurlyBracesOpenToken)
		{
			m_iPosition = iOriginalPosition;
			return false;
		}
		IncrementPosition();
		bool bNoStatements = false;
		poStatementSequence = ParseStatementSequence(iLevel);
		if(poStatementSequence == NULL)
		{
			bNoStatements = true;
		}
		Token* poCloseBracesToken = GetToken();
		if(poCloseBracesToken->GetKind() != CurlyBracesCloseToken)
		{
			m_iPosition = iOriginalPosition;
			if(!bNoStatements)
			{
				poStatementSequence->DeleteChildren();
				delete poStatementSequence;
			}
			return false;
		}
		IncrementPosition();		
		poOpenBrace = new TreeNode<Token*>;
		poOpenBrace->SetLevel(iLevel);
		poOpenBrace->SetData(poOpenBracesToken);
		poOpenBrace->SetLabel(poOpenBracesToken->GeneratePrintString());
		poOpenBrace->SetLeaf(true);
		
		poCloseBrace = new TreeNode<Token*>;
		poCloseBrace->SetLevel(iLevel);
		poCloseBrace->SetData(poCloseBracesToken);
		poCloseBrace->SetLabel(poCloseBracesToken->GeneratePrintString());
		poCloseBrace->SetLeaf(true);
		
		return true;
	}
	TreeNode<Token*>* Parser::ParseIdentifierList(const unsigned int& iLevel)
	{
		// IdentList = ident {"," ident}.
		unsigned int iOriginalPosition = m_iPosition;
		Token* poIdentifierToken = GetToken();
		if(poIdentifierToken->GetKind() != IdentifierToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		// create the identifier token
		TreeNode<Token*>* poIdentifierNode = new TreeNode<Token*>;
		poIdentifierNode->SetLevel(iLevel + 1);
		poIdentifierNode->SetData(poIdentifierToken);
		poIdentifierNode->SetLabel(poIdentifierToken->GeneratePrintString());
		poIdentifierNode->SetLeaf(true);
			
		list<TreeNode<Token*>*> lpoIdentifiers;
		lpoIdentifiers.push_back(poIdentifierNode);
		list<TreeNode<Token*>*> lpoCommaNodes;
		Token* poCommaToken = NULL;
		TreeNode<Token*>* poCommaNode = NULL;
		// look for any number of commas followed by identifiers
		while(true)
		{
			poCommaToken = GetToken();
			if(poCommaToken->GetKind() != CommaToken)		break;
			IncrementPosition();
			poIdentifierToken = GetToken();
			if(poIdentifierToken->GetKind() != IdentifierToken)
			{
				// reset position
				m_iPosition = iOriginalPosition;
				// delete all created nodes
				while(!lpoIdentifiers.empty())
				{
					poIdentifierNode = lpoIdentifiers.front();
					if(poIdentifierNode != NULL)
					{
						poIdentifierNode->DeleteChildren();
						delete poIdentifierNode;
					}
					lpoIdentifiers.pop_front();
				}
				while(!lpoCommaNodes.empty())
				{
					poCommaNode = lpoCommaNodes.front();
					if(poCommaNode != NULL)
					{
						poCommaNode->DeleteChildren();
						delete poCommaNode;
					}
					lpoCommaNodes.pop_front();
				}
				return NULL;
			}
			IncrementPosition();
			// found a comma followed by an expression, push them into the list
			poCommaNode = new TreeNode<Token*>;
			poCommaNode->SetLevel(iLevel + 1);
			poCommaNode->SetData(poCommaToken);
			poCommaNode->SetLabel(poCommaToken->GeneratePrintString());
			poCommaNode->SetLeaf(true);
			
			poIdentifierNode = new TreeNode<Token*>;
			poIdentifierNode->SetLevel(iLevel + 1);
			poIdentifierNode->SetData(poIdentifierToken);
			poIdentifierNode->SetLabel(poIdentifierToken->GeneratePrintString());
			poIdentifierNode->SetLeaf(true);
		
			lpoCommaNodes.push_back(poCommaNode);
			lpoIdentifiers.push_back(poIdentifierNode);
		}
		// now we have a list of identifier nodes and a list of comma nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poIdentifierListNode = new TreeNode<Token*>;
		poIdentifierListNode->SetLevel(iLevel);
		poIdentifierListNode->SetLabel("IdentifierList");
		// add the first expression to the parent node
		poIdentifierNode = lpoIdentifiers.front();
		lpoIdentifiers.pop_front();
		poIdentifierListNode->AddChild(poIdentifierNode);
		poIdentifierNode->SetParent(poIdentifierListNode);
		// add the rest of the expressions and the commas to the parent node
		while(!lpoIdentifiers.empty())
		{
			poCommaNode = lpoCommaNodes.front();
			lpoCommaNodes.pop_front();
			poIdentifierListNode->AddChild(poCommaNode);
			poCommaNode->SetParent(poIdentifierListNode);
			
			poIdentifierNode = lpoIdentifiers.front();
			lpoIdentifiers.pop_front();
			poIdentifierListNode->AddChild(poIdentifierNode);
			poIdentifierNode->SetParent(poIdentifierListNode);
		}
		return poIdentifierListNode;
	}
	TreeNode<Token*>* Parser::ParseArrayType(const unsigned int& iLevel)
	{
		// ArrayType = "array" ExpList "of" type.
		unsigned int iOriginalPosition = m_iPosition;
		// parse the array token
		Token* poArrayToken = GetToken();
		if(poArrayToken->GetKind() != ArrayToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		// parse the expression list
		TreeNode<Token*>* poExpressionList = ParseExpressionList(iLevel + 1);
		if(poExpressionList == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		// parse the of token
		Token* poOfToken = GetToken();
		if(poOfToken->GetKind() != OfToken)
		{
			m_iPosition = iOriginalPosition;
			poExpressionList->DeleteChildren();
			delete poExpressionList;
			return NULL;
		}
		IncrementPosition();
		// parse the type
		TreeNode<Token*>* poType = ParseType(iLevel + 1);
		if(poType == NULL)
		{
			m_iPosition = iOriginalPosition;
			poExpressionList->DeleteChildren();
			delete poExpressionList;
			return NULL;
		}
		// now we have everything, build the tree
		TreeNode<Token*>* poArrayNode = new TreeNode<Token*>;
		poArrayNode->SetLevel(iLevel + 1);
		poArrayNode->SetData(poArrayToken);
		poArrayNode->SetLabel(poArrayToken->GeneratePrintString());
		poArrayNode->SetLeaf(true);
		
		TreeNode<Token*>* poOfNode = new TreeNode<Token*>;
		poOfNode->SetLevel(iLevel + 1);
		poOfNode->SetData(poOfToken);
		poOfNode->SetLabel(poOfToken->GeneratePrintString());
		poOfNode->SetLeaf(true);
		
		TreeNode<Token*>* poArrayType = new TreeNode<Token*>;
		poArrayType->SetLevel(iLevel);
		poArrayType->SetLabel("ArrayType");
		poArrayType->AddChild(poArrayNode);
		poArrayType->AddChild(poExpressionList);
		poArrayType->AddChild(poOfNode);
		poArrayType->AddChild(poType);
		
		poArrayNode->SetParent(poArrayType);
		poExpressionList->SetParent(poArrayType);
		poOfNode->SetParent(poArrayType);
		poType->SetParent(poArrayType);
		
		return poArrayType;
	}
	TreeNode<Token*>* Parser::ParseClassType(const unsigned int& iLevel)
	{
		// ClassType = "class" ["(" qualident ")"] "{" [FieldListSequence] "}".
		unsigned int iOriginalPosition = m_iPosition;
		Token* poClassToken = GetToken();
		if(poClassToken->GetKind() != ClassToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		bool bIsInherited = false;
		Token* poOpenBracketToken = GetToken();
		TreeNode<Token*>* poBaseClassNode = NULL;
		Token* poCloseBracketToken = NULL;
		if(poOpenBracketToken->GetKind() == RoundBracketOpenToken)
		{
			IncrementPosition();
			poBaseClassNode = ParseQualifiedIdentifier(iLevel + 1);
			if(poBaseClassNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
			poCloseBracketToken = GetToken();
			if(poCloseBracketToken->GetKind() != RoundBracketCloseToken)
			{
				m_iPosition = iOriginalPosition;
				poBaseClassNode->DeleteChildren();
				delete poBaseClassNode;
				return NULL;
			}
			IncrementPosition();
			bIsInherited = true;
		}
		Token* poOpenBracesToken = GetToken();
		if(poOpenBracesToken->GetKind() != CurlyBracesOpenToken)
		{
			m_iPosition = iOriginalPosition;
			if(bIsInherited)
			{
				poBaseClassNode->DeleteChildren();
				delete poBaseClassNode;
			}
			return NULL;
		}
		IncrementPosition();
		bool bEmptyClass = false;
		TreeNode<Token*>* poFieldListSequence = ParseFieldListSequence(iLevel + 1);
		if(poFieldListSequence == NULL)
		{
			bEmptyClass = true;
		}
		Token* poCloseBracesToken = GetToken();
		if(poCloseBracesToken->GetKind() != CurlyBracesCloseToken)
		{
			m_iPosition = iOriginalPosition;
			if(bIsInherited)
			{
				poBaseClassNode->DeleteChildren();
				delete poBaseClassNode;
			}
			if(!bEmptyClass)
			{
				poFieldListSequence->DeleteChildren();
				delete poFieldListSequence;
			}
			return NULL;
		}
		IncrementPosition();
		TreeNode<Token*>* poOpenBrace = new TreeNode<Token*>;
		poOpenBrace->SetLevel(iLevel + 1);
		poOpenBrace->SetData(poOpenBracesToken);
		poOpenBrace->SetLabel(poOpenBracesToken->GeneratePrintString());
		poOpenBrace->SetLeaf(true);
		
		TreeNode<Token*>* poCloseBrace = new TreeNode<Token*>;
		poCloseBrace->SetLevel(iLevel + 1);
		poCloseBrace->SetData(poCloseBracesToken);
		poCloseBrace->SetLabel(poCloseBracesToken->GeneratePrintString());
		poCloseBrace->SetLeaf(true);

		TreeNode<Token*>* poClassType = new TreeNode<Token*>;
		poClassType->SetLevel(iLevel);
		poClassType->SetLabel("ClassType");
		if(bIsInherited)
		{
			TreeNode<Token*>* poOpenBracket = new TreeNode<Token*>;
			poOpenBracket->SetLevel(iLevel + 1);
			poOpenBracket->SetData(poOpenBracketToken);
			poOpenBracket->SetLabel(poOpenBracketToken->GeneratePrintString());
			poOpenBracket->SetLeaf(true);
			TreeNode<Token*>* poCloseBracket = new TreeNode<Token*>;
			poCloseBracket->SetLevel(iLevel + 1);
			poCloseBracket->SetData(poCloseBracketToken);
			poCloseBracket->SetLabel(poCloseBracketToken->GeneratePrintString());
			poCloseBracket->SetLeaf(true);
			poClassType->AddChild(poOpenBracket);
			poClassType->AddChild(poBaseClassNode);
			poClassType->AddChild(poCloseBracket);
			poOpenBracket->SetParent(poClassType);
			poBaseClassNode->SetParent(poClassType);
			poCloseBracket->SetParent(poClassType);
		}
		poClassType->AddChild(poOpenBrace);
		poOpenBrace->SetParent(poClassType);
		if(!bEmptyClass)
		{
			poClassType->AddChild(poFieldListSequence);
			poFieldListSequence->SetParent(poClassType);
		}
		poClassType->AddChild(poCloseBrace);
		poCloseBrace->SetParent(poClassType);
		
		return poClassType;
	}
	TreeNode<Token*>* Parser::ParseProcedureType(const unsigned int& iLevel)
	{
		// ProcedureType = "procedure" [FormalParameters].
		// ClassType = "class" ["(" qualident ")"] "{" [FieldListSequence] "}".
		unsigned int iOriginalPosition = m_iPosition;
		Token* poProcedureToken = GetToken();
		if(poProcedureToken->GetKind() != ProcedureToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		bool bNoFormalParameters = false;
		TreeNode<Token*>* poFormalParameters = ParseFormalParameters(iLevel + 1);
		if(poFormalParameters == NULL)
		{
			bNoFormalParameters = true;
		}
		TreeNode<Token*>* poProcedureNode = new TreeNode<Token*>;
		poProcedureNode->SetLevel(iLevel + 1);
		poProcedureNode->SetData(poProcedureToken);
		poProcedureNode->SetLabel(poProcedureToken->GeneratePrintString());
		poProcedureNode->SetLeaf(true);

		TreeNode<Token*>* poProcedureType = new TreeNode<Token*>;
		poProcedureType->SetLevel(iLevel);
		poProcedureType->SetLabel("ProcedureType");
		poProcedureType->AddChild(poProcedureNode);
		poProcedureNode->SetParent(poProcedureType);
		if(!bNoFormalParameters)
		{
			poProcedureType->AddChild(poFormalParameters);
			poFormalParameters->SetParent(poProcedureType);
		}
		
		return poProcedureType;
	}
	TreeNode<Token*>* Parser::ParseStructuredType(const unsigned int& iLevel)
	{
		// StrucType = ArrayType | ClassType | ProcedureType.
		unsigned int iOriginalPosition = m_iPosition;
		Token* poToken = GetToken();
		TokenKind eKind = poToken->GetKind();
		TreeNode<Token*>* poSubNode = NULL;
		if(eKind == ArrayToken)
		{
			poSubNode = ParseArrayType(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		else if(eKind == ClassToken)
		{
			poSubNode = ParseClassType(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		else if(eKind == ProcedureToken)
		{
			poSubNode = ParseProcedureType(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		else
		{
			// wrong structured type token, fail and return
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		TreeNode<Token*>* poStructuredTypeNode = new TreeNode<Token*>;
		poStructuredTypeNode->SetLevel(iLevel);
		poStructuredTypeNode->SetLabel("StructuredType");
		poStructuredTypeNode->AddChild(poSubNode);
		poSubNode->SetParent(poStructuredTypeNode);
		return poStructuredTypeNode;
	}
	TreeNode<Token*>* Parser::ParseType(const unsigned int& iLevel)
	{
		// type = qualident | StrucType.
		unsigned int iOriginalPosition = m_iPosition;
		Token* poToken = GetToken();
		TokenKind eKind = poToken->GetKind();
		TreeNode<Token*>* poSubNode = NULL;
		if((eKind == ArrayToken) || (eKind == ClassToken) || (eKind == ProcedureToken))
		{
			// this must be a structured type, parse it
			poSubNode = ParseStructuredType(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		else
		{
			// or it can be a qualified identifier type, parse it
			poSubNode = ParseQualifiedIdentifier(iLevel + 1);
			if(poSubNode == NULL)
			{
				m_iPosition = iOriginalPosition;
				return NULL;
			}
		}
		TreeNode<Token*>* poTypeNode = new TreeNode<Token*>;
		poTypeNode->SetLevel(iLevel);
		poTypeNode->SetLabel("Type");
		poTypeNode->AddChild(poSubNode);
		poSubNode->SetParent(poTypeNode);
		return poTypeNode;
	}
	TreeNode<Token*>* Parser::ParseFieldListSequence(const unsigned int& iLevel)
	{
		// FieldListSequence = VariableDeclaration {VariableDeclaration}.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poVariableDeclaration = ParseVarDeclaration(iLevel + 1);		
		if(poVariableDeclaration == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		list<TreeNode<Token*>*> lpoFieldLists;
		lpoFieldLists.push_back(poVariableDeclaration);
		while(true)
		{
			poVariableDeclaration = ParseVarDeclaration(iLevel + 1);
			if(poVariableDeclaration == NULL)
			{
				break;
			}
			// found a semicolon followed by a statement, push them into the list
			lpoFieldLists.push_back(poVariableDeclaration);
		}
		// now we have a list of field list nodes and a list of semi-colon nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poFieldListSequenceNode = new TreeNode<Token*>;
		poFieldListSequenceNode->SetLevel(iLevel);
		poFieldListSequenceNode->SetLabel("FieldListSequence");
		// add the first field list to the parent node
		poVariableDeclaration = lpoFieldLists.front();
		lpoFieldLists.pop_front();
		poFieldListSequenceNode->AddChild(poVariableDeclaration);
		poVariableDeclaration->SetParent(poFieldListSequenceNode);
		// add the rest of the expressions and the commas to the parent node
		while(!lpoFieldLists.empty())
		{
			poVariableDeclaration = lpoFieldLists.front();
			lpoFieldLists.pop_front();
			poFieldListSequenceNode->AddChild(poVariableDeclaration);
			poVariableDeclaration->SetParent(poFieldListSequenceNode);
		}
		return poFieldListSequenceNode;
	}
	TreeNode<Token*>* Parser::ParseVarDeclaration(const unsigned int& iLevel)
	{
		// VariableDeclaration = IdentList ":" type ";".
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poIdentifierList = ParseIdentifierList(iLevel + 1);
		if(poIdentifierList == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		Token* poColonToken = GetToken();
		if(poColonToken->GetKind() != ColonToken)
		{
			m_iPosition = iOriginalPosition;
			poIdentifierList->DeleteChildren();
			delete poIdentifierList;
			return NULL;
		}
		IncrementPosition();
		
		TreeNode<Token*>* poType = ParseType(iLevel + 1);
		if(poType == NULL)
		{
			m_iPosition = iOriginalPosition;
			poIdentifierList->DeleteChildren();
			delete poIdentifierList;
			return NULL;
		}
		
		Token* poSemiColonToken = GetToken();
		if(poSemiColonToken->GetKind() != SemiColonToken)
		{
			m_iPosition = iOriginalPosition;
			poIdentifierList->DeleteChildren();
			delete poIdentifierList;
			poType->DeleteChildren();
			delete poType;
			return NULL;
		}
		IncrementPosition();
		
		// now we have everything, build the tree
		TreeNode<Token*>* poColonNode = new TreeNode<Token*>;
		poColonNode->SetLevel(iLevel + 1);
		poColonNode->SetData(poColonToken);
		poColonNode->SetLabel(poColonToken->GeneratePrintString());
		poColonNode->SetLeaf(true);
		
		TreeNode<Token*>* poSemiColonNode = new TreeNode<Token*>;
		poSemiColonNode->SetLevel(iLevel + 1);
		poSemiColonNode->SetData(poSemiColonToken);
		poSemiColonNode->SetLabel(poSemiColonToken->GeneratePrintString());
		poSemiColonNode->SetLeaf(true);
		
		TreeNode<Token*>* poVarDeclaration = new TreeNode<Token*>;
		poVarDeclaration->SetLevel(iLevel);
		poVarDeclaration->SetLabel("VarDeclaration");
		
		poVarDeclaration->AddChild(poIdentifierList);
		poVarDeclaration->AddChild(poColonNode);
		poVarDeclaration->AddChild(poType);
		poVarDeclaration->AddChild(poSemiColonNode);
		
		poIdentifierList->SetParent(poVarDeclaration);
		poColonNode->SetParent(poVarDeclaration);
		poType->SetParent(poVarDeclaration);
		poSemiColonNode->SetParent(poVarDeclaration);
		
		return poVarDeclaration;
	}
	TreeNode<Token*>* Parser::ParseFormalParameter(const unsigned int& iLevel)
	{
		// FormalParameter = Type ["#"] ident
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poType = ParseType(iLevel + 1);
		if(poType == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		bool bIsVariable = false;
		Token* poHashToken = GetToken();
		if(poHashToken->GetKind() == HashToken)
		{
			IncrementPosition();
			bIsVariable = true;
		}
		Token* poIdentifierToken = GetToken();
		if(poIdentifierToken->GetKind() != IdentifierToken)
		{
			m_iPosition = iOriginalPosition;
			poType->DeleteChildren();
			delete poType;
			return NULL;
		}
		IncrementPosition();
		
		TreeNode<Token*>* poIdentifierNode = new TreeNode<Token*>;
		poIdentifierNode->SetLevel(iLevel + 1);
		poIdentifierNode->SetData(poIdentifierToken);
		poIdentifierNode->SetLabel(poIdentifierToken->GeneratePrintString());
		poIdentifierNode->SetLeaf(true);
		
		TreeNode<Token*>* poFormalParameterNode = new TreeNode<Token*>;
		poFormalParameterNode->SetLevel(iLevel);
		poFormalParameterNode->SetLabel("FormalParameter");
		
		poFormalParameterNode->AddChild(poType);
		poType->SetParent(poFormalParameterNode);
		
		if(bIsVariable)
		{
			TreeNode<Token*>* poHashNode = new TreeNode<Token*>;
			poHashNode->SetLevel(iLevel + 1);
			poHashNode->SetData(poHashToken);
			poHashNode->SetLabel(poHashToken->GeneratePrintString());
			poHashNode->SetLeaf(true);
			poFormalParameterNode->AddChild(poHashNode);
			poHashNode->SetParent(poFormalParameterNode);
		}
		
		poFormalParameterNode->AddChild(poIdentifierNode);
		poIdentifierNode->SetParent(poFormalParameterNode);
		
		return poFormalParameterNode;
	}
	TreeNode<Token*>* Parser::ParseFormalParameterList(const unsigned int& iLevel)
	{
		// FormalParameterList = FormalParameter {"," FormalParameter}
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poFormalParameter = ParseFormalParameter(iLevel + 1);
		if(poFormalParameter == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		list<TreeNode<Token*>*> lpoFormalParameters;
		lpoFormalParameters.push_back(poFormalParameter);
		list<TreeNode<Token*>*> lpoCommaNodes;
		Token* poCommaToken = NULL;
		TreeNode<Token*>* poCommaNode = NULL;
		while(true)
		{
			poCommaToken = GetToken();
			if(poCommaToken->GetKind() != CommaToken)		break;
			IncrementPosition();
			poFormalParameter = ParseFormalParameter(iLevel + 1);
			if(poFormalParameter == NULL)
			{
				// reset position
				m_iPosition = iOriginalPosition;
				// delete all created nodes
				while(!lpoFormalParameters.empty())
				{
					poFormalParameter = lpoFormalParameters.front();
					if(poFormalParameter != NULL)
					{
						poFormalParameter->DeleteChildren();
						delete poFormalParameter;
					}
					lpoFormalParameters.pop_front();
				}
				while(!lpoCommaNodes.empty())
				{
					poCommaNode = lpoCommaNodes.front();
					if(poCommaNode != NULL)
					{
						poCommaNode->DeleteChildren();
						delete poCommaNode;
					}
					lpoCommaNodes.pop_front();
				}
				return NULL;
			}
			// found a comma followed by a formal parameter, push them into the list
			poCommaNode = new TreeNode<Token*>;
			poCommaNode->SetLevel(iLevel + 1);
			poCommaNode->SetData(poCommaToken);
			poCommaNode->SetLabel(poCommaToken->GeneratePrintString());
			poCommaNode->SetLeaf(true);
			lpoCommaNodes.push_back(poCommaNode);
			lpoFormalParameters.push_back(poFormalParameter);
		}
		// now we have a list of expression nodes and a list of comma nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poFormalParameterList = new TreeNode<Token*>;
		poFormalParameterList->SetLevel(iLevel);
		poFormalParameterList->SetLabel("FormalParameterList");
		// add the first formal parameter to the parent node
		poFormalParameter = lpoFormalParameters.front();
		lpoFormalParameters.pop_front();
		poFormalParameterList->AddChild(poFormalParameter);
		poFormalParameter->SetParent(poFormalParameterList);
		// add the rest of the expressions and the commas to the parent node
		while(!lpoFormalParameters.empty())
		{
			poCommaNode = lpoCommaNodes.front();
			lpoCommaNodes.pop_front();
			poFormalParameterList->AddChild(poCommaNode);
			poCommaNode->SetParent(poFormalParameterList);
			
			poFormalParameter = lpoFormalParameters.front();
			lpoFormalParameters.pop_front();
			poFormalParameterList->AddChild(poFormalParameter);
			poFormalParameter->SetParent(poFormalParameterList);
		}
		return poFormalParameterList;
	}
	TreeNode<Token*>* Parser::ParseFormalParameters(const unsigned int& iLevel)
	{
		// FormalParameters = "(" [FormalParameterList] ")" [":" qualident].
		unsigned int iOriginalPosition = m_iPosition;
		Token* poOpenBracketToken = GetToken();
		if(poOpenBracketToken->GetKind() != RoundBracketOpenToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		TreeNode<Token*>* poFormalParametersList = ParseFormalParameterList(iLevel + 1);
		bool bNoParameters = false;
		if(poFormalParametersList == NULL)
		{
			bNoParameters = true;
		}
		Token* poCloseBracketToken = GetToken();
		if(poCloseBracketToken->GetKind() != RoundBracketCloseToken)
		{
			m_iPosition = iOriginalPosition;
			if(!bNoParameters)
			{
				poFormalParametersList->DeleteChildren();
				delete poFormalParametersList;
			}
			return NULL;
		}
		IncrementPosition();
		bool bReturnType = false;
		Token* poColonToken = GetToken();
		TreeNode<Token*>* poReturnType = NULL;
		if(poColonToken->GetKind() == ColonToken)
		{
			IncrementPosition();
			poReturnType = ParseQualifiedIdentifier(iLevel + 1);
			if(poReturnType == NULL)
			{
				m_iPosition = iOriginalPosition;
				if(!bNoParameters)
				{
					poFormalParametersList->DeleteChildren();
					delete poFormalParametersList;
				}
				return NULL;
			}
			bReturnType = true;
		}
		// build the tree
		TreeNode<Token*>* poFormalParameters = new TreeNode<Token*>;
		poFormalParameters->SetLevel(iLevel);
		poFormalParameters->SetLabel("FormalParameters");
		
		TreeNode<Token*>* poBracketOpenNode = new TreeNode<Token*>;
		poBracketOpenNode->SetLevel(iLevel + 1);
		poBracketOpenNode->SetData(poOpenBracketToken);
		poBracketOpenNode->SetLabel(poOpenBracketToken->GeneratePrintString());
		poBracketOpenNode->SetLeaf(true);
		
		TreeNode<Token*>* poBracketCloseNode = new TreeNode<Token*>;
		poBracketCloseNode->SetLevel(iLevel + 1);
		poBracketCloseNode->SetData(poCloseBracketToken);
		poBracketCloseNode->SetLabel(poCloseBracketToken->GeneratePrintString());
		poBracketCloseNode->SetLeaf(true);
		
		poFormalParameters->AddChild(poBracketOpenNode);
		poBracketOpenNode->SetParent(poFormalParameters);
		if(!bNoParameters)
		{
			poFormalParameters->AddChild(poFormalParametersList);
			poFormalParametersList->SetParent(poFormalParameters);
		}
		poFormalParameters->AddChild(poBracketCloseNode);
		poBracketCloseNode->SetParent(poFormalParameters);
		
		if(bReturnType)
		{
			TreeNode<Token*>* poColonNode = new TreeNode<Token*>;
			poColonNode->SetLevel(iLevel + 1);
			poColonNode->SetData(poColonToken);
			poColonNode->SetLabel(poColonToken->GeneratePrintString());
			poColonNode->SetLeaf(true);
			
			poFormalParameters->AddChild(poColonNode);
			poColonNode->SetParent(poFormalParameters);
			
			poFormalParameters->AddChild(poReturnType);
			poReturnType->SetParent(poFormalParameters);
		}
		
		return poFormalParameters;
	}
	TreeNode<Token*>* Parser::ParseConstDeclaration(const unsigned int& iLevel)
	{
		// ConstDeclaration = "const" ident "=" expression ";". 
		unsigned int iOriginalPosition = m_iPosition;
		Token* poConstToken = GetToken();
		if(poConstToken->GetKind() != ConstToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		Token* poIdentifierToken = GetToken();
		if(poIdentifierToken->GetKind() != IdentifierToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		Token* poEqualToken = GetToken();
		if(poEqualToken->GetKind() != AssignToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		TreeNode<Token*>* poExpression = ParseExpression(iLevel + 1);
		
		if(poExpression == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		
		Token* poSemiColonToken = GetToken();
		if(poSemiColonToken->GetKind() != SemiColonToken)
		{
			m_iPosition = iOriginalPosition;
			poExpression->DeleteChildren();
			delete poExpression;
			return NULL;
		}
		IncrementPosition();
		
		// build the tree
		TreeNode<Token*>* poConstNode = new TreeNode<Token*>;
		poConstNode->SetLevel(iLevel + 1);
		poConstNode->SetData(poConstToken);
		poConstNode->SetLabel(poConstToken->GeneratePrintString());
		poConstNode->SetLeaf(true);
		
		TreeNode<Token*>* poIdentifierNode = new TreeNode<Token*>;
		poIdentifierNode->SetLevel(iLevel + 1);
		poIdentifierNode->SetData(poIdentifierToken);
		poIdentifierNode->SetLabel(poIdentifierToken->GeneratePrintString());
		poIdentifierNode->SetLeaf(true);
		
		TreeNode<Token*>* poAssignNode = new TreeNode<Token*>;
		poAssignNode->SetLevel(iLevel + 1);
		poAssignNode->SetData(poEqualToken);
		poAssignNode->SetLabel(poEqualToken->GeneratePrintString());
		poAssignNode->SetLeaf(true);
		
		TreeNode<Token*>* poSemiColonNode = new TreeNode<Token*>;
		poSemiColonNode->SetLevel(iLevel + 1);
		poSemiColonNode->SetData(poSemiColonToken);
		poSemiColonNode->SetLabel(poSemiColonToken->GeneratePrintString());
		poSemiColonNode->SetLeaf(true);
		
		TreeNode<Token*>* poConstDeclaration = new TreeNode<Token*>;
		poConstDeclaration->SetLevel(iLevel);
		poConstDeclaration->SetLabel("ConstDeclaration");
		poConstDeclaration->AddChild(poConstNode);
		poConstDeclaration->AddChild(poIdentifierNode);
		poConstDeclaration->AddChild(poAssignNode);
		poConstDeclaration->AddChild(poExpression);
		poConstDeclaration->AddChild(poSemiColonNode);
		
		poConstNode->SetParent(poConstDeclaration);
		poIdentifierNode->SetParent(poConstDeclaration);
		poAssignNode->SetParent(poConstDeclaration);
		poExpression->SetParent(poConstDeclaration);
		poSemiColonNode->SetParent(poConstDeclaration);
		
		return poConstDeclaration;
	}
	TreeNode<Token*>* Parser::ParseTypeDeclaration(const unsigned int& iLevel)
	{
		// TypeDeclaration = "type" ident "=" StrucType ";".
		unsigned int iOriginalPosition = m_iPosition;
		Token* poTypeToken = GetToken();
		if(poTypeToken->GetKind() != TypeToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		Token* poIdentifierToken = GetToken();
		if(poIdentifierToken->GetKind() != IdentifierToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		Token* poEqualToken = GetToken();
		if(poEqualToken->GetKind() != AssignToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		TreeNode<Token*>* poStructuredType = ParseStructuredType(iLevel + 1);
		
		if(poStructuredType == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		
		Token* poSemiColonToken = GetToken();
		if(poSemiColonToken->GetKind() != SemiColonToken)
		{
			m_iPosition = iOriginalPosition;
			poStructuredType->DeleteChildren();
			delete poStructuredType;
			return NULL;
		}
		IncrementPosition();
		
		// build the tree
		TreeNode<Token*>* poTypeNode = new TreeNode<Token*>;
		poTypeNode->SetLevel(iLevel + 1);
		poTypeNode->SetData(poTypeToken);
		poTypeNode->SetLabel(poTypeToken->GeneratePrintString());
		poTypeNode->SetLeaf(true);
		
		TreeNode<Token*>* poIdentifierNode = new TreeNode<Token*>;
		poIdentifierNode->SetLevel(iLevel + 1);
		poIdentifierNode->SetData(poIdentifierToken);
		poIdentifierNode->SetLabel(poIdentifierToken->GeneratePrintString());
		poIdentifierNode->SetLeaf(true);
		
		TreeNode<Token*>* poAssignNode = new TreeNode<Token*>;
		poAssignNode->SetLevel(iLevel + 1);
		poAssignNode->SetData(poEqualToken);
		poAssignNode->SetLabel(poEqualToken->GeneratePrintString());
		poAssignNode->SetLeaf(true);
		
		TreeNode<Token*>* poSemiColonNode = new TreeNode<Token*>;
		poSemiColonNode->SetLevel(iLevel + 1);
		poSemiColonNode->SetData(poSemiColonToken);
		poSemiColonNode->SetLabel(poSemiColonToken->GeneratePrintString());
		poSemiColonNode->SetLeaf(true);
			
		TreeNode<Token*>* poTypeDeclaration = new TreeNode<Token*>;
		poTypeDeclaration->SetLevel(iLevel);
		poTypeDeclaration->SetLabel("TypeDeclaration");
		poTypeDeclaration->AddChild(poTypeNode);
		poTypeDeclaration->AddChild(poIdentifierNode);
		poTypeDeclaration->AddChild(poAssignNode);
		poTypeDeclaration->AddChild(poStructuredType);
		poTypeDeclaration->AddChild(poSemiColonNode);
		
		poTypeNode->SetParent(poTypeDeclaration);
		poIdentifierNode->SetParent(poTypeDeclaration);
		poAssignNode->SetParent(poTypeDeclaration);
		poStructuredType->SetParent(poTypeDeclaration);
		poSemiColonNode->SetParent(poTypeDeclaration);
		
		return poTypeDeclaration;
	}
	TreeNode<Token*>* Parser::ParseProcedureDeclaration(const unsigned int& iLevel)
	{
		// ProcedureDeclaration = "procedure" ident [FormalParameters] ProcedureBody ";".
		unsigned int iOriginalPosition = m_iPosition;
		Token* poProcedureToken = GetToken();
		if(poProcedureToken->GetKind() != ProcedureToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		Token* poIdentifierToken = GetToken();
		if(poIdentifierToken->GetKind() != IdentifierToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		bool bNoParameters = false;
		TreeNode<Token*>* poParameters = ParseFormalParameters(iLevel + 1);
		if(poParameters == NULL)
		{
			bNoParameters = true;
		}
		
		TreeNode<Token*>* poBody = ParseProcedureBody(iLevel + 1);
		if(poBody == NULL)
		{
			m_iPosition = iOriginalPosition;
			if(!bNoParameters)
			{
				poParameters->DeleteChildren();
				delete poParameters;
			}
			return NULL;
		}
		
		Token* poSemiColonToken = GetToken();
		if(poSemiColonToken->GetKind() != SemiColonToken)
		{
			m_iPosition = iOriginalPosition;
			if(!bNoParameters)
			{
				poParameters->DeleteChildren();
				delete poParameters;
			}
			poBody->DeleteChildren();
			delete poBody;
			return NULL;
		}
		IncrementPosition();
		
		// build the tree
		TreeNode<Token*>* poProcedureNode = new TreeNode<Token*>;
		poProcedureNode->SetLevel(iLevel + 1);
		poProcedureNode->SetData(poProcedureToken);
		poProcedureNode->SetLabel(poProcedureToken->GeneratePrintString());
		poProcedureNode->SetLeaf(true);
		
		TreeNode<Token*>* poIdentifierNode = new TreeNode<Token*>;
		poIdentifierNode->SetLevel(iLevel + 1);
		poIdentifierNode->SetData(poIdentifierToken);
		poIdentifierNode->SetLabel(poIdentifierToken->GeneratePrintString());
		poIdentifierNode->SetLeaf(true);
		
		TreeNode<Token*>* poSemiColonNode = new TreeNode<Token*>;
		poSemiColonNode->SetLevel(iLevel + 1);
		poSemiColonNode->SetData(poSemiColonToken);
		poSemiColonNode->SetLabel(poSemiColonToken->GeneratePrintString());
		poSemiColonNode->SetLeaf(true);
		
		TreeNode<Token*>* poProcedureDeclaration = new TreeNode<Token*>;
		poProcedureDeclaration->SetLevel(iLevel);
		poProcedureDeclaration->SetLabel("ProcedureDeclaration");
		poProcedureDeclaration->AddChild(poProcedureNode);
		poProcedureNode->SetParent(poProcedureDeclaration);
		poProcedureDeclaration->AddChild(poIdentifierNode);
		poIdentifierNode->SetParent(poProcedureDeclaration);
		if(!bNoParameters)
		{
			poProcedureDeclaration->AddChild(poParameters);
			poParameters->SetParent(poProcedureDeclaration);
		}
		poProcedureDeclaration->AddChild(poBody);
		poBody->SetParent(poProcedureDeclaration);
		
		poProcedureDeclaration->AddChild(poSemiColonNode);
		poSemiColonNode->SetParent(poProcedureDeclaration);
		
		return poProcedureDeclaration;
	}
	TreeNode<Token*>* Parser::ParseDeclaration(const unsigned int& iLevel)
	{
		// Declaration = ConstDeclaration  | TypeDeclaration | "var" VariableDeclaration | ProcedureDeclaration.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poSubDeclaration = NULL;
		TreeNode<Token*>* poDeclaration = new TreeNode<Token*>;
		poDeclaration->SetLevel(iLevel);
		poDeclaration->SetLabel("Declaration");
		
		Token* poFirstToken = GetToken();
		TokenKind eKind = poFirstToken->GetKind();
		bool bDeclarationFound = true;
		if(eKind == ConstToken)
		{
			poSubDeclaration = ParseConstDeclaration(iLevel + 1);
			if(poSubDeclaration == NULL)
			{
				m_iPosition = iOriginalPosition;
				bDeclarationFound = false;
			}
		}
		else if(eKind == TypeToken)
		{
			poSubDeclaration = ParseTypeDeclaration(iLevel + 1);
			if(poSubDeclaration == NULL)
			{
				m_iPosition = iOriginalPosition;
				bDeclarationFound = false;
			}
		}
		else if(eKind == VarToken)
		{
			IncrementPosition();
			poSubDeclaration = ParseVarDeclaration(iLevel + 1);
			if(poSubDeclaration == NULL)
			{
				m_iPosition = iOriginalPosition;
				bDeclarationFound = false;
			}
			TreeNode<Token*>* poVarNode = new TreeNode<Token*>;
			poVarNode->SetLevel(iLevel + 1);
			poVarNode->SetData(poFirstToken);
			poVarNode->SetLabel(poFirstToken->GeneratePrintString());
			poVarNode->SetLeaf(true);
			poDeclaration->AddChild(poVarNode);
			poVarNode->SetParent(poDeclaration);
		}
		else if(eKind == ProcedureToken)
		{
			poSubDeclaration = ParseProcedureDeclaration(iLevel + 1);
			if(poSubDeclaration == NULL)
			{
				m_iPosition = iOriginalPosition;
				bDeclarationFound = false;
			}
		}
		else
		{
			m_iPosition = iOriginalPosition;
			bDeclarationFound = false;
		}
		if(!bDeclarationFound)
		{
			poDeclaration->DeleteChildren();
			delete poDeclaration;
			return NULL;
		}
		poDeclaration->AddChild(poSubDeclaration);
		poSubDeclaration->SetParent(poDeclaration);
		
		return poDeclaration;
	}
	TreeNode<Token*>* Parser::ParseDeclarationSequence(const unsigned int& iLevel)
	{
		// DeclarationSequence = Declaration { Declaration}.
		unsigned int iOriginalPosition = m_iPosition;
		TreeNode<Token*>* poDeclaration = ParseDeclaration(iLevel + 1);		
		if(poDeclaration == NULL)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		list<TreeNode<Token*>*> lpoDeclarations;
		lpoDeclarations.push_back(poDeclaration);
		while(true)
		{
			poDeclaration = ParseDeclaration(iLevel + 1);
			if(poDeclaration == NULL)
			{
				break;
			}
			lpoDeclarations.push_back(poDeclaration);
		}
		// now we have a list of expression nodes and a list of comma nodes
		// position has been properly incremented
		// create the tree
		TreeNode<Token*>* poDeclarationSequenceNode = new TreeNode<Token*>;
		poDeclarationSequenceNode->SetLevel(iLevel);
		poDeclarationSequenceNode->SetLabel("DeclarationSequenceNode");
		// add the first statement to the parent node
		poDeclaration = lpoDeclarations.front();
		lpoDeclarations.pop_front();
		poDeclarationSequenceNode->AddChild(poDeclaration);
		poDeclaration->SetParent(poDeclarationSequenceNode);
		// add the rest of the expressions and the commas to the parent node
		while(!lpoDeclarations.empty())
		{
			poDeclaration = lpoDeclarations.front();
			lpoDeclarations.pop_front();
			poDeclarationSequenceNode->AddChild(poDeclaration);
			poDeclaration->SetParent(poDeclarationSequenceNode);
		}
		return poDeclarationSequenceNode;
	}
	TreeNode<Token*>* Parser::ParseProcedureBody(const unsigned int& iLevel)
	{
		// ProcedureBody = "{" [DeclarationSequence]  [StatementSequence] ["return" expression] "}".
		unsigned int iOriginalPosition = m_iPosition;
		Token* poOpenBracesToken = GetToken();
		if(poOpenBracesToken->GetKind() != CurlyBracesOpenToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		TreeNode<Token*>* poDeclarationSequence = ParseDeclarationSequence(iLevel + 1);
		bool bNoDeclarations = false;
		if(poDeclarationSequence == NULL)
		{
			bNoDeclarations = true;
		}
		
		TreeNode<Token*>* poStatementSequence = ParseStatementSequence(iLevel + 1);
		bool bNoStatements = false;
		if(poStatementSequence == NULL)
		{
			bNoStatements = true;
		}
		
		Token* poReturnToken = GetToken();
		bool bNoReturn = true;
		TreeNode<Token*>* poReturnExpression = NULL;
		if(poReturnToken->GetKind() == ReturnToken)
		{
			IncrementPosition();
			poReturnExpression = ParseExpression(iLevel + 1);
			if(poReturnExpression == NULL)
			{
				m_iPosition = iOriginalPosition;
				if(!bNoDeclarations)
				{
					poDeclarationSequence->DeleteChildren();
					delete poDeclarationSequence;
				}
				if(!bNoStatements)
				{
					poStatementSequence->DeleteChildren();
					delete poStatementSequence;
				}
				return NULL;
			}
			bNoReturn = false;
		}
		
		Token* poCloseBracesToken = GetToken();
		if(poCloseBracesToken->GetKind() != CurlyBracesCloseToken)
		{
			m_iPosition = iOriginalPosition;
			if(!bNoDeclarations)
			{
				poDeclarationSequence->DeleteChildren();
				delete poDeclarationSequence;
			}
			if(!bNoStatements)
			{
				poStatementSequence->DeleteChildren();
				delete poStatementSequence;
			}
			if(!bNoReturn)
			{
				poReturnExpression->DeleteChildren();
				delete poReturnExpression;
			}
			return NULL;
		}
		IncrementPosition();
		
		// build the tree
		TreeNode<Token*>* poProcedureBody = new TreeNode<Token*>;
		poProcedureBody->SetLevel(iLevel);
		poProcedureBody->SetLabel("ProcedureBody");
		
		TreeNode<Token*>* poBracesOpenNode = new TreeNode<Token*>;
		poBracesOpenNode->SetLevel(iLevel + 1);
		poBracesOpenNode->SetData(poOpenBracesToken);
		poBracesOpenNode->SetLabel(poOpenBracesToken->GeneratePrintString());
		poBracesOpenNode->SetLeaf(true);
		
		TreeNode<Token*>* poBracesCloseNode = new TreeNode<Token*>;
		poBracesCloseNode->SetLevel(iLevel + 1);
		poBracesCloseNode->SetData(poCloseBracesToken);
		poBracesCloseNode->SetLabel(poCloseBracesToken->GeneratePrintString());
		poBracesCloseNode->SetLeaf(true);
		
		poProcedureBody->AddChild(poBracesOpenNode);
		poBracesOpenNode->SetParent(poProcedureBody);
		if(!bNoDeclarations)
		{
			poProcedureBody->AddChild(poDeclarationSequence);
			poDeclarationSequence->SetParent(poProcedureBody);
		}
		if(!bNoStatements)
		{
			poProcedureBody->AddChild(poStatementSequence);
			poStatementSequence->SetParent(poProcedureBody);
		}
		if(!bNoReturn)
		{
			TreeNode<Token*>* poReturnNode = new TreeNode<Token*>;
			poReturnNode->SetLevel(iLevel + 1);
			poReturnNode->SetData(poReturnToken);
			poReturnNode->SetLabel(poReturnToken->GeneratePrintString());
			poReturnNode->SetLeaf(true);
			poProcedureBody->AddChild(poReturnNode);
			poReturnNode->SetParent(poProcedureBody);
			poProcedureBody->AddChild(poReturnExpression);
			poReturnExpression->SetParent(poProcedureBody);
		}
		poProcedureBody->AddChild(poBracesCloseNode);
		poBracesCloseNode->SetParent(poProcedureBody);
		
		return poProcedureBody;
	}
	TreeNode<Token*>* Parser::ParseProgram(const unsigned int& iLevel)
	{
		// Program = "program" ident "{" [DeclarationSequence] [StatementSequence] "}" .
		unsigned int iOriginalPosition = m_iPosition;
		Token* poProgramToken = GetToken();
		if(poProgramToken->GetKind() != ProgramToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		Token* poIdentifierToken = GetToken();
		if(poIdentifierToken->GetKind() != IdentifierToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		
		Token* poOpenBracesToken = GetToken();
		if(poOpenBracesToken->GetKind() != CurlyBracesOpenToken)
		{
			m_iPosition = iOriginalPosition;
			return NULL;
		}
		IncrementPosition();
		TreeNode<Token*>* poDeclarationSequence = ParseDeclarationSequence(iLevel + 1);
		bool bNoDeclarations = false;
		if(poDeclarationSequence == NULL)
		{
			bNoDeclarations = true;
		}
		
		TreeNode<Token*>* poStatementSequence = ParseStatementSequence(iLevel + 1);
		bool bNoStatements = false;
		if(poStatementSequence == NULL)
		{
			bNoStatements = true;
		}
		
		Token* poCloseBracesToken = GetToken();
		if(poCloseBracesToken->GetKind() != CurlyBracesCloseToken)
		{
			m_iPosition = iOriginalPosition;
			if(!bNoDeclarations)
			{
				poDeclarationSequence->DeleteChildren();
				delete poDeclarationSequence;
			}
			if(!bNoStatements)
			{
				poStatementSequence->DeleteChildren();
				delete poStatementSequence;
			}
			return NULL;
		}
		IncrementPosition();
		
		// build the tree
		TreeNode<Token*>* poProgram = new TreeNode<Token*>;
		poProgram->SetLevel(iLevel);
		poProgram->SetLabel("Program");
		
		TreeNode<Token*>* poProgramNode = new TreeNode<Token*>;
		poProgramNode->SetLevel(iLevel + 1);
		poProgramNode->SetData(poProgramToken);
		poProgramNode->SetLabel(poProgramToken->GeneratePrintString());
		poProgramNode->SetLeaf(true);
		
		TreeNode<Token*>* poIdentifierNode = new TreeNode<Token*>;
		poIdentifierNode->SetLevel(iLevel + 1);
		poIdentifierNode->SetData(poIdentifierToken);
		poIdentifierNode->SetLabel(poIdentifierToken->GeneratePrintString());
		poIdentifierNode->SetLeaf(true);
		
		TreeNode<Token*>* poBracesOpenNode = new TreeNode<Token*>;
		poBracesOpenNode->SetLevel(iLevel + 1);
		poBracesOpenNode->SetData(poOpenBracesToken);
		poBracesOpenNode->SetLabel(poOpenBracesToken->GeneratePrintString());
		poBracesOpenNode->SetLeaf(true);
		
		TreeNode<Token*>* poBracesCloseNode = new TreeNode<Token*>;
		poBracesCloseNode->SetLevel(iLevel + 1);
		poBracesCloseNode->SetData(poCloseBracesToken);
		poBracesCloseNode->SetLabel(poCloseBracesToken->GeneratePrintString());
		poBracesCloseNode->SetLeaf(true);
		
		poProgram->AddChild(poProgramNode);
		poProgramNode->SetParent(poProgram);
		poProgram->AddChild(poIdentifierNode);
		poIdentifierNode->SetParent(poProgram);
		poProgram->AddChild(poBracesOpenNode);
		poBracesOpenNode->SetParent(poProgram);
		if(!bNoDeclarations)
		{
			poProgram->AddChild(poDeclarationSequence);
			poDeclarationSequence->SetParent(poProgram);
		}
		if(!bNoStatements)
		{
			poProgram->AddChild(poStatementSequence);
			poStatementSequence->SetParent(poProgram);
		}

		poProgram->AddChild(poBracesCloseNode);
		poBracesCloseNode->SetParent(poProgram);
		
		return poProgram;
	}
	/*bool Parser::ParseProgram()
	{
		AddStartAction("Program");
		// Program = "PROGRAM" identifier ";" Declarations ["BEGIN" Instructions] "END" identifier "." EOF .
		// look for a program token
		Token* poToken = GetCurrentToken();
		if(poToken->GetKind() != ProgramToken)
		{
			FlagError(NonSkippableError,"Missing symbol \"PROGRAM\" at the beginning of the Program");
			return false;
		}
		AddMatchAction(poToken);
		// if found, look for an identifer
		poToken = GetCurrentToken();
		if(poToken->GetKind() != IdentifierToken)
		{
			FlagError(NonSkippableError,"Missing an identifier after \"PROGRAM\" at the beginning of the Program");
			return false;
		}
		AddMatchAction(poToken);
		// if found, look for a semicolon
		poToken = GetCurrentToken();
		if(poToken->GetKind() != SemiColonToken)
		{
			FlagError(SkippableError,"Missing symbol \";\" after \"PROGRAM\" at the beginning of the Program");
		}
		else
		{
			AddMatchAction(poToken);
		}
		// if found, the next tokens should be declarations, parse them
		while(!ParseDeclarations())
		{
			return false;
		}
		// when done, there might be a begin token followed by some
		// instructions, check to see if we have it
		poToken = GetCurrentToken();
		if(poToken->GetKind() == BeginToken)
		{
			AddMatchAction(poToken);
			// if it is a begin token, there should be some
			// instructions following, parse them
			if(!ParseInstructions())
			{
				return false;
			}
		}
		// in all cases, we should find an end token at this point
		poToken = GetCurrentToken();
		if(poToken->GetKind() != EndToken)
		{
			FlagError(SkippableError,"Missing symbol \"END\" at the end of the Program");
		}
		else
		{
			AddMatchAction(poToken);
		}

		// if found, look for an identifier token
		poToken = GetCurrentToken();
		if(poToken->GetKind() != IdentifierToken)
		{
			FlagError(NonSkippableError,"Missing an identifier at the end of the Program");
			return false;
		}
		AddMatchAction(poToken);
		// if found, look for a terminating period
		poToken = GetCurrentToken();
		if(poToken->GetKind() != PeriodToken)
		{
			FlagError(SkippableError,"Missing symbol \".\" at the end of the Program");
		}
		else
		{
			AddMatchAction(poToken);
		}
		// finally, look for the EOF, no action is required for this, the only
		// reason why we care about it is because we don't want to allow anything in the
		// input after the end of the program
		poToken = GetCurrentToken();
		if(poToken->GetKind() != EOFToken)
		{
			FlagError(NonSkippableError,"Found code after the end of the program");
			return false;
		}
		// if we reached this point, then the parsing of the program has been successful, return true
		AddEndAction("Program");
		return true;
	}
	bool Parser::ParseDeclarations()
	{
		AddStartAction("Declarations");
		// Declarations = { ConstDecl | TypeDecl | VarDecl } .
		Token* poToken = NULL;
		TokenKind eKind = NullTokenKind;
		while(true)
		{
			poToken = GetCurrentToken();
			eKind = poToken->GetKind();
			if(eKind == ConstToken)
			{
				if(!ParseConstDeclaration())
				{
					if(SeekToNextStrongToken())
					{
						ParseDeclarations();
					}
					return false;
				}
			}
			else if(eKind == TypeToken)
			{
				if(!ParseTypeDeclaration())
				{
					if(SeekToNextStrongToken())
					{
						ParseDeclarations();
					}
					return false;
				}
			}
			else if(eKind == VarToken)
			{
				if(!ParseVarDeclaration())
				{
					if(SeekToNextStrongToken())
					{
						ParseDeclarations();
					}
					return false;
				}
			}
			else if(eKind == ProcedureToken)
			{
				if(!ParseProcDeclaration())
				{
					if(SeekToNextStrongToken())
					{
						ParseDeclarations();
					}
					return false;
				}
			}
			else
			{
				break;
			}
		}
		AddEndAction("Declaration");
		return true;
	}
	bool Parser::ParseConstDeclaration()
	{
		AddStartAction("ConstDecl");
		// ConstDecl = "CONST" {identifier "=" Expression ";"} .
		Token* poToken = GetCurrentToken();
		if(poToken->GetKind() != ConstToken)
		{
			FlagError(NonSkippableError,"Missing symbol \"CONST\" in a ConstDecl statement");
			return false;
		}
		AddMatchAction(poToken);
		while(true)
		{
			poToken = GetCurrentToken();
			if(poToken->GetKind() == IdentifierToken)
			{
				AddMatchAction(poToken);
				poToken = GetCurrentToken();
				if(poToken->GetKind() == EqualToken)
				{
					AddMatchAction(poToken);
					if(!ParseExpression())
					{
						return false;
					}
					poToken = GetCurrentToken();
					if(poToken->GetKind() != SemiColonToken)
					{
						FlagError(SkippableError,"Missing symbol \";\" at the end of a ConstDecl statement");
					}
					else
					{
						AddMatchAction(poToken);
					}
				}
				else
				{
					FlagError(NonSkippableError,"Missing symbol \"=\" in a ConstDecl statement");
					return false;
				}
			}
			else if(poToken->GetKind() == EqualToken)
			{
				// the identifier was forgotten !!
				FlagError(NonSkippableError,"Missing identifier in a ConstDecl statement");
				return false;
			}
			else
			{
				break;
			}
		}
		AddEndAction("ConstDecl");
		return true;
	}
	bool Parser::ParseTypeDeclaration()
	{
		AddStartAction("TypeDecl");
		// TypeDecl = "TYPE" {identifier "=" Type ";"} .
		Token* poToken = GetCurrentToken();
		if(poToken->GetKind() != TypeToken)
		{
			FlagError(NonSkippableError,"Missing symbol \"TYPE\" in a TypeDecl statement");
			return false;
		}
		AddMatchAction(poToken);
		while(true)
		{
			poToken = GetCurrentToken();
			if(poToken->GetKind() == IdentifierToken)
			{
				AddMatchAction(poToken);
				poToken = GetCurrentToken();
				if(poToken->GetKind() == EqualToken)
				{
					AddMatchAction(poToken);
					if(!ParseType())
					{
						return false;
					}
					poToken = GetCurrentToken();
					if(poToken->GetKind() != SemiColonToken)
					{
						FlagError(SkippableError,"Missing symbol \";\" at the end of a TypeDecl statement");
					}
					else
					{
						AddMatchAction(poToken);
					}
				}
				else
				{
					FlagError(NonSkippableError,"Missing symbol \"=\" in a TypeDecl statement");
					return false;
				}
			}
			else if(poToken->GetKind() == EqualToken)
			{
				// the identifier was forgotten !!
				FlagError(NonSkippableError,"Missing identifier in a TypeDecl statement");
				return false;
			}
			else
			{
				break;
			}
		}
		AddEndAction("TypeDecl");
		return true;
	}
	bool Parser::ParseVarDeclaration()
	{
		AddStartAction("VarDecl");
		// VarDecl = "VAR" {IdentifierList ":" Type ";"} .
		Token* poToken = GetCurrentToken();
		if(poToken->GetKind() != VarToken)
		{
			FlagError(NonSkippableError,"Missing symbol \"VAR\" in a VarDecl statement");
			return false;
		}
 		AddMatchAction(poToken);
 		unsigned int iCurrentPosition = 0;
 		while(true)
 		{
 			iCurrentPosition = m_iPosition;
 			if(ParseIdentifierList())
 			{
 				poToken = GetCurrentToken();
 				if(poToken->GetKind() != ColonToken)
 				{
 					FlagError(NonSkippableError,"Missing symbol \":\" in a VarDecl statement");
					return false;
				}
				AddMatchAction(poToken);
				if(!ParseType())
				{
					return false;
				}
				poToken = GetCurrentToken();
				if(poToken->GetKind() != SemiColonToken)
				{
					FlagError(SkippableError,"Missing symbol \";\" at the end of a VarDecl statement");
				}
				else
				{
					AddMatchAction(poToken);
				}
 			}
 			else if(poToken->GetKind() == ColonToken)
			{
				// the identifier was forgotten !!
				FlagError(NonSkippableError,"Missing identifier in a VarDecl statement");
				return false;
			}
 			else
 			{
 				AddRetractAndGoUpAction();
 				DropLastError();
 				SetPosition(iCurrentPosition);
 				break;
 			}
 		}
		AddEndAction("VarDecl");
		return true;
	}
	bool Parser::ParseProcDeclaration()
	{
		AddStartAction("ProcDecl");
		// ProcDecl = "PROCEDURE" identifier "(" [Formals] ")" [":" Type] ";" { VarDecl } [ "BEGIN" Instructions ] [ "RETURN" Expression ] "END" identifier ";" .
		// look for a procedure token
		Token* poToken = GetCurrentToken();
		if(poToken->GetKind() != ProcedureToken)
		{
			FlagError(NonSkippableError,"Missing symbol \"PROCEDURE\" at the beginning of a procedure declaration");
			return false;
		}
		AddMatchAction(poToken);
		// if found, look for an identifer
		poToken = GetCurrentToken();
		if(poToken->GetKind() != IdentifierToken)
		{
			FlagError(NonSkippableError,"Missing an identifier after \"PROCEDURE\" at the beginning of a procedure declaration");
			return false;
		}
		AddMatchAction(poToken);
		// if found, look for a round opening bracket
		poToken = GetCurrentToken();
		if(poToken->GetKind() != RoundBracketOpenToken)
		{
			FlagError(NonSkippableError,"Missing symbol \"(\" after \"PROCEDURE\" at the beginning of a procedure declaration");
			return false;
		}
		else
		{
			AddMatchAction(poToken);
		}
		// if found, the next tokens might be formals declarations, parse them
		if(GetCurrentToken()->GetKind() != RoundBracketCloseToken)
		{
			ParseFormals();
		}
		poToken = GetCurrentToken();
		if(poToken->GetKind() != RoundBracketCloseToken)
		{
			FlagError(SkippableError,"Missing symbol \")\" after \"PROCEDURE\" at the beginning of a procedure declaration");
		}
		else
		{
			AddMatchAction(poToken);
		}
		// when done, there might be a colon token followed by a return type, check to see if we have it
		poToken = GetCurrentToken();
		if(poToken->GetKind() == ColonToken)
		{
			AddMatchAction(poToken);
			// if it is a colon token, there should be a type following, parse it
			if(!ParseType())
			{
				return false;
			}
		}
		// in all cases, there should be a semicolon token here
		poToken = GetCurrentToken();
		if(poToken->GetKind() != SemiColonToken)
		{
			FlagError(SkippableError,"Missing symbol \";\" at the end of a procedure declaration");
		}
		else
		{
			AddMatchAction(poToken);
		}
		// here there might be many variable declarations
		while(true)
		{
			poToken = GetCurrentToken();
			TokenKind eKind = poToken->GetKind();
			if(eKind == VarToken)
			{
				if(!ParseVarDeclaration())
				{
					if(SeekToNextStrongToken())
					{
						ParseVarDeclaration();
					}
					return false;
				}
			}
			else
			{
				break;
			}
		}
		// when done, there might be a begin token followed by some instructions, check to see if we have it
		poToken = GetCurrentToken();
		if(poToken->GetKind() == BeginToken)
		{
			AddMatchAction(poToken);
			m_bParsingProcedureInstructions = true;
			// if it is a begin token, there should be some instructions following, parse them
			if(!ParseInstructions())
			{
				return false;
			}
			m_bParsingProcedureInstructions = false;
		}
		// when done, there might be a return token followed by an expression check to see if we have it
		poToken = GetCurrentToken();
		if(poToken->GetKind() == ReturnToken)
		{
			AddMatchAction(poToken);
			// if it is a return token, there should an expression following, parse it
			if(!ParseExpression())
			{
				return false;
			}
		}
		// in all cases, we should find an end token at this point
		poToken = GetCurrentToken();
		if(poToken->GetKind() != EndToken)
		{
			FlagError(SkippableError,"Missing symbol \"END\" at the end of a procedure");
		}
		else
		{
			AddMatchAction(poToken);
		}

		// if found, look for an identifier token
		poToken = GetCurrentToken();
		if(poToken->GetKind() != IdentifierToken)
		{
			FlagError(NonSkippableError,"Missing an identifier at the end of a procedure");
			return false;
		}
		AddMatchAction(poToken);
		// if found, look for a terminating semicolon
		poToken = GetCurrentToken();
		if(poToken->GetKind() != SemiColonToken)
		{
			FlagError(SkippableError,"Missing symbol \";\" at the end of a procedure");
		}
		else
		{
			AddMatchAction(poToken);
		}
		AddEndAction("ProcDecl");
		return true;
	}
	bool Parser::ParseType()
	{
		AddStartAction("Type");
		// Type = identifier | "ARRAY" Expression "OF" Type | "RECORD" {IdentifierList ":" Type ";"} "END" .
		Token* poToken = GetCurrentToken();
		TokenKind eKind = poToken->GetKind();
		if(eKind == IdentifierToken)
		{
			AddMatchAction(poToken);
			AddEndAction("Type");
			return true;
		}
		else if(eKind == ArrayToken)
		{
			AddMatchAction(poToken);
			if(!ParseExpression())
			{
				return false;
			}
			poToken = GetCurrentToken();
			if(poToken->GetKind() != OfToken)
			{
				FlagError(NonSkippableError,"Missing symbol \"OF\" after \"ARRAY\" in a Type statement");
				return false;
			}
			AddMatchAction(poToken);
			if(!ParseType())
			{
				return false;
			}
			AddEndAction("Type");
			return true;
		}
		else if(eKind == RecordToken)
		{
			AddMatchAction(poToken);
			poToken = GetCurrentToken();
			unsigned int iCurrentPosition = 0;
			while(true)
			{
				iCurrentPosition = m_iPosition;
				if(ParseIdentifierList())
				{
					poToken = GetCurrentToken();
					if(poToken->GetKind() != ColonToken)
					{
						FlagError(NonSkippableError,"Missing symbol \":\" after \"RECORD\" in a Type statement");
						return false;
					}
					AddMatchAction(poToken);
					if(!ParseType())
					{
						return false;
					}
					poToken = GetCurrentToken();
					if(poToken->GetKind() != SemiColonToken)
					{
						FlagError(SkippableError,"Missing symbol \";\" at the end of a Type statement");
					}
					else
					{
						AddMatchAction(poToken);
					}
				}
				else
				{
					AddRetractAndGoUpAction();
					SetPosition(iCurrentPosition);
					DropLastError();
					break;
				}
			}
			poToken = GetCurrentToken();
			if(poToken->GetKind() == EndToken)
			{
				AddMatchAction(poToken);
				AddEndAction("Type");
				return true;
			}
		}
		char cWrite[1024];
		sprintf(cWrite,"Unallowed token %s found in a Type statement",GetCurrentToken()->GetString().c_str());
		FlagError(NonSkippableError,cWrite);
		return false;
	}
	bool Parser::ParseIdentifierList()
	{
		AddStartAction("IdentifierList");
		// IdentifierList = identifier {"," identifier} .
		Token* poToken = GetCurrentToken();
		if(poToken->GetKind() != IdentifierToken)
		{
			FlagError(NonSkippableError,"Missing first identifier in an IdentifierList statement");
			return false;
		}
		AddMatchAction(poToken);
		// look for any number of comma separated identifiers
		while(true)
		{
			poToken = GetCurrentToken();
			if(poToken->GetKind() == CommaToken)
			{
				AddMatchAction(poToken);
				poToken = GetCurrentToken();
				if(poToken->GetKind() != IdentifierToken)
				{
					FlagError(NonSkippableError,"Missing an identifier in an IdentifierList statement");
					return false;
				}
				AddMatchAction(poToken);
			}
			else
			{
				break;
			}
		}
		AddEndAction("IdentifierList");
		return true;
	}
	bool Parser::ParseFormals()
	{
		AddStartAction("Formals");
		// Formals = Formal { ";" Formal } .
		if(!ParseFormal())
		{
			return false;
		}
		// look for any number of semi-colon separated expressions
		Token* poToken = NULL;
		while(true)
		{
			poToken = GetCurrentToken();
			if(poToken->GetKind() == SemiColonToken)
			{
				AddMatchAction(poToken);
				if(!ParseFormal())
				{
					return false;
				}
			}
			else
			{
				break;
			}
		}
		AddEndAction("Formals");
		return true;
	}
	bool Parser::ParseFormal()
	{
		AddStartAction("Formal");
		// Formal = IdentifierList ":" Type .
		Token* poToken = GetCurrentToken();
		if(ParseIdentifierList())
		{
			poToken = GetCurrentToken();
			if(poToken->GetKind() != ColonToken)
			{
				FlagError(NonSkippableError,"Missing symbol \":\" in a Formal statement");
				return false;
			}
			AddMatchAction(poToken);
			if(!ParseType())
			{
				return false;
			}
		}
		else if(poToken->GetKind() == ColonToken)
		{
			// the identifier was forgotten !!
			FlagError(NonSkippableError,"Missing identifier in a Formal statement");
			return true;
		}
		else
		{
			return false;
		}
		AddEndAction("Formal");
		return true;
	}
	bool Parser::ParseActuals()
	{
		AddStartAction("Actuals");
		// Actuals = ExpressionList .
		if(!ParseExpressionList())
		{
			return false;
		}
		AddEndAction("Actuals");
		return true;
	}*/
	void Parser::PrintSymbolTable(const bool& bPrintGraphicalTable)
	{
		// do nothing
	}
	void Parser::PrintAST(const bool& bPrintGraphicalTable)
	{
		// do nothing
	}
	void Parser::PrintCST(TreeNode<Token*>* poCSTree)
	{
		string sPrintString = "";
		unsigned int iLevel = poCSTree->GetLevel();
		unsigned int i = 0;
		for(i = 0 ; i < iLevel ; i++)
		{
			sPrintString = sPrintString + ' ' + ' ';
		}
		sPrintString = sPrintString + poCSTree->GetLabel();
		printf("%s\n",sPrintString.c_str());
		list< TreeNode<Token*>* >* plpoChildren = poCSTree->GetChildren();
		list< TreeNode<Token*>* >::iterator liChilren;
		for(liChilren = plpoChildren->begin() ; liChilren != plpoChildren->end() ; liChilren++)
		{
			PrintCST((*liChilren));
		}
	}
}



