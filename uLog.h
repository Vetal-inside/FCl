//---------------------------------------------------------------------------

#ifndef uLogH
#define uLogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TLog : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *LogWnd;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	void Add(UnicodeString ToLog);
	__fastcall TLog(TComponent* Owner);
	String Header;
};
UnicodeString RemoveLowChar(UnicodeString Source);
char _DebugStrLowChar(char);
//---------------------------------------------------------------------------
extern PACKAGE TLog *Log;
//---------------------------------------------------------------------------
#endif
