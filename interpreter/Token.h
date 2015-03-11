// draft language project by Ahmed M. Hussein (am.hussin@gmail.com) February 2015

#ifndef TOKEN_H_
#define TOKEN_H_

#include "string"

using namespace std;

namespace DraftCompiler
{
	enum TokenKind
	{
		NullTokenKind = 0,
		EOFToken = 1,
		IntegerToken = 2,
		IdentifierToken = 3,
		// symbols
		PeriodToken = 4,
		ExponentEToken = 5,
		PlusToken = 6,
		MinusToken = 7,
		MultiplyToken = 8,
		DivideToken = 9,
		IntegerDivideToken = 10,
		ModulusDivideToken = 11,
		AssignToken = 12,
		CommaToken = 13,
		ColonToken = 14,
		SemiColonToken = 15,
		RoundBracketOpenToken = 16,
		RoundBracketCloseToken = 17,
		SquareBracketOpenToken = 18,
		SquareBracketCloseToken = 19,
		EqualToken = 20,
		NotEqualToken = 21,
		LessThanToken = 22,
		GreaterThanToken = 23,
		LessThanOrEqualToken = 24,
		GreaterThanOrEqualToken = 25,
		ImportAssignToken = 26,
		OrToken = 27,
		AndToken = 28,
		NotToken = 29,
		TrueToken = 30,
		FalseToken = 31,
		NullToken = 32,
		QuoteToken = 33,
		HashToken = 34,
		CaretToken = 35,
		// keywords
		CurlyBracesOpenToken = 37,
		CurlyBracesCloseToken = 38,
		ConstToken = 39,
		TypeToken = 40,
		VarToken = 41,
		ArrayToken = 42,
		OfToken = 43,
		ClassToken = 44,
		IfToken = 45,
		ElseIfToken = 47,
		ElseToken = 48,
		WhileToken = 49,
		PrintToken = 50,
		ProcedureToken = 51,
		ProgramToken = 52,
		ImportToken = 53,
		ReturnToken = 54,
		InvalidToken = 55
	};
	
	class Token
	{
	public:
		Token();
		Token(const string& sString);
		Token(const Token& oToken);
		~Token();
		Token& operator=(const Token& oToken);
		void Reset();
		void SetString(const string& sString);
		string GetString() const;
		void SetKind(const TokenKind& eKind);
		TokenKind GetKind() const;
		void SetStartPosition(const unsigned int& iPosition);
		unsigned int GetStartPosition() const;
		void SetEndPosition(const unsigned int& iPosition);
		unsigned int GetEndPosition() const;
		void SetValue();
		long unsigned int GetValue() const;
		string GeneratePrintString() const;
		void Print() const;
		
	private:
	
	protected:
		void Initialize();
		string m_sString;
		TokenKind m_eKind;
		unsigned int m_iGlobalStartPosition;
		unsigned int m_iGlobalEndPosition;
		int m_iValue;
	};
}

#endif


