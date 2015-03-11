//Ahmed Hussein
//ahussei4@jhu.edu

#include "Scanner.h"
#include "stdio.h"

namespace DraftCompiler
{
	Scanner::Scanner()
	{
		Initialize();
	}
	Scanner::Scanner(const Scanner& oScanner)
	{
		*this = oScanner;
	}
	Scanner::Scanner(const string& sText)
	{
		Initialize();
		SetSourceText(sText);
	}
	Scanner::~Scanner()
	{
	
	}
	Scanner& Scanner::operator=(const Scanner& oScanner)
	{
		m_sSourceText = oScanner.m_sSourceText;
		m_iReaderPosition = oScanner.m_iReaderPosition;
		m_iSourceSize = oScanner.m_iSourceSize;
		return *this;
	}
	void Scanner::Rewind()
	{
		m_iReaderPosition = 0;
	}
	void Scanner::SetSourceText(const string& sText)
	{
		m_sSourceText = sText;
		m_iSourceSize = (unsigned int)m_sSourceText.size();
		m_iReaderPosition = 0;
	}
	string Scanner::GetSourceText() const
	{
		return m_sSourceText;
	}
	unsigned int Scanner::GetSourceTextSize() const
	{
		return m_iSourceSize;
	}
	void Scanner::Initialize()
	{
		m_sSourceText = "";
		m_iReaderPosition = 0;
		m_iSourceSize = 0;
	}
	string Scanner::PopSignificantCharactersChunk()
	{
		string sChunk = "";
		char cReadCharacter = 0;
		if(m_iReaderPosition >= m_iSourceSize)
		{
			// the entire source has been processed, return an empty chunk of characters
			return sChunk;
		}
		// process the remaining source
		bool bChunkBeginningFound = false;
		while(m_iReaderPosition < m_iSourceSize)
		{
			cReadCharacter = m_sSourceText[m_iReaderPosition];		// read next character
			if(bChunkBeginningFound)
			{
				if(IsWhiteSpace(cReadCharacter))
				{
					break;
				}
				else
				{
					sChunk = sChunk + cReadCharacter;
					m_iReaderPosition++;
				}
			}
			else
			{
				if(IsWhiteSpace(cReadCharacter))
				{
					m_iReaderPosition++;
				}
				else
				{
					if(cReadCharacter == 47 && m_sSourceText[m_iReaderPosition + 1] == 47)
					{
						SkipComment();
					}
					else
					{
						bChunkBeginningFound = true;
						sChunk = sChunk + cReadCharacter;
						m_iReaderPosition++;
					}
				}
			}
		}
		return sChunk;
	}
	Token Scanner::GetNextToken()
	{
		Token oToken;
		// get the next characters chunk without the whitespace around it
		string sChunk = PopSignificantCharactersChunk();
		if(sChunk.empty())
		{
			// end of source text reached, create an EOF token and return
			oToken.SetKind(EOFToken);
			oToken.SetStartPosition(m_iReaderPosition);
			oToken.SetEndPosition(m_iReaderPosition);
		}
		else
		{
			// we still have significant chunks, create a regular token
			SyntacticUnitType eType = NullSyntacticUnit;
			string sUnit = "";
			unsigned int iUnitSize = 0;
			// separate words from symbols, words can be either identifiers, in which
			// case they may contain digits as well, or keywords, like "while" or "module"
			// at this point, we will just separate the words from the symbols
			// the symbols require special handling because they are fixed combination
			// of strings like '<=' or '==', or sometimes just ';'
			// any symbol that is not legal will cause an error to be thrown
			// (e.g the double quotes are not symbols in draft, then encountring them 
			// in a significant chunk will cause an error to be thrown
			sUnit = GetFirstSyntacticUnit(sChunk,eType);
			// retract the reader position to after the first unit so that the next time 
			// it starts reading from the beginning of the unit skipped this time
			m_iReaderPosition = m_iReaderPosition - ((unsigned int)sChunk.size() - (unsigned int)sUnit.size());
			// if the unit is an integer, create an integer token for it and return it, 
			// if it is a word, create a token with the entire string and then check for
			// its kind, based on the language grammar, if it is a sign, or a sequence of 
			// signs, first, we have to check its validity, so that sequences like "*~@" or
			// "-$!" or even single character signs such as '@' which are not allowed in 
			// draft are filtered out, if it passes the validity test, create a token with
			// the entire string and check for its kind
			oToken.SetString(sUnit);
			iUnitSize = (unsigned int)sUnit.size();
			oToken.SetStartPosition(m_iReaderPosition - iUnitSize);
			oToken.SetEndPosition(m_iReaderPosition - 1);
			if(eType == Integer)
			{
				oToken.SetKind(IntegerToken);
				oToken.SetValue();
			}
			else if(eType == Word)
			{
				oToken.SetKind(ClassifyWord(sUnit));
			}
			else if(eType == SignSequence)
			{
				oToken.SetKind(ClassifySignSequence(sUnit));
			}
			else
			{
				oToken.SetKind(InvalidToken);
			}
		}
		return oToken;
	}
	bool Scanner::GenerateTokens(vector<Token*>& vpoTokens)
	{
		Rewind();
		list<Token> loTokens;
		Token oToken;
		while(true)
		{
			oToken = GetNextToken();
			loTokens.push_back(oToken);
			if(oToken.GetKind() == EOFToken)
			{
				break;
			}
		}
		
		list<Token>::iterator liTokens;
		unsigned int iSize = loTokens.size();
		vpoTokens.clear();
		vpoTokens.resize(iSize);
		unsigned int i = 0;
		for(liTokens = loTokens.begin() ; liTokens != loTokens.end() ; liTokens++)
		{
			if((*liTokens).GetKind() != InvalidToken)
			{
				vpoTokens[i] = new Token((*liTokens));
				i = i + 1;
			}
			else
			{
				fprintf(stderr,"error: invalid token %s between positions (%d,%d)\n",(*liTokens).GetString().c_str(),(*liTokens).GetStartPosition(),(*liTokens).GetEndPosition());
				return false;
			}
		}
		return true;
	}
	bool Scanner::IsWhiteSpace(const char& cReadCharacter)
	{
		// check for line feeds, vertical tabs, and carriage returns
		if((cReadCharacter == 10) || (cReadCharacter == 11) || (cReadCharacter == 13))
		{
			return true;
		}
		// check for horizontal tabs, form feeds, and white spaces
		if((cReadCharacter == 9)  || (cReadCharacter == 12) || (cReadCharacter == 32))
		{
			return true;
		}
		return false;
	}
	void Scanner::SkipComment()
	{
		// this function is called ONCE the comment opening symbol '//' is found, at that
		// point, the reader position is still at the first '/'. The function must exit when the 
		// reader position is at end of the that line
		while(m_iReaderPosition < m_iSourceSize)
		{
			m_iReaderPosition++;
			if((m_sSourceText[m_iReaderPosition] >= 10) && (m_sSourceText[m_iReaderPosition] <= 13))
			{
				break;
			}
		}
	}
	string Scanner::GetFirstSyntacticUnit(const string& sChunk,SyntacticUnitType& eType)
	{
		unsigned int iSize = (unsigned int)sChunk.size();
		unsigned int i = 0;
		char cReadCharacter = 0;
		string sUnit = "";
		eType = NullSyntacticUnit;
		for(i = 0 ; i < iSize ; i++)
		{
			cReadCharacter = sChunk[i];
			if(eType == NullSyntacticUnit)
			{
				// happens only when reading the first character, read it and set the type
				// based on that
				if(IsLetter(cReadCharacter))
				{
					eType = Word;
				}
				else if(IsDigit(cReadCharacter))
				{
					eType = Integer;
				}
				else
				{
					eType = SignSequence;
				}
				sUnit = sUnit + cReadCharacter;
			}
			else if(eType == SignSequence)
			{
				// already reading a sign, or a sequence of signs, keep going till a letter
				// or a digit appears
				if(IsLetter(cReadCharacter) || IsDigit(cReadCharacter))
				{
					break;
				}
				else
				{
					sUnit = sUnit + cReadCharacter;
				}
			}
			else if(eType == Integer)
			{
				// already reading an integer, keep going till something different than a 
				// digit appears
				if(!IsDigit(cReadCharacter))
				{
					break;
				}
				else
				{
					sUnit = sUnit + cReadCharacter;
				}
			}
			else if(eType == Word)
			{
				// already reading a word, which is a sequence of letters and digits, 
				// keep going till a sign appears
				if(!IsLetter(cReadCharacter) && !IsDigit(cReadCharacter))
				{
					break;
				}
				else
				{
					sUnit = sUnit + cReadCharacter;
				}
			}
		}
		// we have to return the first acceptable sign sequence if it exists
		// by acceptable, it is meant that the sign sequence is OK as a sequence 
		// as far as the precedence rules are concerned, but it doesn't mean that
		// it is a grammatically valid sequence, so a sequence "@" is acceptable but
		// invalid. the validity of the sequence will be determined elsewhere
		if(eType == SignSequence)
		{
			sUnit = GetFirstAcceptableSignSequence(sUnit);
		}
		return sUnit;
	}
	string Scanner::GetFirstAcceptableSignSequence(const string& sSequence)
	{
		// apply precedence rules for sign sequences
		unsigned int iSize = (unsigned int)sSequence.size();
		if(iSize == 1)
		{
			return sSequence;
		}
		else
		{
			string sOneCharacterSequence = "";
			sOneCharacterSequence = sOneCharacterSequence + sSequence[0];
			string sTwoCharactersSequence = sOneCharacterSequence + sSequence[1];
			if(sSequence[1] == '=')
			{
				if((sSequence[0] == '=') || (sSequence[0] == '<') || (sSequence[0] == '>') || (sSequence[0] == '!'))
				{
					return sTwoCharactersSequence;
				}
			}
			return sOneCharacterSequence;
		}
	}
	bool Scanner::IsLetter(const char& cCharacter)
	{
		// check if the input is a letter, based on its ASCII value
		if((cCharacter >= 65) && (cCharacter <= 90))
		{
			return true;
		}
		if((cCharacter >= 97) && (cCharacter <= 122))
		{
			return true;
		}
		// the underscore (ASCII 95) is a valid letter
		if(cCharacter == 95)
		{
			return true;
		}
		return false;
	}
	bool Scanner::IsDigit(const char& cCharacter)
	{
		// check if the input is a digit, based on its ASCII value
		if((cCharacter >= 48) && (cCharacter <= 57))
		{
			return true;
		}
		return false;
	}
	TokenKind Scanner::ClassifyWord(const string& sWord)
	{
		if(sWord.compare("div") == 0)
		{
			return IntegerDivideToken;
		}
		if(sWord.compare("mod") == 0)
		{
			return ModulusDivideToken;
		}
		if(sWord.compare("e") == 0)
		{
			return ExponentEToken;
		}
		if(sWord.compare("or") == 0)
		{
			return OrToken;
		}
		if(sWord.compare("and") == 0)
		{
			return AndToken;
		}
		if(sWord.compare("not") == 0)
		{
			return NotToken;
		}
		if(sWord.compare("true") == 0)
		{
			return TrueToken;
		}
		if(sWord.compare("false") == 0)
		{
			return FalseToken;
		}
		if(sWord.compare("null") == 0)
		{
			return NullToken;
		}
		if(sWord.compare("program") == 0)
		{
			return ProgramToken;
		}
		if(sWord.compare("procedure") == 0)
		{
			return ProcedureToken;
		}
		if(sWord.compare("import") == 0)
		{
			return ImportToken;
		}
		if(sWord.compare("return") == 0)
		{
			return ReturnToken;
		}
		if(sWord.compare("const") == 0)
		{
			return ConstToken;
		}
		if(sWord.compare("type") == 0)
		{
			return TypeToken;
		}
		if(sWord.compare("var") == 0)
		{
			return VarToken;
		}
		if(sWord.compare("array") == 0)
		{
			return ArrayToken;
		}
		if(sWord.compare("of") == 0)
		{
			return OfToken;
		}
		if(sWord.compare("class") == 0)
		{
			return ClassToken;
		}
		if(sWord.compare("if") == 0)
		{
			return IfToken;
		}
		if(sWord.compare("elseif") == 0)
		{
			return ElseIfToken;
		}
		if(sWord.compare("else") == 0)
		{
			return ElseToken;
		}
		if(sWord.compare("while") == 0)
		{
			return WhileToken;
		}
		if(sWord.compare("print") == 0)
		{
			return PrintToken;
		}
		return IdentifierToken;
	}
	TokenKind Scanner::ClassifySignSequence(const string& sWord)
	{
		if(sWord.compare(":") == 0)
		{
			return ColonToken;
		}
		if(sWord.compare(";") == 0)
		{
			return SemiColonToken;
		}
		if(sWord.compare("+") == 0)
		{
			return PlusToken;
		}
		if(sWord.compare("-") == 0)
		{
			return MinusToken;
		}
		if(sWord.compare("*") == 0)
		{
			return MultiplyToken;
		}
		if(sWord.compare("/") == 0)
		{
			return DivideToken;
		}
		if(sWord.compare("(") == 0)
		{
			return RoundBracketOpenToken;
		}
		if(sWord.compare(")") == 0)
		{
			return RoundBracketCloseToken;
		}
		if(sWord.compare(":=") == 0)
		{
			return ImportAssignToken;
		}
		if(sWord.compare("==") == 0)
		{
			return EqualToken;
		}
		if(sWord.compare("!=") == 0)
		{
			return NotEqualToken;
		}
		if(sWord.compare("<") == 0)
		{
			return LessThanToken;
		}
		if(sWord.compare(">") == 0)
		{
			return GreaterThanToken;
		}
		if(sWord.compare("<=") == 0)
		{
			return LessThanOrEqualToken;
		}
		if(sWord.compare(">=") == 0)
		{
			return GreaterThanOrEqualToken;
		}
		if(sWord.compare("[") == 0)
		{
			return SquareBracketOpenToken;
		}
		if(sWord.compare("]") == 0)
		{
			return SquareBracketCloseToken;
		}
		if(sWord.compare(".") == 0)
		{
			return PeriodToken;
		}
		if(sWord.compare(",") == 0)
		{
			return CommaToken;
		}
		if(sWord.compare("=") == 0)
		{
			return AssignToken;
		}
		if(sWord.compare("\"") == 0)
		{
			return QuoteToken;
		}
		if(sWord.compare("#") == 0)
		{
			return HashToken;
		}
		if(sWord.compare("^") == 0)
		{
			return CaretToken;
		}
		if(sWord.compare("{") == 0)
		{
			return CurlyBracesOpenToken;
		}
		if(sWord.compare("}") == 0)
		{
			return CurlyBracesCloseToken;
		}
		return InvalidToken;
	}
}


