#ifndef TPF_MEMORY_LEAK_DETECT_HPP
#define TPF_MEMORY_LEAK_DETECT_HPP

/* Programmed by: Thomas Kim
	
	First Created: Nov. 5, 2017
	Last Modified: Nov. 7, 2017

	Version: 1.2
*/

#include <iostream>
#include <sstream>
#include <string>

#ifdef UNICODE
	#define STDSTRING std::wstring
	#define STRSTREAM std::wostringstream
	#define UNISTR(txt)		L##txt	
	#define UNITXT(txt)		UNISTR(#txt)
	#define STDCOUT	std::wcout
#else
	#define STDSTRING std::string
	#define STRSTREAM std::ostringstream
	#define UNISTR(txt)		txt
	#define UNITXT(txt)		UNISTR(#txt)
	#define STDCOUT	std::cout
#endif

#if !defined(_WINDOWS) && !defined(_AFXDLL)

	#ifdef _DEBUG
		// order of #define 
		// and #include should be preserved
		#define _CRTDBG_MAP_ALLOC
		#include <cstdlib>
		#include <crtdbg.h>
		
		#ifndef new
			#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
		#endif
		
		class CMemLeakCheck
		{
		protected:
			STDSTRING m_filename;
			STDSTRING m_varname;
			int m_lineno;
			size_t m_total;

		public:
			CMemLeakCheck(const STDSTRING& filename=UNISTR(""),
				int lineno=0, const STDSTRING& varname=UNISTR("")) :
				m_filename(filename), m_lineno(lineno), m_varname(varname), m_total(0)
			{
				// does not work
				Reset(); 
			}

			void Reset() {
				
				_CrtMemState mem_state;
				_CrtMemCheckpoint(&mem_state);
				this->m_total = mem_state.lSizes[_NORMAL_BLOCK];
			}
			
			size_t Difference() {
				
				_CrtMemState mem_state;
				_CrtMemCheckpoint(&mem_state);

				return mem_state.lSizes[_NORMAL_BLOCK] - this->m_total;
			}
			
			STDSTRING Message(int lineno = 0, bool from_destructor=false)
			{
				size_t bytes = Difference();
				
				STRSTREAM stm;
				
				if (bytes > 0)
				{
					stm << UNISTR("File: ") << m_filename << UNISTR("\nAt line: ") << (lineno ? lineno : m_lineno)
						<< UNISTR(", Checkpoint: ") << m_varname << UNISTR(", ") << bytes
						<< (from_destructor ? UNISTR(" bytes LEAKED.") : UNISTR(" bytes ALIVE."));
				}
				else
				{
					stm << UNISTR("Memory State CLEAN.");
				}

				return stm.str();
			}

			void Report(int lineno = 0, bool from_destructor=false)
			{
				STDCOUT << UNISTR("\n") << Message(lineno, from_destructor) << UNISTR("\n");
			}

			~CMemLeakCheck()
			{
				if (Difference() > 0) { Report(0, true); }
			}
		};


		#define MemLeakCheckReport(varname)	varname.Report(__LINE__)
		#define MemLeakCheckMessage(varname) varname.Message(__LINE__)

	#endif // end of debugging


#else	// MFC Console or MFC Window

	#ifdef _DEBUG

		#ifndef new 
		#define new DEBUG_NEW
		#endif
	
		class CMemLeakCheck
		{
		protected:
			CString m_filename;
			CString m_varname;
			int m_lineno;
			size_t m_total;

			CMemoryState mem_state;

		public:
			CMemLeakCheck(const CString& filename= UNISTR(""),
				int lineno=0, const CString& varname= UNISTR("")) :
				m_filename(filename), m_lineno(lineno), m_varname(varname), m_total(0)
			{
				// this does not work
				// Reset();
			}

			void Reset() 
			{ 
				mem_state.Checkpoint();
				this->m_total = mem_state.m_lSizes[_NORMAL_BLOCK];
			}

			size_t Difference() 
			{
				mem_state.Checkpoint();
				return (mem_state.m_lSizes[_NORMAL_BLOCK]) - this->m_total;
			}

		#ifdef _CONSOLE
			STDSTRING 
		#else
			CString
		#endif
				Message(int lineno=0, bool from_destructor=false)
			{
				size_t bytes = Difference();

				STRSTREAM stm;

				if (bytes > 0)
				{

					stm << UNISTR("File: ") << m_filename.GetString() << UNISTR("\nAt line: ") << (lineno ? lineno : m_lineno)
						<< UNISTR(", Checkpoint: ") << m_varname.GetString() << UNISTR(", ") << bytes <<
						(from_destructor ? UNISTR(" bytes LEAKED.") : UNISTR(" bytes are ALIVE."));
				}
				else
				{
					stm << UNISTR("Memory Clean.");
				}
				
				#ifdef _CONSOLE
				
					return stm.str();
				
				#else
				
					return CString(stm.str().c_str());
				#endif
			}

			void Report(int lineno = 0, bool from_destructor=false)
			{
				#ifdef _CONSOLE
					STDCOUT <<std::endl<<Message(lineno, from_destructor) << std::endl;

				#else
					CString msg = Message(lineno, from_destructor);
					AfxMessageBox(msg);
				#endif
			}

			~CMemLeakCheck()
			{
				if (Difference()  > 0) { Report(0, true); }
			}
		};

		#define MemLeakCheckReport(varname)	varname.Report(__LINE__)
		#define MemLeakCheckMessage(varname) varname.Message(__LINE__)
		
		//#define MemLeakCheckReport(varname)	AfxMessageBox(_T("MemLeakCheckReport is not supported in MFC Windows App"))
		//#define MemLeakCheckMessage(varname) AfxMessageBox(_T("MemLeakCheckMessage is not supported in MFC Windows App"))
		
	#endif // end of debugging
#endif // 

#ifdef _DEBUG

	#ifdef UNICODE
		#define Tpf_MemLeakCheck(varname)	CMemLeakCheck varname(__FILEW__, __LINE__, UNITXT(varname)) ; varname.Reset()
	#else
		#define Tpf_MemLeakCheck(varname)	CMemLeakCheck varname(__FILE__, __LINE__, UNITXT(varname)) ; varname.Reset()
	#endif

	#define Tpf_MemLeakCheckReport(varname)	varname.Report(__LINE__)
	#define Tpf_MemLeakCheckMessage(varname) varname.Message(__LINE__)
#else	
	// non debugging mode
	#define Tpf_MemLeakCheck(varname)
	#define Tpf_MemLeakCheckReport(varname)
	#define Tpf_MemLeakCheckMessage(varname)
#endif

#endif // end of file