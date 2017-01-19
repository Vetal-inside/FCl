//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <vector>

//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *LogWnd;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
class TLog
{
public:
UnicodeString* Header;
UnicodeString* Body;
TRichEdit* Output;
short LogLevel;
short LogVersion;

TLog(TRichEdit*);
void Add(UnicodeString ToLog);
void RemoveLowChar();
~TLog();
};
extern PACKAGE TForm2 *Form2;
extern TLog* Log;
//---------------------------------------------------------------------------
#endif
