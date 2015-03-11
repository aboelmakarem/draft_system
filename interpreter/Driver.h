//Ahmed Hussein
//ahussei4@jhu.edu

#ifndef DRIVER_H_
#define DRIVER_H_

#include "list"
#include "Scanner.h"

using namespace std;

namespace DraftCompiler
{
	enum TaskType
	{
		NullTaskType = 0,
		ScanTask = 1,
		ParseTask = 2,
		SemanticAnalysisTask = 3,
		ASTGenerationTask = 4,
		InterpreterTask = 5,
		DefaultTask = 6,
	};
	
	class Driver
	{
	public:
		Driver();
		Driver(const Driver& oDriver);
		~Driver();
		Driver& operator=(const Driver& oDriver);
		bool ParseRunCommand(int argc,char** argv);
		static void PrintUsageInstructions();
		void Execute();

	private:
	
	protected:
		void Initialize();
		bool SetRunTask(char* cOptionArgument);
		bool GetSourceText(string& sSourceText);
		void Scan(Scanner* poScanner);
		TaskType m_eTaskType;
		string m_sSourceFile;
		bool m_bGenerateGraphicalOutput;
	};
}

#endif


