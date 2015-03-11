//Ahmed Hussein
//ahussei4@jhu.edu

#ifndef SCANNER_H_
#define SCANNER_H_

#include "Token.h"
#include "vector"
#include "list"

using namespace std;

namespace DraftCompiler
{
	enum SyntacticUnitType
	{
		NullSyntacticUnit = 0,
		SignSequence = 1,
		Integer = 2,
		Word = 3
	};
	
	class Scanner
	{
	public:
		Scanner();
		Scanner(const Scanner& oScanner);
		Scanner(const string& sText);
		~Scanner();
		Scanner& operator=(const Scanner& oScanner);
		void Rewind();
		void SetSourceText(const string& sText);
		string GetSourceText() const;
		unsigned int GetSourceTextSize() const;
		bool GenerateTokens(vector<Token*>& vpoTokens);
		
	private:
	
	protected:
		void Initialize();
		Token GetNextToken();
		string PopSignificantCharactersChunk();
		static bool IsWhiteSpace(const char& cReadCharacter);
		void SkipComment();
		string GetFirstSyntacticUnit(const string& sChunk,SyntacticUnitType& eType);
		string GetFirstAcceptableSignSequence(const string& sSequence);
		static bool IsLetter(const char& cCharacter);
		static bool IsDigit(const char& cCharacter);
		static TokenKind ClassifyWord(const string& sWord);
		static TokenKind ClassifySignSequence(const string& sWord);
		string m_sSourceText;
		unsigned int m_iReaderPosition;
		unsigned int m_iSourceSize;
	};
}

#endif


