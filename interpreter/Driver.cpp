//Ahmed Hussein
//ahussei4@jhu.edu

#include "iostream"
#include "Driver.h"
#include "Scanner.h"
#include "Parser.h"
//#include "SemanticAnalyzer.h"
//#include "Interpreter.h"
//#include "CodeGenerator.h"
//#include "ParseTreeTextPrinter.h"
//#include "ParseTreeDOTGenerator.h"
#include "stdio.h"
#include "string.h"

using namespace DraftCompiler;

namespace DraftCompiler
{
	Driver::Driver()
	{
		Initialize();
	}
	Driver::Driver(const Driver& oDriver)
	{
		*this = oDriver;
	}
	Driver::~Driver()
	{

	}
	Driver& Driver::operator=(const Driver& oDriver)
	{
		m_eTaskType = oDriver.m_eTaskType;
		m_sSourceFile = oDriver.m_sSourceFile;
		m_bGenerateGraphicalOutput = oDriver.m_bGenerateGraphicalOutput;
		return *this;
	}
	bool Driver::ParseRunCommand(int argc,char** argv)
	{
		Initialize();
		// if no arguments were supplied, set the default task and return
		if(argc == 1)
		{
			return true;
		}
		else if(argc == 2)
		{
			if(argv[1][0] == '-')
			{
				if(argv[1][1] == 'g')
				{
					fprintf(stderr,"error: '-g' option cannot be used alone\n");
					return false;
				}
				else
				{
					// it is an option, set the corresponding task
					if(!SetRunTask(argv[1]))
					{
						fprintf(stderr,"error: unknown run option %s\n",argv[1]);
						PrintUsageInstructions();
						return false;
					}
				}
			}
			else
			{
				// it is a file name, set it
				m_sSourceFile = argv[1];
			}
		}
		else if(argc == 3)
		{
			if(argv[1][0] == '-')
			{
				if(argv[1][1] == 'g')
				{
					fprintf(stderr,"error: '-g' option cannot be set alone or before other options\n");
					return false;
				}
				else
				{
					// it is an option, set the corresponding task
					if(!SetRunTask(argv[1]))
					{
						fprintf(stderr,"error: unknown run option %s\n",argv[1]);
						PrintUsageInstructions();
						return false;
					}
				}
			}
			else
			{
				fprintf(stderr,"error: second argument in a 3 arguments invocation must be an option, found : %s\n",argv[1]);
				PrintUsageInstructions();
				return false;
			}

			if(argv[2][0] == '-')
			{
				if(argv[2][1] == 'g')
				{
					m_bGenerateGraphicalOutput = true;
				}
				else
				{
					fprintf(stderr,"error: the second option, if present cannot be anything other than '-g', found %s\n",argv[2]);
					return false;
				}
			}
			else
			{
				m_sSourceFile = argv[2];
			}
		}
		else if(argc == 4)
		{
			if(argv[1][0] == '-')
			{
				if(argv[1][1] == 'g')
				{
					fprintf(stderr,"error: '-g' option cannot be set alone or before other options\n");
					return false;
				}
				else
				{
					// it is an option, set the corresponding task
					if(!SetRunTask(argv[1]))
					{
						fprintf(stderr,"error: unknown run option %s\n",argv[1]);
						PrintUsageInstructions();
						return false;
					}
				}
			}
			else
			{
				fprintf(stderr,"error: second argument in a 4 arguments invocation must be an option, found : %s\n",argv[1]);
				PrintUsageInstructions();
				return false;
			}

			if((argv[2][0] == '-') && (argv[2][1] == 'g'))
			{
				m_bGenerateGraphicalOutput = true;
			}
			else
			{
				fprintf(stderr,"error: third argument in a 4 arguments invocation must be the option '-g', found : %s\n",argv[2]);
				PrintUsageInstructions();
				return false;
			}

			if(argv[3][0] == '-')
			{
				fprintf(stderr,"error: last argument in a 4 arguments invocation must be the filename, found an option : %s\n",argv[2]);
				PrintUsageInstructions();
				return false;
			}
			else
			{
				m_sSourceFile = argv[3];
			}
		}
		else
		{
			fprintf(stderr,"error: too many input arguments\n");
			PrintUsageInstructions();
			return false;
		}
		return true;
	}
	void Driver::PrintUsageInstructions()
	{
		printf("\n");
		printf("draft Compiler Usage Instructions : ./dc [-sctai] [filename]\n");
		printf("\n");
		printf("\t [-sctai] : Compiler Invocation Options : \n");
		printf("\t\t -s : Scan source text\n");
		printf("\t\t -c : Parse source text\n");
		printf("\t\t -t : Generate symbol table\n");
		printf("\t\t -a : Generate abstract syntax tree A\n");
		printf("\t\t -i : Run interpreter\n");
		printf("\t\t default (no options): Run default task (interpreter)\n");
		printf("\n");
		printf("\t[filename] : Source File Name : \n");
		printf("\t\t If no source file is supplied, the standard input will be used instead\n");
		printf("\n");
	}
	void Driver::Initialize()
	{
		m_eTaskType = DefaultTask;
		m_sSourceFile = "";
		m_bGenerateGraphicalOutput = false;
	}
	bool Driver::SetRunTask(char* cOptionArgument)
	{
		// all the options have the form '-x' where x is a SINGLE character
		if(strlen(cOptionArgument) != 2)
		{
			return false;
		}
		// skip the first '-'
		if(cOptionArgument[1] == 's')
		{
			m_eTaskType = ScanTask;
		}
		else if(cOptionArgument[1] == 'c')
		{
			m_eTaskType = ParseTask;
		}
		else if(cOptionArgument[1] == 't')
		{
			m_eTaskType = SemanticAnalysisTask;
		}
		else if(cOptionArgument[1] == 'a')
		{
			m_eTaskType = ASTGenerationTask;
		}
		else if(cOptionArgument[1] == 'i')
		{
			m_eTaskType = InterpreterTask;
		}
		else if(cOptionArgument[1] == 'x')
		{
			m_eTaskType = DefaultTask;
		}
		else
		{
			return false;
		}
		return true;
	}
	void Driver::Execute()
	{
		// execute the compiler tasks as specified
		// at this point, only the scanner and parser tasks can be invoked
		// in all cases, we need the source text and a scanner for any task
		string sSourceText;
		if(!GetSourceText(sSourceText))
		{
			return;
		}
		Scanner oScanner(sSourceText);
		if(m_eTaskType == ScanTask)
		{
			Scan(&oScanner);
		}
		else if((m_eTaskType == ParseTask) || (m_eTaskType == SemanticAnalysisTask) || (m_eTaskType == ASTGenerationTask))
		{
			Parser* poParser = NULL;
			if(m_eTaskType == ParseTask)
			{
				poParser = new Parser;
			}
			//else
			//{
			//	poParser = new SemanticAnalyzer;
			//}
			// turn AST generation on in the symantic analyzer
			//if(m_eTaskType == ASTGenerationTask)
			//{
			//	((SemanticAnalyzer*)poParser)->EnableASTGeneration();
			//}
			
			vector<Token*> vpoTokens;
			if(!oScanner.GenerateTokens(vpoTokens))
			{
				return;
			}
			poParser->SetTokens(&vpoTokens);
			/*ParserObserver* poParserObserver = NULL;
			if((m_eTaskType == ParseTask))
			{
				if(m_bGenerateGraphicalOutput)
				{
				//	poParserObserver = new ParseTreeDOTGenerator;
				}
				else
				{
					poParserObserver = new ParseTreeTextPrinter;
				}
				poParser->AddObserver(poParserObserver);
				poParserObserver->SetParser(poParser);
			}*/
			bool bDidParse = poParser->Parse();
			//list<ParserError>* ploErrors = poParser->GetErrors();
			/*if(bDidParse)
			{
				if(m_eTaskType == ParseTask)
				{
					//poParserObserver->GenerateTree();
				}
				//else if(m_eTaskType == SemanticAnalysisTask)
				//{
				//	poParser->PrintSymbolTable(m_bGenerateGraphicalOutput);
				//}
				//else if(m_eTaskType == ASTGenerationTask)
				//{
				//	poParser->PrintAST(m_bGenerateGraphicalOutput);
				//}
			}
			else
			{
				list<ParserError>::iterator liErrors;
				for(liErrors = ploErrors->begin() ; liErrors != ploErrors->end() ; liErrors++)
				{
					fprintf(stderr,"error: around position %d - %s\n",(*liErrors).GetPosition(),(*liErrors).GetString().c_str());
				}
			}
			delete poParserObserver;*/
			delete poParser;
		}
		/*else if((m_eTaskType == InterpreterTask) || (m_eTaskType == DefaultTask))
		{
			SemanticAnalyzer* poParser = new SemanticAnalyzer;
			poParser->EnableASTGeneration();

			vector<Token*> vpoTokens;
			if(!oScanner.GenerateTokens(vpoTokens))
			{
				return;
			}
			poParser->SetTokens(&vpoTokens);
			poParser->parse();
			list<ParserError>* ploErrors = poParser->GetErrors();
			if(ploErrors->size() == 0)
			{
				if(m_eTaskType == InterpreterTask)
				{
					// start interpreting the program
					Interpreter* poInterpreter = new Interpreter;
					poInterpreter->BuildEnvironment(poParser->GetSymbolTable());
					poInterpreter->Interpret(poParser->GetFirstInstruction());
					delete poInterpreter;
				}
				else if(m_eTaskType == DefaultTask)
				{
					m_sAssemblyFile = "";
					if(m_sSourceFile != "")
					{
						size_t iPosition = m_sSourceFile.find(".sim");
						if(iPosition != m_sSourceFile.npos)
						{
							m_sAssemblyFile = m_sSourceFile.substr(0,iPosition);
							m_sAssemblyFile = m_sAssemblyFile + ".s";
						}
						else
						{
							m_sAssemblyFile = m_sSourceFile + ".s";
						}
					}
					// start generating code for the program
					CodeGenerator* poGenerator = new CodeGenerator;
					poGenerator->BuildEnvironment(poParser->GetSymbolTable());
					if(poGenerator->Generate(poParser->GetSymbolTable(),poParser->GetFirstInstruction()))
					{
						if(m_sAssemblyFile == "")
						{
							printf("%s",poGenerator->GetCode().c_str());
						}
						else
						{
							FILE* fpFile = fopen(m_sAssemblyFile.c_str(),"w");
							fprintf(fpFile,"%s",poGenerator->GetCode().c_str());
							fclose(fpFile);
						}
					}
					delete poGenerator;
				}
			}
			else
			{
				list<ParserError>::iterator liErrors;
				for(liErrors = ploErrors->begin() ; liErrors != ploErrors->end() ; liErrors++)
				{
					// do not show AST generation error for a non AST generation task
					if((*liErrors).GetType() == ASTGenerationError)
					{
						if(m_eTaskType != ASTGenerationTask)
						{
							continue;
						}
					}
					fprintf(stderr,"error: around position %d - %s\n",(*liErrors).GetPosition(),(*liErrors).GetString().c_str());
				}
			}
			delete poParser;
		}*/
	}
	void Driver::Scan(Scanner* poScanner)
	{
		vector<Token*> vpoTokens;
		bool bValidSource = poScanner->GenerateTokens(vpoTokens);
		unsigned int i = 0;
		unsigned int iSize = (unsigned int)vpoTokens.size();
		for(i = 0 ; i < iSize ; i++)
		{
			if(bValidSource)
			{
				vpoTokens[i]->Print();
			}
			delete vpoTokens[i];
		}
		vpoTokens.clear();
	}
	bool Driver::GetSourceText(string& sSourceText)
	{
		sSourceText = "";
		int iReadCharacter = 0;
		if(m_sSourceFile == "")
		{
			while(!feof(stdin))
			{
				iReadCharacter = fgetc(stdin);
				if(iReadCharacter == -1)			// end of file
				{
					break;
				}
				else
				{
					sSourceText = sSourceText + (char)iReadCharacter;
				}
			}
		}
		else
		{
			FILE* fpFile = fopen(m_sSourceFile.c_str(),"r");
			if(fpFile == NULL)
			{
				fprintf(stderr,"error: input file %s was not found\n",m_sSourceFile.c_str());
				return false;
			}
			while(!feof(fpFile))
			{
				iReadCharacter = fgetc(fpFile);
				if(iReadCharacter == -1)			// end of file
				{
					break;
				}
				else
				{
					sSourceText = sSourceText + (char)iReadCharacter;
				}
			}
			fclose(fpFile);
		}
		return true;
	}
}

int main(int argc,char** argv)
{
	Driver oDriver;
	if(!oDriver.ParseRunCommand(argc,argv))
	{
		return 1;
	}

	// the user has supplied a valid combination of options
	oDriver.Execute();
	return 0;
}



