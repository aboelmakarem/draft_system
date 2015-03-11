// draft language project by Ahmed M. Hussein (am.hussin@gmail.com) February 2015


#include "Token.h"
#include "stdio.h"

namespace DraftCompiler
{	
	Token::Token()
	{
		Initialize();
	}
	Token::Token(const string& sString)
	{
		Initialize();
		SetString(sString);
	}
	Token::Token(const Token& oToken)
	{
		*this = oToken;
	}
	Token::~Token()
	{
	
	}
	Token& Token::operator=(const Token& oToken)
	{
		m_sString = oToken.m_sString;
		m_eKind = oToken.m_eKind;
		m_iGlobalStartPosition = oToken.m_iGlobalStartPosition;
		m_iGlobalEndPosition = oToken.m_iGlobalEndPosition;
		m_iValue = oToken.m_iValue;
		return *this;
	}
	void Token::Reset()
	{
		Initialize();
	}
	void Token::SetString(const string& sString)
	{
		m_sString = sString;
	}
	string Token::GetString() const
	{
		return m_sString;
	}
	void Token::SetKind(const TokenKind& eKind)
	{
		m_eKind = eKind;
	}
	TokenKind Token::GetKind() const
	{
		return m_eKind;
	}
	void Token::SetStartPosition(const unsigned int& iPosition)
	{
		m_iGlobalStartPosition = iPosition;
	}
	unsigned int Token::GetStartPosition() const
	{
		return m_iGlobalStartPosition;
	}
	void Token::SetEndPosition(const unsigned int& iPosition)
	{
		m_iGlobalEndPosition = iPosition;
	}
	unsigned int Token::GetEndPosition() const
	{
		return m_iGlobalEndPosition;
	}
	void Token::SetValue()
	{
		sscanf(m_sString.c_str(),"%d",&m_iValue);
	}
	long unsigned int Token::GetValue() const
	{
		return m_iValue;
	}
	string Token::GeneratePrintString() const
	{
		char cWrite[1024];
		if((m_eKind == NullTokenKind) || (m_eKind == InvalidToken))
		{
			// do not print anything
		}
		else if(m_eKind == EOFToken)
		{
			sprintf(cWrite,"eof@(%d, %d)",m_iGlobalStartPosition,m_iGlobalEndPosition);
		}
		else if(m_eKind == IntegerToken)
		{
			sprintf(cWrite,"integer<%d>@(%d, %d)",m_iValue,m_iGlobalStartPosition,m_iGlobalEndPosition);
		}
		else if(m_eKind == IdentifierToken)
		{
			sprintf(cWrite,"identifier<%s>@(%d, %d)",m_sString.c_str(),m_iGlobalStartPosition,m_iGlobalEndPosition);
		}
		else
		{
			sprintf(cWrite,"%s@(%d, %d)",m_sString.c_str(),m_iGlobalStartPosition,m_iGlobalEndPosition);
		}
		string sPrintString = cWrite;
		return sPrintString;
	}
	void Token::Print() const
	{
		printf("%s\n",GeneratePrintString().c_str());
	}
	void Token::Initialize()
	{
		m_sString = "";
		m_eKind = NullTokenKind;
		m_iGlobalStartPosition = 0;
		m_iGlobalEndPosition = 0;
		m_iValue = 0;
	}
}



